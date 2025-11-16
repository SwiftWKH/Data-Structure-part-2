#ifndef AMBULANCEDISPATCHER_HPP
#define AMBULANCEDISPATCHER_HPP

struct Ambulance {
    int id;
    char driverName[50];
    char status[20];
};

struct TimeSlot {
    char shiftName[20];
    char timeRange[20];
    int assignedAmbulanceId;
};

class AmbulanceCircularQueue {
private:
    Ambulance* ambulances;
    TimeSlot* schedule;
    int ambulanceCount, maxAmbulances;
    int currentSlot, totalSlots;
    int nextId;
    bool loading;
    
public:
    AmbulanceCircularQueue(int maxAmb = 10);
    ~AmbulanceCircularQueue();
    bool registerAmbulance(const Ambulance& amb);
    bool rotateShift();
    void displaySchedule();
    bool isEmpty();
    bool isFull();
    void saveToFile();
    void loadFromFile();
    int getNextId();
    void initializeSchedule();
    void assignInitialSchedule();
};

void ambulanceDispatcherMenu();

#endif
