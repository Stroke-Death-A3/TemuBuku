#include <bits/stdc++.h>
#include "../core/RBTree.h"
#include "../core/File.h"          // Include the header file
#include "../core/Book.h"           // Include the header file
#include <iostream>


int main() {
    // Create RBTree instance
    RBTree rbTree1;    // Define file path
    std::string path = "./datasets/Books_df.csv";
    // Create File instance with RBTree reference
    File fileHandler(rbTree1);
    
    try {
        // Open file
        fileHandler.openFile(path);
        
        // Parse file and insert books into RBTree
        fileHandler.parseFile();
        
        std::string InputJudul;
        // Example search (modify as needed)
        std::getline(std::cin, InputJudul);
        rbTree1.searchValue(InputJudul);
        rbTree1.addToFavorites(InputJudul);
        rbTree1.displayFavorites();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
