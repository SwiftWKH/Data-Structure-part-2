#include <iostream>
#include "PatientAdmissionClerk.hpp"
using namespace std;

// Patient Admission Clerk
void patientAdmissionClerkMenu() {
    int choice;
    
    while (true) {
        cout << "\n--- Patient Admission Clerk Menu ---" << endl;
        cout << "1. Admit Patient" << endl;
        cout << "2. Discharge Patient" << endl;
        cout << "3. View Patient Queue" << endl;
        cout << "4. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Admit Patient - To be implemented" << endl;
                break;
            case 2:
                cout << "Discharge Patient - To be implemented" << endl;
                break;
            case 3:
                cout << "View Patient Queue - To be implemented" << endl;
                break;
            case 4:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
