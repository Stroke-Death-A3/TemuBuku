#include "RBTree.h"
using namespace std;

// Remove duplicate constructor implementation
Node::Node(string value, bool isGimmick)
{
    this->data = value;
    this->isGimmick = isGimmick;
    color = RED;
    left = right = parent = nullptr;
    
    if (isGimmick) {
        if (value == "CALCULATOR" || value == "calculator") {
            gimmickType = "CALCULATOR";
        } else if (value == "DICE" || value == "dice") {
            gimmickType = "DICE";
        }
    }
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
    if (!root) return temp_result;

    // Cache the lowercase search term
    static std::string searchTermLower;
    searchTermLower = ptr->data;
    std::transform(searchTermLower.begin(), searchTermLower.end(), searchTermLower.begin(), ::tolower);

    // Process current node
    std::string nodeData = root->data;
    std::transform(nodeData.begin(), nodeData.end(), nodeData.begin(), ::tolower);
    
    // Use string_view for more efficient string operations
    std::string_view nodeView(nodeData);
    if (nodeView.find(searchTermLower) != std::string_view::npos) {
        temp_result.push_back(root);
    }

    // Search subtrees and combine results
    auto leftResults = searchBST(root->left, ptr);
    auto rightResults = searchBST(root->right, ptr);
    
    temp_result.reserve(temp_result.size() + leftResults.size() + rightResults.size());
    temp_result.insert(temp_result.end(), leftResults.begin(), leftResults.end());
    temp_result.insert(temp_result.end(), rightResults.begin(), rightResults.end());

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
    std::vector<Node*> result;
    
    // Check for special commands
    if (n == "calculator" || n == "Calculator") {
        result.push_back(new Node("CALCULATOR", true));
        return result;
    } else if (n == "dice" || n == "Dice") {
        result.push_back(new Node("DICE", true));
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
    

    Node* node = new Node(n, false);
    std::vector<Node*> results = searchBST(root, node);
    delete node;
    return results;
}

// Add helper method to check for gimmick nodes
bool RBTree::isGimmickNode(Node* node) {
    return node && node->isGimmick;
}

void RBTree::insertValue(string n)
{
    Node *node = new Node(n, false);
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
    Node searchNode(bookIdentifier, false);
    auto results = searchBST(root, &searchNode);
    
    if (results.empty()) {
        std::cout << "No book found with the given identifier.\n";
        return;
    }

    Node *selectedBook = results[0];
    if (results.size() > 1) {
        std::cout << "Multiple matches found. Select book:\n";
        for (size_t i = 0; i < results.size(); ++i) {
            std::cout << i + 1 << ". " << results[i]->data << '\n';
        }

        size_t choice;
        std::cout << "Enter choice (1-" << results.size() << "): ";
        std::cin >> choice;
        
        if (choice < 1 || choice > results.size()) {
            std::cout << "Invalid choice.\n";
            return;
        }
        selectedBook = results[choice - 1];
    }

    if (!selectedBook->isFavorite) {
        selectedBook->isFavorite = true;
        favoriteList.push_back(selectedBook);
        std::cout << "Added to favorites: " << selectedBook->data << '\n';
    } else {
        std::cout << "Already in favorites.\n";
    }
}

void RBTree::removeFromFavorites(const std::string &bookIdentifier)
{
    auto it = std::find_if(favoriteList.begin(), favoriteList.end(),
        [&bookIdentifier](const Node* book) {
            return book->data.find(bookIdentifier) != std::string::npos;
        });
        
    if (it != favoriteList.end()) {
        (*it)->isFavorite = false;
        favoriteList.erase(it);
        std::cout << "Removed from favorites\n";
    } else {
        std::cout << "Not found in favorites\n";
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

// Destructor to handle memory cleanup
RBTree::~RBTree() {
    deleteTree(root);
}

void RBTree::deleteTree(Node* node) {
    if (node) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}