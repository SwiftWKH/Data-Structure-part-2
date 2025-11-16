#include <iostream>
#include "EmergencyDepartmentOfficer.hpp"
using namespace std;

// Emergency Department Officer
void emergencyDepartmentOfficerMenu() {
    int choice;
    
    while (true) {
        cout << "\n--- Emergency Department Officer Menu ---" << endl;
        cout << "1. Log Emergency Case" << endl;
        cout << "2. Process Most Critical Case" << endl;
        cout << "3. View Pending Emergency Cases" << endl;
        cout << "4. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Log Emergency Case - To be implemented" << endl;
                break;
            case 2:
                cout << "Process Most Critical Case - To be implemented" << endl;
                break;
            case 3:
                cout << "View Pending Emergency Cases - To be implemented" << endl;
                break;
            case 4:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
