#include "rb_tree.h"
#include <queue>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "../models/suspect.h"
// Constructor
template <typename T>
RBTree<T>::RBTree() {
    TNULL = new RBNode<T>(T());
    TNULL->color = BLACK;
    TNULL->left = TNULL->right = TNULL;
    root = TNULL;
}

// Destructor
template <typename T>
RBTree<T>::~RBTree() {
    clear();
    delete TNULL;
}

// Initialize NULL node
template <typename T>
void RBTree<T>::initializeNULLNode(RBNode<T>* node, RBNode<T>* parent) {
    node->data = T();
    node->parent = parent;
    node->left = TNULL;
    node->right = TNULL;
    node->color = BLACK;
}

// Left rotate
template <typename T>
void RBTree<T>::leftRotate(RBNode<T>* x) {
    RBNode<T>* y = x->right;
    x->right = y->left;
    
    if (y->left != TNULL)
        y->left->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    
    y->left = x;
    x->parent = y;
}

// Right rotate
template <typename T>
void RBTree<T>::rightRotate(RBNode<T>* x) {
    RBNode<T>* y = x->left;
    x->left = y->right;
    
    if (y->right != TNULL)
        y->right->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    
    y->right = x;
    x->parent = y;
}

// Insert fixup
template <typename T>
void RBTree<T>::insertFix(RBNode<T>* k) {
    RBNode<T>* u;
    while (k->parent != nullptr && k->parent->color == RED) {
        if (k->parent == k->parent->parent->left) {
            u = k->parent->parent->right;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(k->parent->parent);
            }
        } else {
            u = k->parent->parent->left;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(k->parent->parent);
            }
        }
        if (k == root) break;
    }
    root->color = BLACK;
}

// Public insert
template <typename T>
void RBTree<T>::insert(T key) {
    RBNode<T>* node = new RBNode<T>(key);
    node->parent = nullptr;
    node->left = TNULL;
    node->right = TNULL;
    node->color = RED;

    RBNode<T>* y = nullptr;
    RBNode<T>* x = root;

    while (x != TNULL) {
        y = x;
        if (node->data < x->data)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;
    if (y == nullptr)
        root = node;
    else if (node->data < y->data)
        y->left = node;
    else
        y->right = node;

    if (node->parent == nullptr) {
        node->color = BLACK;
        return;
    }

    if (node->parent->parent == nullptr)
        return;

    insertFix(node);
}

// Transplant
template <typename T>
void RBTree<T>::transplant(RBNode<T>* u, RBNode<T>* v) {
    if (u->parent == nullptr)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

// Delete fixup
template <typename T>
void RBTree<T>::deleteFix(RBNode<T>* x) {
    RBNode<T>* s;
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                s = x->parent->right;
            }

            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->right->color == BLACK) {
                    s->left->color = BLACK;
                    s->color = RED;
                    rightRotate(s);
                    s = x->parent->right;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            s = x->parent->left;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                s = x->parent->left;
            }

            if (s->right->color == BLACK && s->left->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->left->color == BLACK) {
                    s->right->color = BLACK;
                    s->color = RED;
                    leftRotate(s);
                    s = x->parent->left;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

// Minimum node
template <typename T>
RBNode<T>* RBTree<T>::minimum(RBNode<T>* node) {
    while (node->left != TNULL)
        node = node->left;
    return node;
}

// Maximum node
template <typename T>
RBNode<T>* RBTree<T>::maximum(RBNode<T>* node) {
    while (node->right != TNULL)
        node = node->right;
    return node;
}

// Delete node helper
template <typename T>
void RBTree<T>::deleteNodeHelper(RBNode<T>* node, T key) {
    RBNode<T>* z = TNULL;
    RBNode<T>* x, *y;
    
    // Find the node to delete
    z = root;
    while (z != TNULL) {
        if (z->data == key) {
            break;
        }
        if (z->data < key) {
            z = z->right;
        } else {
            z = z->left;
        }
    }

    if (z == TNULL) {
        std::cout << "Key not found in the tree\n";
        return;
    }

    y = z;
    Color yOriginalColor = y->color;
    
    if (z->left == TNULL) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == TNULL) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    delete z;
    
    if (yOriginalColor == BLACK)
        deleteFix(x);
}

// Public remove
template <typename T>
void RBTree<T>::remove(T key) {
    deleteNodeHelper(root, key);
}

// Search helper
template <typename T>
RBNode<T>* RBTree<T>::searchTreeHelper(RBNode<T>* node, T key) {
    if (node == TNULL || key == node->data)
        return node;
    if (key < node->data)
        return searchTreeHelper(node->left, key);
    return searchTreeHelper(node->right, key);
}

// Const search helper
template <typename T>
const RBNode<T>* RBTree<T>::searchTreeHelper(const RBNode<T>* node, T key) const {
    if (node == TNULL || key == node->data)
        return node;
    if (key < node->data)
        return searchTreeHelper(node->left, key);
    return searchTreeHelper(node->right, key);
}

// Public search
template <typename T>
T* RBTree<T>::search(T key) {
    RBNode<T>* res = searchTreeHelper(root, key);
    if (res == TNULL) return nullptr;
    return &res->data;
}

// Const public search
template <typename T>
const T* RBTree<T>::search(T key) const {
    const RBNode<T>* res = searchTreeHelper(root, key);
    if (res == TNULL) return nullptr;
    return &res->data;
}

// Check if contains key
template <typename T>
bool RBTree<T>::contains(T key) {
    return search(key) != nullptr;
}

// Clear tree
template <typename T>
void RBTree<T>::clear() {
    clearTree(root);
    root = TNULL;
}

template <typename T>
void RBTree<T>::clearTree(RBNode<T>* node) {
    if (node == TNULL) return;
    clearTree(node->left);
    clearTree(node->right);
    if (node != TNULL) delete node;
}

// In-order traversal
template <typename T>
void RBTree<T>::inOrderHelper(RBNode<T>* node, std::function<void(T*)> fn) {
    if (node == TNULL) return;
    inOrderHelper(node->left, fn);
    fn(&node->data);
    inOrderHelper(node->right, fn);
}

template <typename T>
void RBTree<T>::inOrderTraversal(std::function<void(T*)> fn) {
    inOrderHelper(root, fn);
}

// Const in-order traversal
template <typename T>
void RBTree<T>::inOrderHelper(const RBNode<T>* node, std::function<void(const T*)> fn) const {
    if (node == TNULL) return;
    inOrderHelper(node->left, fn);
    fn(&node->data);
    inOrderHelper(node->right, fn);
}

template <typename T>
void RBTree<T>::inOrderTraversal(std::function<void(const T*)> fn) const {
    inOrderHelper(root, fn);
}

// Pre-order traversal
template <typename T>
void RBTree<T>::preOrderHelper(RBNode<T>* node, std::function<void(T*)> fn) {
    if (node == TNULL) return;
    fn(&node->data);
    preOrderHelper(node->left, fn);
    preOrderHelper(node->right, fn);
}

template <typename T>
void RBTree<T>::preOrderTraversal(std::function<void(T*)> fn) {
    preOrderHelper(root, fn);
}

// Post-order traversal
template <typename T>
void RBTree<T>::postOrderHelper(RBNode<T>* node, std::function<void(T*)> fn) {
    if (node == TNULL) return;
    postOrderHelper(node->left, fn);
    postOrderHelper(node->right, fn);
    fn(&node->data);
}

template <typename T>
void RBTree<T>::postOrderTraversal(std::function<void(T*)> fn) {
    postOrderHelper(root, fn);
}

// Level-order traversal
template <typename T>
void RBTree<T>::levelOrderTraversal(std::function<void(T*)> fn) {
    if (root == TNULL) return;
    
    std::queue<RBNode<T>*> q;
    q.push(root);
    
    while (!q.empty()) {
        RBNode<T>* current = q.front();
        q.pop();
        
        fn(&current->data);
        
        if (current->left != TNULL) q.push(current->left);
        if (current->right != TNULL) q.push(current->right);
    }
}

// Get tree height
template <typename T>
int RBTree<T>::getHeight() {
    return getHeight(root);
}

template <typename T>
int RBTree<T>::getHeight(RBNode<T>* node) {
    if (node == TNULL) return 0;
    return 1 + std::max(getHeight(node->left), getHeight(node->right));
}

// Get tree size
template <typename T>
int RBTree<T>::getSize() const {
    return getSize(root);
}

template <typename T>
int RBTree<T>::getSize(RBNode<T>* node) const {
    if (node == TNULL) return 0;
    return 1 + getSize(node->left) + getSize(node->right);
}

// Check if empty
template <typename T>
bool RBTree<T>::isEmpty() {
    return root == TNULL;
}

// Get minimum value
template <typename T>
T* RBTree<T>::getMin() {
    RBNode<T>* minNode = minimum(root);
    return minNode != TNULL ? &minNode->data : nullptr;
}

// Get maximum value
template <typename T>
T* RBTree<T>::getMax() {
    RBNode<T>* maxNode = maximum(root);
    return maxNode != TNULL ? &maxNode->data : nullptr;
}

// Get black height
template <typename T>
int RBTree<T>::getBlackHeight() {
    return countBlackNodes(root);
}

template <typename T>
int RBTree<T>::countBlackNodes(RBNode<T>* node) {
    if (node == TNULL) return 1;
    int leftBlack = countBlackNodes(node->left);
    int rightBlack = countBlackNodes(node->right);
    return (node->color == BLACK ? 1 : 0) + std::max(leftBlack, rightBlack);
}

// Search by criteria
template <typename T>
T* RBTree<T>::searchByCriteria(std::function<bool(const T&)> criteria) {
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
std::vector<T> RBTree<T>::toVector() {
    std::vector<T> result;
    collectNodes(root, result);
    return result;
}

template <typename T>
void RBTree<T>::collectNodes(RBNode<T>* node, std::vector<T>& collection) {
    if (node == TNULL) return;
    collectNodes(node->left, collection);
    collection.push_back(node->data);
    collectNodes(node->right, collection);
}

// Filter tree
template <typename T>
std::vector<T> RBTree<T>::filter(std::function<bool(const T&)> criteria) {
    std::vector<T> result;
    inOrderTraversal([&](T* data) {
        if (criteria(*data)) {
            result.push_back(*data);
        }
    });
    return result;
}

// Validate RB tree properties
template <typename T>
bool RBTree<T>::isValidRBTree() {
    if (root == TNULL) return true;
    if (root->color != BLACK) return false;
    
    int blackCount = -1;
    return isValidRBTreeHelper(root, blackCount, 0);
}

template <typename T>
bool RBTree<T>::isValidRBTreeHelper(RBNode<T>* node, int& blackCount, int currentBlackCount) {
    if (node == TNULL) {
        if (blackCount == -1) {
            blackCount = currentBlackCount;
            return true;
        }
        return currentBlackCount == blackCount;
    }
    
    // Check for consecutive red nodes
    if (node->color == RED) {
        if ((node->left != TNULL && node->left->color == RED) ||
            (node->right != TNULL && node->right->color == RED)) {
            return false;
        }
    }
    
    int nextBlackCount = currentBlackCount + (node->color == BLACK ? 1 : 0);
    
    return isValidRBTreeHelper(node->left, blackCount, nextBlackCount) &&
           isValidRBTreeHelper(node->right, blackCount, nextBlackCount);
}

// Clone tree
template <typename T>
RBTree<T> RBTree<T>::clone() {
    RBTree<T> newTree;
    inOrderTraversal([&](T* data) {
        newTree.insert(*data);
    });
    return newTree;
}

// Print tree (visual representation)
template <typename T>
void RBTree<T>::printTree() {
    printTreeHelper(root, "", true);
}

template <typename T>
void RBTree<T>::printTreeHelper(RBNode<T>* node, std::string indent, bool last) {
    if (node == TNULL) return;
    
    std::cout << indent;
    if (last) {
        std::cout << "└── ";
        indent += "    ";
    } else {
        std::cout << "├── ";
        indent += "│   ";
    }
    
    std::string colorStr = node->color == RED ? "RED" : "BLACK";
    std::cout << node->data << " (" << colorStr << ")" << std::endl;
    
    printTreeHelper(node->left, indent, false);
    printTreeHelper(node->right, indent, true);
}

// Display tree statistics
template <typename T>
void RBTree<T>::displayStats() {
    std::cout << "Red-Black Tree Statistics:\n";
    std::cout << "Height: " << getHeight() << "\n";
    std::cout << "Size: " << getSize() << "\n";
    std::cout << "Black Height: " << getBlackHeight() << "\n";
    std::cout << "Valid RB Tree: " << (isValidRBTree() ? "Yes" : "No") << "\n";
    std::cout << "Empty: " << (isEmpty() ? "Yes" : "No") << "\n";
    
    T* minVal = getMin();
    T* maxVal = getMax();
    if (minVal) std::cout << "Min Value: " << *minVal << "\n";
    else std::cout << "Min Value: None\n";
    if (maxVal) std::cout << "Max Value: " << *maxVal << "\n";
    else std::cout << "Max Value: None\n";
}
/*
// Main function to test RB Tree
int main() {
    std::cout << "=== Testing Red-Black Tree ===\n\n";
    
    RBTree<int> tree;
    
    // Test insertion
    std::cout << "Inserting values: 10, 20, 30, 40, 50, 25, 35, 5\n";
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);
    tree.insert(35);
    tree.insert(5);
    
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
    RBTree<int> clonedTree = tree.clone();
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
    
    // Verify RB tree properties
    std::cout << "\nValid RB Tree after deletion: " << (tree.isValidRBTree() ? "Yes" : "No") << "\n";
    
    // Test clear
    std::cout << "\nClearing tree...\n";
    tree.clear();
    std::cout << "Tree empty: " << (tree.isEmpty() ? "Yes" : "No") << "\n";
    
    return 0;
}
*/
// Explicit template instantiation
template class RBTree<int>;
template class RBTree<std::string>;
template class RBTree<double>;
template class RBTree<Suspect>;