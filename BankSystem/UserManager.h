#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: UserManager.h                                    ||
//  || Section: User Manager                                  ||
//  || Functions to manage users: add, update, delete,        ||
//  || find, list, and display user information.              ||
//  ||========================================================||

#include "Globals.h"
#include "Utilities.h"
#include "InputManager.h"
#include "FileManager.h"
#include "Logger.h"

//=====================================================
//==================== User Manager ===================
//=====================================================

// Forward declare hashPassword (defined in AuthManager.h)
string hashPassword(const string& password);
bool   verifyPassword(const string& password, const string& hashedPassword);

// Find user by username
strUser* findUserByUsername(const string& userName, vector<strUser>& vUsers) {
    for (auto& user : vUsers) {
        if (user.UserName == userName)
            return &user;
    }
    return nullptr;
}
// Mark User for deletion using pointer
bool markUserForDelete(strUser* user) {
    if (user == nullptr)
        return false;

    user->MarkForDelete = true;
    return true;
}
// Read user data interactively (with password & permissions)
strUser readUserData(const string& userName) {
    strUser user;
    user.UserName = userName;
    string rawPassword = readPassword();
    user.Password = hashPassword(rawPassword);
    user.Permissions = readUserPermissions();
    return user;
}
// Verify raw password against hashed password
bool verifyPassword(const string& password, const string& hashedPassword) {
    if (hashedPassword.empty()) {
        return false;
    }
    return crypto_pwhash_str_verify(
        hashedPassword.c_str(),
        password.c_str(),
        password.length()) == 0;
}
// Verify user password using struct pointer
bool verifyUserPassword(const string& password, strUser* user) {
    if (user == nullptr) {
        return false;
    }
    return verifyPassword(password, user->Password);
}
// Format permissions integer to readable string
string formatPermissions(int permissions) {
    if (permissions == Permission::pAll)
        return "Full Access";

    vector<pair<int, string>> mapping = {
        {Permission::pListClients,  "List Clients"},
        {Permission::pAddClient,    "Add Clients"},
        {Permission::pDeleteClient, "Delete Clients"},
        {Permission::pUpdateClient, "Update Clients"},
        {Permission::pFindClient,   "Find Clients"},
        {Permission::pTransactions, "Transactions"},
        {Permission::pManageUsers,  "Manage Users"}
    };

    vector<string> resultList;
    for (auto& m : mapping) {
        if (permissions & m.first) resultList.push_back(m.second);
    }

    if (resultList.empty()) return "No Permissions";

    string result;
    for (size_t i = 0; i < resultList.size(); i++) {
        result += resultList[i];
        if (i < resultList.size() - 1) result += ", ";
    }
    return result;
}
// Display user info in formatted card
void showUserCard(strUser* User) {
    cout << "\n";
    showBorderLine(102, '=');
    cout << "|  " << left << setw(100) << "User Information" << "|\n";
    cout << "+" << string(102, '=') << "+\n";

    cout << "|  Username    : " << left << setw(86) << User->UserName << "|\n";
    cout << "|  Permissions : " << left << setw(86) << formatPermissions(User->Permissions) << "|\n";

    showBorderLine(102, '=');
}
// Count how many users have full access permissions
int countFullAccessUsers(const vector<strUser>& vUsers) {
    int count = 0;
    for (const strUser& user : vUsers) {
        if (user.Permissions == Permission::pAll) {
            count++;
        }
    }
    return count;
}
// Display all users in table format
void showUsersListScreen(const vector<strUser>& vUsers) {
    clearScreen();
    showScreenHeader("Users List");

    cout << "Total Users: " << vUsers.size() << "\n\n";

    if (vUsers.size() == 0) {
        showErrorMessage("No users available in the system!");
        backToMenu();
        return;
    }

    showBorderLine(120, '-', CYAN);
    cout << CYAN << "| " << left << setw(25) << "Username"
        << "| " << setw(92) << "Permissions" << "|\n";
    showBorderLine(120, '-', CYAN);

    for (const strUser& user : vUsers) {
        cout << CYAN << "| " << RESET << left << setw(25) << user.UserName
            << CYAN << "| " << RESET << setw(92) << formatPermissions(user.Permissions) << CYAN << "|\n" << RESET;
    }

    showBorderLine(120, '-', CYAN);
    cout << "\n";

    backToMenu();
}
// Add new user with unique username
void addNewUser(vector<strUser>& vUsers) {
    clearScreen();
    showScreenHeader("Add New User Screen");

    string name = readNonEmptyString("Please Enter UserName? ");
    strUser* existingUser = findUserByUsername(name, vUsers);

    while (existingUser != nullptr) {
        showErrorMessage("User with name [" + name + "] already Exits, enter a different username?");
        name = readNonEmptyString("Please Enter UserName? ");
        existingUser = findUserByUsername(name, vUsers);
    }

    strUser newUser = readUserData(name);
    vUsers.push_back(newUser);
    appendLineToFile(UsersFileName, serializeUserRecord(newUser));

    showSuccessMessage("User Added Successfully!");
}
// Show add user screen (loop for multiple)
void showAddUserScreen(vector<strUser>& vUsers) {
    clearScreen();
    showScreenHeader("Add New User Screen");
    showBackOrExit(false);
    showLine();

    while (true) {
        string name = readNonEmptyString("Please Enter UserName (or 0 to Back)? ");
        if (name == "0") break;

        strUser* existingUser = findUserByUsername(name, vUsers);
        while (existingUser != nullptr) {
            showErrorMessage("User with name [" + name + "] already exists, enter a different username?");
            name = readNonEmptyString("Please Enter UserName (or 0 to Back)? ");
            if (name == "0") return;
            existingUser = findUserByUsername(name, vUsers);
        }

        strUser newUser = readUserData(name);
        vUsers.push_back(newUser);
        appendLineToFile(UsersFileName, serializeUserRecord(newUser));
        showSuccessMessage("User Added Successfully!");
        pressEnterToContinue();
    }
}
// Delete user with username & password
bool deleteUserWithCredentials(const string& userName, const string& password, vector<strUser>& vUsers) {
    strUser* user = findUserByUsername(userName, vUsers);

    if (user == nullptr) {
        showErrorMessage("User not found.");
        return false;
    }

    if (user->Permissions == Permission::pAll && countFullAccessUsers(vUsers) <= 1) {
        showErrorMessage("You cannot delete the last Admin user. At least one full access user must remain.");
        return false;
    }

    if (verifyUserPassword(password, user)) {
        showUserCard(user);
        if (confirmAction("Are you sure you want delete this user ?")) {
            markUserForDelete(user);
            saveUsersToFile(UsersFileName, vUsers);
            vUsers = loadUsersDataFromFile(UsersFileName);
            showSuccessMessage("User Deleted Successfully.");
            return true;
        }
        else {
            return false;
        }
    }
    else {
        showErrorMessage("Invalid username or password.");
        return false;
    }
}
// Show delete user screen
void showDeleteUserScreen(vector<strUser>& vUsers) {
    clearScreen();
    showScreenHeader("Delete User Screen");
    showBackOrExit(false);
    showLine();

    string name = readNonEmptyString("Please Enter UserName (or 0 to Back)? ");
    if (name == "0") return;
    string password = readNonEmptyString("Please Enter Password (or 0 to Back)? ");
    if (password == "0") return;

    deleteUserWithCredentials(name, password, vUsers);
    backToMenu();
}
// Update user information with credentials
bool updateUserWithCredentials(const string& userName, const string& password, vector<strUser>& vUsers) {
    strUser* user = findUserByUsername(userName, vUsers);
    if (verifyUserPassword(password, user)) {
        showUserCard(user);
        if (confirmAction("Are you sure you want update this User ?")) {
            for (strUser& u : vUsers) {
                if (u.UserName == userName) {
                    cout << "Enter new user data:\n";
                    u.UserName = userName;
                    if (confirmAction("Do you want to update password?")) {
                        string rawPassword = readPassword();
                        u.Password = hashPassword(rawPassword);
                    }

                    int newPermissions = readUserPermissions();

                    if (u.Permissions == Permission::pAll && newPermissions != Permission::pAll
                        && countFullAccessUsers(vUsers) <= 1) {
                        showErrorMessage("You cannot remove full access from the last Admin user.");
                        return false;
                    }

                    u.Permissions = newPermissions;
                    break;
                }
            }
            saveUsersToFile(UsersFileName, vUsers);
            showSuccessMessage("User Updated Successfully.");
            return true;
        }
        return false;
    }
    else {
        showErrorMessage("User with name (" + userName + ") and password (" + password + ") is not found!.");
        return false;
    }
}
// Show update user screen
void showUpdateUserScreen(vector<strUser>& vUsers) {
    clearScreen();
    showScreenHeader("Update User Screen");
    showBackOrExit(false);
    showLine();

    bool success = false;
    do {
        string name = readNonEmptyString("Please Enter UserName (or 0 to Back)? ");
        if (name == "0") return;
        string password = readNonEmptyString("Please Enter Password (or 0 to Back)? ");
        if (password == "0") return;

        success = updateUserWithCredentials(name, password, vUsers);
    } while (!success);
    backToMenu();
}
// Show find user screen and display information
void showFindUserScreen(vector<strUser>& vUsers) {
    clearScreen();
    showScreenHeader("Find User Screen");
    showBackOrExit(false);
    showLine();

    string name = readNonEmptyString("Please Enter UserName (or 0 to Back)? ");
    if (name == "0") return;

    strUser* user = findUserByUsername(name, vUsers);
    string password = readNonEmptyString("Please Enter Password (or 0 to Back)? ");
    if (password == "0") return;

    if (verifyUserPassword(password, user)) {
        showUserCard(user);
    }
    else {
        showErrorMessage("User with name (" + name + ") and password (" + password + ") is not found!.");
    }
    backToMenu();
}
// Execute selected user management option
void executeUserOption(UserManagementOption manageUsersOptions, vector<strUser>& vUsers) {
    switch (manageUsersOptions) {
    case UserManagementOption::ListUser:
        showUsersListScreen(vUsers);
        break;
    case UserManagementOption::AddNewUser:
        showAddUserScreen(vUsers);
        break;
    case UserManagementOption::DeleteUser:
        showDeleteUserScreen(vUsers);
        break;
    case UserManagementOption::FindUser:
        showFindUserScreen(vUsers);
        break;
    case UserManagementOption::UpdateUser:
        showUpdateUserScreen(vUsers);
        break;
    case UserManagementOption::MainMenu:
        return;
    }
}
