#include <iostream>
#include <string>
#include <iomanip>

using namespace std;
class Vehicle {
private:
    int vehicleID;
    string vehicleType;
    int entryTime;
    int parkingDuration;
    double parkingFee;

public:
    Vehicle()
        : vehicleID(0), vehicleType(""), entryTime(0),
          parkingDuration(0), parkingFee(0.0) {}

    Vehicle(int id, const string& type, int entry, int duration)
        : vehicleID(id), vehicleType(type), entryTime(entry),
          parkingDuration(duration), parkingFee(0.0) {
        parkingFee = calculateFee();
    }

    int getVehicleID() const { return vehicleID; }
    string getVehicleType() const { return vehicleType; }
    int getEntryTime() const { return entryTime; }
    int getParkingDuration() const { return parkingDuration; }
    double getParkingFee() const { return parkingFee; }

    void setVehicleID(int id) { vehicleID = id; }
    void setVehicleType(const string& type) {
        vehicleType = type;
        parkingFee = calculateFee();
    }
    void setEntryTime(int time) {
        entryTime = time;
        parkingFee = calculateFee();
    }
    void setParkingDuration(int duration) {
        parkingDuration = duration;
        parkingFee = calculateFee();
    }
    double calculateFee() const {
        double unitPrice = 0.0;
        double totalFee = 0.0;

        if (vehicleType == "car") {
            unitPrice = 6.0;
        } else if (vehicleType == "motorcycle") {
            unitPrice = 4.0;
        } else if (vehicleType == "bicycle") {
            unitPrice = 2.0;
        }

        for (int i = 0; i < parkingDuration; ++i) {
            int hour = (entryTime + i) % 24;
            bool peakHour = (hour >= 6 && hour <= 10) ||
                            (hour >= 15 && hour <= 18);

            if (i <= 2) {
                totalFee += peakHour ? unitPrice * 1.3
                                     : unitPrice * 0.95;
            } else {
                totalFee += unitPrice * 0.9;
            }
        }
        return totalFee;
    }

    void display() const {
        cout << "Ma xe: " << vehicleID << '\n'
             << "Loai xe: " << vehicleType << '\n'
             << "Gio vao: " << entryTime << "h\n"
             << "Thoi gian do xe: " << parkingDuration << "h\n"
             << "Phi do xe: " << fixed << setprecision(2)
             << parkingFee << "$\n";
    }
};
class ParkingLot {
private:
    int parkID;
    char size;
    int capacity;
    int currentVehicleCount;
    Vehicle** vehicles;

    void initializeArray() {
        for (int i = 0; i < capacity; ++i) {
            vehicles[i] = nullptr;
        }
    }

    void releaseMemory() {
        if (vehicles != nullptr) {
            for (int i = 0; i < currentVehicleCount; ++i) {
