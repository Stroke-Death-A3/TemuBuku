#include "../core/File.h"
#include "../core/Book.h"
#include <algorithm>
#include <limits>
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
        else if ((ch == ';') && !inQuotes)
        {
            // Remove surrounding quotes if present
            if (!currentField.empty() && currentField.front() == '"' && currentField.back() == '"') {
                currentField = currentField.substr(1, currentField.length() - 2);
            }
            result.push_back(currentField);
            currentField.clear();
        }
        else
        {
            currentField += ch;
        }
    }

    // Don't forget to add the last field
    if (!currentField.empty())
    {
        if (currentField.front() == '"' && currentField.back() == '"') {
            currentField = currentField.substr(1, currentField.length() - 2);
        }
        result.push_back(currentField);
    }

    return result;
}

template <typename T>
T File::stringToType(const std::string &str)
{
    std::string numStr = str;

    // Remove quotes if present
    if (numStr.front() == '"' && numStr.back() == '"') {
        numStr = numStr.substr(1, numStr.length() - 2);
    }

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
            // Try converting to long long first
            long long longValue = std::stoll(numStr);
            
            // If the value is too large for int, return 0 or use a default strategy
            if (longValue > std::numeric_limits<int>::max() || 
                longValue < std::numeric_limits<int>::min())
            {
                std::cerr << "Number too large for int: " << longValue 
                          << ". Using last 4 digits." << std::endl;
                
                // Strategy: Use last 4 digits
                return static_cast<int>(longValue % 10000);
            }
            
            return static_cast<int>(longValue);
        }
        else if constexpr (std::is_same_v<T, long long>)
        {
            return std::stoll(numStr);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            return std::stod(numStr);
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Conversion error for: " << str << " - " << e.what() << std::endl;
        return 0;
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
            if (fields.size() < 8)  // Changed from 10 to 8
            {
                std::cerr << "Insufficient fields in line: " << line << std::endl;
                continue;
            }

            // Careful parsing with error checking
            Book book(
                stringToType<int>(fields[0]),    // id
                fields[1],                       // title
                fields[2],                       // author
                fields[3],                       // category/year
                fields[4],                       // publisher
                "",                             // format (empty)
                0.0,                            // default rating
                0.0,                            // default reviews
                fields[5],                      // url (first URL)
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