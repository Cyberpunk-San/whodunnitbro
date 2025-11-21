#ifndef RB_TREE_H
#define RB_TREE_H

#include <functional>
#include <vector>
#include <string>

enum Color { RED, BLACK };

template <typename T>
struct RBNode {
    T data;
    RBNode* parent;
    RBNode* left;
    RBNode* right;
    Color color;
    
    RBNode(T value) : data(value), parent(nullptr), left(nullptr), right(nullptr), color(RED) {}
};

template <typename T>
class RBTree {
private:
    RBNode<T>* root;
    RBNode<T>* TNULL;

    // Helper functions
    void initializeNULLNode(RBNode<T>* node, RBNode<T>* parent);
    void leftRotate(RBNode<T>* x);
    void rightRotate(RBNode<T>* x);
    void insertFix(RBNode<T>* k);
    void transplant(RBNode<T>* u, RBNode<T>* v);
    void deleteFix(RBNode<T>* x);
    void deleteNodeHelper(RBNode<T>* node, T key);
    RBNode<T>* minimum(RBNode<T>* node);
    RBNode<T>* maximum(RBNode<T>* node);
    RBNode<T>* searchTreeHelper(RBNode<T>* node, T key);
    const RBNode<T>* searchTreeHelper(const RBNode<T>* node, T key) const;
    void clearTree(RBNode<T>* node);
    void inOrderHelper(RBNode<T>* node, std::function<void(T*)> fn);
    void inOrderHelper(const RBNode<T>* node, std::function<void(const T*)> fn) const;
    void preOrderHelper(RBNode<T>* node, std::function<void(T*)> fn);
    void postOrderHelper(RBNode<T>* node, std::function<void(T*)> fn);
    int getHeight(RBNode<T>* node);
    int getSize(RBNode<T>* node) const;
    int countBlackNodes(RBNode<T>* node);
    void collectNodes(RBNode<T>* node, std::vector<T>& collection);
    bool isValidRBTreeHelper(RBNode<T>* node, int& blackCount, int currentBlackCount);
    void printTreeHelper(RBNode<T>* node, std::string indent, bool last);

public:
    // Constructor and Destructor
    RBTree();
    ~RBTree();

    // Basic operations
    void insert(T key);
    void remove(T key);
    T* search(T key);
    const T* search(T key) const;
    bool contains(T key);
    void clear();

    // Traversals
    void inOrderTraversal(std::function<void(T*)> fn);
    void inOrderTraversal(std::function<void(const T*)> fn) const;
    void preOrderTraversal(std::function<void(T*)> fn);
    void postOrderTraversal(std::function<void(T*)> fn);
    void levelOrderTraversal(std::function<void(T*)> fn);

    // Tree properties
    int getHeight();
    int getSize() const;
    bool isEmpty();
    T* getMin();
    T* getMax();
    int getBlackHeight();
    bool isValidRBTree();

    // Utility functions
    T* searchByCriteria(std::function<bool(const T&)> criteria);
    std::vector<T> toVector();
    std::vector<T> filter(std::function<bool(const T&)> criteria);
    RBTree<T> clone();

    // Display functions
    void printTree();
    void displayStats();
};

#endif // RB_TREE_H