#include <bits/stdc++.h>
#include "../core/RBTree.cpp"
#include "File.cpp"
#include <iostream>
using namespace std;

int main()
{
    RBTree rbTree1;
    int data;
    int n;
    std::string  path = "./datasets/test.txt";
    openFile(path);
    cin >> n ;
    cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
    for (int i = 1; i < n+1; i++)
    {
        std::string data;
        std::getline(cin,data);
        rbTree1.insertValue(data);
    }
    cin.clear();
    cin.get();
    rbTree1.preorder();
    int e;
    e = 1;
    for (int i = 0; i < e; i++)
    {
        string key = "data";
        rbTree1.searchValue(key);
    }
}