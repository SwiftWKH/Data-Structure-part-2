#include <iostream>
#include <fstream>
#include <cstring>
#include "AmbulanceDispatcher.hpp"
using namespace std;

AmbulanceCircularQueue ambulanceQueue;

AmbulanceCircularQueue::AmbulanceCircularQueue(int maxAmb) {
    maxAmbulances = maxAmb;
    totalSlots = 9; // 3 days × 3 shifts = 9 slots
    
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
        ambulances[i].totalShifts = 0;
        ambulances[i].isActive = false;
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
    const char* shifts[9][2] = {
        {"Mon Morning", "8AM-4PM"},
        {"Mon Evening", "4PM-12AM"},
        {"Mon Night", "12AM-8AM"},
        {"Tue Morning", "8AM-4PM"},
        {"Tue Evening", "4PM-12AM"},
        {"Tue Night", "12AM-8AM"},
        {"Wed Morning", "8AM-4PM"},
        {"Wed Evening", "4PM-12AM"},
        {"Wed Night", "12AM-8AM"}
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
    ambulances[ambulanceCount].isActive = (ambulanceCount < 3); // First 3 are active
    ambulanceCount++;
    
    // Assign to schedule if active
    if (amb.isActive && ambulanceCount <= totalSlots) {
        for (int i = 0; i < totalSlots; i++) {
            if (schedule[i].assignedAmbulanceId == 0) {
                schedule[i].assignedAmbulanceId = amb.id;
                break;
            }
        }
    }
    
    if (!loading) saveToFile();
    return true;
}

bool AmbulanceCircularQueue::rotateShift() {
    if (ambulanceCount == 0) return false;
    
    // Move to next time slot (for display purposes)
    currentSlot = (currentSlot + 1) % totalSlots;
    
    // Conveyor belt rotation - move the entire "window" of 3 people
    static int conveyorPosition = 0;
    conveyorPosition = (conveyorPosition + 1) % ambulanceCount;
    
    // Fill the 9-slot schedule with groups of 3 from the conveyor
    for (int slot = 0; slot < totalSlots; slot++) {
        int groupPosition = slot % 3; // Which position in the group of 3
        int ambulanceIndex = (conveyorPosition + groupPosition) % ambulanceCount;
        schedule[slot].assignedAmbulanceId = ambulances[ambulanceIndex].id;
    }
    
    return true;
}

void AmbulanceCircularQueue::assignInitialSchedule() {
    if (ambulanceCount == 0) return;
    
    // Initial conveyor position - show first 3 people
    for (int slot = 0; slot < totalSlots; slot++) {
        int groupPosition = slot % 3; // Which position in the group of 3
        int ambulanceIndex = groupPosition % ambulanceCount;
        schedule[slot].assignedAmbulanceId = ambulances[ambulanceIndex].id;
    }
}

int AmbulanceCircularQueue::findLeastWorkedAmbulance() {
    if (ambulanceCount == 0) return 0;
    
    int minShifts = ambulances[0].totalShifts;
    int selectedId = ambulances[0].id;
    
    for (int i = 1; i < ambulanceCount; i++) {
        if (ambulances[i].totalShifts < minShifts) {
            minShifts = ambulances[i].totalShifts;
            selectedId = ambulances[i].id;
        }
    }
    
    return selectedId;
}

void AmbulanceCircularQueue::displaySchedule() {
    if (ambulanceCount == 0) {
        cout << "No ambulances registered." << endl;
        return;
    }
    
    cout << "\n=== Ambulance Dispatch Schedule ===" << endl;
    cout << "Current Shift: " << schedule[currentSlot].shiftName 
         << " (" << schedule[currentSlot].timeRange << ")";
    
    // Find current ambulance name
    for (int i = 0; i < ambulanceCount; i++) {
        if (ambulances[i].id == schedule[currentSlot].assignedAmbulanceId) {
            cout << " - AMB" << ambulances[i].id << " (" << ambulances[i].driverName << ")";
            break;
        }
    }
    cout << "\n" << endl;
    
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
        if (shift == currentSlot) cout << "*";
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
        if (shift == currentSlot) cout << "*";
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
        if (shift == currentSlot) cout << "*";
        cout << "\t\t";
    }
    cout << endl;
    cout << "\n* = Current Shift" << endl;
    
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
        
        // Visual bar for current shifts
        for (int j = 0; j < currentShifts && j < 10; j++) {
            cout << "#";
        }
        for (int j = currentShifts; j < 10; j++) {
            cout << "-";
        }
        
        cout << " " << currentShifts << " shifts";
        if (currentShifts == 0) cout << " (standby)";
        cout << endl;
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
    
    file << "ID,DriverName,Status,TotalShifts,IsActive" << endl;
    for (int i = 0; i < ambulanceCount; i++) {
        file << ambulances[i].id << "," 
             << ambulances[i].driverName << "," 
             << ambulances[i].status << ","
             << ambulances[i].totalShifts << ","
             << (ambulances[i].isActive ? "YES" : "NO") << endl;
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
        size_t pos3 = line.find(',', pos2 + 1);
        size_t pos4 = line.find(',', pos3 + 1);
        
        if (pos1 != string::npos && pos4 != string::npos) {
            Ambulance amb;
            amb.id = stoi(line.substr(0, pos1));
            if (amb.id > maxId) maxId = amb.id;
            
            strcpy(amb.driverName, line.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
            strcpy(amb.status, line.substr(pos2 + 1, pos3 - pos2 - 1).c_str());
            amb.totalShifts = stoi(line.substr(pos3 + 1, pos4 - pos3 - 1));
            amb.isActive = (line.substr(pos4 + 1) == "YES");
            
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
                amb.totalShifts = 0;
                amb.isActive = true;
                
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
