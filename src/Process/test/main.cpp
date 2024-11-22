#include <fstream>
#include <iostream>
#include <string>
 
int main()
{
    std::string filename = "~/datasets/test.txt";
 
    std::fstream fs;
    fs.open(filename);
 
    if (!fs.is_open())
    {
        fs.clear();
        fs.close();
        fs.open(filename);
    }
 
    std::cout << std::boolalpha;
    std::cout << "fs.is_open() = " << fs.is_open() << '\n';
    std::cout << "fs.good() = " << fs.good() << '\n';
}

