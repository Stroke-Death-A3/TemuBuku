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
    int key = 6;
    rbTree1.preorder();
    rbTree1.searchValue(6);
}