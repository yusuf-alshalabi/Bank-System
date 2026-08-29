#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: AuthManager.h                                    ||
//  || Section: Auth Manager                                  ||
//  || Functions for authentication: hashing, verifying       ||
//  || passwords, login, and default admin creation.          ||
//  ||========================================================||

#include "Globals.h"
#include "Utilities.h"
#include "InputManager.h"
#include "FileManager.h"
#include "Logger.h"
#include "Session.h"
#include "UserManager.h"

//=====================================================
//==================== Auth Manager ===================
//=====================================================

// Forward declare showMainMenu (defined in MenuManager.h)
void showMainMenu(vector<strClient>& vClients);

// Hash password securely using libsodium
string hashPassword(const string& password) {
    char hashed[crypto_pwhash_STRBYTES];

    if (crypto_pwhash_str(
        hashed,
        password.c_str(),
        password.length(),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,
        crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        throw runtime_error("Password hashing failed - out of memory");
    }

    return string(hashed);
}
// Helper to start a user session and open main menu
void startSession(const strUser& user) {
    CurrentUser = user;
    saveCurrentUserSession(CurrentUser);

    vector<strClient> vClients = loadClientsDataFromFile(ClientsFileName);
    showMainMenu(vClients);
}
// Handle user login and session management
void login() {
    strUser sessionUser;
    vector<strUser> vUsers = loadUsersDataFromFile(UsersFileName);

    if (!vUsers.empty() && loadCurrentUserSession(sessionUser)) {
        startSession(sessionUser);
        return;
    }

    bool found = false;

    do {
        clearScreen();
        showScreenHeader("Login Screen");
        string name = readNonEmptyString("Please Enter UserName? ");
        string password = readNonEmptyString("Please Enter Password? ");

        strUser* user = findUserByUsername(name, vUsers);
        found = verifyUserPassword(password, user);

        if (found) {
            logLoginAttempt(user->UserName, true);
            showSuccessMessage("Login successful! Welcome, " + user->UserName + "!");
            pressEnterToContinue();

            startSession(*user);
            return;
        }
        else {
            showErrorMessage("Invalid username or password, try again.");
            logLoginAttempt(name, false);
            pressEnterToContinue();
        }
    } while (!found);
}
// Create first admin user interactively if none exists
void createDefaultAdmin() {
    vector<strUser> vUsers = loadUsersDataFromFile(UsersFileName);

    if (vUsers.empty()) {
        cout << "\nNo users found in the system.\n";
        cout << "Please create the first Admin account:\n";

        strUser adminUser;
        adminUser.UserName = readNonEmptyString("Enter Admin Username: ");
        string rawPassword = readPassword();
        adminUser.Password = hashPassword(rawPassword);
        adminUser.Permissions = Permission::pAll;

        vUsers.push_back(adminUser);
        saveUsersToFile(UsersFileName, vUsers);

        showSuccessMessage("Admin user created successfully!");
        cout << "Username: " << adminUser.UserName << "\n";
        cout << "Please remember your password securely.\n";
        pressEnterToContinue();

        startSession(adminUser);
    }
}
