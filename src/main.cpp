#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "./core/Book.h"
#include "./core/RBTree.cpp"
#include "./Process/File.cpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "./Process/Gimik.cpp"

using TextureID = unsigned long long;

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// First, add debug logging to LoadTextureFromURL
TextureID LoadTextureFromURL(const char *url)
{
    std::cout << "\n=== Attempting to load image ===" << std::endl;
    std::cout << "URL: " << url << std::endl;

    // Debug: Check if URL is accessible
    FILE *file = fopen(url, "rb");
    if (!file)
    {
        std::cerr << "Error: Cannot access URL/file: " << url << std::endl;
        std::cerr << "Error details: " << strerror(errno) << std::endl;
        return 0;
    }
    fclose(file);

    // Load image
    int width, height, channels;
    unsigned char *image = stbi_load(url, &width, &height, &channels, STBI_rgb_alpha);

    if (!image)
    {
        std::cerr << "stbi_load failed:" << std::endl;
        std::cerr << "  Reason: " << stbi_failure_reason() << std::endl;
        return 0;
    }

    std::cout << "Image loaded successfully:" << std::endl;
    std::cout << "  Dimensions: " << width << "x" << height << std::endl;
    std::cout << "  Channels: " << channels << std::endl;

    // Create OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    if (textureID == 0)
    {
        std::cerr << "Failed to generate OpenGL texture" << std::endl;
        stbi_image_free(image);
        return 0;
    }

    std::cout << "Created OpenGL texture ID: " << textureID << std::endl;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(image);

    std::cout << "Texture loaded successfully" << std::endl;
    std::cout << "==============================\n"
              << std::endl;

    // Cast to TextureID for return
    return static_cast<TextureID>(textureID);
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
    ImFont *customFont = io.Fonts->AddFontFromFileTTF("../font/Play-Chickens.otf", 24.0f); // Regular size
    ImFont *titleFont = io.Fonts->AddFontFromFileTTF("../font/Roboto-Regular.ttf", 24.0f); // Larger for titles

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
    std::string path = "../Process/datasets/books.csv";
    RBTree rbtree1;
    File fileHandler(rbtree1);

    try
    {
        fileHandler.openFile(path);
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

        // Process search if triggered by either method
        if (searchTriggered)
        {
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
                bool alternate = false;

                // Set background color to dark gray when search is triggered
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Dark gray color

                // Main scrollable container
                ImGui::BeginChild("SearchResults", ImVec2(0, ImGui::GetWindowHeight() * 0.7f), true);

                for (Node *result : searchResults)
                {
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
                        title = fullData; // Fallback to full data if parsing fails
                    }

                    // Style settings remain the same
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));

                    if (alternate)
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
                    }
                    else
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                    }

                    float rowHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y + 10;

                    // Display only title in Selectable
                    if (ImGui::Selectable(title.c_str(), false, ImGuiSelectableFlags_None, ImVec2(0, rowHeight)))
                    {
                        selectedBook = result;
                        showDetailsModal = true;
                        popupPos = ImGui::GetMousePos();
                    }

                    ImGui::PopStyleColor();
                    ImGui::PopStyleVar(2);
                    ImGui::PopID();
                    alternate = !alternate;
                }

                ImGui::EndChild(); // End main scrollable container

                // Pop the color change for child background
                ImGui::PopStyleColor();
            }
        }
        // Replace the existing code
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

            // Calculate required width based on title if we have a selected book
            float popupWidth = 400.0f; // Default minimum width
            if (selectedBook != nullptr)
            {
                std::string data = selectedBook->data;
                size_t titleStart = data.find('|');
                size_t titleEnd = data.find('|', titleStart + 1);
                if (titleStart != std::string::npos && titleEnd != std::string::npos)
                {
                    std::string title = data.substr(titleStart + 1, titleEnd - titleStart - 1);
                    float titleWidth = ImGui::CalcTextSize(title.c_str()).x;
                    // Add padding and ensure minimum width
                    popupWidth = std::max(400.0f, titleWidth + 100.0f);
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
                    // Display book info
                    ImGui::Text("ISBN:");
                    ImGui::SameLine(100);
                    ImGui::TextWrapped("%s", bookData[0].c_str());

                    ImGui::Spacing();
                    ImGui::Text("Title:");
                    ImGui::SameLine(100);
                    ImGui::TextWrapped("%s", bookData[1].c_str());

                    ImGui::Spacing();
                    ImGui::Text("Author:");
                    ImGui::SameLine(100);
                    ImGui::TextWrapped("%s", bookData[2].c_str());

                    ImGui::Spacing();

                    // Image handling with proper type casting
                    static TextureID bookCoverTexture = 0;
                    static std::string lastImageUrl;

                    // Get medium size image URL (index 6 contains MZZZZZZZ version)
                    std::string imageUrl = bookData[6];
                    imageUrl.erase(remove(imageUrl.begin(), imageUrl.end(), '"'), imageUrl.end());

                    if (lastImageUrl != imageUrl)
                    {
                        // Clear previous texture
                        if (bookCoverTexture)
                        {
                            GLuint texID = static_cast<GLuint>(bookCoverTexture);
                            glDeleteTextures(1, &texID);
                            bookCoverTexture = 0;
                        }

                        // Load new texture
                        TextureID newTexture = LoadTextureFromURL(imageUrl.c_str());
                        if (newTexture)
                        {
                            bookCoverTexture = newTexture;
                            lastImageUrl = imageUrl;
                            std::cout << "Successfully loaded texture ID: " << newTexture << std::endl;
                        }
                    }

                    if (bookCoverTexture)
                    {
                        float windowWidth = ImGui::GetWindowWidth();
                        float imageWidth = 200;
                        float centeredX = (windowWidth - imageWidth) * 0.5f;
                        ImGui::SetCursorPosX(centeredX);
                        ImGui::Image(reinterpret_cast<ImTextureID>(bookCoverTexture), ImVec2(200, 300));
                    }
                    else
                    {
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                                           "Failed to load image from URL: %s", imageUrl.c_str());
                    }
                }
                else if (bookData.size() >= 3)
                {
                    ImGui::Text("ISBN:");
                    ImGui::SameLine(100);
                    ImGui::TextWrapped("%s", bookData[0].c_str());

                    ImGui::Spacing();
                    ImGui::Text("Title:");
                    ImGui::SameLine(100);
                    ImGui::TextWrapped("%s", bookData[1].c_str());

                    ImGui::Spacing();
                    ImGui::Text("Author:");
                    ImGui::SameLine(100);
                    ImGui::TextWrapped("%s", bookData[2].c_str());
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

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
