#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: MenuManager.h                                    ||
//  || Section: Menu Manager                                  ||
//  || Functions to build, display, and execute main menu     ||
//  || and user management menu options.                      ||
//  ||========================================================||

#include "Globals.h"
#include "Utilities.h"
#include "InputManager.h"
#include "PermissionManager.h"
#include "ClientManager.h"
#include "TransactionManager.h"
#include "UserManager.h"
#include "Session.h"

//=====================================================
//==================== Menu Manager ===================
//=====================================================

// Forward declare login (defined in AuthManager.h)
void login();

// Build main menu options based on permissions
vector<string> buildMainMenuOptions() {
    vector<string> options;
    if (hasPermission(Permission::pAll))
        options = { "Show Client List","Add New Client","Delete Client","Update Client","Find Client","Transactions","Manage Users" };
    else {
        if (hasPermission(Permission::pListClients))  options.push_back("Show Client List");
        if (hasPermission(Permission::pAddClient))    options.push_back("Add New Client");
        if (hasPermission(Permission::pDeleteClient)) options.push_back("Delete Client");
        if (hasPermission(Permission::pUpdateClient)) options.push_back("Update Client");
        if (hasPermission(Permission::pFindClient))   options.push_back("Find Client");
        if (hasPermission(Permission::pTransactions)) options.push_back("Transactions");
        if (hasPermission(Permission::pManageUsers))  options.push_back("Manage Users");
    }
    options.push_back("Logout");

    return options;
}
// Convert numeric choice to menu option enum
MainMenuOption convertChoiceToMainMenuOption(int choice, const vector<string>& options) {
    string selectedOption = options[choice - 1];

    if (selectedOption == "Show Client List") return MainMenuOption::ShowClientList;
    if (selectedOption == "Add New Client")   return MainMenuOption::AddNewClient;
    if (selectedOption == "Delete Client")    return MainMenuOption::DeleteClient;
    if (selectedOption == "Update Client")    return MainMenuOption::UpdateClient;
    if (selectedOption == "Find Client")      return MainMenuOption::FindClient;
    if (selectedOption == "Transactions")     return MainMenuOption::Transactions;
    if (selectedOption == "Manage Users")     return MainMenuOption::ManageUsers;
    if (selectedOption == "Logout")           return MainMenuOption::Logout;
    if (selectedOption == "Exit")             return MainMenuOption::Exit;

    return MainMenuOption::Exit;
}
// Execute selected main menu option
void executeMainMenuOption(MainMenuOption MainMenuOption, vector<strClient>& vClients) {
    bool userHasPermission = true;
    switch (MainMenuOption) {
    case MainMenuOption::ShowClientList:
        userHasPermission = hasPermission(Permission::pListClients);
        if (userHasPermission) {
            showAllClientsReport(vClients);
        }
        break;
    case MainMenuOption::AddNewClient:
        userHasPermission = hasPermission(Permission::pAddClient);
        if (userHasPermission) {
            showAddClientScreen(vClients);
            vClients = loadClientsDataFromFile(ClientsFileName);
        }
        break;
    case MainMenuOption::DeleteClient:
        userHasPermission = hasPermission(Permission::pDeleteClient);
        if (userHasPermission) {
            showDeleteClientScreen(vClients);
        }
        break;
    case MainMenuOption::UpdateClient:
        userHasPermission = hasPermission(Permission::pUpdateClient);
        if (userHasPermission) {
            showUpdateClientScreen(vClients);
            vClients = loadClientsDataFromFile(ClientsFileName);
        }
        break;
    case MainMenuOption::FindClient:
        userHasPermission = hasPermission(Permission::pFindClient);
        if (userHasPermission) {
            showFindClientScreen(vClients);
        }
        break;
    case MainMenuOption::Transactions:
        userHasPermission = hasPermission(Permission::pTransactions);
        if (userHasPermission) {
            manageTransactions(vClients);
            vClients = loadClientsDataFromFile(ClientsFileName);
        }
        break;
    case MainMenuOption::ManageUsers: {
        userHasPermission = hasPermission(Permission::pManageUsers);
        if (userHasPermission) {
            vector<strUser> vUsers = loadUsersDataFromFile(UsersFileName);
            showManageUsersMenu(vUsers);
            saveUsersToFile(UsersFileName, vUsers);
        }
        break;
    }
    case MainMenuOption::Logout:
        if (confirmAction("Are you sure you want to logout?")) {
            clearCurrentUserSession();
            showSuccessMessage("You have been logged out successfully. Session cleared.");
            logUserAction("LOGOUT", "User: " + CurrentUser.UserName);
            pressEnterToContinue();
            login();
        }
        break;
    case MainMenuOption::Exit:
        showExitScreen();
        exit(0);
        break;
    default:
        showErrorMessage("Invalid option.");
        break;
    }
    if (!userHasPermission) {
        clearScreen();
        cout << "\n------------------------------------\n";
        showErrorMessage("Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.");
        cout << "\n------------------------------------\n";
    }
}
// Main loop: show menu, execute options, repeat until exit
void showMainMenu(vector<strClient>& vClients) {
    int choiceNum;
    strUser sessionUser;
    MainMenuOption Choice;
    do {
        clearScreen();
        showScreenHeader("Main Menu Screen");
        if (loadCurrentUserSession(sessionUser)) {
            showSuccessMessage("Welcome back, " + sessionUser.UserName + "!");
        }
        vector<string> options = buildMainMenuOptions();
        showOptions(options);
        showBackOrExit(true);
        showLine(60, '-', CYAN);

        choiceNum = readMenuOption(1, options.size());
        if (choiceNum == 0) {
            showExitScreen();
            exit(0);
        }
        Choice = convertChoiceToMainMenuOption(choiceNum, options);

        executeMainMenuOption(Choice, vClients);

    } while (choiceNum != 0 && Choice != MainMenuOption::Exit);
}
// Show manage users menu loop
void showManageUsersMenu(vector<strUser>& vUsers) {
    int choice;
    do {
        showManageUsersScreen();
        choice = readMenuOption(1, 5);
        if (choice == 0) break;
        executeUserOption((UserManagementOption)choice, vUsers);
    } while (choice != 0);
}
// Show manage users menu options
void showManageUsersScreen() {
    clearScreen();
    showScreenHeader("Manage Users Menu Screen");
    vector<string> options = { "List Users","Add New User","Delete User","Update User","Find User" };
    showOptions(options);
    showBackOrExit();
    showLine(60, '-', CYAN);
}
// Show exit screen
void showExitScreen() {
    clearScreen();
    showScreenHeader("Program Ends :-)");
    showSuccessMessage("Thank you for using BankSystem. Goodbye!");
}
