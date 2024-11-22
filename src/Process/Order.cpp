#include <bits/stdc++.h>
#include "../core/RBTree.cpp"
using namespace std;

int main()
{
    int data;
    RBTree rbTree1;
    int n;
    cin >> n;
    for (int i = 1; i < n; i++)
    {
        data = i;
        rbTree1.insertValue(data);
    }
    cin.get();
    rbTree1.preorder();
    int e;
    cin >> e;
    for (int i = 0; i < e; i++)
    {
        int key = rand() % 1000;
        rbTree1.searchValue(key);
    }
}