#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> 

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "TemuBuku", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to create GLFW window\n");
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    // Load fonts
    ImFont* play_chickens_font = io.Fonts->AddFontFromFileTTF("./font/Play-Chickens.otf", 30.0f);
    ImFont* roboto_font = io.Fonts->AddFontFromFileTTF("./font/Roboto-Regular.ttf", 20.0f);
    if (!play_chickens_font || !roboto_font) {
        fprintf(stderr, "Error: Failed to load fonts!\n");
        return 1;
    }

    static char buffer[256] = ""; // Buffer untuk InputText

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Set window size for the main content
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_Always); // Fullscreen size
        ImGui::Begin("Main Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // Centering the content vertically and horizontally
        ImVec2 screen_center = ImVec2(640, 720 / 2); // Define the center point of the screen

        // Judul: Align Center with Play Chickens font
        ImGui::PushFont(play_chickens_font);
        ImVec2 text_size = ImGui::CalcTextSize("Selamat datang di TemuBuku!");
        ImVec2 text_pos = ImVec2(screen_center.x - text_size.x / 2, screen_center.y - 300); // Slightly above center
        ImGui::GetForegroundDrawList()->AddText(text_pos, IM_COL32(255, 255, 255, 255), "Selamat datang di TemuBuku!");
        ImGui::PopFont();

        // Input Field: Set manual position
        float input_pos_x = 190.0f; // Horizontal position (X) for input field
        float input_pos_y = 300.0f; // Vertical position (Y) for input field

        // Label: Set manual position
        float label_pos_x = input_pos_x; // Horizontal position (X) for label
        float label_pos_y = input_pos_y - 25.0f; // Vertical position (Y) for label, slightly above input field

        // Set position for the label and input field
        ImGui::PushFont(roboto_font);
        ImGui::SetCursorPos(ImVec2(label_pos_x, label_pos_y)); 
        ImGui::Text("Masukkan nama buku:"); // Label for input field
        ImGui::PopFont();

        // Set position for input field
        ImGui::PushFont(roboto_font);
        ImGui::SetCursorPos(ImVec2(input_pos_x, input_pos_y)); 
        ImGui::InputText("##Input", buffer, IM_ARRAYSIZE(buffer)); // Input field
        ImGui::PopFont();

        // Button: Set center position horizontally
        ImGui::PushFont(roboto_font);
        ImGui::SetCursorPosY(screen_center.y + 80); // Place button below the input field
        ImGui::SetCursorPosX((1280 - 100) / 2); // Center the button horizontally
        if (ImGui::Button("Cari Buku")) {
            printf("Input: %s\n", buffer);
        }
        ImGui::PopFont();

        // Bottom Text: Set center position horizontally using Roboto font
        ImGui::PushFont(roboto_font);
        ImVec2 bottom_text_size = ImGui::CalcTextSize("Cari buku, temukan cerita. TemuBuku hadir untuk menjelajah, memilih dan membaca tanpa batas!");
        float bottom_text_pos_x = (1280 - bottom_text_size.x) / 2; // Center text horizontally
        float bottom_text_pos_y = screen_center.y + 400; // Set position for bottom text
        ImGui::PopFont();

        ImGui::End(); // End main window

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}