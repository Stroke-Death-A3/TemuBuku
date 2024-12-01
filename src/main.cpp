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

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char **)

{
    static ImVec2 popupPos(0,0);
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

                // Set border radius for each row (using PushStyleVar directly)
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);         // Set border radius
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10)); // Add padding to make the selectable button larger

                // Use the same background color for alternating rows
                if (alternate)
                {
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Darker gray for even rows
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Darker gray for odd rows
                }

                // Adjust row height dynamically based on font size
                float rowHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y + 10; // Include item spacing

                // Create the Selectable button with dynamic height

                if (ImGui::Selectable(result->data.c_str(), false, ImGuiSelectableFlags_None, ImVec2(0, rowHeight)))
                {
                    selectedBook = result; // Store selected book
                    showDetailsModal = true;
                    popupPos = ImGui::GetMousePos(); // Store cursor position
                }

                // Pop style changes after rendering the row
                ImGui::PopStyleColor();
                ImGui::PopStyleVar(2); // Pop both the border radius and padding changes

                ImGui::PopID();

                alternate = !alternate;
            }

            ImGui::EndChild(); // End main scrollable container

            // Pop the color change for child background
            ImGui::PopStyleColor();
        }
        else if (searchTriggered) // Only show "No results" if a search was performed
        {
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("No results found.").x) * 0.5f);
            ImGui::Text("No results found.");
        }

        // Restore default style settings after search results
        ImGui::PopStyleColor(2); // Pop the text and window background color

        ImGui::End();

        // Book Details Modal
        if (showDetailsModal)
        {
            ImGui::OpenPopup("Book Details");
            ImGui::SetNextWindowPos(popupPos); // Set popup position
            showDetailsModal = false;
        }

        if (ImGui::BeginPopupModal("Book Details", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
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
                if (bookData.size() >= 3)
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
