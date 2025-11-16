#include <iostream>
#include <fstream>
#include <string>
#include "PatientAdmissionClerk/PatientAdmissionClerk.hpp"
#include "MedicalSupplyManager/MedicalSupplyManager.hpp"
#include "EmergencyDepartmentOfficer/EmergencyDepartmentOfficer.hpp"
#include "AmbulanceDispatcher/AmbulanceDispatcher.hpp"
using namespace std;

bool authenticateUser(const string& id, const string& password, string& role) {
    ifstream file("login_data.csv");
    string line;
    
    if (!file.is_open()) {
        cout << "Error: Could not open login_data.csv" << endl;
        return false;
    }
    
    // Skip header line
    getline(file, line);
    
    while (getline(file, line)) {
        size_t pos1 = line.find(',');
        if (pos1 == string::npos) continue;
        
        size_t pos2 = line.find(',', pos1 + 1);
        if (pos2 == string::npos) continue;
        
        size_t pos3 = line.find(',', pos2 + 1);
        if (pos3 == string::npos) continue;
        
        string userId = line.substr(0, pos1);
        string pass = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string userRole = line.substr(pos2 + 1, pos3 - pos2 - 1);
        
        if (userId == id && pass == password) {
            role = userRole;
            file.close();
            return true;
        }
    }
    
    file.close();
    return false;
}

void routeToMember(const string& role) {
    if (role == "Patient Admission Clerk") {
        cout << "\n=== Patient Admission Clerk System ===" << endl;
        patientAdmissionClerkMenu();
    }
    else if (role == "Medical Supply Manager") {
        cout << "\n=== Medical Supply Manager System ===" << endl;
        medicalSupplyManagerMenu();
    }
    else if (role == "Emergency Department Officer") {
        cout << "\n=== Emergency Department Officer System ===" << endl;
        emergencyDepartmentOfficerMenu();
    }
    else if (role == "Ambulance Dispatcher") {
        cout << "\n=== Ambulance Dispatcher System ===" << endl;
        ambulanceDispatcherMenu();
    }
}

int main() {
    string userId, password, role;
    
    cout << "========================================" << endl;
    cout << "  Hospital Management System Login" << endl;
    cout << "========================================" << endl;
    
    while (true) {
        cout << "\nEnter User ID: ";
        cin >> userId;
        cout << "Enter Password: ";
        cin >> password;
        
        if (authenticateUser(userId, password, role)) {
            cout << "\nLogin successful! Welcome " << userId << " (" << role << ")" << endl;
            routeToMember(role);
            break;
        } else {
            cout << "\nInvalid credentials! Please try again." << endl;
        }
    }
    
    return 0;
}
