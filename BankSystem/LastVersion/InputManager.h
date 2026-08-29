#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: InputManager.h                                   ||
//  || Section: Input Manager                                 ||
//  || Functions to read and validate user input (strings,    ||
//  || numbers, passwords, confirmations).                    ||
//  ||========================================================||

#include "Globals.h"
#include "Utilities.h"

//=====================================================
//==================== Input Manager ==================
//=====================================================

// Read non-empty string input from user
string readNonEmptyString(string s) {
    string line;
    do {
        cout << s;
        getline(cin >> ws, line);
        line = trim(line);
    } while (line.empty());
    return line;
}
// Read a positive number input from user
double readPositiveNumber(string prompt) {
    double num = 0;
    bool validInput = false;

    do {
        cout << prompt;
        string input;
        getline(cin, input);

        stringstream ss(input);
        if (ss >> num && num >= 0) {
            char remaining;
            if (ss >> remaining) {
                showErrorMessage("Invalid input. Please enter a valid number.");
            }
            else {
                validInput = true;
            }
        }
        else {
            showErrorMessage("Invalid input. Please enter a positive number.");
        }
    } while (!validInput);

    return num;
}
// Validate account number format (alphanumeric, specific length)
bool isValidAccountNumber(const string& accountNumber) {
    if (accountNumber.empty() || accountNumber.length() < 5 || accountNumber.length() > 20) {
        return false;
    }
    for (char c : accountNumber) {
        if (!isalnum(c)) {
            return false;
        }
    }
    return true;
}
// Validate phone number format (digits only, 10-15 chars)
bool isValidPhoneNumber(const string& phone) {
    if (phone.empty() || phone.length() < 10 || phone.length() > 15) {
        return false;
    }
    for (char c : phone) {
        if (!isdigit(c) && c != ' ' && c != '+' && c != '-' && c != '(' && c != ')') {
            return false;
        }
    }
    return true;
}
// Read validated account number with format checking
string readValidatedAccountNumber(const string& prompt) {
    string accountNumber;
    bool valid = false;

    do {
        accountNumber = readNonEmptyString(prompt);
        if (!isValidAccountNumber(accountNumber)) {
            showErrorMessage("Invalid account number format! Must be 5-20 alphanumeric characters.");
        }
        else {
            valid = true;
        }
    } while (!valid);

    return accountNumber;
}
// Read validated phone number
string readValidatedPhoneNumber(const string& prompt) {
    string phone;
    bool valid = false;

    do {
        phone = readNonEmptyString(prompt);
        if (!isValidPhoneNumber(phone)) {
            showErrorMessage("Invalid phone number! Must be 10-15 characters with digits, +, -, (), space only.");
        }
        else {
            valid = true;
        }
    } while (!valid);

    return phone;
}
// Read client data interactively
strClient readClientData(const string& AccountNumber) {
    strClient Client;
    Client.AccountNumber = AccountNumber;
    Client.PinCode = readNonEmptyString("Enter PinCode? ");
    Client.Name = readNonEmptyString("Enter Name? ");
    Client.Phone = readValidatedPhoneNumber("Enter Phone? ");
    Client.AccountBalance = readPositiveNumber("Enter AccountBalance? ");
    return Client;
}
// Read menu choice between given range or zero for return
int readMenuOption(int from, int to) {
    int choice;
    do {
        cout << "Choose option [0 for Back, " << from << " to " << to << "] ? ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            showErrorMessage("Invalid input! Please enter 0 for Back or a number between " + formatInt(from) + " and " + formatInt(to) + ".");
            continue;
        }

        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    } while (((choice < from) || (choice > to)) && choice != 0);

    return choice;
}
// Read password input (min 8 chars, with uppercase, lowercase, digit)
string readPassword() {
    string password;
    do {
        password = readNonEmptyString("Please Enter Password? ");
        if (password.length() < 8) {
            showErrorMessage("Password must be at least 8 characters!");
        }
        else {
            bool hasUpper = false, hasLower = false, hasDigit = false;
            for (char c : password) {
                if (isupper(c)) hasUpper = true;
                else if (islower(c)) hasLower = true;
                else if (isdigit(c)) hasDigit = true;
            }
            if (!hasUpper || !hasLower || !hasDigit) {
                showErrorMessage("Password must contain uppercase, lowercase, and a digit!");
                password.clear();
            }
        }
    } while (password.empty() || password.length() < 8);
    return password;
}
// Ask user for confirmation (y/n)
bool confirmAction(string s) {
    char c;
    do {
        cout << MAGENTA << s << RESET << " "
            << GREEN << "y" << RESET << "/"
            << RED << "n" << RESET << " ?\n";
        cin >> c;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');
    return (c == 'Y' || c == 'y');
}
