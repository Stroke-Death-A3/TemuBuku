#include <bits/stdc++.h>
#include "../core/RBTree.cpp"
#include "File.cpp"
using namespace std;

int main()
{
    RBTree rbTree1;
    int data;
    int n;
    string  path = "./datasets/test.txt";
    openFile(path);
    cin >> n;
    for (int i = 1; i < n+1; i++)
    {
        std::string data;
        cin >> data;
        rbTree1.insertValue(data);
    }
    cin.get();
    rbTree1.preorder();
    int e;
    cin >> e;
    for (int i = 0; i < e; i++)
    {
        std::string key = "data";
        rbTree1.searchValue(key);
    }
}