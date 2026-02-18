#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: ClientManager.h                                  ||
//  || Section: Client Manager                                ||
//  || Functions to manage clients: add, update, delete,      ||
//  || find, and display client information.                  ||
//  ||========================================================||

#include "Globals.h"
#include "Utilities.h"
#include "InputManager.h"
#include "FileManager.h"
#include "Logger.h"

//=====================================================
//==================== Client Manager =================
//=====================================================

// Search for a client by account number, return pointer to client if found
strClient* findClientByAccountNumber(const string& accountNumber, vector<strClient>& vClients) {
    for (auto& c : vClients) {
        if (c.AccountNumber == accountNumber)
            return &c;
    }
    return nullptr;
}
// Mark client for deletion using pointer
bool markClientForDelete(strClient* client) {
    if (client == nullptr)
        return false;

    client->MarkForDelete = true;
    return true;
}
// Display client information in formatted card
void showClientCard(const strClient& client) {
    cout << "\n";
    showBorderLine(58, '=');
    cout << "|  " << left << setw(56) << "Client Information" << "|\n";
    cout << "+" << string(58, '=') << "+\n";

    cout << "|  Account Number : " << left << setw(39) << client.AccountNumber << "|\n";
    cout << "|  PIN Code       : " << left << setw(39) << client.PinCode << "|\n";
    cout << "|  Name           : " << left << setw(39) << client.Name << "|\n";
    cout << "|  Phone          : " << left << setw(39) << client.Phone << "|\n";
    cout << "|  Balance        : " << left << setw(39) << formatCurrency(client.AccountBalance) << "|\n";

    showBorderLine(58, '=');
}
// Display all clients in table format
void showAllClientsReport(const vector<strClient>& vClients) {
    clearScreen();
    showScreenHeader("Clients List");

    cout << "Total Clients: " << vClients.size() << "\n\n";

    if (vClients.size() == 0) {
        showErrorMessage("No clients available in the system!");
        backToMenu();
        return;
    }

    showBorderLine(105, '-', CYAN);
    cout << CYAN << "| " << left << setw(18) << "Account Number"
        << "| " << setw(12) << "PIN Code"
        << "| " << setw(30) << "Client Name"
        << "| " << setw(15) << "Phone"
        << "| " << setw(21) << "Balance" << "|\n";
    showBorderLine(105, '-', CYAN);

    for (const strClient& Client : vClients) {
        string balanceColor = (Client.AccountBalance >= 0) ? GREEN : RED;
        cout << CYAN << "| " << RESET << left << setw(18) << Client.AccountNumber
            << CYAN << "| " << RESET << setw(12) << Client.PinCode
            << CYAN << "| " << RESET << setw(30) << Client.Name
            << CYAN << "| " << RESET << setw(15) << Client.Phone
            << CYAN << "| " << RESET << balanceColor << setw(21)
            << fixed << setprecision(2) << formatCurrency(Client.AccountBalance)
            << CYAN << "|\n" << RESET;
    }

    showBorderLine(105, '-', CYAN);
    backToMenu();
}
// Add client with unique account number
void addNewClient(vector<strClient>& vClients) {
    strClient newClient;
    string accountNumber = readValidatedAccountNumber("\nPlease enter AccountNumber? ");
    strClient* findClient = findClientByAccountNumber(accountNumber, vClients);

    while (findClient != nullptr) {
        showErrorMessage("Client with accountNumber [" + accountNumber + "] already exists, enter another account number?");
        accountNumber = readValidatedAccountNumber("\nPlease enter AccountNumber? ");
        findClient = findClientByAccountNumber(accountNumber, vClients);
    }

    newClient = readClientData(accountNumber);
    vClients.push_back(newClient);
    appendLineToFile(ClientsFileName, serializeClientRecord(newClient));
    showSuccessMessage("Client Added Successfully!");

    logUserAction("ADD_CLIENT", "Account: " + newClient.AccountNumber + " - Name: " + newClient.Name);
}
// Show Add New Client screen (loop for multiple)
void showAddClientScreen(vector<strClient>& vClients) {
    clearScreen();
    showScreenHeader("Add New Client Screen");
    showBackOrExit();
    showLine();

    while (true) {
        string accountNumber = readNonEmptyString("\nPlease enter AccountNumber (or 0 to Back)? ");
        if (accountNumber == "0") break;

        strClient* findClient = findClientByAccountNumber(accountNumber, vClients);
        while (findClient != nullptr) {
            showErrorMessage("Client with accountNumber [" + accountNumber + "] already exists, enter another account number?");
            accountNumber = readNonEmptyString("\nPlease enter AccountNumber (or 0 to Back)? ");
            if (accountNumber == "0") return;
            findClient = findClientByAccountNumber(accountNumber, vClients);
        }

        strClient newClient = readClientData(accountNumber);
        vClients.push_back(newClient);
        appendLineToFile(ClientsFileName, serializeClientRecord(newClient));
        showSuccessMessage("Client Added Successfully!");
        pressEnterToContinue();
    }
}
// Delete a client by account number
bool deleteClient(const string& accountNumber, vector<strClient>& vClients) {
    strClient* client = findClientByAccountNumber(accountNumber, vClients);
    if (!client) {
        showErrorMessage("Client with Account Number (" + accountNumber + ") is not Found!");
        logUserAction("DELETE_CLIENT_FAILED", "Account not found: " + accountNumber);
        return false;
    }

    showClientCard(*client);
    if (confirmAction("Are you sure you want delete this client ?")) {
        markClientForDelete(client);
        saveClientsToFile(ClientsFileName, vClients);
        vClients = loadClientsDataFromFile(ClientsFileName);
        showSuccessMessage("Client Deleted Successfully.");
        logUserAction("DELETE_CLIENT", "Account: " + accountNumber);
        return true;
    }
    return false;
}
// Show Delete Client screen
void showDeleteClientScreen(vector<strClient>& vClients) {
    clearScreen();
    showScreenHeader("Delete Client Screen");
    showBackOrExit();
    showLine();

    string accountNumber = readNonEmptyString("\nPlease enter AccountNumber (or 0 to Back)? ");
    if (accountNumber == "0") return;

    deleteClient(accountNumber, vClients);
    backToMenu();
}
// Update client information by account number
bool updateClient(const string& accountNumber, vector<strClient>& vClients) {
    strClient* client = findClientByAccountNumber(accountNumber, vClients);
    if (!client) {
        showErrorMessage("Client with Account Number (" + accountNumber + ") is not Found!");
        logUserAction("UPDATE_CLIENT_FAILED", "Account not found: " + accountNumber);
        pressEnterToContinue();
        return false;
    }

    showClientCard(*client);
    if (confirmAction("Are you sure you want update this client ?")) {
        *client = readClientData(accountNumber);
        saveClientsToFile(ClientsFileName, vClients);
        vClients = loadClientsDataFromFile(ClientsFileName);
        showSuccessMessage("Client Updated Successfully.");
        logUserAction("UPDATE_CLIENT", "Account: " + accountNumber);
        return true;
    }
    return false;
}
// Show Update Client screen
void showUpdateClientScreen(vector<strClient>& vClients) {
    clearScreen();
    showScreenHeader("Update Info Client Screen");
    showBackOrExit(false);
    showLine();

    bool success = false;
    do {
        string accountNumber = readNonEmptyString("\nPlease enter AccountNumber (or 0 to Back)? ");
        if (accountNumber == "0") return;
        success = updateClient(accountNumber, vClients);
    } while (!success);
    backToMenu();
}
// Show Find Client screen and display information
void showFindClientScreen(vector<strClient>& vClients) {
    clearScreen();
    showScreenHeader("Find Client Screen");
    showBackOrExit(false);
    showLine();

    string accountNumber = readNonEmptyString("\nPlease enter AccountNumber (or 0 to Back)? ");
    if (accountNumber == "0") return;

    strClient* client = findClientByAccountNumber(accountNumber, vClients);
    if (!client) {
        showErrorMessage("Client with account Number [" + accountNumber + "] is not found!");
    }
    else {
        showClientCard(*client);
    }
    backToMenu();
}
