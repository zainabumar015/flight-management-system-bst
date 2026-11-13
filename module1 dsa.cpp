
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Flight
{
    int flightID;
    string source;
    string destination;
    string airline;
    string departureTime;
    string arrivalTime;
    string duration;
    double ticketPrice;
    string status;
};

struct Node
{
    Flight data;
    Node* left;
    Node* right;
    Node* p;
};

Node* root = NULL;


void InsertFlight(Flight f)
{
    Node* newNode = new Node;

    newNode->data = f;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->p = NULL;

    Node* x = root;
    Node* y = NULL;

    while (x != NULL)
    {
        y = x;

        if (f.flightID == x->data.flightID)
        {
            cout << "Duplicate Flight ID!";
            delete newNode;
            return;
        }

        else if (f.flightID < x->data.flightID)
            x = x->left;

        else
            x = x->right;
    }

    newNode->p = y;

    if (y == NULL)
        root = newNode;

    else if (f.flightID < y->data.flightID)
        y->left = newNode;

    else
        y->right = newNode;

    cout << "Flight added successfully!";
}


Node* SearchFlight(Node* x, int id)
{
    if (x == NULL || id == x->data.flightID)
        return x;

    if (id < x->data.flightID)
        return SearchFlight(x->left, id);

    else
        return SearchFlight(x->right, id);
}


Node* TreeMinimum(Node* x)
{
    while (x->left != NULL)
        x = x->left;

    return x;
}


void Transplant(Node* u, Node* v)
{
    if (u->p == NULL)
        root = v;

    else if (u == u->p->left)
        u->p->left = v;

    else
        u->p->right = v;

    if (v != NULL)
        v->p = u->p;
}


void DeleteFlight(int id)
{
    Node* z = SearchFlight(root, id);

    if (z == NULL)
    {
        cout << "Flight not found!";
        return;
    }

    if (z->left == NULL)
        Transplant(z, z->right);

    else if (z->right == NULL)
        Transplant(z, z->left);

    else
    {
        Node* y = TreeMinimum(z->right);

        if (y->p != z)
        {
            Transplant(y, y->right);

            y->right = z->right;
            y->right->p = y;
        }

        Transplant(z, y);

        y->left = z->left;
        y->left->p = y;
    }

    delete z;

    cout << "Flight deleted successfully!";
}


void DisplayFlight(Node* x)
{
    cout << "\nFlight ID: " << x->data.flightID;
    cout << "\nSource: " << x->data.source;
    cout << "\nDestination: " << x->data.destination;
    cout << "\nAirline: " << x->data.airline;
    cout << "\nDeparture: " << x->data.departureTime;
    cout << "\nArrival: " << x->data.arrivalTime;
    cout << "\nDuration: " << x->data.duration;
    cout << "\nTicket Price: " << x->data.ticketPrice;
    cout << "\nStatus: " << x->data.status << endl;
}


void Inorder(Node* x)
{
    if (x != NULL)
    {
        Inorder(x->left);

        DisplayFlight(x);

        Inorder(x->right);
    }
}


bool ValidStatus(string status)
{
    if (status == "Scheduled" ||
        status == "Delayed" ||
        status == "Cancelled")
        return true;

    return false;
}


void AddFlight()
{
    Flight f;

    cout << "\nEnter Flight ID: ";
    cin >> f.flightID;

    if (SearchFlight(root, f.flightID) != NULL)
    {
        cout << "Duplicate Flight ID!";
        return;
    }

    cout << "Enter Source: ";
    cin >> f.source;

    cout << "Enter Destination: ";
    cin >> f.destination;

    cout << "Enter Airline: ";
    cin >> f.airline;

    cout << "Enter Departure Time (HH:MM): ";
    cin >> f.departureTime;

    cout << "Enter Arrival Time (HH:MM): ";
    cin >> f.arrivalTime;

    cout << "Enter Duration (1:45 or 1.75): ";
    cin >> f.duration;

    cout << "Enter Ticket Price: ";
    cin >> f.ticketPrice;

    cout << "Enter Status (Scheduled/Delayed/Cancelled): ";
    cin >> f.status;

    while (!ValidStatus(f.status))
    {
        cout << "Invalid Status! Enter again: ";
        cin >> f.status;
    }

    InsertFlight(f);
}


void FindFlight()
{
    int id;

    cout << "\nEnter Flight ID: ";
    cin >> id;

    Node* x = SearchFlight(root, id);

    if (x == NULL)
        cout << "Flight not found!";

    else
        DisplayFlight(x);
}


int TimeToMinutes(string time)
{
    int hour = (time[0] - '0') * 10 + (time[1] - '0');
    int minute = (time[3] - '0') * 10 + (time[4] - '0');

    return hour * 60 + minute;
}


int DurationToMinutes(string duration)
{
    int hour = 0;
    int minute = 0;
    int i = 0;

    while (duration[i] != ':' && duration[i] != '.')
    {
        hour = hour * 10 + (duration[i] - '0');
        i++;
    }

    if (duration[i] == ':')
    {
        minute = (duration[i + 1] - '0') * 10
            + (duration[i + 2] - '0');
    }

    else
    {
        i++;

        int fraction = 0;
        int denominator = 1;

        while (duration[i] != '\0')
        {
            fraction = fraction * 10 + (duration[i] - '0');
            denominator = denominator * 10;
            i++;
        }

        minute = (fraction * 60 + denominator / 2) / denominator;
    }

    if (minute >= 60)
    {
        hour++;
        minute = minute - 60;
    }

    return hour * 60 + minute;
}


void CalculateETA(Node* x)
{
    if (x != NULL)
    {
        CalculateETA(x->left);

        int departure = TimeToMinutes(x->data.departureTime);
        int duration = DurationToMinutes(x->data.duration);

        int eta = departure + duration;

        if (eta >= 1440)
            eta = eta - 1440;

        int actual = TimeToMinutes(x->data.arrivalTime);

        int difference = actual - eta;

        if (difference < -720)
            difference = difference + 1440;

        else if (difference > 720)
            difference = difference - 1440;

        cout << "\nFlight ID: " << x->data.flightID;

        if (x->data.status == "Cancelled")
            cout << "\nFlight Cancelled";

        else
        {
            cout << "\nExpected Arrival: ";

            if (eta / 60 < 10)
                cout << "0";

            cout << eta / 60 << ":";

            if (eta % 60 < 10)
                cout << "0";

            cout << eta % 60;

            if (difference == 0)
                cout << "\nOn-Time";

            else if (difference > 0)
                cout << "\nDelayed";

            else
                cout << "\nEarly";
        }

        cout << endl;

        CalculateETA(x->right);
    }
}


void SearchDestination(Node* x, string destination, bool& found)
{
    if (x != NULL)
    {
        SearchDestination(x->left, destination, found);

        if (x->data.destination == destination)
        {
            DisplayFlight(x);
            found = true;
        }

        SearchDestination(x->right, destination, found);
    }
}


void SurpriseFeature()
{
    string destination;
    bool found = false;

    cout << "\nEnter Destination: ";
    cin >> destination;

    SearchDestination(root, destination, found);

    if (!found)
        cout << "\nNo flights found for this destination!";
}


void SaveFlights(Node* x, ofstream& file)
{
    if (x != NULL)
    {
        SaveFlights(x->left, file);

        file << x->data.flightID << ","
            << x->data.source << ","
            << x->data.destination << ","
            << x->data.airline << ","
            << x->data.departureTime << ","
            << x->data.arrivalTime << ","
            << x->data.duration << ","
            << x->data.ticketPrice << ","
            << x->data.status << endl;

        SaveFlights(x->right, file);
    }
}


void SaveToFile()
{
    ofstream file("flights.txt");

    if (!file)
    {
        cout << "File cannot be opened!";
        return;
    }

    SaveFlights(root, file);

    file.close();

    cout << "Records saved successfully!";
}


void ClearTree(Node* x)
{
    if (x != NULL)
    {
        ClearTree(x->left);
        ClearTree(x->right);

        delete x;
    }
}


void LoadFromFile()
{
    ifstream file("flights.txt");

    if (!file)
    {
        cout << "No saved file found!";
        return;
    }

    ClearTree(root);
    root = NULL;

    Flight f;

    while (file >> f.flightID)
    {
        file.ignore();
        file >> f.source;
        file.ignore();
        file >> f.destination;
        file.ignore();
        file >> f.airline;
        file.ignore();
        file >> f.departureTime;
        file.ignore();
        file >> f.arrivalTime;
        file.ignore();
        file >> f.duration;
        file.ignore();
        file >> f.ticketPrice;
        file.ignore();
        file >> f.status;

        InsertFlight(f);
    }

    file.close();

    cout << "\nRecords loaded successfully!";
}


int main()
{
    int choice;

    LoadFromFile();

    do
    {
        cout << "\n\n========== FLIGHT MANAGEMENT SYSTEM ==========";
        cout << "\n1. Add New Flight";
        cout << "\n2. Search Flight by ID";
        cout << "\n3. Delete Flight by ID";
        cout << "\n4. Display All Flights";
        cout << "\n5. Calculate Duration and ETA";
        cout << "\n6. Surprise Feature";
        cout << "\n7. Save Flights to File";
        cout << "\n8. Load Flights from File";
        cout << "\n0. Exit";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            AddFlight();
            break;

        case 2:
            FindFlight();
            break;

        case 3:
        {
            int id;

            cout << "Enter Flight ID: ";
            cin >> id;

            DeleteFlight(id);
            break;
        }

        case 4:
            if (root == NULL)
                cout << "No flights available!";
            else
                Inorder(root);

            break;

        case 5:
            if (root == NULL)
                cout << "No flights available!";
            else
                CalculateETA(root);

            break;

        case 6:
            SurpriseFeature();
            break;

        case 7:
            SaveToFile();
            break;

        case 8:
            LoadFromFile();
            break;

        case 0:
            cout << "Program ended.";
            break;

        default:
            cout << "Invalid choice!";
        }

    } while (choice != 0);

    return 0;
}
