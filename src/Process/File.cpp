#include "../core/File.h"
#include "../core/Book.h"
#include "../core/global.h"
#include <algorithm>

std::vector<std::string> File::splitCSVLine(const std::string &line)
{
    std::vector<std::string> result;
    std::string currentField;
    bool inQuotes = false;

    for (size_t i = 0; i < line.length(); ++i)
    {
        char ch = line[i];
        if (ch == '"')
        {
            inQuotes = !inQuotes;
        }
        else if (ch == ',' && !inQuotes)
        {
            // Trim whitespace from the field
            currentField.erase(0, currentField.find_first_not_of(" \t"));
            currentField.erase(currentField.find_last_not_of(" \t") + 1);
            result.push_back(currentField);
            currentField.clear();
        }
        else
        {
            currentField += ch;
        }
    }

    // Add the last field
    currentField.erase(0, currentField.find_first_not_of(" \t"));
    currentField.erase(currentField.find_last_not_of(" \t") + 1);
    result.push_back(currentField);

    return result;
}

template <typename T>
T File::stringToType(const std::string &str)
{
    std::string numStr = str;

    // Remove currency symbols
    size_t pos = numStr.find_first_of("₹$€£");
    if (pos != std::string::npos)
    {
        numStr = numStr.substr(pos + 1);
    }

    // Remove commas from numeric strings
    numStr.erase(std::remove(numStr.begin(), numStr.end(), ','), numStr.end());

    try
    {
        if constexpr (std::is_same_v<T, int>)
        {
            return std::stoi(numStr);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            return std::stod(numStr);
        }
    }
    catch (const std::invalid_argument &e)
    {
        // Log the error but provide a default value
        std::cerr << "Conversion error for: " << str << " - " << e.what() << std::endl;
        return 0; // or some other appropriate default
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Out of range error for: " << str << " - " << e.what() << std::endl;
        return 0;
    }

    return T();
}
void File::openFile(const std::string &path)
{
    file.open(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }
    std::cout << "Successfully opened file!" << std::endl;
}

void File::parseFile()
{
    if (!file.is_open())
    {
        std::cerr << "No file is open!" << std::endl;
        return;
    }

    std::string line;
    bool isFirstLine = true;

    while (std::getline(file, line))
    {
        // Skip header
        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        try
        {
            auto fields = splitCSVLine(line);

            // Ensure we have enough fields
            if (fields.size() < 10)
            {
                std::cerr << "Insufficient fields in line: " << line << std::endl;
                continue;
            }

            // Careful parsing with error checking
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
                &rbTree);

             std::string bookKey = std::to_string(book.getId()) + "|" + 
                                  book.getTitle() + "|" + 
                                  book.getAuthor();
            // Insert into your data structure
            rbTree.insertValue(bookKey);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing line: " << line << std::endl;
            std::cerr << "Error details: " << e.what() << std::endl;
            // Continue processing other lines
            continue;
        }
    }

    file.close();
}

File::~File()
{
    if (file.is_open())
    {
        file.close();
    }
}