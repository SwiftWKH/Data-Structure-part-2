# Hospital Patient Care Management System

## Team Structure
- **PatientAdmissionClerk**: Manages patient registration and discharge (Queue/FIFO)
- **MedicalSupplyManager**: Handles supply inventory (Stack/LIFO) 
- **EmergencyDepartmentOfficer**: Manages emergency cases by priority (Priority Queue)
- **AmbulanceDispatcher**: Schedules ambulance rotations (Circular Queue)

## Setup Instructions

### 1. Compilation
```bash
# Using Makefile
make

# Or manually
g++ -std=c++11 -o hospital_system main.cpp PatientAdmissionClerk/PatientAdmissionClerk.cpp MedicalSupplyManager/MedicalSupplyManager.cpp EmergencyDepartmentOfficer/EmergencyDepartmentOfficer.cpp AmbulanceDispatcher/AmbulanceDispatcher.cpp
```

### 2. Run Program
```bash
./hospital_system
```

### 3. Login Credentials
- member1/pass1 - Patient Admission Clerk
- member2/pass2 - Medical Supply Manager
- member3/pass3 - Emergency Department Officer
- member4/pass4 - Ambulance Dispatcher

## Development Guidelines

### Each Member Should:
1. Implement their 3 required functionalities in their respective folder
2. Choose appropriate data structure and justify the choice
3. Add data structures (no STL containers like vector, list)
4. Create CSV/text files for data storage if needed
5. Test their module thoroughly

### File Structure
```
├── main.cpp (login system - DO NOT MODIFY)
├── login_data.csv (authentication data)
├── [YourRole]/
│   ├── [YourRole].hpp (your header file)
│   └── [YourRole].cpp (your implementation)
```

### Submission Requirements
- ZIP file format: GroupNo_TeamLeaderID_Member1ID_Member2ID_Member3ID.zip
- Include only .cpp, .hpp, and csv/text files
- Individual 30-minute recording demonstration required

## Notes
- No STL containers allowed (no vector, list, etc.)
- Must use core data structures: Stack, Queue, Priority Queue, Circular Queue
- Each member gets 15 marks for implementation + 15 marks for Q&A justification
