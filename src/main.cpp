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
    glfwSwapInterval(1); 

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    // Load fonts
    ImFont* judul_utama = io.Fonts->AddFontFromFileTTF("C:/engine_project/src/font/AntipastoPro-Bold_trial.ttf", 50.0f);
    ImFont* font_quotes = io.Fonts->AddFontFromFileTTF("C:/engine_project/src/font/AntipastoPro-Bold_trial.ttf", 20.0f);
    ImFont* font_elemen = io.Fonts->AddFontFromFileTTF("C:/engine_project/src/font/AntipastoPro-Bold_trial.ttf", 25.0f);

    static char buffer[256] = ""; 
    // Warna abu-abu dongker
    ImVec4 dark_gray = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // RGB: (51, 51, 51)

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_Always);
        ImGui::Begin("Main Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImVec2 screen_center = ImVec2(640, 720 / 2); // Define the center point of the screen

        // Judul: Align Center with Play Chickens font
        ImGui::PushFont(judul_utama);
        ImVec2 text_size = ImGui::CalcTextSize("Selamat datang di TemuBuku!");
        ImVec2 text_pos = ImVec2(screen_center.x - text_size.x / 2, screen_center.y - 300); // Slightly above center
        ImGui::GetForegroundDrawList()->AddText(text_pos, IM_COL32(255, 255, 255, 255), "Selamat datang di TemuBuku!");
        ImGui::PopFont();

        // Input Field: Set manual position
        float input_pos_x = 200.0f;
        float input_pos_y = 180.0f;

        // Label: Set manual position
        float label_pos_x = input_pos_x + 300;
        float label_pos_y = input_pos_y - 28.0f;

        // Set position for the label and input field
        ImGui::PushFont(font_elemen);
        ImGui::SetCursorPos(ImVec2(label_pos_x, label_pos_y - 10)); 
        ImGui::Text("Masukkan nama buku:");
        ImGui::PopFont();

        // Set position and style for input field
        ImGui::PushFont(font_quotes);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f); // Set corner radius to 5

        // Ganti warna input field ke abu-abu dongker
        ImGui::PushStyleColor(ImGuiCol_FrameBg, dark_gray); // Background warna input field
        ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(100, 100, 100, 255)); // Border warna abu-abu lebih terang

        ImGui::SetCursorPos(ImVec2(input_pos_x, input_pos_y)); 
        ImGui::InputText("##Input", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue);
        
        ImGui::PopStyleColor(2); // Pop warna input field dan border
        ImGui::PopStyleVar(); // Restore rounding
        ImGui::PopFont();

        // Tombol: Set center position horizontally
        ImGui::PushFont(font_elemen);
        ImGui::SetCursorPosY(screen_center.y - 140);
        ImGui::SetCursorPosX((1280 - 300) / 2);

        // Tombol dengan warna abu-abu dongker
        ImVec2 button_size = ImVec2(300, 40);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));

        // Ganti warna tombol ke abu-abu dongker
        ImGui::PushStyleColor(ImGuiCol_Button, dark_gray);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(120, 120, 120, 255)); // Hovered lebih terang
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(90, 90, 90, 255)); // Active lebih gelap

        if (ImGui::Button("Cari Buku", button_size)) {
            printf("Input: %s\n", buffer);
        }

        ImGui::PopStyleColor(3); // Pop warna tombol
        ImGui::PopStyleVar(2); // Restore padding dan rounding
        ImGui::PopFont();        // Bottom Text: Set center position horizontally using Roboto font
        ImGui::PushFont(font_quotes);
        ImVec2 bottom_text_size = ImGui::CalcTextSize("Cari buku, temukan cerita. TemuBuku hadir untuk menjelajah, memilih dan membaca tanpa batas!");
        float bottom_text_pos_x = (1280 - bottom_text_size.x) / 2; // Center text horizontally
        float bottom_text_pos_y = screen_center.y + 300; // Set position for bottom text
        ImGui::SetCursorPos(ImVec2(bottom_text_pos_x, bottom_text_pos_y)); 
        ImGui::Text("Cari buku, temukan cerita. TemuBuku hadir untuk menjelajah, memilih dan membaca tanpa batas!");
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
