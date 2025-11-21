#include "avl_tree.h"
#include <queue>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "../models/case.h"
// Constructor
template <typename T>
AVLTree<T>::AVLTree() : root(nullptr) {}

// Destructor
template <typename T>
AVLTree<T>::~AVLTree() {
    clear();
}

// Get height of node
template <typename T>
int AVLTree<T>::getHeight(AVLNode<T>* node) {
    return node ? node->height : 0;
}

// Get balance factor
template <typename T>
int AVLTree<T>::getBalance(AVLNode<T>* node) {
    if (!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Right rotation
template <typename T>
AVLNode<T>* AVLTree<T>::rightRotate(AVLNode<T>* y) {
    AVLNode<T>* x = y->left;
    AVLNode<T>* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Left rotation
template <typename T>
AVLNode<T>* AVLTree<T>::leftRotate(AVLNode<T>* x) {
    AVLNode<T>* y = x->right;
    AVLNode<T>* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Balance node
template <typename T>
AVLNode<T>* AVLTree<T>::balanceNode(AVLNode<T>* node) {
    if (!node) return node;

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0)
        return rightRotate(node);

    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0)
        return leftRotate(node);

    // Right Left Case
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Insert node
template <typename T>
AVLNode<T>* AVLTree<T>::insertNode(AVLNode<T>* node, T value) {
    if (!node) return new AVLNode<T>(value);

    if (value < node->data)
        node->left = insertNode(node->left, value);
    else if (value > node->data)
        node->right = insertNode(node->right, value);
    else
        return node; // Duplicate values not allowed

    return balanceNode(node);
}

// Public insert
template <typename T>
void AVLTree<T>::insert(T value) {
    root = insertNode(root, value);
}

// Find minimum value node
template <typename T>
AVLNode<T>* AVLTree<T>::minValueNode(AVLNode<T>* node) {
    AVLNode<T>* current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

// Delete node
template <typename T>
AVLNode<T>* AVLTree<T>::deleteNode(AVLNode<T>* node, T value) {
    if (!node) return node;

    if (value < node->data)
        node->left = deleteNode(node->left, value);
    else if (value > node->data)
        node->right = deleteNode(node->right, value);
    else {
        if (!node->left || !node->right) {
            AVLNode<T>* temp = node->left ? node->left : node->right;
            if (!temp) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            AVLNode<T>* temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data);
        }
    }

    if (!node) return node;
    return balanceNode(node);
}

// Public remove
template <typename T>
void AVLTree<T>::remove(T value) {
    root = deleteNode(root, value);
}

// Search
template <typename T>
T* AVLTree<T>::search(T value) {
    AVLNode<T>* current = root;
    while (current) {
        if (value == current->data)
            return &current->data;
        else if (value < current->data)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

// Const search
template <typename T>
const T* AVLTree<T>::search(T value) const {
    const AVLNode<T>* current = root;
    while (current) {
        if (value == current->data)
            return &current->data;
        else if (value < current->data)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

// Check if value exists
template <typename T>
bool AVLTree<T>::contains(T value) {
    return search(value) != nullptr;
}

// Clear tree
template <typename T>
void AVLTree<T>::clear() {
    clearTree(root);
    root = nullptr;
}

template <typename T>
void AVLTree<T>::clearTree(AVLNode<T>* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

// In-order traversal
template <typename T>
void AVLTree<T>::inOrderNodes(AVLNode<T>* node, std::function<void(T*)> fn) {
    if (!node) return;
    inOrderNodes(node->left, fn);
    fn(&node->data);
    inOrderNodes(node->right, fn);
}

template <typename T>
void AVLTree<T>::inOrderTraversal(std::function<void(T*)> fn) {
    inOrderNodes(root, fn);
}

// Const in-order traversal
template <typename T>
void AVLTree<T>::inOrderNodes(const AVLNode<T>* node, std::function<void(const T*)> fn) const {
    if (!node) return;
    inOrderNodes(node->left, fn);
    fn(&node->data);
    inOrderNodes(node->right, fn);
}

template <typename T>
void AVLTree<T>::inOrderTraversal(std::function<void(const T*)> fn) const {
    inOrderNodes(root, fn);
}

// Pre-order traversal
template <typename T>
void AVLTree<T>::preOrderNodes(AVLNode<T>* node, std::function<void(T*)> fn) {
    if (!node) return;
    fn(&node->data);
    preOrderNodes(node->left, fn);
    preOrderNodes(node->right, fn);
}

template <typename T>
void AVLTree<T>::preOrderTraversal(std::function<void(T*)> fn) {
    preOrderNodes(root, fn);
}

// Post-order traversal
template <typename T>
void AVLTree<T>::postOrderNodes(AVLNode<T>* node, std::function<void(T*)> fn) {
    if (!node) return;
    postOrderNodes(node->left, fn);
    postOrderNodes(node->right, fn);
    fn(&node->data);
}

template <typename T>
void AVLTree<T>::postOrderTraversal(std::function<void(T*)> fn) {
    postOrderNodes(root, fn);
}

// Level-order traversal
template <typename T>
void AVLTree<T>::levelOrderTraversal(std::function<void(T*)> fn) {
    if (!root) return;
    
    std::queue<AVLNode<T>*> q;
    q.push(root);
    
    while (!q.empty()) {
        AVLNode<T>* current = q.front();
        q.pop();
        
        fn(&current->data);
        
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
}

// Get tree height
template <typename T>
int AVLTree<T>::getHeight() {
    return getHeight(root);
}

// Get tree size
template <typename T>
int AVLTree<T>::getSize() const {
    return countNodes(root);
}

template <typename T>
int AVLTree<T>::countNodes(AVLNode<T>* node) const {
    if (!node) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

// Check if empty
template <typename T>
bool AVLTree<T>::isEmpty() {
    return root == nullptr;
}

// Get minimum value
template <typename T>
T* AVLTree<T>::getMin() {
    AVLNode<T>* minNode = minValueNode(root);
    return minNode ? &minNode->data : nullptr;
}

// Get maximum value
template <typename T>
T* AVLTree<T>::getMax() {
    if (!root) return nullptr;
    AVLNode<T>* current = root;
    while (current->right)
        current = current->right;
    return &current->data;
}

// Search by criteria
template <typename T>
T* AVLTree<T>::searchByCriteria(std::function<bool(const T&)> criteria) {
    T* result = nullptr;
    inOrderTraversal([&](T* data) {
        if (!result && criteria(*data)) {
            result = data;
        }
    });
    return result;
}

// Convert to vector
template <typename T>
std::vector<T> AVLTree<T>::toVector() {
    std::vector<T> result;
    collectNodes(root, result);
    return result;
}

template <typename T>
void AVLTree<T>::collectNodes(AVLNode<T>* node, std::vector<T>& collection) {
    if (!node) return;
    collectNodes(node->left, collection);
    collection.push_back(node->data);
    collectNodes(node->right, collection);
}

// Filter tree
template <typename T>
std::vector<T> AVLTree<T>::filter(std::function<bool(const T&)> criteria) {
    std::vector<T> result;
    inOrderTraversal([&](T* data) {
        if (criteria(*data)) {
            result.push_back(*data);
        }
    });
    return result;
}

// Check if tree is balanced
template <typename T>
bool AVLTree<T>::isBalanced() {
    return isBalanced(root);
}

template <typename T>
bool AVLTree<T>::isBalanced(AVLNode<T>* node) {
    if (!node) return true;
    
    int balance = getBalance(node);
    return std::abs(balance) <= 1 && 
           isBalanced(node->left) && 
           isBalanced(node->right);
}

// Check if tree is complete
template <typename T>
bool AVLTree<T>::isComplete() {
    if (!root) return true;
    
    int nodeCount = countNodes(root);
    return isComplete(root, 0, nodeCount);
}

template <typename T>
bool AVLTree<T>::isComplete(AVLNode<T>* node, int index, int nodeCount) {
    if (!node) return true;
    
    if (index >= nodeCount) return false;
    
    return isComplete(node->left, 2 * index + 1, nodeCount) &&
           isComplete(node->right, 2 * index + 2, nodeCount);
}

// Clone tree
template <typename T>
AVLTree<T> AVLTree<T>::clone() {
    AVLTree<T> newTree;
    inOrderTraversal([&](T* data) {
        newTree.insert(*data);
    });
    return newTree;
}

// Merge with another tree
template <typename T>
void AVLTree<T>::merge(const AVLTree<T>& other) {
    other.inOrderTraversal([&](const T* data) {
        this->insert(*data);
    });
}

// Print tree (visual representation)
template <typename T>
void AVLTree<T>::printTree() {
    printTreeHelper(root, "", true);
}

template <typename T>
void AVLTree<T>::printTreeHelper(AVLNode<T>* node, std::string indent, bool last) {
    if (!node) return;
    
    std::cout << indent;
    if (last) {
        std::cout << "└── ";
        indent += "    ";
    } else {
        std::cout << "├── ";
        indent += "│   ";
    }
    std::cout << node->data << " (h:" << node->height << ")" << std::endl;
    
    printTreeHelper(node->left, indent, false);
    printTreeHelper(node->right, indent, true);
}

// Display tree statistics
template <typename T>
void AVLTree<T>::displayStats() {
    std::cout << "AVL Tree Statistics:\n";
    std::cout << "Height: " << getHeight() << "\n";
    std::cout << "Size: " << getSize() << "\n";
    std::cout << "Balanced: " << (isBalanced() ? "Yes" : "No") << "\n";
    std::cout << "Complete: " << (isComplete() ? "Yes" : "No") << "\n";
    std::cout << "Empty: " << (isEmpty() ? "Yes" : "No") << "\n";
    
    T* minVal = getMin();
    T* maxVal = getMax();
    if (minVal) std::cout << "Min Value: " << *minVal << "\n";
    else std::cout << "Min Value: None\n";
    if (maxVal) std::cout << "Max Value: " << *maxVal << "\n";
    else std::cout << "Max Value: None\n";
}

// Main function to test AVL Tree
/*int main() {
    std::cout << "=== Testing AVL Tree ===\n\n";
    
    AVLTree<int> tree;
    
    // Test insertion
    std::cout << "Inserting values: 10, 20, 30, 40, 50, 25\n";
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);
    
    // Display tree
    std::cout << "\nTree structure:\n";
    tree.printTree();
    
    // Display statistics
    std::cout << "\n";
    tree.displayStats();
    
    // Test traversals
    std::cout << "\nIn-order traversal: ";
    tree.inOrderTraversal([](const int* value) {
        std::cout << *value << " ";
    });
    std::cout << "\n";
    
    std::cout << "Pre-order traversal: ";
    tree.preOrderTraversal([](const int* value) {
        std::cout << *value << " ";
    });
    std::cout << "\n";
    
    std::cout << "Level-order traversal: ";
    tree.levelOrderTraversal([](const int* value) {
        std::cout << *value << " ";
    });
    std::cout << "\n";
    
    // Test search
    std::cout << "\nSearch operations:\n";
    int searchValue = 30;
    const int* found = tree.search(searchValue);
    if (found) {
        std::cout << "Found value: " << *found << "\n";
    } else {
        std::cout << "Value " << searchValue << " not found\n";
    }
    
    searchValue = 99;
    found = tree.search(searchValue);
    if (found) {
        std::cout << "Found value: " << *found << "\n";
    } else {
        std::cout << "Value " << searchValue << " not found\n";
    }
    
    // Test contains
    std::cout << "\nContains 25: " << (tree.contains(25) ? "Yes" : "No") << "\n";
    std::cout << "Contains 99: " << (tree.contains(99) ? "Yes" : "No") << "\n";
    
    // Test min/max
    int* minVal = tree.getMin();
    int* maxVal = tree.getMax();
    if (minVal) std::cout << "Minimum value: " << *minVal << "\n";
    if (maxVal) std::cout << "Maximum value: " << *maxVal << "\n";
    
    // Test toVector
    std::vector<int> vec = tree.toVector();
    std::cout << "\nTree as vector: ";
    for (int val : vec) {
        std::cout << val << " ";
    }
    std::cout << "\n";
    
    // Test filter
    std::cout << "\nValues greater than 25: ";
    std::vector<int> filtered = tree.filter([](const int& val) {
        return val > 25;
    });
    for (int val : filtered) {
        std::cout << val << " ";
    }
    std::cout << "\n";
    
    // Test search by criteria
    std::cout << "\nSearch for first even number greater than 20: ";
    int* result = tree.searchByCriteria([](const int& val) {
        return val > 20 && val % 2 == 0;
    });
    if (result) {
        std::cout << *result << "\n";
    } else {
        std::cout << "Not found\n";
    }
    
    // Test clone
    std::cout << "\nCloning tree...\n";
    AVLTree<int> clonedTree = tree.clone();
    std::cout << "Cloned tree in-order: ";
    clonedTree.inOrderTraversal([](const int* value) {
        std::cout << *value << " ";
    });
    std::cout << "\n";
    
    // Test deletion
    std::cout << "\nDeleting value 30\n";
    tree.remove(30);
    std::cout << "Tree after deletion:\n";
    tree.printTree();
    
    std::cout << "\nIn-order after deletion: ";
    tree.inOrderTraversal([](const int* value) {
        std::cout << *value << " ";
    });
    std::cout << "\n";
    
    // Test clear
    std::cout << "\nClearing tree...\n";
    tree.clear();
    std::cout << "Tree empty: " << (tree.isEmpty() ? "Yes" : "No") << "\n";
    
    return 0;
}
*/
// Explicit template instantiations
// Explicit template instantiation
template class AVLTree<int>;
template class AVLTree<std::string>;
template class AVLTree<double>;
template class AVLTree<Case>;