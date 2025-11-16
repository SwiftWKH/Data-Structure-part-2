#include <iostream>
#include <fstream>
#include <cstring>
#include "AmbulanceDispatcher.hpp"
using namespace std;

AmbulanceCircularQueue ambulanceQueue;

AmbulanceCircularQueue::AmbulanceCircularQueue(int size) {
    capacity = size;
    queue = new Ambulance[capacity];
    front = rear = count = 0;
    nextId = 101;
    loading = false;
    loadFromFile();
}

AmbulanceCircularQueue::~AmbulanceCircularQueue() {
    delete[] queue;
}

bool AmbulanceCircularQueue::enqueue(const Ambulance& amb) {
    if (isFull()) return false;
    queue[rear] = amb;
    rear = (rear + 1) % capacity;
    count++;
    if (!loading) saveToFile();
    return true;
}

bool AmbulanceCircularQueue::rotate() {
    if (isEmpty()) return false;
    front = (front + 1) % capacity;
    return true;
}

void AmbulanceCircularQueue::display() {
    if (isEmpty()) {
        cout << "No ambulances in rotation." << endl;
        return;
    }
    
    cout << "\n--- Ambulance Schedule ---" << endl;
    cout << "ID\tDriver\t\tStatus" << endl;
    cout << "--------------------------------" << endl;
    
    int current = front;
    for (int i = 0; i < count; i++) {
        cout << queue[current].id << "\t" 
             << queue[current].driverName << "\t\t" 
             << queue[current].status << endl;
        current = (current + 1) % capacity;
    }
    
    if (count > 0) {
        cout << "\nCurrent on duty: " << queue[front].driverName 
             << " (ID: " << queue[front].id << ")" << endl;
    }
}

bool AmbulanceCircularQueue::isEmpty() {
    return count == 0;
}

bool AmbulanceCircularQueue::isFull() {
    return count == capacity;
}

void AmbulanceCircularQueue::saveToFile() {
    ofstream file("AmbulanceDispatcher/ambulances.csv");
    if (!file.is_open()) return;
    
    file << "ID,DriverName,Status" << endl;
    int current = front;
    for (int i = 0; i < count; i++) {
        file << queue[current].id << "," 
             << queue[current].driverName << "," 
             << queue[current].status << endl;
        current = (current + 1) % capacity;
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
    
    while (getline(file, line) && count < capacity) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        
        if (pos1 != string::npos && pos2 != string::npos) {
            Ambulance amb;
            amb.id = stoi(line.substr(0, pos1));
            if (amb.id > maxId) maxId = amb.id;
            
            string driverName = line.substr(pos1 + 1, pos2 - pos1 - 1);
            strcpy(amb.driverName, driverName.c_str());
            
            string status = line.substr(pos2 + 1);
            strcpy(amb.status, status.c_str());
            
            queue[rear] = amb;
            rear = (rear + 1) % capacity;
            count++;
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
                
                if (ambulanceQueue.enqueue(amb)) {
                    cout << "Ambulance registered successfully! ID: " << amb.id << endl;
                } else {
                    cout << "Failed to register ambulance!" << endl;
                }
                break;
            }
            case 2:
                if (ambulanceQueue.rotate()) {
                    cout << "Ambulance shift rotated successfully!" << endl;
                } else {
                    cout << "No ambulances to rotate!" << endl;
                }
                break;
            case 3:
                ambulanceQueue.display();
                break;
            case 4:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
