#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "EmergencyDepartmentOfficer.hpp"
using namespace std;

// Global Priority Queue instance for Emergency Department
EmergencyPriorityQueue emergencyQueue(100);

// EmergencyPriorityQueue Implementation (Max-Heap using Array)

// Constructor: Initialize the priority queue with given capacity
EmergencyPriorityQueue::EmergencyPriorityQueue(int maxCapacity) {
    capacity = maxCapacity;
    size = 0;
    heap = new EmergencyCase[capacity];
    loadFromFile();  // Load existing data from CSV file
}

// Destructor: Free allocated memory
EmergencyPriorityQueue::~EmergencyPriorityQueue() {
    delete[] heap;
}

// Get parent index of a node at given index
int EmergencyPriorityQueue::getParent(int index) {
    return (index - 1) / 2;
}

// Get left child index of a node at given index
int EmergencyPriorityQueue::getLeftChild(int index) {
    return (2 * index) + 1;
}

// Get right child index of a node at given index
int EmergencyPriorityQueue::getRightChild(int index) {
    return (2 * index) + 2;
}

// Swap two elements in the heap
void EmergencyPriorityQueue::swap(int i, int j) {
    EmergencyCase temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

// Heapify Up: Maintain min-heap property when inserting (bubble up)
// Lower priority number = higher urgency, so I use min-heap :)
void EmergencyPriorityQueue::heapifyUp(int index) {
    while (index > 0 && heap[getParent(index)].priority > heap[index].priority) {
        swap(index, getParent(index));
        index = getParent(index);
    }
}

// Heapify Down: Maintain min-heap property when removing (bubble down)
// Lower priority number = higher urgency
void EmergencyPriorityQueue::heapifyDown(int index) {
    int minIndex = index;
    int left = getLeftChild(index);
    int right = getRightChild(index);
    
    // Compare with left child (lower number = higher priority)
    if (left < size && heap[left].priority < heap[minIndex].priority) {
        minIndex = left;
    }
    
    // Compare with right child (lower number = higher priority)
    if (right < size && heap[right].priority < heap[minIndex].priority) {
        minIndex = right;
    }
    
    // If current node is not the minimum, swap and continue heapifying
    if (index != minIndex) {
        swap(index, minIndex);
        heapifyDown(minIndex);
    }
}

// Function 1: Log Emergency Case (Insert with priority)
bool EmergencyPriorityQueue::logEmergencyCase(string patientName, string emergencyType, int priority) {
    // Check if heap is full
    if (size >= capacity) {
        cout << "Error: Emergency queue is full! Cannot add more cases." << endl;
        return false;
    }
    
    // Validate priority (assuming 1-5 scale, where 1 is highest priority)
    if (priority < 1 || priority > 5) {
        cout << "Error: Priority must be between 1 (highest) and 5 (lowest)." << endl;
        return false;
    }
    
    // Create new emergency case
    EmergencyCase newCase;
    newCase.patientName = patientName;
    newCase.emergencyType = emergencyType;
    newCase.priority = priority;
    
    // Insert at the end of the heap
    heap[size] = newCase;
    size++;
    
    // Maintain heap property by bubbling up
    heapifyUp(size - 1);
    
    // Save to file after adding
    saveToFile();
    
    return true;
}

// Function 2: Process Most Critical Case (Extract minimum priority number = highest urgency)
bool EmergencyPriorityQueue::processMostCriticalCase(EmergencyCase& processedCase) {
    // Check if queue is empty
    if (isEmpty()) {
        return false;
    }
    
    // The root of min-heap contains the highest priority case (lowest priority number)
    processedCase = heap[0];
    
    // Replace root with last element
    heap[0] = heap[size - 1];
    size--;
    
    // Maintain heap property by bubbling down
    if (size > 0) {
        heapifyDown(0);
    }
    
    // Save to file after removing
    saveToFile();
    
    return true;
}

// Function 3: View Pending Emergency Cases (Display in priority order)
void EmergencyPriorityQueue::viewPendingEmergencyCases() {
    if (isEmpty()) {
        cout << "\nNo pending emergency cases." << endl;
        return;
    }
    
    // Create a temporary copy of the heap to extract and display in sorted order
    // without modifying the original heap
    EmergencyCase* tempHeap = new EmergencyCase[capacity];
    int tempSize = size;
    
    // Copy the heap
    for (int i = 0; i < size; i++) {
        tempHeap[i] = heap[i];
    }
    
    cout << "\n========================================" << endl;
    cout << "   PENDING EMERGENCY CASES (Priority Order)" << endl;
    cout << "========================================" << endl;
    cout << left << setw(25) << "Patient Name" 
         << setw(25) << "Emergency Type" 
         << setw(15) << "Priority Level" << endl;
    cout << "----------------------------------------" << endl;
    
    // Extract and display cases in priority order (highest first)
    // Continue until all cases are displayed
    while (tempSize > 0) {
        // Extract the root (highest priority - lowest priority number)
        EmergencyCase current = tempHeap[0];
        
        // Display the case
        string priorityLabel;
        switch(current.priority) {
            case 1: priorityLabel = "Critical (1)"; break;
            case 2: priorityLabel = "High (2)"; break;
            case 3: priorityLabel = "Medium (3)"; break;
            case 4: priorityLabel = "Low (4)"; break;
            case 5: priorityLabel = "Minor (5)"; break;
            default: priorityLabel = "Unknown"; break;
        }
        
        cout << left << setw(25) << current.patientName
             << setw(25) << current.emergencyType
             << setw(15) << priorityLabel << endl;
        
        // If only one element left, breakkk
        if (tempSize == 1) {
            tempSize = 0;
            break;
        }
        
        // Remove root and maintain heap property
        tempHeap[0] = tempHeap[tempSize - 1];
        tempSize--;
        
        // Heapify down on temporary heap
        int index = 0;
        while (index < tempSize) {
            int minIndex = index;
            int left = (2 * index) + 1;
            int right = (2 * index) + 2;
            
            if (left < tempSize && tempHeap[left].priority < tempHeap[minIndex].priority) {
                minIndex = left;
            }
            if (right < tempSize && tempHeap[right].priority < tempHeap[minIndex].priority) {
                minIndex = right;
            }
            
            if (index != minIndex) {
                EmergencyCase temp = tempHeap[index];
                tempHeap[index] = tempHeap[minIndex];
                tempHeap[minIndex] = temp;
                index = minIndex;
            } else {
                break;
            }
        }
    }
    
    delete[] tempHeap;
    
    cout << "========================================" << endl;
    cout << "Total pending cases: " << size << endl;
}

// Check if the priority queue is empty
bool EmergencyPriorityQueue::isEmpty() {
    return size == 0;
}

// Get current number of cases
int EmergencyPriorityQueue::getSize() {
    return size;
}

// Load emergency cases from CSV file
void EmergencyPriorityQueue::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        // Try alternative path (in case program runs from different directory)
        string altPath = "emergency_cases.csv";
        file.open(altPath);
        if (!file.is_open()) {
            // File doesn't exist yet, start with empty queue
            return;
        }
    }
    
    string line;
    getline(file, line); // Skip header line
    
    int loadedCount = 0;
    
    // Read each line and add to priority queue
    while (getline(file, line) && size < capacity) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string patientName, emergencyType, priorityStr;
        
        // Parse CSV line: PatientName,EmergencyType,Priority
        getline(ss, patientName, ',');
        getline(ss, emergencyType, ',');
        getline(ss, priorityStr, ',');
        
        // Trim whitespace if any
        if (!patientName.empty() && !emergencyType.empty() && !priorityStr.empty()) {
            try {
                int priority = stoi(priorityStr);
                
                // Validate priority
                if (priority >= 1 && priority <= 5) {
                    // Insert directly into heap (we'll heapify after loading all)
                    heap[size].patientName = patientName;
                    heap[size].emergencyType = emergencyType;
                    heap[size].priority = priority;
                    size++;
                    loadedCount++;
                }
            } catch (...) {
                // Skip invalid priority values
                continue;
            }
        }
    }
    
    file.close();
    
    // Rebuild heap property after loading all data
    // Start from the last parent node and heapify down
    if (size > 0) {
        for (int i = (size - 2) / 2; i >= 0; i--) {
            heapifyDown(i);
        }
    }
}

// Save emergency cases to CSV file
void EmergencyPriorityQueue::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Warning: Could not save to file: " << filename << endl;
        return;
    }
    
    // Write header
    file << "PatientName,EmergencyType,Priority" << endl;
    
    // Create a temporary copy to extract in sorted order without modifying original
    EmergencyCase* tempHeap = new EmergencyCase[capacity];
    int tempSize = size;
    
    // Copy the heap
    for (int i = 0; i < size; i++) {
        tempHeap[i] = heap[i];
    }
    
    // Extract and save cases in priority order
    while (tempSize > 0) {
        // Extract the root (highest priority)
        EmergencyCase current = tempHeap[0];
        
        // Write to file
        file << current.patientName << ","
             << current.emergencyType << ","
             << current.priority << endl;
        
        // Remove root and maintain heap property
        tempHeap[0] = tempHeap[tempSize - 1];
        tempSize--;
        
        // Heapify down on temporary heap
        int index = 0;
        while (true) {
            int minIndex = index;
            int left = (2 * index) + 1;
            int right = (2 * index) + 2;
            
            if (left < tempSize && tempHeap[left].priority < tempHeap[minIndex].priority) {
                minIndex = left;
            }
            if (right < tempSize && tempHeap[right].priority < tempHeap[minIndex].priority) {
                minIndex = right;
            }
            
            if (index != minIndex) {
                EmergencyCase temp = tempHeap[index];
                tempHeap[index] = tempHeap[minIndex];
                tempHeap[minIndex] = temp;
                index = minIndex;
            } else {
                break;
            }
        }
    }
    
    delete[] tempHeap;
    file.close();
}

// Emergency Department Officer Menu

void emergencyDepartmentOfficerMenu() {
    int choice;
    string patientName, emergencyType;
    int priority;
    EmergencyCase processedCase;
    
    while (true) {
        cout << "\n--- Emergency Department Officer Menu ---" << endl;
        cout << "1. Log Emergency Case" << endl;
        cout << "2. Process Most Critical Case" << endl;
        cout << "3. View Pending Emergency Cases" << endl;
        cout << "4. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        // Clear input buffer
        cin.ignore();
        
        switch (choice) {
            case 1: {
                // Log Emergency Case
                cout << "\n--- Log Emergency Case ---" << endl;
                cout << "Enter patient name: ";
                getline(cin, patientName);
                
                cout << "Enter type of emergency: ";
                getline(cin, emergencyType);
                
                cout << "Enter priority level (1=Critical, 2=High, 3=Medium, 4=Low, 5=Minor): ";
                cin >> priority;
                
                if (emergencyQueue.logEmergencyCase(patientName, emergencyType, priority)) {
                    cout << "\n✓ Emergency case logged successfully!" << endl;
                } else {
                    cout << "\n✗ Failed to log emergency case." << endl;
                }
                break;
            }
            
            case 2: {
                // Process Most Critical Case
                cout << "\n--- Process Most Critical Case ---" << endl;
                
                if (emergencyQueue.processMostCriticalCase(processedCase)) {
                    cout << "\n✓ Processing most critical case:" << endl;
                    cout << "  Patient Name: " << processedCase.patientName << endl;
                    cout << "  Emergency Type: " << processedCase.emergencyType << endl;
                    cout << "  Priority Level: " << processedCase.priority << endl;
                    cout << "\nCase has been processed and removed from queue." << endl;
                } else {
                    cout << "\n✗ No emergency cases available to process." << endl;
                }
                break;
            }
            
            case 3: {
                // View Pending Emergency Cases
                emergencyQueue.viewPendingEmergencyCases();
                break;
            }
            
            case 4: {
                cout << "Logging out..." << endl;
                return;
            }
            
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
