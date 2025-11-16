#include <iostream>
#include <fstream>
#include <limits>
#include "PatientAdmissionClerk.hpp"
using namespace std;

PatientQueue patientQueue;

void clearInput() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Constructor
PatientQueue::PatientQueue() {
    front = nullptr;
    rear = nullptr;
    count = 0;
    nextId = 1001;
    loadFromFile();
}

// Destructor
PatientQueue::~PatientQueue() {
    cout << "\n(PatientAdmissionClerk: Cleaning up queue...)" << endl;
    while (!isEmpty()) {
        QueueNode* temp = front;
        front = front->next;
        delete temp;
    }
}

bool PatientQueue::isEmpty() {
    return front == nullptr;
}

// Enqueue - Add patient to rear of queue
void PatientQueue::enqueue(string name, string condition) {
    Patient newPatient;
    newPatient.id = nextId++;
    newPatient.name = name;
    newPatient.condition = condition;
    
    QueueNode* newNode = new QueueNode(newPatient);
    
    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    
    count++;
    cout << "\n[SUCCESS] Patient admitted: " << name << " (ID: " << newPatient.id << ")" << endl;
    saveToFile();
}

// Dequeue - Remove patient from front of queue
bool PatientQueue::dequeue() {
    if (isEmpty()) {
        cout << "\n[EMPTY] No patients in queue to discharge." << endl;
        return false;
    }
    
    QueueNode* temp = front;
    Patient discharged = temp->data;
    
    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }
    
    delete temp;
    count--;
    
    cout << "\n[DISCHARGED] Patient: " << discharged.name 
         << " (ID: " << discharged.id << ") - " << discharged.condition << endl;
    
    saveToFile();
    return true;
}

// Display all patients in queue
void PatientQueue::display() {
    if (isEmpty()) {
        cout << "\n[EMPTY] No patients currently in queue." << endl;
        return;
    }
    
    cout << "\n--- Patient Queue (FIFO) ---" << endl;
    cout << "Total patients waiting: " << count << endl;
    cout << "--------------------------------" << endl;
    
    QueueNode* current = front;
    int position = 1;
    
    while (current != nullptr) {
        cout << position << ". ID: " << current->data.id 
             << " | " << current->data.name 
             << " | " << current->data.condition << endl;
        current = current->next;
        position++;
    }
    cout << "--------------------------------" << endl;
}

void PatientQueue::saveToFile() {
    ofstream file("PatientAdmissionClerk/patients.csv");
    if (!file.is_open()) return;
    
    file << "ID,Name,Condition" << endl;
    
    QueueNode* current = front;
    while (current != nullptr) {
        file << current->data.id << "," 
             << current->data.name << "," 
             << current->data.condition << endl;
        current = current->next;
    }
    file.close();
}

void PatientQueue::loadFromFile() {
    ifstream file("PatientAdmissionClerk/patients.csv");
    if (!file.is_open()) return;
    
    string line;
    getline(file, line); // Skip header
    
    int maxId = 1000;
    
    while (getline(file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        
        if (pos1 != string::npos && pos2 != string::npos) {
            int id = stoi(line.substr(0, pos1));
            string name = line.substr(pos1 + 1, pos2 - pos1 - 1);
            string condition = line.substr(pos2 + 1);
            
            if (id > maxId) maxId = id;
            
            Patient patient = {id, name, condition};
            QueueNode* newNode = new QueueNode(patient);
            
            if (isEmpty()) {
                front = rear = newNode;
            } else {
                rear->next = newNode;
                rear = newNode;
            }
            count++;
        }
    }
    
    nextId = maxId + 1;
    file.close();
}

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
            case 1: {
                string name, condition;
                cout << "Enter Patient Name: ";
                clearInput();
                getline(cin, name);
                cout << "Enter Condition: ";
                getline(cin, condition);
                
                if (!name.empty() && !condition.empty()) {
                    patientQueue.enqueue(name, condition);
                } else {
                    cout << "\n[ERROR] Name and condition cannot be empty." << endl;
                }
                break;
            }
            case 2:
                patientQueue.dequeue();
                break;
            case 3:
                patientQueue.display();
                break;
            case 4:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
