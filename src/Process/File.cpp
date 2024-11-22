#include <string>
#include <fstream>
#include <iostream>
std::ifstream file;


void openFile(std::string path){
    file.open(path);
    if(!file.is_open()){
        std::cout << file.is_open();
        return;
    }
    std::cout << "berhasil Membuka file!";

}