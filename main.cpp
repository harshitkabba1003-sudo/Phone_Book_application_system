#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

class Contact {
private:
    string name;
    string phone;
    string email;

public:
    Contact() {}
    Contact(string n, string p, string e) : name(n), phone(p), email(e) {}

    string getName() const { return name; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }

    void displayRow() const {
        cout << left << setw(20) << name 
             << setw(15) << phone 
             << setw(25) << email << endl;
    }

    void writeToFile(ofstream &outFile) const {
        outFile << name << "," << phone << "," << email << "\n";
    }
};

class PhoneBook {
private:
    const string fileName = "contacts.txt";

public:
    void addContact() {
        string name, phone, email;
        cout << "\n--- Add New Contact ---\n";
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Phone Number: ";
        getline(cin, phone);
        cout << "Enter Email Address: ";
        getline(cin, email);

        ofstream outFile(fileName, ios::app);
        if (!outFile) {
            cerr << "Error opening file!\n";
            return;
        }

        Contact contact(name, phone, email);
        contact.writeToFile(outFile);
        outFile.close();

        cout << "Contact saved successfully!\n";
    }

    void displayAll() {
        ifstream inFile(fileName);
        if (!inFile) {
            cout << "\nNo contacts found or file empty.\n";
            return;
        }

        string line;
        cout << "\n=========================================================\n";
        cout << left << setw(20) << "Name" << setw(15) << "Phone" << setw(25) << "Email" << endl;
        cout << "=========================================================\n";

        bool empty = true;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            empty = false;
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos) {
                string name = line.substr(0, pos1);
                string phone = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string email = line.substr(pos2 + 1);
                Contact c(name, phone, email);
                c.displayRow();
            }
        }

        if (empty) {
            cout << "No saved contacts found.\n";
        }
        cout << "=========================================================\n";
        inFile.close();
    }

    void searchContact() {
        ifstream inFile(fileName);
        if (!inFile) {
            cout << "\nNo records found to search.\n";
            return;
        }

        string searchName;
        cout << "\nEnter Name to Search: ";
        cin.ignore();
        getline(cin, searchName);

        string line;
        bool found = false;
        while (getline(inFile, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos) {
                string name = line.substr(0, pos1);
                string phone = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string email = line.substr(pos2 + 1);

                if (name == searchName) {
                    if (!found) {
                        cout << "\nMatch Found:\n";
                        cout << left << setw(20) << "Name" << setw(15) << "Phone" << setw(25) << "Email" << endl;
                        cout << "---------------------------------------------------------\n";
                    }
                    Contact c(name, phone, email);
                    c.displayRow();
                    found = true;
                }
            }
        }

        if (!found) {
            cout << "No contact found with the name \"" << searchName << "\".\n";
        }
        inFile.close();
    }

    void deleteContact() {
        ifstream inFile(fileName);
        if (!inFile) {
            cout << "\nFile empty or not found.\n";
            return;
        }

        string targetName;
        cout << "\nEnter Name of Contact to Delete: ";
        cin.ignore();
        getline(cin, targetName);

        ofstream tempFile("temp.txt");
        string line;
        bool deleted = false;

        while (getline(inFile, line)) {
            size_t pos1 = line.find(',');
            if (pos1 != string::npos) {
                string name = line.substr(0, pos1);
                if (name == targetName) {
                    deleted = true;
                    continue; // Skip writing this contact
                }
            }
            tempFile << line << "\n";
        }

        inFile.close();
        tempFile.close();

        remove(fileName.c_str());
        rename("temp.txt", fileName.c_str());

        if (deleted) {
            cout << "Contact deleted successfully!\n";
        } else {
            cout << "Contact name not found.\n";
        }
    }
};

int main() {
    PhoneBook app;
    int choice;

    do {
        cout << "\n======== PHONE BOOK MANAGEMENT SYSTEM ========\n";
        cout << "1. Add Contact\n";
        cout << "2. Display All Contacts\n";
        cout << "3. Search Contact\n";
        cout << "4. Delete Contact\n";
        cout << "5. Exit\n";
        cout << "Enter your choice (1-5): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: app.addContact(); break;
            case 2: app.displayAll(); break;
            case 3: app.searchContact(); break;
            case 4: app.deleteContact(); break;
            case 5: cout << "\nExiting Application. Goodbye!\n"; break;
            default: cout << "Invalid selection! Try again.\n";
        }
        
    } while (choice != 5);

    return 0;
}
