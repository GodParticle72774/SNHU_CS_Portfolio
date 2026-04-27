/*
Mark Andrew Brannan

Project Three

4/19/2026

CS-210
*/


#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <limits>

using namespace std;

class GroceryTracker {
private:
    map<string, int> itemFrequencies;

public:
    GroceryTracker() {
        LoadData();
        CreateBackup();
    }

    void LoadData() {
        ifstream inFS;
        string item;

        inFS.open("CS210_Project_Three_Input_File.txt");
        if (!inFS.is_open()) {
            cout << "Could not open input file." << endl;
            return;
        }

        // Initialize the Map with items from the daily record.
        while (inFS >> item) {
            itemFrequencies[item]++;
        }
        inFS.close();
    }

    void CreateBackup() {
        ofstream outFS;
        outFS.open("frequency.dat");

        if (!outFS.is_open()) {
            cout << "Could not create backup file." << endl;
            return;
        }

        for (auto const& pair : itemFrequencies) {
            outFS << pair.first << " " << pair.second << endl;
        }
        outFS.close();
    }

    void PrintFrequency(string searchItem) {
        if (itemFrequencies.count(searchItem)) {
            cout << searchItem << " was purchased " << itemFrequencies[searchItem] << " times." << endl;
        }
        else {
            cout << searchItem << " was not found in today's records." << endl;
        }
    }

    void PrintAll() {
        for (auto const& pair : itemFrequencies) {
            cout << pair.first << " " << pair.second << endl;
        }
    }

    void PrintHistogram() {
        for (auto const& pair : itemFrequencies) {
            cout << pair.first << " ";
            for (int i = 0; i < pair.second; i++) {
                cout << "*";
            }
            cout << endl;
        }
    }
};

void DisplayMenu() {
    cout << "\nCorner Grocer Tracking Menu" << endl;
    cout << "1. Find frequency of a specific item" << endl;
    cout << "2. Print all item frequencies" << endl;
    cout << "3. Print histogram of all items" << endl;
    cout << "4. Exit program" << endl;
    cout << "Enter your choice: ";
}

int main() {
    GroceryTracker tracker;
    int menuChoice = 0;
    string searchWord;

    while (menuChoice != 4) {
        DisplayMenu();

        // validate user input to prevent infinite loops
        if (!(cin >> menuChoice)) {
            cout << "Invalid input. Please enter a number between 1 and 4." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (menuChoice == 1) {
            cout << "Enter the item you are looking for: ";
            cin >> searchWord;
            tracker.PrintFrequency(searchWord);
        }
        else if (menuChoice == 2) {
            tracker.PrintAll();
        }
        else if (menuChoice == 3) {
            tracker.PrintHistogram();
        }
        else if (menuChoice == 4) {
            cout << "Exiting program. Have a great day!" << endl;
        }
        else {
            cout << "Please select a valid option (1, 2, 3, or 4)." << endl;
        }
    }

    return 0;
}