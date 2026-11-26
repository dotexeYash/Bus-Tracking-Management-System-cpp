#include <iostream>
#include <string>
using namespace std;

class Bus {
public:
    int id;
    string busNumber;
    string depotName;
    string routeNumber;
    string routeId;
    string day;
    string driverId;
    string conductorId;
    string departureTime;   // format HH:MM (example: 08:30)
    string arrivalTime;     // format HH:MM (example: 10:30)
    double currentPosition;
    bool assigned;

    Bus() {
        id = 0;
        currentPosition = 0;
        assigned = false;
    }

    void display() {
        cout << "\nBus ID: " << id;
        cout << "\nRegistration Number: " << busNumber;
        cout << "\nDepot: " << depotName;

        if (assigned) {
            cout << "\nRoute Number: " << routeNumber;
            cout << "\nRoute ID: " << routeId;
            cout << "\nDay: " << day;
            cout << "\nDeparture Time: " << departureTime;
            cout << "\nArrival Time: " << arrivalTime;
            cout << "\nDriver ID: " << driverId;
            cout << "\nConductor ID: " << conductorId;
            cout << "\nCurrent Position: " << currentPosition << " km";
        } else {
            cout << "\nRoute: Not assigned";
        }
        cout << "\n-----------------------------------\n";
    }
};

Bus buses[50];
int totalBuses = 0;
int nextId = 1;   // always increases, so IDs never repeat (even after deleting)

// ---------- helper functions ----------

// Reads an integer safely (keeps asking if the user types letters)
int readInt(string message) {
    int x;
    cout << message;
    while (!(cin >> x)) {
        cin.clear();               // clear the error
        cin.ignore(1000, '\n');    // throw away the wrong input
        cout << "Invalid input. Enter a number: ";
    }
    cin.ignore(1000, '\n');        // remove the leftover newline
    return x;
}

// Reads a decimal number safely
double readDouble(string message) {
    double x;
    cout << message;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Enter a number: ";
    }
    cin.ignore(1000, '\n');
    return x;
}

// Converts text to lowercase so "Vijayawada" and "vijayawada" match
string toLower(string s) {
    int n = s.length();
    for (int i = 0; i < n; i++)
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] + 32;   // capital letter -> small letter
    return s;
}

int findBus(int id) {
    for (int i = 0; i < totalBuses; i++) {
        if (buses[i].id == id)
            return i;
    }
    return -1;
}

void checkCloseBuses();   // declared early because other functions call it

// ---------- main features ----------

void addBus() {
    if (totalBuses == 50) {
        cout << "Bus list is full!\n";
        return;
    }

    Bus b;
    b.id = nextId++;

    cout << "Enter bus registration number: ";
    getline(cin, b.busNumber);

    cout << "Enter depot name: ";
    getline(cin, b.depotName);

    buses[totalBuses] = b;
    totalBuses++;

    cout << "Bus added successfully. Bus ID: " << b.id << "\n";
}

void assignRoute() {
    int id = readInt("Enter bus ID: ");

    int i = findBus(id);
    if (i == -1) {
        cout << "Bus not found!\n";
        return;
    }

    cout << "Enter route number: ";
    getline(cin, buses[i].routeNumber);

    cout << "Enter route ID: ";
    getline(cin, buses[i].routeId);

    cout << "Enter day (example: Monday): ";
    getline(cin, buses[i].day);

    cout << "Enter departure time (HH:MM, example: 08:30): ";
    getline(cin, buses[i].departureTime);

    cout << "Enter arrival time (HH:MM, example: 10:30): ";
    getline(cin, buses[i].arrivalTime);

    cout << "Enter driver ID: ";
    getline(cin, buses[i].driverId);

    cout << "Enter conductor ID: ";
    getline(cin, buses[i].conductorId);

    buses[i].currentPosition = readDouble("Enter current position on route in km (example: 12.5): ");

    buses[i].assigned = true;
    cout << "Route assigned successfully!\n";

    checkCloseBuses();   // check for close buses right after assigning
}

void viewAllBuses() {
    if (totalBuses == 0) {
        cout << "No buses available.\n";
        return;
    }

    cout << "\n========== BUS DETAILS ==========\n";
    for (int i = 0; i < totalBuses; i++)
        buses[i].display();
}

void trackBus() {
    int id = readInt("Enter bus ID: ");

    int i = findBus(id);
    if (i == -1) {
        cout << "Bus not found!\n";
    } else if (!buses[i].assigned) {
        cout << "No route has been assigned to this bus.\n";
    } else {
        cout << "Bus " << buses[i].busNumber
             << " is on route " << buses[i].routeNumber
             << " (" << buses[i].routeId << ") on " << buses[i].day
             << ". Current position: " << buses[i].currentPosition
             << " km.\n";
    }
}

void updatePosition() {
    int id = readInt("Enter bus ID: ");

    int i = findBus(id);
    if (i == -1) {
        cout << "Bus not found!\n";
        return;
    }

    if (!buses[i].assigned) {
        cout << "Assign a route to this bus first.\n";
        return;
    }

    buses[i].currentPosition = readDouble("Enter updated position in km: ");
    cout << "Bus position updated!\n";
    checkCloseBuses();
}

// Alert if two buses on the same route and day, running at the same time,
// are within 2 km of each other.
void checkCloseBuses() {
    bool alertFound = false;

    for (int i = 0; i < totalBuses; i++) {
        if (!buses[i].assigned)
            continue;

        for (int j = i + 1; j < totalBuses; j++) {
            if (!buses[j].assigned)
                continue;

            bool sameRoute = (buses[i].routeId == buses[j].routeId &&
                              buses[i].day == buses[j].day);

            // Times overlap if each bus departs before the other one arrives.
            // (HH:MM text can be compared directly with < and >)
            bool timeOverlap = (buses[i].departureTime < buses[j].arrivalTime &&
                                buses[j].departureTime < buses[i].arrivalTime);

            if (sameRoute && timeOverlap) {
                double distance = buses[i].currentPosition - buses[j].currentPosition;
                if (distance < 0)
                    distance = -distance;   // make it positive

                if (distance <= 2.0) {
                    cout << "\n*** ALERT: Two buses are very close! ***\n";
                    cout << "Bus " << buses[i].busNumber
                         << " and Bus " << buses[j].busNumber
                         << " are on route " << buses[i].routeNumber
                         << " on " << buses[i].day << ".\n";
                    cout << "Distance between them: " << distance
                         << " km (alert limit: 2 km).\n";
                    alertFound = true;
                }
            }
        }
    }

    if (!alertFound)
        cout << "No close buses found on the same route and day.\n";
}

void viewDepotBuses() {
    string depot;
    cout << "Enter depot name: ";
    getline(cin, depot);

    bool found = false;
    for (int i = 0; i < totalBuses; i++) {
        if (toLower(buses[i].depotName) == toLower(depot)) {
            buses[i].display();
            found = true;
        }
    }

    if (!found)
        cout << "No buses found for this depot.\n";
}

void deleteBus() {
    int id = readInt("Enter bus ID to delete: ");

    int i = findBus(id);
    if (i == -1) {
        cout << "Bus not found!\n";
        return;
    }

    for (int j = i; j < totalBuses - 1; j++) {
        buses[j] = buses[j + 1];
    }

    totalBuses--;
    cout << "Bus deleted successfully.\n";
}

int main() {
    int choice;

    do {
        cout << "\n===== BUS TRACKING AND MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Bus\n";
        cout << "2. Assign Route and Staff\n";
        cout << "3. View All Buses\n";
        cout << "4. Track Bus\n";
        cout << "5. Update Bus Position\n";
        cout << "6. Check Close Buses Alert\n";
        cout << "7. View Buses by Depot\n";
        cout << "8. Delete Bus\n";
        cout << "0. Exit\n";

        choice = readInt("Enter choice: ");

        switch (choice) {
            case 1:
                addBus();
                break;
            case 2:
                assignRoute();
                break;
            case 3:
                viewAllBuses();
                break;
            case 4:
                trackBus();
                break;
            case 5:
                updatePosition();
                break;
            case 6:
                checkCloseBuses();
                break;
            case 7:
                viewDepotBuses();
                break;
            case 8:
                deleteBus();
                break;
            case 0:
                cout << "Thank you!\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}
