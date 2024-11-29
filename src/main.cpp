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
#include <algorithm> // Untuk std::transform

// Kelas Book
class Book {
public:
    int id;
    std::string title;
    std::string author;
    std::string category;
    std::string subcategory;
    std::string format;
    double rating;
    double reviews;
    std::string url;

    Book(int id, const std::string& title, const std::string& author, const std::string& category,
         const std::string& subcategory, const std::string& format, double rating, double reviews, const std::string& url)
        : id(id), title(title), author(author), category(category), subcategory(subcategory),
          format(format), rating(rating), reviews(reviews), url(url) {}
};

// Kelas RBTree (sederhana)
class RBTree {
    std::vector<Book> books;

public:
    void insert(const Book& book) {
        books.push_back(book); // Menambahkan buku ke dalam vector
    }

    std::vector<Book> search(const std::string& title) {
        std::vector<Book> results;
        std::string lowerTitle = title;
        
        // Ubah kata kunci menjadi huruf kecil
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);

        for (const auto& book : books) {
            std::string lowerBookTitle = book.title;
            // Ubah judul buku menjadi huruf kecil
            std::transform(lowerBookTitle.begin(), lowerBookTitle.end(), lowerBookTitle.begin(), ::tolower);

            if (lowerBookTitle.find(lowerTitle) != std::string::npos) {
                results.push_back(book);
            }
        }
        return results;
    }

    void loadFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> data;

            while (std::getline(ss, token, ',')) {
                data.push_back(token);
            }

            if (data.size() >= 9) { // Pastikan ada cukup data
                int id = std::stoi(data[0]);
                std::string title = data[1];
                std::string author = data[2];
                std::string category = data[3];
                std::string subcategory = data[4];
                std::string format = data[5];
                double rating = std::stod(data[6]);
                double reviews = std::stod(data[7]);
                std::string url = data[8];

                insert(Book(id, title, author, category, subcategory, format, rating, reviews, url));
            }
        }
        std::cout << "Loaded " << books.size() << " books from CSV." << std::endl; // Debug
    }
};

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char**) {
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
    glfwMakeContextCurrent(window); // Perbaiki pengetikan di sini
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    // Inisialisasi RBTree dan memuat data dari CSV
    RBTree bookTree;
    bookTree.loadFromCSV("Books_df.csv");

    // Variabel untuk menyimpan hasil pencarian
    std::vector<Book> searchResults;
    char buffer[256] = ""; // Deklarasi buffer untuk input pencarian

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Tampilkan input pencarian
        ImGui::SetNextWindowPos(ImVec2(100, 100));
        ImGui::SetNextWindowSize(ImVec2(300, 50));
        ImGui::Begin("Search", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::InputText("##search", buffer, IM_ARRAYSIZE(buffer));
        ImGui::SameLine();
        if (ImGui::Button("Search")) {
            std::cout << "Searching for: " << buffer << std::endl; // Debug
            searchResults = bookTree.search(buffer);
            std::cout << "Found " << searchResults.size() << " results." << std::endl; // Debug
        }
        ImGui::End();

        // Tampilkan hasil pencarian
        ImGui::SetNextWindowPos(ImVec2(100, 200));
        ImGui::SetNextWindowSize(ImVec2(600, 400));
        ImGui::Begin("Search Results", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        for (const auto& book : searchResults) {
            ImGui::Text("%d. %s", book.id, book.title.c_str());
        }
        ImGui::End();

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
