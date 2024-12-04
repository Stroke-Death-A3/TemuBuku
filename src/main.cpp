#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_GLEXT_PROTOTYPES
#ifdef _WIN32
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#endif
#include "glfw3.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include "./core/Book.h"
#include "./core/RBTree.cpp"
#include "./Process/File.cpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "./Process/Gimik.cpp"
#include <filesystem>
#include "font_resources.h"

using TextureID = GLuint;

// Add this after TextureID declaration
std::unordered_map<std::string, TextureID> textureCache;

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// First, add debug logging to LoadTextureFromURL
TextureID LoadTextureFromURL(const char *url)
{
    int width, height, channels;
    unsigned char* image = stbi_load(url, &width, &height, &channels, 4); // Force RGBA
    
    if (!image) {
        std::cerr << "Failed to load image: " << url << std::endl;
        std::cerr << "STB Reason: " << stbi_failure_reason() << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    stbi_image_free(image);
    return textureID;
}

int main(int, char **)

{
    static ImVec2 popupPos(0, 0);
    // Add at the top with other state variables
    static bool showNoResults = false;
    static float noResultsTimer = 0.0f;
    // Initialize GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }

    // Setup window
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "TemuBuku", nullptr, nullptr);
    if (window == nullptr)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    // Load custom font
    std::string playChickensPath = getFontPath("Play-Chickens.otf");
    std::string robotoPath = getFontPath("Roboto-Regular.ttf");
    
    ImFont *customFont = io.Fonts->AddFontFromFileTTF(playChickensPath.c_str(), 24.0f);
    ImFont *titleFont = io.Fonts->AddFontFromFileTTF(robotoPath.c_str(), 24.0f);

    if (!customFont || !titleFont) {
        fprintf(stderr, "Failed to load fonts!\n");
        // Fall back to default font
        customFont = io.Fonts->AddFontDefault();
        titleFont = io.Fonts->AddFontDefault();
    }

    // Set default font
    io.FontDefault = titleFont;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Set border radius for input fields and buttons
    ImGuiStyle &style = ImGui::GetStyle();
    style.FrameRounding = 5.0f; // Set border radius for input fields and buttons
    style.GrabRounding = 5.0f;  // Set border radius for sliders, etc.

    // Modify color for input fields and buttons
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);       // Dark gray (input fields)
    style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);        // Dark gray (buttons)
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f); // Darker gray (button hover)
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);  // Darkest gray (button active)
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Background color
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    // Book search setup
    std::filesystem::path datasetDir = "./datasets";
    std::filesystem::path booksPath = datasetDir / "books.csv";

    // Create datasets directory if it doesn't exist
    if (!std::filesystem::exists(datasetDir)) {
        std::filesystem::create_directories(datasetDir);
    }

    // Initialize book handling
    RBTree rbtree1;
    File fileHandler(rbtree1);

    try
    {
        fileHandler.openFile(booksPath.string());
        fileHandler.parseFile();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading books: " << e.what() << std::endl;
        return 1;
    }

    Gimik gimik(rbtree1);

    // Search variables

    char searchTerm[256] = "";

    bool showDetailsModal = false;

    std::vector<Node *> searchResults; // Store results persistently

    File file(rbtree1);

    std::string line;

    auto fields = file.splitCSVLine(line);

    Node *selectedBook = nullptr;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("TemuBuku", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse);

        // Set background color and text color for the main window
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Dark gray background
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));     // White text

        // Use title font for header
        ImGui::PushFont(titleFont);
        ImGui::Text("Search Book : ");
        ImGui::PopFont();

        ImGui::SameLine();

        // Search bar (uses default font)
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.7f);
        bool searchTriggered = false;

        // Handle Enter key in input
        if (ImGui::InputTextWithHint("##search", "Enter book title...", searchTerm, IM_ARRAYSIZE(searchTerm),
                                     ImGuiInputTextFlags_EnterReturnsTrue))
        {
            searchTriggered = true;
        }

        ImGui::SameLine();

        // Handle Search button
        if (ImGui::Button("Search"))
        {
            searchTriggered = true;
        }

        // Modify the search trigger section:
        if (searchTriggered)
        {
            // Clear previous results to prevent memory buildup
            searchResults.clear();
            std::string searchString(searchTerm);
            searchResults = rbtree1.searchValue(searchString);
        }

        // Display results section
        ImGui::Separator();
        if (!searchResults.empty())
        {
            // Check if this is a special feature
            bool isSpecialFeature = false;

            if (searchResults[0]->data == "CALCULATOR")
            {
                isSpecialFeature = true;
                gimik.renderCalculator();
            }
            else if (searchResults[0]->data == "DICE")
            {
                isSpecialFeature = true;
                gimik.renderDiceRoller();
            }

            // Only show regular search UI if not a special feature
            if (!isSpecialFeature)
            {
                ImGui::PushFont(titleFont);
                ImGui::Text("Found %d matches:", searchResults.size());
                ImGui::PopFont();

                // Constants for pagination
                const int ITEMS_PER_PAGE = 100;
                static int currentPage = 0;
                int totalPages = (searchResults.size() + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;

                // Pagination controls
                if (ImGui::Button("<<")) currentPage = 0;
                ImGui::SameLine();
                if (ImGui::Button("<") && currentPage > 0) currentPage--;
                ImGui::SameLine();
                ImGui::Text("Page %d/%d", currentPage + 1, totalPages);
                ImGui::SameLine();
                if (ImGui::Button(">") && currentPage < totalPages - 1) currentPage++;
                ImGui::SameLine();
                if (ImGui::Button(">>")) currentPage = totalPages - 1;

                // Main scrollable container with clipper
                ImGui::BeginChild("SearchResults", ImVec2(0, ImGui::GetWindowHeight() * 0.7f), true);
                ImGuiListClipper clipper;
                
                int startIdx = currentPage * ITEMS_PER_PAGE;
                int endIdx = std::min(startIdx + ITEMS_PER_PAGE, (int)searchResults.size());
                
                clipper.Begin(endIdx - startIdx);
                while (clipper.Step())
                {
                    for (int i = clipper.DisplayStart + startIdx; i < clipper.DisplayEnd + startIdx && i < endIdx; i++)
                    {
                        Node* result = searchResults[i];
                        ImGui::PushID(result);

                        // Extract title from full data string
                        std::string fullData = result->data;
                        std::string title;
                        size_t titleStart = fullData.find('|');
                        size_t titleEnd = fullData.find('|', titleStart + 1);
                        if (titleStart != std::string::npos && titleEnd != std::string::npos)
                        {
                            title = fullData.substr(titleStart + 1, titleEnd - titleStart - 1);
                        }
                        else
                        {
                            title = fullData;
                        }

                        // Set background color for alternating rows
                        ImGui::PushStyleColor(ImGuiCol_Button, (i % 2 == 0) ? 
                            ImVec4(0.2f, 0.2f, 0.2f, 1.0f) : 
                            ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                        
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

                        // Calculate row height and full width
                        float rowHeight = ImGui::GetTextLineHeight() + 20.0f;
                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
                        
                        // Create full-width button with text aligned left
                        if (ImGui::Button(title.c_str(), ImVec2(-FLT_MIN, rowHeight)))
                        {
                            selectedBook = result;
                            showDetailsModal = true;
                            popupPos = ImGui::GetMousePos();
                        }

                        ImGui::PopStyleVar();
                        ImGui::PopStyleColor(2);
                        ImGui::PopID();
                    }
                }

                ImGui::EndChild();
            }
        }
        else if (searchTriggered) 
        {
            showNoResults = true;
            noResultsTimer = 5.0f; // Show for 2 seconds
        }

        // Add this in your ImGui render loop
        if (showNoResults)
        {
            if (noResultsTimer > 0.0f)
            {
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("No results found.").x) * 0.5f);
                ImGui::Text("No results found.");
                noResultsTimer -= ImGui::GetIO().DeltaTime; // Decrease timer each frame
            }
            else
            {
                showNoResults = false;
            }
        }

        // Restore default style settings after search results
        ImGui::PopStyleColor(2); // Pop the text and window background color

        gimik.render();

        ImGui::End();

        // Before BeginPopupModal, calculate required width
        if (showDetailsModal)
        {
            ImGui::OpenPopup("Book Details");

            // Calculate required width based on title and content
            float minWidth = 400.0f;  // Minimum width
            float maxWidth = ImGui::GetIO().DisplaySize.x * 0.8f;  // Maximum 80% of screen width
            float popupWidth = minWidth;

            if (selectedBook != nullptr)
            {
                std::string data = selectedBook->data;
                std::vector<std::string> bookData;
                std::stringstream ss(data);
                std::string item;

                while (std::getline(ss, item, '|'))
                {
                    item.erase(0, item.find_first_not_of(" \t"));
                    item.erase(item.find_last_not_of(" \t") + 1);
                    if (!item.empty())
                    {
                        bookData.push_back(item);
                    }
                }

                if (!bookData.empty())
                {
                    // Calculate width needed for each field
                    float titleWidth = ImGui::CalcTextSize(bookData[1].c_str()).x;
                    float isbnWidth = ImGui::CalcTextSize(bookData[0].c_str()).x;
                    float authorWidth = ImGui::CalcTextSize(bookData[2].c_str()).x;
                    
                    // Add padding for labels and copy buttons
                    float labelWidth = 100.0f;  // Width reserved for labels
                    float buttonWidth = 50.0f;  // Width for copy button
                    float padding = 40.0f;      // Extra padding
                    
                    // Find the maximum width needed
                    popupWidth = std::max({
                        titleWidth,
                        isbnWidth,
                        authorWidth
                    }) + labelWidth + buttonWidth + padding;
                    
                    // Clamp width between min and max
                    popupWidth = std::min(std::max(popupWidth, minWidth), maxWidth);
                }
            }

            // Set popup position and size
            ImGui::SetNextWindowPos(popupPos);
            ImGui::SetNextWindowSize(ImVec2(popupWidth, 0), ImGuiCond_Always);
            showDetailsModal = false;
        }

        // Update BeginPopupModal flags to allow sizing
        if (ImGui::BeginPopupModal("Book Details", nullptr,
                                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::PushFont(customFont);
            ImGui::Text("Book Information");
            ImGui::PopFont();
            ImGui::Separator();

            if (selectedBook != nullptr)
            {
                // Parse the CSV string
                std::vector<std::string> bookData;
                std::string data = selectedBook->data;
                std::stringstream ss(data);
                std::string item;

                while (std::getline(ss, item, '|'))
                {
                    // Trim whitespace
                    item.erase(0, item.find_first_not_of(" \t"));
                    item.erase(item.find_last_not_of(" \t") + 1);
                    if (!item.empty())
                    {
                        bookData.push_back(item);
                    }
                }

                // Display formatted book information
                if (bookData.size() >= 8)
                { // Ensure we have enough fields including image URLs
                    // Function to create a copy button for text
                    auto AddCopyButton = [](const char* label, const char* text) {
                        ImGui::Text("%s:", label);
                        ImGui::SameLine(100);
                        ImGui::TextWrapped("%s", text);
                        ImGui::SameLine();
                        ImGui::PushID(label);
                        if (ImGui::SmallButton("Copy"))
                        {
                            ImGui::SetClipboardText(text);
                        }
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::SetTooltip("Copy to clipboard");
                        }
                        ImGui::PopID();
                    };

                    // Display book info with copy buttons
                    AddCopyButton("ISBN", bookData[0].c_str());
                    ImGui::Spacing();
                    AddCopyButton("Title", bookData[1].c_str());
                    ImGui::Spacing();
                    AddCopyButton("Author", bookData[2].c_str());
                    ImGui::Spacing();

                    // Add copy all button
                    if (ImGui::Button("Copy All Info"))
                    {
                        std::string allInfo = "ISBN: " + bookData[0] + "\n"
                                            "Title: " + bookData[1] + "\n"
                                            "Author: " + bookData[2];
                        ImGui::SetClipboardText(allInfo.c_str());
                    }
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("Copy all information to clipboard");
                    }

                    ImGui::Spacing();
                    ImGui::Separator();

                    // Image handling
                    std::string imageUrl = bookData[6];
                    imageUrl.erase(remove(imageUrl.begin(), imageUrl.end(), '"'), imageUrl.end());

                    // Try to load texture if not in cache
                    if (textureCache.find(imageUrl) == textureCache.end()) {
                        TextureID tex = LoadTextureFromURL(imageUrl.c_str());
                        if (tex) {
                            auto it = textureCache.find(imageUrl);
                            if (it != textureCache.end()) {
                                // Delete old texture before replacing
                                glDeleteTextures(1, &it->second);
                                textureCache.erase(it);
                            }
                            textureCache[imageUrl] = tex;
                        }
                    }

                    // Display the image
                    if (textureCache[imageUrl]) {
                        // Calculate aspect ratio for proper display
                        float aspectRatio = 0.75f; // Default book cover ratio (3:4)
                        float displayHeight = 300.0f;
                        float displayWidth = displayHeight * aspectRatio;
                        
                        ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - displayWidth) * 0.5f);
                        ImGui::Image((ImTextureID)(intptr_t)textureCache[imageUrl], 
                                    ImVec2(displayWidth, displayHeight));
                    } else {
                        ImGui::TextWrapped("Image loading failed");
                    }
                }
                else if (bookData.size() >= 3)
                {
                    // Same copy button functionality for smaller dataset
                    auto AddCopyButton = [](const char* label, const char* text) {
                        ImGui::Text("%s:", label);
                        ImGui::SameLine(100);
                        ImGui::TextWrapped("%s", text);
                        ImGui::SameLine();
                        ImGui::PushID(label);
                        if (ImGui::SmallButton("Copy"))
                        {
                            ImGui::SetClipboardText(text);
                        }
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::SetTooltip("Copy to clipboard");
                        }
                        ImGui::PopID();
                    };

                    AddCopyButton("ISBN", bookData[0].c_str());
                    ImGui::Spacing();
                    AddCopyButton("Title", bookData[1].c_str());
                    ImGui::Spacing();
                    AddCopyButton("Author", bookData[2].c_str());
                }

                ImGui::Separator();
                if (ImGui::Button("Close"))
                {
                    ImGui::CloseCurrentPopup();
                    selectedBook = nullptr;
                }
            }
            ImGui::EndPopup();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup textures
    for (const auto& pair : textureCache) {
        if (pair.second) {
            glDeleteTextures(1, &pair.second);
        }
    }
    textureCache.clear();

    // Clear search results
    searchResults.clear();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
