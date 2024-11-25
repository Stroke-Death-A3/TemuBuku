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
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "TemuBuku", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f); 


    ImFont* custom_font = io.Fonts->AddFontFromFileTTF("C:/engine_project/src/Process/datasets/font/Play Chickens.otf", 40.0f); 
    ImFont* roboto_font = io.Fonts->AddFontFromFileTTF("C:/engine_project/src/Process/datasets/font/Roboto-Regular.ttf", 20.0f); 

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImVec2 screen_center = ImVec2(640, 200); 
        ImGui::PushFont(custom_font);
        ImVec2 text_size = ImGui::CalcTextSize("Selamat datang di TemuBuku!");
        ImVec2 text_pos = ImVec2(screen_center.x - text_size.x / 2, screen_center.y - text_size.y / 2);
        ImGui::GetForegroundDrawList()->AddText(text_pos, IM_COL32(255, 255, 255, 255), "Selamat datang di TemuBuku!");
        ImGui::PopFont();

        ImVec2 input_size = ImVec2(700, 60);
        ImVec2 input_pos = ImVec2(screen_center.x - input_size.x / 2, screen_center.y + text_size.y / 2 + 20);

        ImGui::SetNextWindowPos(input_pos); 
        ImGui::SetNextWindowSize(input_size, ImGuiCond_Always);
        ImGui::Begin("Input Field", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::End();

        ImGui::PushFont(roboto_font);
        ImVec2 bottom_text_size = ImGui::CalcTextSize("Cari buku, temukan cerita. TemuBuku hadir untuk menjelajah, memilih dan membaca tanpa batas!");
        ImVec2 bottom_text_pos = ImVec2(screen_center.x - bottom_text_size.x / 2, 650); 
        ImGui::GetForegroundDrawList()->AddText(bottom_text_pos, IM_COL32(255, 255, 255, 255), "Cari buku, temukan cerita. TemuBuku hadir untuk menjelajah, memilih dan membaca tanpa batas!");
        ImGui::PopFont();

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
