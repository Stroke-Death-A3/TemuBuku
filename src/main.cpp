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
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Background color
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    // Book search setup
    std::string path = "../Process/datasets/Books_df.csv";
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

    std::vector<Node*> searchResults;  // Store results persistently

    File file(rbtree1);

    std::string line;

    auto fields = file.splitCSVLine(line);

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

        // Search bar
        ImGui::Text("Book Search");
        ImGui::SameLine();
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
            ImGui::Text("Found %d matches:", searchResults.size());
            for (Node *result : searchResults)
            {
                if (ImGui::Selectable(result->data.c_str()))
                {
                    // Handle selection if needed
                }
            }
        }
        else if (searchTriggered)  // Only show "No results" if a search was performed
        {
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("No results found.").x) * 0.5f);
            ImGui::Text("No results found.");
        }

        ImGui::End();

        // Book Details Modal
        if (showDetailsModal)
        {
            ImGui::OpenPopup("Book Details");
            showDetailsModal = false;
        }

        if (ImGui::BeginPopupModal("Book Details", nullptr,
                                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::Text("Book Information");
            ImGui::Separator();
            Book book(1, "title", "author", "category", "subcategory", "format", 4.5, 1000, "url");
            ImGui::Text("ID: %d", book.getId());
            ImGui::Text("Title: %s", book.getTitle().c_str());
            ImGui::Text("Author: %s", book.getAuthor().c_str());
            ImGui::Text("Category: %s", book.getCategory().c_str());
            ImGui::Text("Subcategory: %s", book.getSubcategory().c_str());
            ImGui::Text("Format: %s", book.getFormat().c_str());
            ImGui::Text("Rating: %.2f", book.getRating());
            ImGui::Text("Reviews: %.0f", book.getReviews());

            ImGui::Separator();
            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
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