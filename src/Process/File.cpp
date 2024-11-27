#include "../core/File.h"

std::vector<std::string> File::splitCSVLine(const std::string& line) {
    std::vector<std::string> result;
    std::string currentField;
    bool inQuotes = false;
    
    for (size_t i = 0; i < line.length(); ++i) {
        char ch = line[i];
        if (ch == '"') {
            inQuotes = !inQuotes;
        } else if (ch == ',' && !inQuotes) {
            result.push_back(currentField);
            currentField.clear();
        } else {
            currentField += ch;
        }
    }
    result.push_back(currentField); // Add the last field
    return result;
}

template <typename T>
T File::stringToType(const std::string& str) {
    std::string numStr = str;
    size_t pos = numStr.find("₹");
    if (pos != std::string::npos) {
        numStr = numStr.substr(pos + 1);
    }
    
    try {
        if constexpr (std::is_same_v<T, int>) {
            return std::stoi(numStr);
        } else if constexpr (std::is_same_v<T, double>) {
            return std::stod(numStr);
        }
    } catch (const std::invalid_argument& e) {
        throw std::domain_error("Invalid data type for field: " + str);
    }
    return T();
}

void File::openFile(const std::string& path) {
    file.open(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }
    std::cout << "Successfully opened file!" << std::endl;
}

void File::parseFile() {
    if (!file.is_open()) {
        std::cerr << "No file is open!" << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        auto fields = splitCSVLine(line);
        if (fields.size() >= 10) {
            try {
                Book book(
                    stringToType<int>(fields[0]),    // id
                    fields[1],                       // title
                    fields[2],                       // author
                    fields[3],                       // category
                    fields[4],                       // subcategory
                    fields[5],                       // format
                    stringToType<double>(fields[7]), // rating
                    stringToType<double>(fields[8]), // reviews
                    fields[9],                       // url
                    rbTree                           // rbTree reference
                );
                
                // Convert book to a string representation for insertion
                std::string bookKey = std::to_string(book.getId()) + "|" + 
                                      book.getTitle() + "|" + 
                                      book.getAuthor() + "|" ;                
                rbTree.insertValue(bookKey);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing line: " << line << std::endl;
                std::cerr << "Error message: " << e.what() << std::endl;
            }
        }
    }
    file.close();
}

File::~File() {
    if (file.is_open()) {
        file.close();
    }
}