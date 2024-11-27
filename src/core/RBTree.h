#ifndef RBTREE_H
#define RBTREE_H

#include <string>
#include <vector>

enum Color { BLACK, RED, DOUBLE_BLACK };

class Node {
public:
    std::string title;
    std::string author;
    bool isFavorite;
    std::string data;
    int color;
    Node *left, *right, *parent;
    explicit Node(std::string value);
};

class RBTree {
public:
    RBTree();
    std::vector<Node*> searchBST(Node* root, Node* key);
    void addToFavorites(const std::string& bookIdentifier);
    void removeFromFavorites(const std::string& bookIdentifier);
    void searchValue(std::string& n);
    void displayFavorites();
    void insertValue(std::string);
    void deleteValue(std::string);
    void merge(RBTree);
    void inorder();
    void preorder();
    std::vector<Node*> favoriteList;

private:
    Node* root;

protected:
    void rotateLeft(Node*&);
    bool areSimiliar(const std::string&, const std::string&);
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
    Node* deleteBST(Node*&, std::string);
    int getBlackHeight(Node*);
};

#endif // RBTREE_H