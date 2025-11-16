#ifndef AMBULANCEDISPATCHER_HPP
#define AMBULANCEDISPATCHER_HPP

struct Ambulance {
    int id;
    char driverName[50];
    char status[20];
};

class AmbulanceCircularQueue {
private:
    Ambulance* queue;
    int front, rear, capacity, count;
    int nextId;
    bool loading;
    
public:
    AmbulanceCircularQueue(int size = 10);
    ~AmbulanceCircularQueue();
    bool enqueue(const Ambulance& amb);
    bool rotate();
    void display();
    bool isEmpty();
    bool isFull();
    void saveToFile();
    void loadFromFile();
    int getNextId();
};

void ambulanceDispatcherMenu();

#endif
