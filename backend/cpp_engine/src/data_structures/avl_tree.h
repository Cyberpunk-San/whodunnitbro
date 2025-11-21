#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <functional>
#include <vector>
#include <string>

template <typename T>
struct AVLNode {
    T data;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(T value) : data(value), left(nullptr), right(nullptr), height(1) {}
};

template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    // Helper functions
    int getHeight(AVLNode<T>* node);
    int getBalance(AVLNode<T>* node);
    AVLNode<T>* rightRotate(AVLNode<T>* y);
    AVLNode<T>* leftRotate(AVLNode<T>* x);
    AVLNode<T>* balanceNode(AVLNode<T>* node);
    AVLNode<T>* insertNode(AVLNode<T>* node, T value);
    AVLNode<T>* deleteNode(AVLNode<T>* node, T value);
    AVLNode<T>* minValueNode(AVLNode<T>* node);
    void clearTree(AVLNode<T>* node);
    void inOrderNodes(AVLNode<T>* node, std::function<void(T*)> fn);
    void inOrderNodes(const AVLNode<T>* node, std::function<void(const T*)> fn) const;
    void preOrderNodes(AVLNode<T>* node, std::function<void(T*)> fn);
    void postOrderNodes(AVLNode<T>* node, std::function<void(T*)> fn);
    int countNodes(AVLNode<T>* node) const;
    void collectNodes(AVLNode<T>* node, std::vector<T>& collection);
    bool isBalanced(AVLNode<T>* node);
    bool isComplete(AVLNode<T>* node, int index, int nodeCount);
    void printTreeHelper(AVLNode<T>* node, std::string indent, bool last);

public:
    // Constructor and Destructor
    AVLTree();
    ~AVLTree();

    // Basic operations
    void insert(T value);
    void remove(T value);
    T* search(T value);
    const T* search(T value) const;
    bool contains(T value);
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
    bool isBalanced();
    bool isComplete();

    // Utility functions
    T* searchByCriteria(std::function<bool(const T&)> criteria);
    std::vector<T> toVector();
    std::vector<T> filter(std::function<bool(const T&)> criteria);
    AVLTree<T> clone();
    void merge(const AVLTree<T>& other);

    // Display functions
    void printTree();
    void displayStats();
};

#endif // AVL_TREE_H