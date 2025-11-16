#include <iostream>
#include <fstream>
#include <cstring>
#include "AmbulanceDispatcher.hpp"
using namespace std;

AmbulanceCircularQueue ambulanceQueue;

AmbulanceCircularQueue::AmbulanceCircularQueue(int maxAmb) {
    maxAmbulances = maxAmb;
    totalSlots = 15; // 5 days × 3 shifts = 15 slots
    
    ambulances = new Ambulance[maxAmbulances];
    schedule = new TimeSlot[totalSlots];
    
    ambulanceCount = 0;
    currentSlot = 0;
    nextId = 101;
    loading = false;
    
    // Initialize arrays
    for (int i = 0; i < maxAmbulances; i++) {
        ambulances[i].id = 0;
        strcpy(ambulances[i].driverName, "");
        strcpy(ambulances[i].status, "");
    }
    
    initializeSchedule();
    loadFromFile();
    assignInitialSchedule();
}

AmbulanceCircularQueue::~AmbulanceCircularQueue() {
    delete[] ambulances;
    delete[] schedule;
}

void AmbulanceCircularQueue::initializeSchedule() {
    const char* shifts[15][2] = {
        {"Mon Morning", "8AM-4PM"},
        {"Mon Evening", "4PM-12AM"},
        {"Mon Night", "12AM-8AM"},
        {"Tue Morning", "8AM-4PM"},
        {"Tue Evening", "4PM-12AM"},
        {"Tue Night", "12AM-8AM"},
        {"Wed Morning", "8AM-4PM"},
        {"Wed Evening", "4PM-12AM"},
        {"Wed Night", "12AM-8AM"},
        {"Thu Morning", "8AM-4PM"},
        {"Thu Evening", "4PM-12AM"},
        {"Thu Night", "12AM-8AM"},
        {"Fri Morning", "8AM-4PM"},
        {"Fri Evening", "4PM-12AM"},
        {"Fri Night", "12AM-8AM"}
    };
    
    for (int i = 0; i < totalSlots; i++) {
        strcpy(schedule[i].shiftName, shifts[i][0]);
        strcpy(schedule[i].timeRange, shifts[i][1]);
        schedule[i].assignedAmbulanceId = 0;
    }
}

bool AmbulanceCircularQueue::registerAmbulance(const Ambulance& amb) {
    if (ambulanceCount >= maxAmbulances) return false;
    
    ambulances[ambulanceCount] = amb;
    ambulanceCount++;
    
    if (!loading) saveToFile();
    return true;
}

bool AmbulanceCircularQueue::rotateShift() {
    if (ambulanceCount == 0) return false;
    
    // Conveyor belt rotation - move the entire "window" of 3 people
    static int conveyorPosition = 0;
    conveyorPosition = (conveyorPosition + 1) % ambulanceCount;
    
    // Fill the 15-slot schedule with groups of 3, but shift their positions across days
    for (int day = 0; day < 5; day++) {
        for (int shift = 0; shift < 3; shift++) {
            int slot = day * 3 + shift;
            int shiftOffset = (shift + day) % 3; // Rotate shifts across days
            int ambulanceIndex = (conveyorPosition + shiftOffset) % ambulanceCount;
            schedule[slot].assignedAmbulanceId = ambulances[ambulanceIndex].id;
        }
    }
    
    return true;
}

void AmbulanceCircularQueue::assignInitialSchedule() {
    if (ambulanceCount == 0) return;
    
    // Initial schedule with shift rotation across days
    for (int day = 0; day < 5; day++) {
        for (int shift = 0; shift < 3; shift++) {
            int slot = day * 3 + shift;
            int shiftOffset = (shift + day) % 3; // Rotate shifts across days
            int ambulanceIndex = shiftOffset % ambulanceCount;
            schedule[slot].assignedAmbulanceId = ambulances[ambulanceIndex].id;
        }
    }
}


void AmbulanceCircularQueue::displaySchedule() {
    if (ambulanceCount == 0) {
        cout << "No ambulances registered." << endl;
        return;
    }
    
    cout << "\n=== Ambulance Dispatch Schedule ===" << endl;
    
    cout << "Weekly Schedule:" << endl;
    cout << "Day\t\tMorning\t\tEvening\t\tNight" << endl;
    cout << "--------------------------------------------------------" << endl;
    
    // Monday (slots 0,1,2)
    cout << "Monday\t\t";
    for (int shift = 0; shift < 3; shift++) {
        if (schedule[shift].assignedAmbulanceId > 0) {
            for (int j = 0; j < ambulanceCount; j++) {
                if (ambulances[j].id == schedule[shift].assignedAmbulanceId) {
                    cout << "AMB" << ambulances[j].id;
                    break;
                }
            }
        } else {
            cout << "Unassigned";
        }
        cout << "\t\t";
    }
    cout << endl;
    
    // Tuesday (slots 3,4,5)
    cout << "Tuesday\t\t";
    for (int shift = 3; shift < 6; shift++) {
        if (schedule[shift].assignedAmbulanceId > 0) {
            for (int j = 0; j < ambulanceCount; j++) {
                if (ambulances[j].id == schedule[shift].assignedAmbulanceId) {
                    cout << "AMB" << ambulances[j].id;
                    break;
                }
            }
        } else {
            cout << "Unassigned";
        }
        cout << "\t\t";
    }
    cout << endl;
    
    // Wednesday (slots 6,7,8)
    cout << "Wednesday\t";
    for (int shift = 6; shift < 9; shift++) {
        if (schedule[shift].assignedAmbulanceId > 0) {
            for (int j = 0; j < ambulanceCount; j++) {
                if (ambulances[j].id == schedule[shift].assignedAmbulanceId) {
                    cout << "AMB" << ambulances[j].id;
                    break;
                }
            }
        } else {
            cout << "Unassigned";
        }
        cout << "\t\t";
    }
    cout << endl;
    
    // Thursday (slots 9,10,11)
    cout << "Thursday\t";
    for (int shift = 9; shift < 12; shift++) {
        if (schedule[shift].assignedAmbulanceId > 0) {
            for (int j = 0; j < ambulanceCount; j++) {
                if (ambulances[j].id == schedule[shift].assignedAmbulanceId) {
                    cout << "AMB" << ambulances[j].id;
                    break;
                }
            }
        } else {
            cout << "Unassigned";
        }
        cout << "\t\t";
    }
    cout << endl;
    
    // Friday (slots 12,13,14)
    cout << "Friday\t\t";
    for (int shift = 12; shift < 15; shift++) {
        if (schedule[shift].assignedAmbulanceId > 0) {
            for (int j = 0; j < ambulanceCount; j++) {
                if (ambulances[j].id == schedule[shift].assignedAmbulanceId) {
                    cout << "AMB" << ambulances[j].id;
                    break;
                }
            }
        } else {
            cout << "Unassigned";
        }
        cout << "\t\t";
    }
    cout << endl;
    
    cout << "\nWork Load Tracker:" << endl;
    cout << "----------------------------------------" << endl;
    for (int i = 0; i < ambulanceCount; i++) {
        // Count current shifts in schedule
        int currentShifts = 0;
        for (int j = 0; j < totalSlots; j++) {
            if (schedule[j].assignedAmbulanceId == ambulances[i].id) {
                currentShifts++;
            }
        }
        
        cout << "AMB" << ambulances[i].id << " (" << ambulances[i].driverName << "): ";
        
        // Visual bar for current shifts (max 5 characters)
        for (int j = 0; j < currentShifts && j < 5; j++) {
            cout << "#";
        }
        for (int j = currentShifts; j < 5; j++) {
            cout << "-";
        }
        
        cout << " " << currentShifts << " shifts" << endl;
    }
}

bool AmbulanceCircularQueue::isEmpty() {
    return ambulanceCount == 0;
}

bool AmbulanceCircularQueue::isFull() {
    return ambulanceCount >= maxAmbulances;
}

void AmbulanceCircularQueue::saveToFile() {
    ofstream file("AmbulanceDispatcher/ambulances.csv");
    if (!file.is_open()) return;
    
    file << "ID,DriverName,Status" << endl;
    for (int i = 0; i < ambulanceCount; i++) {
        file << ambulances[i].id << "," 
             << ambulances[i].driverName << "," 
             << ambulances[i].status << endl;
    }
    file.close();
}

void AmbulanceCircularQueue::loadFromFile() {
    loading = true;
    ifstream file("AmbulanceDispatcher/ambulances.csv");
    if (!file.is_open()) {
        loading = false;
        return;
    }
    
    string line;
    getline(file, line); // Skip header
    
    int maxId = 100;
    ambulanceCount = 0;
    
    while (getline(file, line) && ambulanceCount < maxAmbulances) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        
        if (pos1 != string::npos && pos2 != string::npos) {
            Ambulance amb;
            amb.id = stoi(line.substr(0, pos1));
            if (amb.id > maxId) maxId = amb.id;
            
            strcpy(amb.driverName, line.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
            strcpy(amb.status, line.substr(pos2 + 1).c_str());
            
            ambulances[ambulanceCount++] = amb;
        }
    }
    
    nextId = maxId + 1;
    file.close();
    loading = false;
}

int AmbulanceCircularQueue::getNextId() {
    return nextId++;
}

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
            case 1: {
                if (ambulanceQueue.isFull()) {
                    cout << "Cannot register more ambulances - queue full!" << endl;
                    break;
                }
                
                Ambulance amb;
                amb.id = ambulanceQueue.getNextId();
                cout << "Enter Driver Name: ";
                cin.ignore();
                cin.getline(amb.driverName, 50);
                strcpy(amb.status, "Active");
                
                if (ambulanceQueue.registerAmbulance(amb)) {
                    cout << "Ambulance registered successfully! ID: " << amb.id << endl;
                } else {
                    cout << "Failed to register ambulance!" << endl;
                }
                break;
            }
            case 2:
                if (ambulanceQueue.rotateShift()) {
                    cout << "Shift rotated successfully!" << endl;
                } else {
                    cout << "No ambulances available for rotation!" << endl;
                }
                break;
            case 3:
                ambulanceQueue.displaySchedule();
                break;
            case 4:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
