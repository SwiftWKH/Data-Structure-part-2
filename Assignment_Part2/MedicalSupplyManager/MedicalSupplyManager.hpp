#ifndef MEDICALSUPPLYMANAGER_HPP
#define MEDICALSUPPLYMANAGER_HPP

#include <string>
#include <iostream> 
using namespace std;

/**
 * @struct SupplyItem
 * @brief Holds the data for a single medical supply item.
 * This is the data that will be stored in each stack node.
 */
struct SupplyItem {
    string type;      // e.g., "Syringes", "Bandages"
    int quantity;
    string batch;     // e.g., "B-001"
};

/**
 * @struct Node
 * @brief A node in the linked list for the SupplyStack.
 * Contains the actual supply data and a pointer to the next node.
 */
struct Node {
    SupplyItem data; // The data payload
    Node* next;      // Pointer to the next node in the stack (the one "below" it)

    /**
     * @brief Node constructor for easy initialization.
     * @param item The SupplyItem data to store in this node.
     */
    Node(SupplyItem item) : data(item), next(nullptr) {}
};

/**
 * @class SupplyStack
 * @brief Implements a Stack (LIFO) data structure from scratch
 * using a singly linked list to manage medical supplies.
 * This class does NOT use any STL containers.
 */
class SupplyStack {
private:
    /**
     * @brief Pointer to the top-most node of the stack.
     * If top == nullptr, the stack is empty.
     */
    Node* top;

public:
    /**
     * @brief Constructor. Initializes an empty stack.
     */
    SupplyStack();

    /**
     * @brief Destructor.
     * Crucially, this cleans up all dynamically allocated memory (all Nodes)
     * to prevent any memory leaks when the stack object is destroyed.
     */
    ~SupplyStack();

    /**
     * @brief Checks if the stack is empty.
     * @return true if 'top' is nullptr, false otherwise.
     */
    bool isEmpty();

    /**
     * @brief Adds a new supply item to the top of the stack (push operation).
     * @param type The type of supply (e.g., "Gauze").
     * @param quantity The number of items in this batch.
     * @param batch The batch identifier (e.g., "G-2025").
     */
    void addSupply(string type, int quantity, string batch);


// Removes the last added (top) supply item from the stack (pop operation).
// It also displays the item that was used.
    void useLastSupply();


// Displays all supply items currently in the stack, from top to bottom, without removing any of them.
    void viewSupplies();
};


// Function declaration for the module's main menu, called by main.cpp
void medicalSupplyManagerMenu();

#endif // MEDICALSUPPLYMANAGER_HPP