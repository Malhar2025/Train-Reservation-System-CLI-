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

class bookedTickets
{
public:

    vector<vector<string> > report;
};

class trainDetail
{

public:
    
    vector<vector<string> > v = 
    { 
        { {"37392"}, {"Ahmedabad-0"}, {"Anand-50"}, {"Vadodara-100"}, {"Bharuch-200"}, {"Surat-300"} },
        { {"37392"}, {"S1-15"}, {"S2-20"}, {"S3-50"}, {"B1-36"}, {"B2-48"}, {"H1-5"} },
        { {"29772"}, {"Vadodara-0"}, {"Dahod-150"}, {"Indore-350"} },
        { {"29772"}, {"S1-72"}, {"S2-72"}, {"B1-72"}, {"A1-48"} }
    };
};

class coach : public trainDetail
{
public:

    map<string, string>classes = {{"SL", "S"},{"3A", "B"},{"2A", "A"},{"1A", "H"}};
    vector<vector <string> > coaches;
    vector<vector <string> > seats;

    void coachesinfo()
    {
        vector<string> temp1;
        vector<string> temp2;

        for (int i = 1; i < v.size(); i+=2)
        {
            temp1.push_back(v[i][0]);
            temp2.push_back(v[i][0]);
            for (int j = 0; j < v[i].size(); j++)
            {
                int t = v[i][j].find("-");
                if(t != string::npos)
                {
                    temp1.push_back(v[i][j].substr(0,t));
                    temp2.push_back(v[i][j].substr(t+1));
                }
            }
            coaches.push_back(temp1);
            temp1.clear();
            seats.push_back(temp2);
            temp2.clear();
        }
    }

    void print()
    {
        for (int i = 0; i < coaches.size(); i++)
        {
            for (int j = 0; j < coaches[i].size(); j++)
            {
                cout << coaches[i][j] << " ";
            }
            cout << endl;  
        }
        for (int i = 0; i < seats.size(); i++)
        {
            for (int j = 0; j < seats[i].size(); j++)
            {
                cout << seats[i][j] << " ";
            }
            cout << endl;  
        }
    }
};

class bookingRequest
{

public:

    string from, to, date, classType, passengersStr;
    
};

void bookTickets(int currentPNR)
{
    trainDetail tD;
    bookedTickets bT;
    bookingRequest bR;
    coach c;

    int pass = 0;
    string reqPNR;

    redirect:
    while(pass > 0)
    {
        cout << "\n1. Retrieve booking detail" << endl;
        cout << "2. Generate Report" << endl;
        cout << "3. Book Ticket" << endl;

        input:
        int user_ans;
        cin >> user_ans;

        switch (user_ans)
        {
        case 1:
            cout << "Enter PNR number" << endl;
            cin >> reqPNR;

            for(int i=0;i<bT.report.size();i++)
            {
                if(reqPNR == bT.report[i][0])
                {
                    for(int j=1;j<bT.report[i].size();j++)
                    {
                        if(j==1)
                        {
                            continue;
                        }
                        else if(j==4)
                        {
                            cout << bT.report[i][j] << " ";
                            cout << bT.report[i][1] << " ";
                        }
                        else
                        {
                            cout << bT.report[i][j] << " ";
                        }   
                    }
                    cout << endl;
                }
            }
            goto redirect;
        case 2:
            cout << endl;
            for(int i=0;i<bT.report.size();i++)
            {
                for(int j=0;j<bT.report[i].size();j++)
                {
                    cout << bT.report[i][j] << " ";
                }
                cout << endl;
            }
            goto redirect;
        case 3:
            goto here;
        default:
            cout << "Invalid option try again!" << endl;
            goto input;
        }
    }

    here:

    cout << "\nEnter booking request" << endl;
    cin >> bR.from >> bR.to >> bR.date >> bR.classType >> bR.passengersStr;
    

    if(c.classes.find(bR.classType) != c.classes.end())
    {
        bR.classType = c.classes[bR.classType];
    }

    vector<vector<string> > info;
    vector<string> temp;
    vector<string> temp2;

    int flag = 0;
    int available = 0;
    int connect = 0;
    int row = 0;
    int count=-1;
    int var = 0;

    for (int i = 0; i < tD.v.size(); i++)
    {
        for (int j = 0; j < tD.v[i].size(); j++)
        {
            if(tD.v[i][j].find(bR.from) != string::npos)
            {
                for(int z = j+1; z<tD.v[i].size(); z++)
                {
                    if(tD.v[i][z].find(bR.to) != string::npos)
                    {
                        flag = 1;
                        row = i;
                        temp.push_back(tD.v[i][0]);
                        temp.push_back(tD.v[i][j]);
                        temp.push_back(tD.v[i][z]);
                        temp.push_back(to_string(i));
                        info.push_back(temp);
                        count++;
                        break;
                    }
                    temp.clear();
                }
            }
            int t = tD.v[i][j].find(bR.classType);
            int n = tD.v[i][j].find("-");
            if(t != string::npos and i == row+1)
            {
                if( stoi(bR.passengersStr) <= stoi(tD.v[i][j].substr(n+1)) )
                {
                    if(tD.v[i][j-1].find(bR.classType)!=string::npos and j!=0)
                    {
                        info[count].push_back(tD.v[i][j-1]);
                        connect = 1;
                    }
                    available++;
                    info[count].push_back(tD.v[i][j]);
                    temp2.push_back(to_string(i));
                    break;
                }
            }
        }
    }

    if(flag != 0)
    {
        if(available == 0)
        {
            cout << "\nSeats Not Available" << endl;
            goto redirect;
        }
        cout << "\nTrain found!" << endl;
    }
    else
    {
        cout << "\nTrain NOT found!" << endl;
        goto redirect;
    }
    
    vector<vector<string> > updated_info;

    temp2.erase( unique( temp2.begin(), temp2.end() ), temp2.end() );

    for(int i=0; i<info.size(); i++)
    {
        int p = stoi(info[i][3]) + 1;
    
        for (int j = 0; j < temp2.size(); j++)
        {
            int q = stoi(temp2[j]);
            if(q == p)
            {
                updated_info.push_back(info[i]);
            }
        }
    }

    cout << "\nTrain info" << endl;
    for(int i=0; i<updated_info.size(); i++)
    {
        for (int j = 0; j < updated_info[i].size(); j++)
        {
            cout << updated_info[i][j] << " ";
        }
        cout <<endl;
    }

    cout << "\nTrain choices" << endl;
    for(int i=0; i<updated_info.size(); i++)
    {
        cout << updated_info[i][0] << " ";
    }
    cout <<endl;

    string user_input;
    cout << "\nEnter your train choice: " << endl;
    cin >> user_input;

    int distance;

    for(int i=0;i<updated_info.size();i++)
    {
        if(user_input == updated_info[i][0])
        {
            int d1 = stoi( updated_info[i][2].substr(updated_info[i][2].find("-")) );
            int d2 = stoi( updated_info[i][1].substr(updated_info[i][1].find("-")) );
            bR.classType = updated_info[i][4].substr(0,updated_info[i][4].find("-"));
            distance = d2 - d1;
        }
    }

    cout << "\nDistance: " << distance << endl;

    int fare = calculateFare(bR.classType, distance, stoi(bR.passengersStr));
    cout << "\nPNR: " << currentPNR << endl;
    cout << "Fare: " << fare << endl;
    cout << "CNF/"  << bR.passengersStr << endl;

    int required_seats = stoi(bR.passengersStr);

    for (int i = 1; i < tD.v.size(); i+=2)
    {
        if(tD.v[i][0] == user_input)
        {
            for(int j=0; j<tD.v[i].size(); j++)
            {
                int t = tD.v[i][j].find(bR.classType);
                if(t != string::npos)
                {
                    if(connect == 1)
                    {
                        int available_seats = stoi(tD.v[i][j].substr(3));
                        tD.v[i][j].erase();
                        required_seats = stoi(bR.passengersStr) - available_seats;
                        connect = 0;
                        break;
                    }
                    else
                    {
                        int available_seats = stoi(tD.v[i][j].substr(3));
                        int updated_seats = available_seats - required_seats;
                        string replace = to_string(updated_seats);
                        tD.v[i][j].replace(3,tD.v[i][j].size(),replace);
                        break;
                    }                    
                }
            }
        }
    }

    vector<string> details;
    details.push_back(to_string(currentPNR));
    details.push_back(bR.date);
    details.push_back(user_input);
    details.push_back(bR.from);
    details.push_back(bR.to);
    details.push_back(to_string(distance));
    
    for (int i = 0; i < stoi(bR.passengersStr); i++)
    {
        details.push_back(bR.classType+"-"+to_string(i+1));
    }
    bT.report.push_back(details);

    pass++;
    currentPNR++;
    goto redirect;
}

int main()
{
    // coach c;
    // c.coachesinfo();

    int currentPNR = 100000001;

    trainDetail tD;

    for (int i = 0; i < tD.v.size(); i++)
    {
        for (int j = 0; j < tD.v[i].size(); j++)
        {
            cout << tD.v[i][j] << " ";
        }
        cout << endl;
    }

    bookTickets(currentPNR);

    return 0;
}
