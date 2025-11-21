#include "linked_list.h"
#include <algorithm>
#include <iostream>
#include "../models/character.h"
// Constructor
template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

// Destructor
template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

// Insert at the end
template <typename T>
void LinkedList<T>::insertAtEnd(T data) {
    ListNode* newNode = new ListNode(data);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

// Insert at the beginning
template <typename T>
void LinkedList<T>::insertAtBeginning(T data) {
    ListNode* newNode = new ListNode(data);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    size++;
}

// Delete a node
template <typename T>
void LinkedList<T>::deleteNode(T data) {
    ListNode* current = head;
    while (current) {
        if (current->data == data) {
            if (current->prev)
                current->prev->next = current->next;
            else
                head = current->next;

            if (current->next)
                current->next->prev = current->prev;
            else
                tail = current->prev;

            delete current;
            size--;
            return;
        }
        current = current->next;
    }
}

// Check if list contains data
template <typename T>
bool LinkedList<T>::contains(T data) const {
    ListNode* current = head;
    while (current) {
        if (current->data == data) return true;
        current = current->next;
    }
    return false;
}

// Clear the list
template <typename T>
void LinkedList<T>::clear() {
    ListNode* current = head;
    while (current) {
        ListNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = tail = nullptr;
    size = 0;
}

// Display forward
template <typename T>
void LinkedList<T>::displayForward() const {
    ListNode* current = head;
    while (current) {
        std::cout << current->data;
        if (current->next) std::cout << " <-> ";
        current = current->next;
    }
    std::cout << " -> NULL\n";
}

// Display backward
template <typename T>
void LinkedList<T>::displayBackward() const {
    ListNode* current = tail;
    while (current) {
        std::cout << current->data;
        if (current->prev) std::cout << " <-> ";
        current = current->prev;
    }
    std::cout << " -> NULL\n";
}

// Traverse with function
template <typename T>
void LinkedList<T>::traverse(std::function<void(T&)> func) {
    ListNode* current = head;
    while (current) {
        func(current->data);
        current = current->next;
    }
}

// Const traversal
template <typename T>
void LinkedList<T>::traverseConst(std::function<void(const T&)> func) const {
    ListNode* current = head;
    while (current) {
        func(current->data);
        current = current->next;
    }
}

// Search for data
template <typename T>
typename LinkedList<T>::ListNode* LinkedList<T>::search(T data) {
    ListNode* current = head;
    while (current) {
        if (current->data == data) return current;
        current = current->next;
    }
    return nullptr;
}

// Search by criteria
template <typename T>
typename LinkedList<T>::ListNode* LinkedList<T>::searchByCriteria(std::function<bool(const T&)> criteria) {
    ListNode* current = head;
    while (current) {
        if (criteria(current->data)) return current;
        current = current->next;
    }
    return nullptr;
}

// Get size
template <typename T>
int LinkedList<T>::getSize() const {
    return size;
}

// Check if empty
template <typename T>
bool LinkedList<T>::isEmpty() const {
    return size == 0;
}

// Get first element
template <typename T>
T* LinkedList<T>::getFirst() {
    return head ? &head->data : nullptr;
}

// Get last element
template <typename T>
T* LinkedList<T>::getLast() {
    return tail ? &tail->data : nullptr;
}

// Reverse the list
template <typename T>
void LinkedList<T>::reverse() {
    if (!head || !head->next) return;
    
    ListNode* current = head;
    ListNode* temp = nullptr;
    
    while (current) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }
    
    temp = head;
    head = tail;
    tail = temp;
}

// Remove duplicates
template <typename T>
void LinkedList<T>::removeDuplicates() {
    if (!head) return;
    
    ListNode* current = head;
    while (current && current->next) {
        ListNode* runner = current;
        while (runner->next) {
            if (runner->next->data == current->data) {
                ListNode* duplicate = runner->next;
                runner->next = runner->next->next;
                if (runner->next) runner->next->prev = runner;
                else tail = runner;
                delete duplicate;
                size--;
            } else {
                runner = runner->next;
            }
        }
        current = current->next;
    }
}

// Filter list by criteria
template <typename T>
LinkedList<T> LinkedList<T>::filter(std::function<bool(const T&)> criteria) {
    LinkedList<T> result;
    traverseConst([&](const T& data) {
        if (criteria(data)) {
            result.insertAtEnd(data);
        }
    });
    return result;
}

// Sort the list (bubble sort for simplicity)
template <typename T>
void LinkedList<T>::sort(std::function<bool(const T&, const T&)> comparator) {
    if (!head || !head->next) return;
    
    bool swapped;
    do {
        swapped = false;
        ListNode* current = head;
        
        while (current->next) {
            bool shouldSwap = comparator ? 
                comparator(current->data, current->next->data) : 
                current->data > current->next->data;
                
            if (shouldSwap) {
                std::swap(current->data, current->next->data);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

// Main function to test LinkedList
/*
int main() {
    std::cout << "=== Testing Doubly Linked List ===\n\n";
    
    LinkedList<int> list;
    
    // Test insertion at end
    std::cout << "Inserting at end: 10, 20, 30\n";
    list.insertAtEnd(10);
    list.insertAtEnd(20);
    list.insertAtEnd(30);
    
    std::cout << "List forward: ";
    list.displayForward();
    std::cout << "List backward: ";
    list.displayBackward();
    std::cout << "Size: " << list.getSize() << "\n\n";
    
    // Test insertion at beginning
    std::cout << "Inserting at beginning: 5, 1\n";
    list.insertAtBeginning(5);
    list.insertAtBeginning(1);
    
    std::cout << "List forward: ";
    list.displayForward();
    std::cout << "Size: " << list.getSize() << "\n\n";
    
    // Test search and contains
    std::cout << "Search operations:\n";
    std::cout << "Contains 20: " << (list.contains(20) ? "Yes" : "No") << "\n";
    std::cout << "Contains 99: " << (list.contains(99) ? "Yes" : "No") << "\n";
    
    auto node = list.search(20);
    if (node) {
        std::cout << "Found node with value: " << node->data << "\n";
    }
    
    // Test search by criteria
    auto evenNode = list.searchByCriteria([](const int& val) {
        return val % 2 == 0;
    });
    if (evenNode) {
        std::cout << "First even number: " << evenNode->data << "\n";
    }
    std::cout << "\n";
    
    // Test traversal
    std::cout << "Traversing and doubling values: ";
    list.traverse([](int& val) {
        val *= 2;
        std::cout << val << " ";
    });
    std::cout << "\nList after doubling: ";
    list.displayForward();
    std::cout << "\n";
    
    // Test get first and last
    int* first = list.getFirst();
    int* last = list.getLast();
    if (first) std::cout << "First element: " << *first << "\n";
    if (last) std::cout << "Last element: " << *last << "\n\n";
    
    // Test duplicates
    std::cout << "Adding duplicates: 40, 40, 10\n";
    list.insertAtEnd(40);
    list.insertAtEnd(40);
    list.insertAtEnd(10);
    std::cout << "List before removing duplicates: ";
    list.displayForward();
    
    list.removeDuplicates();
    std::cout << "List after removing duplicates: ";
    list.displayForward();
    std::cout << "\n";
    
    // Test filter
    std::cout << "Filtering even numbers: ";
    LinkedList<int> evenList = list.filter([](const int& val) {
        return val % 2 == 0;
    });
    evenList.displayForward();
    
    // Test sorting
    std::cout << "Sorting list (ascending): ";
    list.sort();
    list.displayForward();
    
    std::cout << "Sorting list (descending): ";
    list.sort([](const int& a, const int& b) {
        return a < b; // Reverse order
    });
    list.displayForward();
    std::cout << "\n";
    
    // Test reverse
    std::cout << "Reversing list: ";
    list.reverse();
    list.displayForward();
    std::cout << "\n";
    
    // Test deletion
    std::cout << "Deleting value 20: ";
    list.deleteNode(20);
    list.displayForward();
    
    std::cout << "Deleting value 10: ";
    list.deleteNode(10);
    list.displayForward();
    
    std::cout << "Deleting value 80: ";
    list.deleteNode(80);
    list.displayForward();
    std::cout << "\n";
    
    // Test clear
    std::cout << "Clearing list...\n";
    list.clear();
    std::cout << "List empty: " << (list.isEmpty() ? "Yes" : "No") << "\n";
    std::cout << "Size: " << list.getSize() << "\n";
    
    return 0;
}
    */

// Explicit template instantiations
// Explicit template instantiation
template class LinkedList<int>;
template class LinkedList<std::string>;
template class LinkedList<double>;
template class LinkedList<Character>;