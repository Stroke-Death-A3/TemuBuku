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

class RBTree {
    std::vector<Book> books;

public:
    void insert(const Book& book) {
        books.push_back(book);
    }

    std::vector<Book> search(const std::string& keyword) {
        std::vector<Book> results;
        std::string lowerKeyword = keyword;

        // Ubah keyword menjadi huruf kecil
        std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);

        for (const auto& book : books) {
            std::string lowerTitle = book.title;

            // Ubah judul buku menjadi huruf kecil
            std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);

            // Cari keyword dalam title
            if (lowerTitle.find(lowerKeyword) != std::string::npos) {
                results.push_back(book);
            }
        }
        return results;
    }

    void loadFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }

        std::string line;
        bool isHeader = true; // Flag untuk melewati header
        while (std::getline(file, line)) {
            if (isHeader) {
                isHeader = false; // Lewati baris pertama (header)
                continue;
            }

            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> data;

            while (std::getline(ss, token, ',')) {
                data.push_back(token);
            }

            if (data.size() >= 10) { // Pastikan jumlah kolom cukup
                try {
                    int id = std::stoi(data[0]);
                    std::string title = data[1];
                    std::string author = data[2];
                    std::string category = data[3];
                    std::string subcategory = data[4];
                    std::string format = data[5];
                    double rating = std::stod(data[7]);
                    double reviews = std::stod(data[8]);
                    std::string url = data[9];

                    insert(Book(id, title, author, category, subcategory, format, rating, reviews, url));
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing line: " << line << " - " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Invalid data line: " << line << std::endl;
            }
        }
        std::cout << "Loaded " << books.size() << " books from CSV." << std::endl;
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
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    RBTree bookTree;
    bookTree.loadFromCSV("../Process/datasets/Books_df.csv");

    std::vector<Book> searchResults;
    char buffer[256] = "";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(100, 100));
        ImGui::SetNextWindowSize(ImVec2(400, 100));
        ImGui::Begin("Search", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::InputText("##search", buffer, IM_ARRAYSIZE(buffer));
        ImGui::SameLine();
        if (ImGui::Button("Search")) {
            searchResults = bookTree.search(buffer);
        }
        ImGui::End();

        // Tampilkan hasil pencarian
        ImGui::SetNextWindowPos(ImVec2(100, 220));
        ImGui::SetNextWindowSize(ImVec2(800, 400));
        ImGui::Begin("Search Results", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        if (!searchResults.empty()) {
            for (const auto& book : searchResults) {
                ImGui::Text("ID: %d | Title: %s | Author: %s | Category: %s", book.id, book.title.c_str(), book.author.c_str(), book.category.c_str());
            }
        } else {
            ImGui::Text("No results found.");
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
