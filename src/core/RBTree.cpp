#include <bits/stdc++.h>
#include "RBTree.h"
using namespace std;

Node::Node(string data)
{
    this->data = data;
    color = RED;
    left = right = parent = nullptr;
}
int RBTree::getColor(Node *&node)
{
    if (node == nullptr)
        return BLACK;

    return node->color;
}

void RBTree::setColor(Node *&node, int color)
{
    if (node == nullptr)
        return;

    node->color = color;
}

Node *RBTree::insertBST(Node *&root, Node *&ptr)
{
    if (root == nullptr)
        return ptr;

    if (ptr->data < root->data)
    {
        root->left = insertBST(root->left, ptr);
        root->left->parent = root;
    }
    else if (ptr->data > root->data)
    {
        root->right = insertBST(root->right, ptr);
        root->right->parent = root;
    }

    return root;
}

bool RBTree::areSimiliar(const std::string &str1, const std::string &str2)
{
    // Remove any trailing numbers from str1
    std::string base1 = str1;
    while (!base1.empty() && std::isdigit(base1.back()))
    {
        base1.pop_back();
    }

    // Remove any trailing numbers from str2
    std::string base2 = str2;
    while (!base2.empty() && std::isdigit(base2.back()))
    {
        base2.pop_back();
    }

    return base1 == base2;
}
std::vector<Node *> RBTree::searchBST(Node *root, Node *ptr) 
{
    std::vector<Node *> temp_result;
    if (root == nullptr) {
        return temp_result;
    }

    // Get search term
    std::string searchTerm = ptr->data;
    std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);

    // Get node data
    std::string nodeData = root->data;
    std::transform(nodeData.begin(), nodeData.end(), nodeData.begin(), ::tolower);

    // Split node data into fields
    std::vector<std::string> fields;
    std::stringstream ss(nodeData);
    std::string field;
    while (getline(ss, field, '|')) {
        fields.push_back(field);
    }

    // Search in all fields (ISBN, title, author, publisher)
    bool found = false;
    for (const auto& field : fields) {
        if (field.find(searchTerm) != std::string::npos) {
            found = true;
            break;
        }
    }

    if (found) {
        temp_result.push_back(root);
    }

    // Search both subtrees
    std::vector<Node *> left_result = searchBST(root->left, ptr);
    std::vector<Node *> right_result = searchBST(root->right, ptr);

    temp_result.insert(temp_result.end(), left_result.begin(), left_result.end());
    temp_result.insert(temp_result.end(), right_result.begin(), right_result.end());

    return temp_result;
}

// Add helper function to count nodes
int RBTree::countNodes(Node* node) {
    if (node == nullptr) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

// Add helper function to get nth node
Node* RBTree::getNthNode(Node* node, int& n) {
    if (node == nullptr) return nullptr;
    
    Node* left = getNthNode(node->left, n);
    if (left) return left;
    
    if (--n == 0) return node;
    
    return getNthNode(node->right, n);
}

// Modify searchValue function
std::vector<Node*> RBTree::searchValue(std::string& n) {
    // Check for special commands
    if (n == "calculator" || n == "Calculator") {
        // Create a dummy node to display calculator
        Node* calcNode = new Node("CALCULATOR");
        std::vector<Node*> result;
        result.push_back(calcNode);
        return result;
    } else if (n == "dice" || n == "Dice") {
        // Create a dummy node for dice roller
        Node* diceNode = new Node("DICE");
        std::vector<Node*> result;
        result.push_back(diceNode);
        return result;
    } else if (n == "random") {
        // Existing random book logic
        int totalNodes = countNodes(root);
        if (totalNodes == 0) {
            return std::vector<Node*>();
        }
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, totalNodes);
        int randomIndex = distrib(gen);
        
        Node* randomNode = getNthNode(root, randomIndex);
        if (randomNode) {
            std::vector<Node*> result;
            result.push_back(randomNode);
            return result;
        }
        return std::vector<Node*>();
    }
    
    // Regular search logic
    Node* node = new Node(n);
    std::vector<Node*> results = searchBST(root, node);
    delete node;
    return results;
}

void RBTree::insertValue(string n)
{
    Node *node = new Node(n);
    root = insertBST(root, node);
    fixInsertRBTree(node);
}

void RBTree::rotateLeft(Node *&ptr)
{
    Node *right_child = ptr->right;
    ptr->right = right_child->left;

    if (ptr->right != nullptr)
        ptr->right->parent = ptr;

    right_child->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = right_child;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = right_child;
    else
        ptr->parent->right = right_child;

    right_child->left = ptr;
    ptr->parent = right_child;
}

void RBTree::rotateRight(Node *&ptr)
{
    Node *left_child = ptr->left;
    ptr->left = left_child->right;

    if (ptr->left != nullptr)
        ptr->left->parent = ptr;

    left_child->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = left_child;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = left_child;
    else
        ptr->parent->right = left_child;

    left_child->right = ptr;
    ptr->parent = left_child;
}

void RBTree::fixInsertRBTree(Node *&ptr)
{
    Node *parent = nullptr;
    Node *grandparent = nullptr;
    while (ptr != root && getColor(ptr) == RED && getColor(ptr->parent) == RED)
    {
        parent = ptr->parent;
        grandparent = parent->parent;
        if (parent == grandparent->left)
        {
            Node *uncle = grandparent->right;
            if (getColor(uncle) == RED)
            {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                ptr = grandparent;
            }
            else
            {
                if (ptr == parent->right)
                {
                    rotateLeft(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                rotateRight(grandparent);
                swap(parent->color, grandparent->color);
                ptr = parent;
            }
        }
        else
        {
            Node *uncle = grandparent->left;
            if (getColor(uncle) == RED)
            {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                ptr = grandparent;
            }
            else
            {
                if (ptr == parent->left)
                {
                    rotateRight(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                rotateLeft(grandparent);
                swap(parent->color, grandparent->color);
                ptr = parent;
            }
        }
    }
    setColor(root, BLACK);
}

void RBTree::fixDeleteRBTree(Node *&node)
{
    if (node == nullptr)
        return;

    if (node == root)
    {
        root = nullptr;
        return;
    }

    if (getColor(node) == RED || getColor(node->left) == RED || getColor(node->right) == RED)
    {
        Node *child = node->left != nullptr ? node->left : node->right;

        if (node == node->parent->left)
        {
            node->parent->left = child;
            if (child != nullptr)
                child->parent = node->parent;
            setColor(child, BLACK);
            delete (node);
        }
        else
        {
            node->parent->right = child;
            if (child != nullptr)
                child->parent = node->parent;
            setColor(child, BLACK);
            delete (node);
        }
    }
    else
    {
        Node *sibling = nullptr;
        Node *parent = nullptr;
        Node *ptr = node;
        setColor(ptr, DOUBLE_BLACK);
        while (ptr != root && getColor(ptr) == DOUBLE_BLACK)
        {
            parent = ptr->parent;
            if (ptr == parent->left)
            {
                sibling = parent->right;
                if (getColor(sibling) == RED)
                {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateLeft(parent);
                }
                else
                {
                    if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK)
                    {
                        setColor(sibling, RED);
                        if (parent != nullptr && getColor(parent) == RED)
                            setColor(parent, BLACK);
                        else if (parent != nullptr)
                            setColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    }
                    else
                    {
                        if (getColor(sibling->right) == BLACK)
                        {
                            setColor(sibling->left, BLACK);
                            setColor(sibling, RED);
                            rotateRight(sibling);
                            sibling = parent->right;
                        }
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->right, BLACK);
                        rotateLeft(parent);
                        break;
                    }
                }
            }
            else
            {
                sibling = parent->left;
                if (getColor(sibling) == RED)
                {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateRight(parent);
                }
                else
                {
                    if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK)
                    {
                        setColor(sibling, RED);
                        if (getColor(parent) == RED)
                            setColor(parent, BLACK);
                        else
                            setColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    }
                    else
                    {
                        if (getColor(sibling->left) == BLACK)
                        {
                            setColor(sibling->right, BLACK);
                            setColor(sibling, RED);
                            rotateLeft(sibling);
                            sibling = parent->left;
                        }
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->left, BLACK);
                        rotateRight(parent);
                        break;
                    }
                }
            }
        }
        if (node == node->parent->left)
            node->parent->left = nullptr;
        else
            node->parent->right = nullptr;
        delete (node);
        setColor(root, BLACK);
    }
}

Node *RBTree::deleteBST(Node *&root, std::string data)
{
    if (root == nullptr)
    {
        return root;
    }

    if (data < root->data)
    {
        return deleteBST(root->left, data);
    }

    if (data > root->data)
    {
        return deleteBST(root->right, data);
    }

    if (root->left == nullptr || root->right == nullptr)
    {
        Node *temp = root->left ? root->left : root->right;
        delete root;
        return temp;
    }

    Node *temp = minValueNode(root->right);
    root->data = temp->data;
    root->right = deleteBST(root->right, temp->data);
    return root;
}

void RBTree::deleteValue(string data)
{
    Node *node = deleteBST(root, data);
    fixDeleteRBTree(node);
}

void RBTree::inorderBST(Node *&ptr)
{
    if (ptr == nullptr)
        return;

    inorderBST(ptr->left);
    cout << ptr->data << " " << ptr->color << endl;
    inorderBST(ptr->right);
}

void RBTree::inorder()
{
    inorderBST(root);
}

void RBTree::preorderBST(Node *&ptr)
{
    if (ptr == nullptr)
        return;

    cout << ptr->data << " " << ptr->color << endl;
    preorderBST(ptr->left);
    preorderBST(ptr->right);
}

void RBTree::preorder()
{
    preorderBST(root);
    cout << "-------" << endl;
}

Node *RBTree::minValueNode(Node *&node)
{

    Node *ptr = node;

    while (ptr->left != nullptr)
        ptr = ptr->left;

    return ptr;
}

Node *RBTree::maxValueNode(Node *&node)
{
    Node *ptr = node;

    while (ptr->right != nullptr)
        ptr = ptr->right;

    return ptr;
}

int RBTree::getBlackHeight(Node *node)
{
    int blackheight = 0;
    while (node != nullptr)
    {
        if (getColor(node) == BLACK)
            blackheight++;
        node = node->left;
    }
    return blackheight;
}

// Test case 1 : 5 2 9 1 6 8 0 20 30 35 40 50 0
// Test case 2 : 3 0 5 0
// Test case 3 : 2 1 3 0 8 9 4 5 0

// Method to add a book to favorites
void RBTree::addToFavorites(const std::string &bookIdentifier)
{
    // Search for the book in the tree

    Node *node = new Node(bookIdentifier);
    Node *selectedBook = nullptr;

    std::vector<Node *> searchtemp_results = searchBST(root, node);
    delete node;
    // If multiple matches, let user choose
    if (searchtemp_results.size() > 1)
    {
        std::cout << "Multiple books found. Please select:" << std::endl;
        for (int i = 1; i < searchtemp_results.size(); i++)
        {
            std::cout << i << ". " << searchtemp_results[i]->data << std::endl;
        }

        int choice;
        std::cout << "Enter the number of the book to add to favorites: ";
        std::cin >> choice;

        if (choice > 0 && choice <= searchtemp_results.size())
        {
            selectedBook = searchtemp_results[choice - 1];
        }
        else
        {
            std::cout << "Invalid choice." << std::endl;
            return;
        }
    }
    else if (searchtemp_results.size() == 1)
    {
        selectedBook = searchtemp_results[0];
    }
    else
    {
        std::cout << "No book found with the given identifier." << std::endl;
        return;
    }

    // Add the selected book to favorites
    if (selectedBook)
    {
        auto it = std::find(favoriteList.begin(), favoriteList.end(), selectedBook);
        if (it == favoriteList.end())
        {
            selectedBook->isFavorite = true;
            favoriteList.push_back(selectedBook);
            std::cout << "Added to favorites: " << selectedBook->title << std::endl;
        }
        else
        {
            std::cout << "Book is already in favorites." << std::endl;
        }
    }
}

// Method to remove a book from favorites
void RBTree::removeFromFavorites(const std::string &bookIdentifier)
{
    auto it = std::find_if(favoriteList.begin(), favoriteList.end(),
                           [&bookIdentifier](Node *book)
                           { return book->data.find(bookIdentifier) != std::string::npos; });
    if (it != favoriteList.end())
    {
        (*it)->isFavorite = false;
        favoriteList.erase(it);
        std::cout << "Removed from favorites: " << (*it)->data << std::endl;
    }
    else
    {
        std::cout << "Book not found in favorites." << std::endl;
    }
}

// Method to display favorite books
void RBTree::displayFavorites()
{
    if (favoriteList.empty())
    {
        std::cout << "No favorite books." << std::endl;
    }
    else
    {
        std::cout << "Favorite Books:" << std::endl;
        for (const auto &book : favoriteList)
        {
            std::cout << book->title << " by " << book->data << std::endl;
        }
    }
}

void RBTree::merge(RBTree rbTree2)
{
    string temp;
    Node *c, *temp_ptr;
    Node *root1 = root;
    Node *root2 = rbTree2.root;
    int initialblackheight1 = getBlackHeight(root1);
    int initialblackheight2 = getBlackHeight(root2);
    if (initialblackheight1 > initialblackheight2)
    {
        c = maxValueNode(root1);
        temp = c->data;
        deleteValue(c->data);
        root1 = root;
    }
    else if (initialblackheight2 > initialblackheight1)
    {
        c = minValueNode(root2);
        temp = c->data;
        rbTree2.deleteValue(c->data);
        root2 = rbTree2.root;
    }
    else
    {
        c = minValueNode(root2);
        temp = c->data;
        rbTree2.deleteValue(c->data);
        root2 = rbTree2.root;
        if (initialblackheight1 != getBlackHeight(root2))
        {
            rbTree2.insertValue(c->data);
            root2 = rbTree2.root;
            c = maxValueNode(root1);
            temp = c->data;
            deleteValue(c->data);
            root1 = root;
        }
    }
    setColor(c, RED);
    int finalblackheight1 = getBlackHeight(root1);
    int finalblackheight2 = getBlackHeight(root2);
    if (finalblackheight1 == finalblackheight2)
    {
        c->left = root1;
        root1->parent = c;
        c->right = root2;
        root2->parent = c;
        setColor(c, BLACK);
        c->data = temp;
        root = c;
    }
    else if (finalblackheight2 > finalblackheight1)
    {
        Node *ptr = root2;
        while (finalblackheight1 != getBlackHeight(ptr))
        {
            temp_ptr = ptr;
            ptr = ptr->left;
        }
        Node *ptr_parent;
        if (ptr == nullptr)
            ptr_parent = temp_ptr;
        else
            ptr_parent = ptr->parent;
        c->left = root1;
        if (root1 != nullptr)
            root1->parent = c;
        c->right = ptr;
        if (ptr != nullptr)
            ptr->parent = c;
        ptr_parent->left = c;
        c->parent = ptr_parent;
        if (getColor(ptr_parent) == RED)
        {
            fixInsertRBTree(c);
        }
        else if (getColor(ptr) == RED)
        {
            fixInsertRBTree(ptr);
        }
        c->data = temp;
        root = root2;
    }
    else
    {
        Node *ptr = root1;
        while (finalblackheight2 != getBlackHeight(ptr))
        {
            ptr = ptr->right;
        }
        Node *ptr_parent = ptr->parent;
        c->right = root2;
        root2->parent = c;
        c->left = ptr;
        ptr->parent = c;
        ptr_parent->right = c;
        c->parent = ptr_parent;
        if (getColor(ptr_parent) == RED)
        {
            fixInsertRBTree(c);
        }
        else if (getColor(ptr) == RED)
        {
            fixInsertRBTree(ptr);
        }
        c->data = temp;
        root = root1;
    }
    return;
}