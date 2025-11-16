CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGET = hospital_system
SOURCES = main.cpp \
          PatientAdmissionClerk/PatientAdmissionClerk.cpp \
          MedicalSupplyManager/MedicalSupplyManager.cpp \
          EmergencyDepartmentOfficer/EmergencyDepartmentOfficer.cpp \
          AmbulanceDispatcher/AmbulanceDispatcher.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean
