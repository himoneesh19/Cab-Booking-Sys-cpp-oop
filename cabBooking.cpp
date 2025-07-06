
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <sstream>
using namespace std;

class sys {
protected:
    string name, username, password, contact;

public:
    int choice;
    void homepage() {
        cout << "\nPress 1: Login as User" << endl;
        cout << "Press 2: Login as Driver" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
    }
};

class user : public sys {
public:
    void loginpage() {
        int user_login;
        cout << "\nPress 1: Login\nPress 2: Create new account\nEnter choice: ";
        cin >> user_login;

        if (user_login == 1) {
            cout << "Username: "; cin >> username;
            cout << "Password: "; cin >> password;
            ifstream file("user.txt");
            if (!file) throw "Error opening user file.";

            string line, u, p;
            bool found = false;
            while (getline(file, line)) {
                stringstream ss(line);
                ss >> name >> u >> p >> contact;
                if (u == username && p == password) {
                    found = true;
                    break;
                }
            }

            if (found) {
                cout << "Login Successful.\n";
                if (!isTripFileEmpty()) printReceipt();
                else bookinginterface();
            } else cout << "Invalid credentials.\n";
        } 
        else if (user_login == 2) {
            cout << "Creating account...\n";
            cout << "Name: "; cin >> name;
            cout << "Username: "; cin >> username;
            cout << "Password: "; cin >> password;
            cout << "Contact: "; cin >> contact;
            ofstream file("user.txt", ios::app);
            if (!file) throw "Error opening user file.";
            file << name << " " << username << " " << password << " " << contact << endl;
            cout << "Account created.\n";
        }
    }

    void bookinginterface() {
        string pickup, drop;
        int vehicle_choice;
        ofstream file("trip.txt", ios::app);

        cout << "Pickup Location: "; cin >> pickup;
        cout << "Drop Location: "; cin >> drop;
        file << pickup << " " << drop << endl;

        cout << "Choose vehicle type:\n1. Hatchback\n2. Sedan\n3. SUV\nChoice: ";
        cin >> vehicle_choice;

        switch (vehicle_choice) {
            case 1: file << "hatchback\n"; break;
            case 2: file << "sedan\n"; break;
            case 3: file << "SUV\n"; break;
            default: cout << "Invalid choice.\n"; break;
        }

        cout << "Ride booked.\n";
    }

    bool isTripFileEmpty() {
        ifstream tripFile("trip.txt");
        return tripFile.peek() == ifstream::traits_type::eof();
    }

    void printReceipt() {
        ifstream tripFile("trip.txt");
        string pickup, drop, vehicleType;
        double fare;
        while (tripFile >> pickup >> drop >> vehicleType >> fare) {
            cout << "\n--- RECEIPT ---\nPickup: " << pickup 
                 << "\nDrop: " << drop 
                 << "\nVehicle: " << vehicleType 
                 << "\nFare: Rs. " << fixed << setprecision(2) << fare << endl;
        }
        tripFile.close();
        remove("trip.txt");
    }
};

class driver : public sys {
    string vehicledetails, vehicleno, vehicletype;

public:
    void loginpage() {
        int driver_login;
        cout << "\nPress 1: Login\nPress 2: Create new account\nEnter choice: ";
        cin >> driver_login;

        if (driver_login == 1) {
            cout << "Username: "; cin >> username;
            cout << "Password: "; cin >> password;
            ifstream file("driver.txt");
            if (!file) throw "Error opening driver file.";

            string line, u, p;
            bool found = false;
            while (getline(file, line)) {
                stringstream ss(line);
                ss >> name >> u >> p >> contact >> vehicledetails >> vehicleno >> vehicletype;
                if (u == username && p == password) {
                    found = true;
                    break;
                }
            }

            if (found) {
                cout << "Login Successful.\n";
                checkIncomingTrip();
            } else cout << "Invalid credentials.\n";
        } 
        else if (driver_login == 2) {
            cout << "Creating account...\n";
            cout << "Name: "; cin >> name;
            cout << "Username: "; cin >> username;
            cout << "Password: "; cin >> password;
            cout << "Contact: "; cin >> contact;
            cout << "Vehicle Details: "; cin >> vehicledetails;
            cout << "Vehicle Number: "; cin >> vehicleno;
            cout << "Vehicle Type: "; cin >> vehicletype;
            ofstream file("driver.txt", ios::app);
            if (!file) throw "Error opening driver file.";
            file << name << " " << username << " " << password << " " << contact << " "
                 << vehicledetails << " " << vehicleno << " " << vehicletype << endl;
            cout << "Account created.\n";
        }
    }

    void checkIncomingTrip() {
        ifstream tripFile("trip.txt");
        if (!tripFile) throw "Error opening trip file.";

        string pickup, drop, vehicle;
        bool tripFound = false;

        while (tripFile >> pickup >> drop >> vehicle) {
            if (vehicle == vehicletype) {
                cout << "\nIncoming Trip - Pickup: " << pickup << ", Drop: " << drop << endl;
                cout << "Accept trip? (1: Yes, 2: No): ";
                int choice; cin >> choice;

                if (choice == 1) {
                    double farePerKm;
                    cout << "Trip accepted. Enter distance (km): ";
                    int distance; cin >> distance;

                    if (vehicle == "hatchback") farePerKm = 10.0;
                    else if (vehicle == "sedan") farePerKm = 15.0;
                    else if (vehicle == "SUV") farePerKm = 25.0;

                    double totalFare = distance * farePerKm;

                    ofstream confirm("trip.txt", ios::app);
                    confirm << totalFare << endl;
                    cout << "Fare: Rs. " << totalFare << endl;
                    tripFound = true;
                    break;
                }
            }
        }

        if (!tripFound) cout << "No matching trip requests.\n";
    }
};

int main() {
    sys s;
    user u;
    driver d;

    cout << "--|| Welcome to Cab Booking System ||--" << endl;

    while (true) {
        s.homepage();
        if (s.choice == 1) u.loginpage();
        else if (s.choice == 2) d.loginpage();
        else cout << "Invalid choice. Try again.\n";
    }

    return 0;
}
