#include <iostream>
#include "AmbulanceDispatcher.hpp"
using namespace std;

// Ambulance Dispatcher
void ambulanceDispatcherMenu() {
    int choice;
    
    while (true) {
        cout << "\n--- Ambulance Dispatcher Menu ---" << endl;
        cout << "1. Register Ambulance" << endl;
        cout << "2. Rotate Ambulance Shift" << endl;
        cout << "3. Display Ambulance Schedule" << endl;
        cout << "4. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Register Ambulance - To be implemented" << endl;
                break;
            case 2:
                cout << "Rotate Ambulance Shift - To be implemented" << endl;
                break;
            case 3:
                cout << "Display Ambulance Schedule - To be implemented" << endl;
                break;
            case 4:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
