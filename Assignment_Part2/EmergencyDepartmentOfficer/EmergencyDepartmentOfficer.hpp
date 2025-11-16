#ifndef EMERGENCYDEPARTMENTOFFICER_HPP
#define EMERGENCYDEPARTMENTOFFICER_HPP

#include <string>
using namespace std;

// Emergency Case Structure
struct EmergencyCase {
    string patientName;
    string emergencyType;
    int priority;  // Lower number = higher priority (1=Critical, 2=High, 3=Medium, 4=Low, 5=Minor)
};

// Priority Queue Class (Min-Heap Implementation using Array)
// Note: Lower priority number = higher urgency (1=Critical, 5=Minor)
class EmergencyPriorityQueue {
private:
    EmergencyCase* heap;      // Array to store emergency cases (heap structure)
    int size;                 // Current number of cases
    int capacity;             // Maximum capacity of the heap
    
    // Helper functions for heap operations
    void heapifyUp(int index);      // Maintain heap property when inserting
    void heapifyDown(int index);    // Maintain heap property when removing
    int getParent(int index);       // Get parent index
    int getLeftChild(int index);    // Get left child index
    int getRightChild(int index);   // Get right child index
    void swap(int i, int j);        // Swap two elements in the heap
    
public:
    // Constructor and Destructor
    EmergencyPriorityQueue(int maxCapacity = 100);
    ~EmergencyPriorityQueue();
    
    // Core functionalities
    bool logEmergencyCase(string patientName, string emergencyType, int priority);
    bool processMostCriticalCase(EmergencyCase& processedCase);
    void viewPendingEmergencyCases();
    bool isEmpty();
    int getSize();
    
    // File I/O operations
    void loadFromFile(const string& filename = "EmergencyDepartmentOfficer/emergency_cases.csv");
    void saveToFile(const string& filename = "EmergencyDepartmentOfficer/emergency_cases.csv");
};

// Emergency Department Officer Menu Function
void emergencyDepartmentOfficerMenu();

#endif
