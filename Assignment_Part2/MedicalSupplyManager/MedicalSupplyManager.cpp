#include <iostream>
#include "MedicalSupplyManager.hpp"
#include <string>
#include <limits> 
using namespace std;

void clearInputBuffer() {
    // Ignore all characters up to the max stream size or until a newline
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//ask valid integer frm user (>= 0).
int getValidInt(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= 0) {
                // Good input, clear buffer and return
                clearInputBuffer();
                return value;
            } else {
                cout << "Input must be a non-negative number." << endl;
            }
        } else {
            // Bad input (e.g., user entered 'abc')
            cout << "Invalid input. Please enter a whole number." << endl;
            cin.clear();        
            clearInputBuffer(); 
        }
    }
}

// --- SupplyStack Class Implementation ---

// Constructor: Initializes the stack, point to top
SupplyStack::SupplyStack() {
    top = nullptr;
}

// Destructor to delete all nodes, prevent memory leaks
SupplyStack::~SupplyStack() {
    cout << "\n(MedicalSupplyManager: Cleaning up all supply nodes...)" << endl;
    
    Node* current = top;
    Node* nextNode = nullptr;

    while (current != nullptr) {
        nextNode = current->next; 
        delete current;           
        current = nextNode;       
    }
    top = nullptr; 
}

// Checks if the stack is empty.
bool SupplyStack::isEmpty() {
    return top == nullptr;
}

// Add Supply - Push Operation
void SupplyStack::addSupply(string type, int quantity, string batch) {
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

    cout << "\n[SUCCESS] Added: " << quantity << "x " << type << " (Batch: " << batch << ") to the stack." << endl;
}

// Use Supply(Removes the last added item frm stack) - Pop Operation
void SupplyStack::useLastSupply() {
    // 1. Check for underflow
    if (isEmpty()) {
        cout << "\n[EMPTY] The medical supply stack is empty. Nothing to use." << endl;
        return;
    }

    // 2a. Store the top node
    Node* temp = top;
    SupplyItem usedItem = temp->data;

    // 2c. Update top to the next node
    top = top->next;

    // 2d. Delete the old top node
    delete temp;
    temp = nullptr; 

    // 2b. Display the data
    cout << "\n[USED] Using last supply: "
         << usedItem.quantity << "x " << usedItem.type
         << " (Batch: " << usedItem.batch << ")" << endl;
}

// View Supplies - Display Operation
void SupplyStack::viewSupplies() {
    // 1. Check if empty
    if (isEmpty()) {
        cout << "\n[EMPTY] The medical supply stack is currently empty." << endl;
        return;
    }

    cout << "\n--- Current Medical Supply Stock (LIFO) ---" << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "[TOP OF STACK]\n" << endl;

    Node* current = top; // 2. Start at the top
    int count = 1;

    // 3. Iterate
    while (current != nullptr) {
        // 4. Print data
        cout << "  " << count << ". \tType:     " << current->data.type << endl;
        cout << "     \tQuantity: " << current->data.quantity << endl;
        cout << "     \tBatch:    " << current->data.batch << endl;
        cout << "     \t---" << endl;
        
        current = current->next; 
        count++;
    }
    cout << "\n[BOTTOM OF STACK]" << endl;
    cout << "-----------------------------------------------" << endl;
}

// --- Medical Supply Manager Menu ---

void medicalSupplyManagerMenu() {
    // 1. Create the stack object. It will exist only for the duration of this menu
    SupplyStack supplyStack;
    int choice;

    do {
        cout << "\n=== Medical Supply Manager Menu ===" << endl;
        cout << "1. Add Supply Stock (Push)" << endl;
        cout << "2. Use Last Added Supply (Pop)" << endl;
        cout << "3. View Current Supplies (Display)" << endl;
        cout << "4. Logout (Return to Main Menu)" << endl;

        choice = getValidInt("Enter your choice (1-4): ");

        switch (choice) {
            case 1: { // Add Supply (Push)
                string type, batch;
                int quantity;

                cout << "\nEnter Supply Type (e.g., Gauze): ";
                getline(cin, type); 

                quantity = getValidInt("Enter Quantity: ");

                cout << "Enter Batch Code (e.g., G-2025): ";
                getline(cin, batch);
                
                if (type.empty() || batch.empty()) {
                    cout << "\n[Error] Type and Batch cannot be empty." << endl;
                    break;
                }

                supplyStack.addSupply(type, quantity, batch);
                break;
            }
            case 2: // Use Supply (Pop)
                supplyStack.useLastSupply();
                break;
            case 3: // View Supplies
                supplyStack.viewSupplies();
                break;
            case 4: // Logout
                cout << "\nLogging out from Medical Supply Manager..." << endl;
                break;
            default:
                cout << "\n[Error] Invalid choice! Please enter a number between 1 and 4." << endl;
        }
    } while (choice != 4);

    // 2. When the loop ends (user chose 4), this function ends.
}