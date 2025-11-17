#include "MedicalSupplyManager.hpp"
#include <iostream>
#include <limits>

static void clearInputBuffer() {
    // Ignore all characters up to the max stream size or until a newline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ask valid integer from user (>= 0).
static int getValidInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            if (value >= 0) {
                // Good input, clear buffer and return
                clearInputBuffer();
                return value;
            } else {
                std::cout << "Input must be a non-negative number." << std::endl;
            }
        } else {
            // Bad input (e.g., user entered 'abc')
            std::cout << "Invalid input. Please enter a whole number." << std::endl;
            std::cin.clear();
            clearInputBuffer();
        }
    }
}

// --- SupplyStack Class Implementation ---

// Constructor: Initializes the stack, point to top
SupplyStack::SupplyStack() : top(nullptr) { }

// Destructor to delete all nodes, prevent memory leaks
SupplyStack::~SupplyStack() {
    Node* current = top;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    top = nullptr;
}

// Checks if the stack is empty.
bool SupplyStack::isEmpty() const {
    return top == nullptr;
}

// Add Supply - Push Operation
bool SupplyStack::addSupply(const std::string& type, int quantity, const std::string& batch) {
    // Validate input
    if (type.empty() || batch.empty() || quantity < 0) {
        return false;
    }

    // 1. Create the item
    SupplyItem newItem;
    newItem.type = type;
    newItem.quantity = quantity;
    newItem.batch = batch;

    // 2. Create the new node (using the Node's constructor)
    Node* newNode = new Node(newItem);

    // 3. Link the new node to the old top
    newNode->next = top;

    // 4. Update top to be the new node
    top = newNode;

    return true;
}

// Use Supply (Removes the last added item from stack) - Pop Operation
bool SupplyStack::useLastSupply(SupplyItem& out) {
    // 1. Check for underflow
    if (isEmpty()) {
        return false;
    }

    // 2. Store the top node's data to out
    Node* temp = top;
    out = temp->data;

    // 3. Update top to the next node
    top = top->next;

    // 4. Delete the old top node
    delete temp;

    return true;
}

// View Supplies - Display Operation
void SupplyStack::viewSupplies() const {
    // 1. Check if empty
    if (isEmpty()) {
        std::cout << "\n[EMPTY] The medical supply stack is currently empty." << std::endl;
        return;
    }

    std::cout << "\n--- Current Medical Supply Stock (LIFO) ---" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "[TOP OF STACK]\n" << std::endl;

    Node* current = top; // 2. Start at the top
    int count = 1;

    // 3. Iterate
    while (current != nullptr) {
        // 4. Print data
        std::cout << "  " << count << ". \tType:     " << current->data.type << std::endl;
        std::cout << "     \tQuantity: " << current->data.quantity << std::endl;
        std::cout << "     \tBatch:    " << current->data.batch << std::endl;
        std::cout << "     \t---" << std::endl;

        current = current->next;
        count++;
    }
    std::cout << "\n[BOTTOM OF STACK]" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}

// --- Medical Supply Manager Menu ---

void medicalSupplyManagerMenu() {
    // 1. Create the stack object. It will exist only for the duration of this menu
    SupplyStack supplyStack;
    int choice;

    do {
        std::cout << "\n=== Medical Supply Manager Menu ===" << std::endl;
        std::cout << "1. Add Supply Stock (Push)" << std::endl;
        std::cout << "2. Use Last Added Supply (Pop)" << std::endl;
        std::cout << "3. View Current Supplies (Display)" << std::endl;
        std::cout << "4. Logout (Return to Main Menu)" << std::endl;

        choice = getValidInt("Enter your choice (1-4): ");

        switch (choice) {
            case 1: { // Add Supply (Push)
                std::string type, batch;
                int quantity;

                std::cout << "\nEnter Supply Type (e.g., Gauze): ";
                std::getline(std::cin, type);

                quantity = getValidInt("Enter Quantity: ");

                std::cout << "Enter Batch Code (e.g., G-2025): ";
                std::getline(std::cin, batch);

                if (!supplyStack.addSupply(type, quantity, batch)) {
                    std::cout << "\n[Error] Invalid supply data. Type and Batch cannot be empty, quantity must be non-negative." << std::endl;
                } else {
                    std::cout << "\n[SUCCESS] Added: " << quantity << "x " << type << " (Batch: " << batch << ") to the stack." << std::endl;
                }
                break;
            }
            case 2: { // Use Supply (Pop)
                SupplyItem used;
                if (supplyStack.useLastSupply(used)) {
                    std::cout << "\n[USED] Using last supply: "
                              << used.quantity << "x " << used.type
                              << " (Batch: " << used.batch << ")" << std::endl;
                } else {
                    std::cout << "\n[EMPTY] The medical supply stack is empty. Nothing to use." << std::endl;
                }
                break;
            }
            case 3: // View Supplies
                supplyStack.viewSupplies();
                break;
            case 4: // Logout
                std::cout << "\nLogging out from Medical Supply Manager..." << std::endl;
                break;
            default:
                std::cout << "\n[Error] Invalid choice! Please enter a number between 1 and 4." << std::endl;
        }
    } while (choice != 4);

    // 2. When the loop ends (user chose 4), this function ends.
}
