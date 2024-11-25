#ifndef FILE_H
#define FILE_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "RBTree.h"
#include "Book.h"
class File {
private:
    std::ifstream file;
    RBTree& rbTree;  // Reference to RBTree object
    
    std::vector<std::string> splitCSVLine(const std::string& line);
    
    template <typename T>
    T stringToType(const std::string& str);

public:
    File(RBTree& tree) : rbTree(tree) {}  // Constructor taking RBTree reference
    void openFile(const std::string& path);
    void parseFile();
    ~File();
};
#endif