#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H

#include <string>
#include <vector>

enum Color { BLACK, RED, DOUBLE_BLACK };

struct Node {
    std::string data;
    int color;
    Node *left, *right, *parent;
    explicit Node(std::string value);
};

class RBTree {
private:
    Node* root;

protected:
    void rotateLeft(Node*&);
    void rotateRight(Node*&);
    void fixInsertRBTree(Node*&);
    void fixDeleteRBTree(Node*&);
    void inorderBST(Node*&);
    void preorderBST(Node*&);
    int getColor(Node*&);
    void setColor(Node*&, int);
    Node* minValueNode(Node*&);
    Node* maxValueNode(Node*&);
    Node* insertBST(Node*&, Node*&);
    std::vector<Node*> searchBST(Node* root, Node* ptr);
    Node* deleteBST(Node*&, std::string);
    int getBlackHeight(Node*);

public:
    RBTree();
    bool areSimilar(const std::string& str1, const std::string& str2);
    void insertValue(std::string);
    void searchValue(std::string);
    void deleteValue(std::string);
    void merge(RBTree);
    void inorder();
    void preorder();
};

#endif // RED_BLACK_TREE_RBTREE_H