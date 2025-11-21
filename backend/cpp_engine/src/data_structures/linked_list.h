#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <functional>
#include "../models/case.h"
#include "../models/character.h"
#include "../models/suspect.h"

template <typename T>
class LinkedList {
private:
    struct ListNode {
        T data;
        ListNode* next;
        ListNode* prev;
        
        ListNode(T val) : data(val), next(nullptr), prev(nullptr) {}
    };
    
    ListNode* head;
    ListNode* tail;
    int size;

public:
    // Constructor and Destructor
    LinkedList();
    ~LinkedList();

    // Basic operations
    void insertAtEnd(T data);
    void insertAtBeginning(T data);
    void deleteNode(T data);
    void clear();

    // Query operations
    bool contains(T data) const;
    ListNode* search(T data);
    ListNode* searchByCriteria(std::function<bool(const T&)> criteria);
    int getSize() const;
    bool isEmpty() const;
    T* getFirst();
    T* getLast();

    // Traversal operations
    void traverse(std::function<void(T&)> func);
    void traverseConst(std::function<void(const T&)> func) const;
    void displayForward() const;
    void displayBackward() const;

    // Utility operations
    void reverse();
    void removeDuplicates();
    LinkedList<T> filter(std::function<bool(const T&)> criteria);
    void sort(std::function<bool(const T&, const T&)> comparator = nullptr);
};

#endif // LINKED_LIST_H