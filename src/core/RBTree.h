#ifndef RBTREE_H
#define RBTREE_H

#include <string>
#include <vector>
#include <random>

// Forward declaration

enum Color { BLACK, RED, DOUBLE_BLACK };

class Node {
public:
    std::string title;
    std::string author;
    std::string data;
    bool isFavorite;
    bool isGimmick;
    std::string gimmickType;
    Node* left;
    Node* right;
    Node* parent;
    int color;
    explicit Node(std::string value);
    
    // Replace both constructors with single declaration
    Node(std::string value, bool isGimmick = false);
    
    bool isCalculator() const { return gimmickType == "CALCULATOR"; }
    bool isDice() const { return gimmickType == "DICE"; }
};

class RBTree {
private:
    Node* root;
    std::vector<Node*> searchBST(Node* root, Node* key);

public:
    RBTree() : root(nullptr) {}
    std::vector<Node*> searchValue(std::string& n);
    void insertValue(std::string n);
    void addToFavorites(const std::string& bookIdentifier);
    void removeFromFavorites(const std::string& bookIdentifier);
    void searchValue(const std::string& value);
    void displayFavorites();
    void deleteValue(std::string);
    void merge(RBTree);
    void inorder();
    void preorder();
    bool isGimmickNode(Node* node);
    std::vector<Node*> favoriteList;

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
    int countNodes(Node*);
    Node* getNthNode(Node*, int&n);
    Node* minValueNode(Node*&);
    Node* maxValueNode(Node*&);
    Node* insertBST(Node*&, Node*&);
    Node* deleteBST(Node*&, std::string);
    int getBlackHeight(Node*);
};

#endif // RBTREE_H