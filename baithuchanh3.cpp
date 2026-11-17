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

