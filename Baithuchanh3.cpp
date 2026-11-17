#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <limits>

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
                delete vehicles[i];
                vehicles[i] = nullptr;
            }
            delete[] vehicles;
            vehicles = nullptr;
        }
        currentVehicleCount = 0;
        capacity = 0;
    }

public:
    ParkingLot()
        : parkID(0), size('S'), capacity(1),
          currentVehicleCount(0), vehicles(nullptr) {
        vehicles = new Vehicle*[capacity];
        initializeArray();
    }

    ParkingLot(int id, char s)
        : parkID(id), size(s),
          capacity(s == 'S' ? 1 : (s == 'M' ? 2 : 3)),
          currentVehicleCount(0), vehicles(nullptr) {
        vehicles = new Vehicle*[capacity];
        initializeArray();
    }
    ParkingLot(const ParkingLot& other)
        : parkID(other.parkID), size(other.size),
          capacity(other.capacity),
          currentVehicleCount(other.currentVehicleCount),
          vehicles(nullptr) {
        vehicles = new Vehicle*[capacity];
        initializeArray();

        try {
            for (int i = 0; i < currentVehicleCount; ++i) {
                vehicles[i] = new Vehicle(*other.vehicles[i]);
            }
        } catch (...) {
            for (int i = 0; i < currentVehicleCount; ++i) {
                delete vehicles[i];
                vehicles[i] = nullptr;
            }
            delete[] vehicles;
            vehicles = nullptr;
            throw;
        }
    }
    ParkingLot& operator=(const ParkingLot& other) {
        if (this == &other) {
            return *this;
        }

        Vehicle** newVehicles = new Vehicle*[other.capacity];
        for (int i = 0; i < other.capacity; ++i) {
            newVehicles[i] = nullptr;
        }

        try {
            for (int i = 0; i < other.currentVehicleCount; ++i) {
                newVehicles[i] = new Vehicle(*other.vehicles[i]);
            }
        } catch (...) {
            for (int i = 0; i < other.currentVehicleCount; ++i) {
                delete newVehicles[i];
            }
            delete[] newVehicles;
            throw;
        }

        releaseMemory();

        parkID = other.parkID;
        size = other.size;
        capacity = other.capacity;
        currentVehicleCount = other.currentVehicleCount;
        vehicles = newVehicles;

        return *this;
    }
    ~ParkingLot() {
        releaseMemory();
    }

    int getParkID() const { return parkID; }
    char getSize() const { return size; }
    int getCurrentVehicleCount() const { return currentVehicleCount; }
    int getCapacity() const { return capacity; }

    bool containsVehicle(int vehicleID) const {
        for (int i = 0; i < currentVehicleCount; ++i) {
            if (vehicles[i] != nullptr &&
                vehicles[i]->getVehicleID() == vehicleID) {
                return true;
            }
        }
        return false;
    }
    void resize(int newCapacity) {
        if (newCapacity <= capacity) {
            return;
        }

        Vehicle** newVehicles = new Vehicle*[newCapacity];
        for (int i = 0; i < newCapacity; ++i) {
            newVehicles[i] = nullptr;
        }

        for (int i = 0; i < currentVehicleCount; ++i) {
            newVehicles[i] = vehicles[i];
        }

        delete[] vehicles;
        vehicles = newVehicles;
        capacity = newCapacity;
    }

    bool addVehicle(const Vehicle& vehicle) {
        if (containsVehicle(vehicle.getVehicleID())) {
            return false;
        }

        if (currentVehicleCount == capacity) {
            resize(capacity * 2);
        }

        vehicles[currentVehicleCount] = new Vehicle(vehicle);
        ++currentVehicleCount;
        return true;
    }

    bool removeVehicle(int vehicleID) {
        int index = -1;

        for (int i = 0; i < currentVehicleCount; ++i) {
            if (vehicles[i] != nullptr &&
                vehicles[i]->getVehicleID() == vehicleID) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            return false;
        }

        delete vehicles[index];
        vehicles[index] = nullptr;

        for (int i = index; i < currentVehicleCount - 1; ++i) {
            vehicles[i] = vehicles[i + 1];
        }

        --currentVehicleCount;
        vehicles[currentVehicleCount] = nullptr;
        return true;
    }

    void display() const {
        cout << "\nBai " << parkID << " [" << size << "] "
             << currentVehicleCount << "/" << capacity << " xe\n";

        if (currentVehicleCount == 0) {
            cout << "  Bai chua co xe.\n";
            return;
        }

        for (int i = 0; i < currentVehicleCount; ++i) {
            if (vehicles[i] != nullptr) {
                cout << "\n--- Xe thu " << i + 1 << " ---\n";
                vehicles[i]->display();
            }
        }
    }

    void displayVehiclesByType(const string& type) const {
        for (int i = 0; i < currentVehicleCount; ++i) {
            if (vehicles[i] != nullptr &&
                vehicles[i]->getVehicleType() == type) {
                vehicles[i]->display();
                cout << "----------------------\n";
            }
        }
    }

    int countVehiclesByType(const string& type) const {
        int count = 0;
        for (int i = 0; i < currentVehicleCount; ++i) {
            if (vehicles[i] != nullptr &&
                vehicles[i]->getVehicleType() == type) {
                ++count;
            }
        }
        return count;
    }

    double totalFee() const {
        double total = 0.0;
        for (int i = 0; i < currentVehicleCount; ++i) {
            if (vehicles[i] != nullptr) {
                total += vehicles[i]->getParkingFee();
            }
        }
        return total;
    }
};
class SystemInterface {
private:
    int lotCapacity;
    int currentLotCount;
    ParkingLot** parkingLots;

    void initializeArray() {
        for (int i = 0; i < lotCapacity; ++i) {
            parkingLots[i] = nullptr;
        }
    }

    void releaseMemory() {
        if (parkingLots != nullptr) {
            for (int i = 0; i < currentLotCount; ++i) {
                delete parkingLots[i];
                parkingLots[i] = nullptr;
            }
            delete[] parkingLots;
            parkingLots = nullptr;
        }
        currentLotCount = 0;
        lotCapacity = 0;
    }

    int findLotIndex(int id) const {
        for (int i = 0; i < currentLotCount; ++i) {
            if (parkingLots[i] != nullptr &&
                parkingLots[i]->getParkID() == id) {
                return i;
            }
        }
        return -1;
    }

    bool vehicleIDExists(int id) const {
        for (int i = 0; i < currentLotCount; ++i) {
            if (parkingLots[i] != nullptr &&
                parkingLots[i]->containsVehicle(id)) {
                return true;
            }
        }
        return false;
    }

public:
    SystemInterface()
        : lotCapacity(5), currentLotCount(0), parkingLots(nullptr) {
        parkingLots = new ParkingLot*[lotCapacity];
        initializeArray();
    }
    SystemInterface(const SystemInterface& other)
        : lotCapacity(other.lotCapacity),
          currentLotCount(other.currentLotCount),
          parkingLots(nullptr) {
        parkingLots = new ParkingLot*[lotCapacity];
        initializeArray();

        try {
            for (int i = 0; i < currentLotCount; ++i) {
                parkingLots[i] = new ParkingLot(*other.parkingLots[i]);
            }
        } catch (...) {
            for (int i = 0; i < currentLotCount; ++i) {
                delete parkingLots[i];
                parkingLots[i] = nullptr;
            }
            delete[] parkingLots;
            parkingLots = nullptr;
            throw;
        }
    }
    SystemInterface& operator=(const SystemInterface& other) {
        if (this == &other) {
            return *this;
        }

        ParkingLot** newLots = new ParkingLot*[other.lotCapacity];
        for (int i = 0; i < other.lotCapacity; ++i) {
            newLots[i] = nullptr;
        }

        try {
            for (int i = 0; i < other.currentLotCount; ++i) {
                newLots[i] = new ParkingLot(*other.parkingLots[i]);
            }
        } catch (...) {
            for (int i = 0; i < other.currentLotCount; ++i) {
                delete newLots[i];
            }
            delete[] newLots;
            throw;
        }

        releaseMemory();

        lotCapacity = other.lotCapacity;
        currentLotCount = other.currentLotCount;
        parkingLots = newLots;

        return *this;
    }
    ~SystemInterface() {
        releaseMemory();
    }

    void resizeSystem(int newCapacity) {
        if (newCapacity <= lotCapacity) {
            return;
        }

        ParkingLot** newLots = new ParkingLot*[newCapacity];
        for (int i = 0; i < newCapacity; ++i) {
            newLots[i] = nullptr;
        }

        for (int i = 0; i < currentLotCount; ++i) {
            newLots[i] = parkingLots[i];
        }

        delete[] parkingLots;
        parkingLots = newLots;
        lotCapacity = newCapacity;
    }

    bool addParkingLot(int id, char size) {
        if (findLotIndex(id) != -1) {
            return false;
        }

        if (currentLotCount == lotCapacity) {
            resizeSystem(lotCapacity * 2);
        }

        parkingLots[currentLotCount] = new ParkingLot(id, size);
        ++currentLotCount;
        return true;
    }

    bool removeParkingLot(int id) {
        int index = findLotIndex(id);
        if (index == -1) {
            return false;
        }

        delete parkingLots[index];
        parkingLots[index] = nullptr;

        for (int i = index; i < currentLotCount - 1; ++i) {
            parkingLots[i] = parkingLots[i + 1];
        }

        --currentLotCount;
        parkingLots[currentLotCount] = nullptr;
        return true;
    }

    bool addVehicleToLot(int lotID, const Vehicle& vehicle) {
        int index = findLotIndex(lotID);
        if (index == -1 || vehicleIDExists(vehicle.getVehicleID())) {
            return false;
        }
        return parkingLots[index]->addVehicle(vehicle);
    }

    bool removeVehicleFromSystem(int vehicleID) {
        for (int i = 0; i < currentLotCount; ++i) {
            if (parkingLots[i]->removeVehicle(vehicleID)) {
                return true;
            }
        }
        return false;
    }

    void displayAllLots() const {
        if (currentLotCount == 0) {
            cout << "Chua co bai do nao!\n";
            return;
        }

        for (int i = 0; i < currentLotCount; ++i) {
            if (parkingLots[i] != nullptr) {
                parkingLots[i]->display();
            }
        }
    }

    void displayLot(int id) const {
        int index = findLotIndex(id);
        if (index == -1) {
            cout << "Khong tim thay bai!\n";
            return;
        }
        parkingLots[index]->display();
    }

    void listVehiclesByType(const string& type) const {
        bool found = false;

        for (int i = 0; i < currentLotCount; ++i) {
            int count = parkingLots[i]->countVehiclesByType(type);
            if (count > 0) {
                cout << "\nBai " << parkingLots[i]->getParkID() << ":\n";
                parkingLots[i]->displayVehiclesByType(type);
                found = true;
            }
        }

        if (!found) {
            cout << "Khong co xe thuoc loai nay!\n";
        }
    }

    void showAverageFee() const {
        int vehicleCount = 0;
        double total = 0.0;

        for (int i = 0; i < currentLotCount; ++i) {
            vehicleCount += parkingLots[i]->getCurrentVehicleCount();
            total += parkingLots[i]->totalFee();
        }

        if (vehicleCount == 0) {
            cout << "Chua co xe nao!\n";
            return;
        }

        cout << fixed << setprecision(2);
        cout << "Tong so xe: " << vehicleCount << '\n';
        cout << "Tong phi: " << total << "$\n";
        cout << "Phi trung binh: "
             << total / vehicleCount << "$\n";
    }

    void runMenu() {
        int choice;

        while (true) {
            cout << "\n========== QUAN LY BAI DO XE ==========\n";
            cout << "1. Tao bai do xe moi\n";
            cout << "2. Them xe vao bai do\n";
            cout << "3. Xoa mot bai do xe\n";
            cout << "4. Lay xe ra khoi bai do\n";
            cout << "5. Hien thi danh sach tat ca bai do\n";
            cout << "6. Hien thi danh sach xe theo bai\n";
            cout << "7. Liet ke xe theo loai phuong tien\n";
            cout << "8. Tom tat phi do xe trung binh\n";
            cout << "9. Thoat chuong trinh\n";
            cout << "Lua chon cua ban: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Vui long nhap mot so tu 1 den 9!\n";
                continue;
            }

            if (choice == 1) {
                int id;
                char lotSize;

                cout << "Nhap ma bai: ";
                cin >> id;
                cout << "Nhap kich thuoc bai (S/M/L): ";
                cin >> lotSize;
                lotSize = static_cast<char>(
                    toupper(static_cast<unsigned char>(lotSize)));

                if (lotSize != 'S' && lotSize != 'M' && lotSize != 'L') {
                    cout << "Kich thuoc khong hop le!\n";
                    continue;
                }

                if (addParkingLot(id, lotSize)) {
                    cout << "Tao bai do thanh cong!\n";
                } else {
                    cout << "Ma bai da ton tai!\n";
                }
            }
            else if (choice == 2) {
                int lotID, vehicleID, entryTime, duration;
                string type;

                cout << "Nhap ma bai: ";
                cin >> lotID;
                cout << "Nhap ma xe: ";
                cin >> vehicleID;
                cout << "Nhap loai xe (car/motorcycle/bicycle): ";
                cin >> type;

                for (size_t i = 0; i < type.length(); ++i) {
                    type[i] = static_cast<char>(
                        tolower(static_cast<unsigned char>(type[i])));
                }

                if (type != "car" && type != "motorcycle" &&
                    type != "bicycle") {
                    cout << "Loai xe khong hop le!\n";
                    continue;
                }

                cout << "Nhap gio vao (0-23): ";
                cin >> entryTime;
                cout << "Nhap thoi gian do (gio): ";
                cin >> duration;

                if (entryTime < 0 || entryTime > 23 || duration <= 0) {
                    cout << "Thoi gian khong hop le!\n";
                    continue;
                }

                Vehicle vehicle(vehicleID, type, entryTime, duration);

                if (addVehicleToLot(lotID, vehicle)) {
                    cout << "Them xe thanh cong!\n";
                } else {
                    cout << "Khong the them xe: bai khong ton tai "
                         << "hoac ma xe da duoc su dung!\n";
                }
            }
            else if (choice == 3) {
                int id;
                cout << "Nhap ma bai can xoa: ";
                cin >> id;

                if (removeParkingLot(id)) {
                    cout << "Da xoa bai do va giai phong cac xe trong bai!\n";
                } else {
                    cout << "Khong tim thay bai do!\n";
                }
            }
            else if (choice == 4) {
                int id;
                cout << "Nhap ma xe can lay ra: ";
                cin >> id;

                if (removeVehicleFromSystem(id)) {
                    cout << "Lay xe thanh cong!\n";
                } else {
                    cout << "Khong tim thay xe!\n";
                }
            }
            else if (choice == 5) {
                displayAllLots();
            }
            else if (choice == 6) {
                int id;
                cout << "Nhap ma bai can xem: ";
                cin >> id;
                displayLot(id);
            }
            else if (choice == 7) {
                string type;
                cout << "Nhap loai xe (car/motorcycle/bicycle): ";
                cin >> type;

                for (size_t i = 0; i < type.length(); ++i) {
                    type[i] = static_cast<char>(
                        tolower(static_cast<unsigned char>(type[i])));
                }

                if (type != "car" && type != "motorcycle" &&
                    type != "bicycle") {
                    cout << "Loai xe khong hop le!\n";
                    continue;
                }

                listVehiclesByType(type);
            }
            else if (choice == 8) {
                showAverageFee();
            }
            else if (choice == 9) {
                cout << "Ket thuc chuong trinh!\n";
                break;
            }
            else {
                cout << "Lua chon khong hop le! Vui long chon tu 1 den 9.\n";
            }
        }
    }
};

int main() {
    SystemInterface system;
    system.runMenu();
    return 0;
}
