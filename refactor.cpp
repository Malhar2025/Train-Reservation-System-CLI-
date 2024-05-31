#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

class TrainReservationSystem
{
private:
    
    struct Train
    {
        string trainNum;
        map<string, int> seatAvailable;
        map<string, int> cityDistance;
    };
    
    struct BookTrain
    {
        int pnr;
        string trainNum;
        string from;
        string to;
        string date;
        string userClass;
        int Fare;
        int seatConfirmed;
        int seatWaiting;
        vector<string> seatNum;
    };

    int nextPNR;

    map<string, Train> trains;
    map<int, BookTrain> bookings;
    map<string, queue<int> > waitingQueue;

    int calcFare(string &userClass, int distance, int passengers)
    {
        if (userClass == "SL") return distance * 1 * passengers;
        if (userClass == "3A") return distance * 2 * passengers;
        if (userClass == "2A") return distance * 3 * passengers;
        if (userClass == "1A") return distance * 4 * passengers;
        return 0;
    }

    string generateSeatNum(string& userClass, int seatIndex)
    {
        return userClass + "-" + to_string(seatIndex);
    }

    int calcDistance(Train& train, string& from, string& to)
    {
        if (train.cityDistance.find(from) == train.cityDistance.end() || train.cityDistance.find(to) == train.cityDistance.end()) {
            return -1; // Invalid stations
        }
        return abs(train.cityDistance.at(to) - train.cityDistance.at(from));
    }

public:

    TrainReservationSystem(): nextPNR(100000001){}

    void addTrain(string trainNum, map<string, int> seats, map<string, int> cityDistance)
    {
        Train train;
        train.trainNum = trainNum;
        train.seatAvailable = seats;
        train.cityDistance = cityDistance;
        trains[trainNum] = train;
    }

    void printTrainDetails(string trainNum)
    {
        if (trains.find(trainNum) == trains.end()) {
            cout << "Train not found.\n";
            return;
        }

        Train& train = trains.at(trainNum);
        cout << train.trainNum << " ";
        for (auto& cityDistance : train.cityDistance) {
            cout << cityDistance.first << "-" << cityDistance.second << " ";
        }
        cout << endl;
        cout << train.trainNum << " ";
        for (auto& seat : train.seatAvailable) {
            cout << seat.first << "-" << seat.second << " ";
        }
    }

    void printAllTrainDetails() {
        for (auto& pair : trains) {
            printTrainDetails(pair.first);
            cout << "\n" << endl;
        }
    }

    void searchTrain(string& from, string& to)
    {
        bool found = false;
        for (auto& pair : trains)
        {
            Train& train = pair.second;
            if (train.cityDistance.find(from) != train.cityDistance.end() && train.cityDistance.find(to) != train.cityDistance.end() && train.cityDistance.at(from) < train.cityDistance.at(to)) {
                found = true;
                printTrainDetails(pair.first);
                cout << endl;
            }
        }
        if (!found) {
            cout << "No trains found for the given route.\n" <<  endl;;
        }
    }    

    void bookTicket(string& trainNum, string& from, string& to, string& date, string& userClass, int numSeats)
    {
        if (trains.find(trainNum) == trains.end())
        {
            cout << "Train not found.\n" << endl;
            return;
        }

        Train& train = trains[trainNum];
        int distance = calcDistance(train, from, to);
        if (distance == -1) {
            cout << "Invalid stations.\n" << endl;
            return;
        }

        int availableSeats = train.seatAvailable[userClass];
        int confirmedSeats = min(availableSeats, numSeats);
        int waitlistedSeats = numSeats - confirmedSeats;

        BookTrain booking;
        booking.pnr = nextPNR++;
        booking.trainNum = trainNum;
        booking.from = from;
        booking.to = to;
        booking.date = date;
        booking.userClass = userClass;
        booking.Fare = calcFare(userClass, distance, numSeats);
        booking.seatConfirmed = confirmedSeats;
        booking.seatWaiting = waitlistedSeats;

        for (int i = 1; i <= confirmedSeats; ++i)
        {
            booking.seatNum.push_back(generateSeatNum(userClass, i));
        }

        if (waitlistedSeats > 0)
        {
            for (int i = 1; i <= waitlistedSeats; ++i) 
            {
                booking.seatNum.push_back("WL-" + to_string(i));
            }
            waitingQueue[trainNum].push(booking.pnr);
        }

        bookings[booking.pnr] = booking;
        train.seatAvailable[userClass] -= confirmedSeats;

        cout << "PNR: " << booking.pnr << " Total Cost: " << booking.Fare << " CNF/" << confirmedSeats;
        if (waitlistedSeats > 0)
        {
            cout << " WL/" << waitlistedSeats;
        }
        cout << "\n" << endl;
    }

    void cancelTicket(int pnr) 
    {
        if (bookings.find(pnr) == bookings.end()) 
        {
            cout << "Booking not found.\n" << endl;
            return;
        }

        BookTrain& booking = bookings[pnr];
        Train& train = trains[booking.trainNum];
        train.seatAvailable[booking.userClass] += booking.seatConfirmed;

        if (booking.seatWaiting > 0)
        {
            queue<int>& waitlistQueue = waitingQueue[booking.trainNum];
            while (!waitlistQueue.empty() && train.seatAvailable[booking.userClass] > 0)
            {
                int nextPNR = waitlistQueue.front();
                waitlistQueue.pop();
                BookTrain& nextBooking = bookings[nextPNR];

                int seatsToConfirm = min(train.seatAvailable[booking.userClass], nextBooking.seatWaiting);
                nextBooking.seatConfirmed += seatsToConfirm;
                nextBooking.seatWaiting -= seatsToConfirm;
                train.seatAvailable[booking.userClass] -= seatsToConfirm;

                for (int i = 0; i < seatsToConfirm; ++i)
                {
                    nextBooking.seatNum[nextBooking.seatConfirmed - seatsToConfirm + i] = generateSeatNum(nextBooking.userClass, nextBooking.seatConfirmed - seatsToConfirm + i + 1);
                }

                if (nextBooking.seatWaiting > 0)
                {
                    waitlistQueue.push(nextPNR);
                }
            }
        }

        bookings.erase(pnr);
        cout << "Booking with PNR " << pnr << " cancelled successfully.\n" << endl;
    }

    void retrieveBookingDetails(int pnr)
    {
        if (bookings.find(pnr) == bookings.end())
        {
            cout << "Booking not found.\n" << endl;
            return;
        }

        BookTrain& booking = bookings.at(pnr);
        cout << booking.pnr << " " << booking.trainNum << " " << booking.from << " " << booking.to << " " << booking.date << " " << booking.Fare << " ";
        for (string& seat : booking.seatNum)
        {
            cout << seat << " ";
        }
        cout << "\n" << endl;
    }

     void generateReport()
     {
        for (auto& pair : bookings)
        {
            retrieveBookingDetails(pair.first);
        }
    }

    void viewWaitlist()
    {
        for (const auto& pair : waitingQueue)
        {
            const string& trainNumber = pair.first;
            const queue<int>& line = pair.second;
            if (!line.empty())
            {
                cout << "Waitlist for train " << trainNumber << ": ";
                queue<int> tempQueue = line;
                while (!tempQueue.empty())
                {
                    cout << tempQueue.front() << " ";
                    tempQueue.pop();
                }
                cout << endl;
            }
        }
        cout << endl;
    }

};

int main()
{
    TrainReservationSystem system;

    system.addTrain("37392", {{"1A", 5}, {"2A", 20}, {"3A", 50}, {"SL", 200}}, {{"Ahmedabad", 0}, {"Anand", 50}, {"Vadodara", 100}, {"Bharuch", 200}, {"Surat", 300}});
    system.addTrain("29772", {{"1A", 5}, {"2A", 48}, {"3A", 72}, {"SL", 300}}, {{"Vadodara", 0}, {"Dahod", 150}, {"Indore", 350}});

    system.printAllTrainDetails();

    while (true) 
    {
        int choice;
        cout << "Menu:\n";
        cout << "1. Book Ticket\n";
        cout << "2. Retrieve Booking Details\n";
        cout << "3. Cancel Ticket\n";
        cout << "4. Generate Report\n";
        cout << "5. View Waitlist\n";
        cout << "6. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice == 6) 
        {
            break;
        }

        switch (choice) 
        {
            case 1:
            {
                string from, to, date, travelClass;
                int numSeats;

                cout << "\nEnter booking details" << endl;
                cin >> from >> to >> date >> travelClass >> numSeats;

                cout << "\nAvailable trains: " << endl;
                system.searchTrain(from, to);

                string trainNumber;
                cout << "\nEnter your choice: ";
                cin >> trainNumber;

                system.bookTicket(trainNumber, from, to, date, travelClass, numSeats);
                break;
            }
            case 2:
            {
                int pnr;
                cout << "Enter PNR number: ";
                cin >> pnr;
                system.retrieveBookingDetails(pnr);
                break;
            }
            case 3:
            {
                int pnr;
                cout << "Enter PNR number: ";
                cin >> pnr;
                system.cancelTicket(pnr);
                break;
            }
            case 4:
            {
                system.generateReport();
                break;
            }
            case 5:
            {
                system.viewWaitlist();
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n" << endl;
        }
    }

    return 0;
}
