# Data Structure Implementation Examples

## Patient Admission Clerk - Queue (FIFO)
```cpp
struct Patient {
    int id;
    string name;
    string condition;
};

class PatientQueue {
private:
    Patient* queue;
    int front, rear, capacity;
public:
    // Implement enqueue, dequeue, display
};
```

## Medical Supply Manager - Stack (LIFO)
```cpp
struct Supply {
    string type;
    int quantity;
    string batch;
};

class SupplyStack {
private:
    Supply* stack;
    int top, capacity;
public:
    // Implement push, pop, display
};
```

## Emergency Department Officer - Priority Queue
```cpp
struct Emergency {
    string patientName;
    string emergencyType;
    int priority; // 1=highest, 5=lowest
};

class EmergencyPriorityQueue {
private:
    Emergency* heap;
    int size, capacity;
public:
    // Implement insert, extractMax, display
};
```

## Ambulance Dispatcher - Circular Queue
```cpp
struct Ambulance {
    int id;
    string driverName;
    bool isActive;
};

class AmbulanceCircularQueue {
private:
    Ambulance* queue;
    int front, rear, capacity;
public:
    // Implement enqueue, rotate, display
};
```
