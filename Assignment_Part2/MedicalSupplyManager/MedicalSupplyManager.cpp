#include <iostream>
#include "MedicalSupplyManager.hpp"
using namespace std;

// Medical Supply Manager
void medicalSupplyManagerMenu() {
    int choice;
    
    while (true) {
        cout << "\n--- Medical Supply Manager Menu ---" << endl;
        cout << "1. Add Supply Stock" << endl;
        cout << "2. Use Last Added Supply" << endl;
        cout << "3. View Current Supplies" << endl;
        cout << "4. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Add Supply Stock - To be implemented" << endl;
                break;
            case 2:
                cout << "Use Last Added Supply - To be implemented" << endl;
                break;
            case 3:
                cout << "View Current Supplies - To be implemented" << endl;
                break;
            case 4:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
