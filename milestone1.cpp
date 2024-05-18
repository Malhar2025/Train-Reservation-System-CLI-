#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

using namespace std;

int calculateFare(string classType, int distance, int passengers) 
{
    switch (classType[0]) {
        case 'S': return distance * 1 * passengers;
        case 'B': return distance * 2 * passengers;
        case 'A': return distance * 3 * passengers;
        case 'H': return distance * 4 * passengers;
        default: return 0;
    }
}

void bookTicket(vector<vector<string> > v, int currentPNR)
{
    string from, to, date, classType, passengersStr;
    redirect:
    cout << "\nEnter booking request" << endl;
    cin >> from >> to >> date >> classType >> passengersStr;

    int flag = 0;
    int row;

    for (int i = 0; i < v.size(); i+=2)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            if(v[i][j].find(from) != string::npos and v[i][j+1].find(to) != string::npos)
            {
                flag++;
                row = i;
            }
        }
    }

    if(flag != 0)
    {
        cout << "\nTrain found!" << endl;
    }
    else
    {
        cout << "\nTrain NOT found!" << endl;
        goto redirect;
    }

    int distance;

    for(int i=0; i<v[row][2].length(); i++)
    {
        if(v[row][2][i] == '-')
        {
            distance = stoi( v[row][2].substr(i+1) );
        }
    }

    int passengers = stoi(passengersStr);
    int available_seats;

    map<string, string>classes = {{"SL", "S"},{"3A", "B"},{"2A", "A"},{"1A", "H"}};

    if(classes.find(classType) != classes.end())
    {
        classType = classes[classType];
    }

    flag = 0;

    for(int j=0; j<v[row+1].size(); j++)
    {
        for(int i=0; i<v[row][j].length(); i++)
        {
            if( v[row+1][i].find(classType) != string::npos and passengers <= stoi(v[row+1][i].substr(3)) )
            {
                available_seats = stoi( v[row+1][i].substr(3) );
                int updated_seats = available_seats - passengers;

                int fare = calculateFare(classType, distance, passengers);

                cout << "\nPNR: " << currentPNR << endl;
                cout << "Fare: " << fare << endl;
                
                string replace = to_string(updated_seats);
                v[row+1][i].replace(3,v[row+1][i].size(),replace);

                currentPNR++;
                goto redirect;
            }
            else
            {
                flag = 1;
            }
        }
    }

    if(flag != 0)
    {
        cout << "\nSeats Not Available" << endl;
        goto redirect;
    }
}

int main() {
    int currentPNR = 100000001;

    int flag=0;    

    vector<vector<string> > v;

    v = { 
        { {"17726"}, {"Rajkot-0"}, {"Mumbai-750"} },
        { {"17226"}, {"S1-72"}, {"S2-72"}, {"B1-72"}, {"A1-48"}, {"H1-24"} },
        { {"37392"}, {"Ahmedabad-0"}, {"Surat-300"} },
        { {"37392"}, {"S1-15"}, {"S2-20"}, {"S3-50"}, {"B1-36"}, {"B2-48"} } 
        };

    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            cout << v[i][j] << " ";
        }
        cout << endl;
    }

    bookTicket(v, currentPNR);

    return 0;
}
