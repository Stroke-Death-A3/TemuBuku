
#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H

enum Color {BLACK, RED, DOUBLE_BLACK};

struct Node
{
    std::string data;
    int color;
    Node *left, *right, *parent;

    explicit Node(std::string);
};

class RBTree
{
    private:
        Node *root;
    protected:
        void rotateLeft(Node *&);
        void rotateRight(Node *&);
        void fixInsertRBTree(Node *&);
        void fixDeleteRBTree(Node *&);
        void inorderBST(Node *&);
        void preorderBST(Node *&);
        int getColor(Node *&);
        void setColor(Node *&, int);
        Node *minValueNode(Node *&);
        Node *maxValueNode(Node *&);
        Node* insertBST(Node *&, Node *&);
        Node *deleteBST(Node *&, std::string);
        Node* searchBST(Node *&,Node *&);
        int getBlackHeight(Node *);
    public:
        RBTree();
        void insertValue(std::string);
        void searchValue(std::string);
        void deleteValue(std::string);
        void merge(RBTree);
        void inorder();
        void preorder();
};


#endif //RED_BLACK_TREE_RBTREE_H