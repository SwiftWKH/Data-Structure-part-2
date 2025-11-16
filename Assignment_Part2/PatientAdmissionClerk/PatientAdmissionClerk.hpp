#ifndef PATIENTADMISSIONCLERK_HPP
#define PATIENTADMISSIONCLERK_HPP

#include <string>
using namespace std;

struct Patient {
    int id;
    string name;
    string condition;
};

struct QueueNode {
    Patient data;
    QueueNode* next;
    
    QueueNode(Patient patient) : data(patient), next(nullptr) {}
};

class PatientQueue {
private:
    QueueNode* front;
    QueueNode* rear;
    int count;
    int nextId;

public:
    PatientQueue();
    ~PatientQueue();
    bool isEmpty();
    void enqueue(string name, string condition);
    bool dequeue();
    void display();
    void saveToFile();
    void loadFromFile();
};

void patientAdmissionClerkMenu();

#endif
