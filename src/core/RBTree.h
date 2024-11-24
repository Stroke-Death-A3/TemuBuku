
#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H

enum Color {BLACK, RED, DOUBLE_BLACK};

struct Book {
    int id;
    std::string title;

    Book() : id(0) {}
};

struct Node
{
    int data;
    int color;
    Node *left, *right, *parent;

    explicit Node(int);
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
        Node *deleteBST(Node *&, int);
        Node* searchBST(Node *&,Node *&);
        int getBlackHeight(Node *);
    public:
        RBTree();
        void insertValue(int);
        void searchValue(int);
        void deleteValue(int);
        void merge(RBTree);
        void inorder();
        void preorder();
};


#endif //RED_BLACK_TREE_RBTREE_H