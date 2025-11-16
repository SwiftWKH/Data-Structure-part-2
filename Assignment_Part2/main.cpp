#include <iostream>
#include <fstream>
#include <string>
#include "PatientAdmissionClerk/PatientAdmissionClerk.hpp"
#include "MedicalSupplyManager/MedicalSupplyManager.hpp"
#include "EmergencyDepartmentOfficer/EmergencyDepartmentOfficer.hpp"
#include "AmbulanceDispatcher/AmbulanceDispatcher.hpp"
using namespace std;

bool authenticateUser(const string& id, const string& password) {
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
        
        string userId = line.substr(0, pos1);
        string pass = line.substr(pos1 + 1, pos2 - pos1 - 1);
        
        if (userId == id && pass == password) {
            file.close();
            return true;
        }
    }
    
    file.close();
    return false;
}

void routeToMember(const string& id) {
    if (id == "member1") {
        cout << "\n=== Patient Admission Clerk System ===" << endl;
        patientAdmissionClerkMenu();
    }
    else if (id == "member2") {
        cout << "\n=== Medical Supply Manager System ===" << endl;
        medicalSupplyManagerMenu();
    }
    else if (id == "member3") {
        cout << "\n=== Emergency Department Officer System ===" << endl;
        emergencyDepartmentOfficerMenu();
    }
    else if (id == "member4") {
        cout << "\n=== Ambulance Dispatcher System ===" << endl;
        ambulanceDispatcherMenu();
    }
}

int main() {
    string userId, password;
    
    cout << "========================================" << endl;
    cout << "  Hospital Management System Login" << endl;
    cout << "========================================" << endl;
    
    while (true) {
        cout << "\nEnter User ID: ";
        cin >> userId;
        cout << "Enter Password: ";
        cin >> password;
        
        if (authenticateUser(userId, password)) {
            cout << "\nLogin successful! Welcome " << userId << endl;
            routeToMember(userId);
            break;
        } else {
            cout << "\nInvalid credentials! Please try again." << endl;
        }
    }
    
    return 0;
}
