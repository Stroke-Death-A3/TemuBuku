#include <bits/stdc++.h>
#include "../core/RBTree.cpp"
#include "File.cpp"
#include <iostream>

int main() {
    // Create RBTree instance
    RBTree rbTree1;
    
    // Define file path
    std::string path = "./datasets/Books_df.csv";
    
    // Create File instance with RBTree reference
    File fileHandler(rbTree1);
    
    try {
        // Open file
        fileHandler.openFile(path);
        
        // Parse file and insert books into RBTree
        fileHandler.parseFile();
        
        string InputJudul;
        // Example search (modify as needed)
        getline(cin,InputJudul);
        rbTree1.searchValue(InputJudul);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
