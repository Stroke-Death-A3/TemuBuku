#include <bits/stdc++.h>
#include "../core/RBTree.cpp"
using namespace std;

int main() {
    int data;
    RBTree rbTree1;

    cin >> data;
    while (data != 0)
    {
        rbTree1.insertValue(data);
        cin >> data;
    }

    rbTree1.preorder();
    return 0;
}