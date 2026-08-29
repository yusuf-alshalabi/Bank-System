#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: Globals.h                                        ||
//  || Section: Structs, Enums, Global Variables &            ||
//  ||          Forward Declarations                          ||
//  ||========================================================||

#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>
#include <functional>
#include <sodium.h>
#include <chrono>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

using namespace std;

//=====================================================
//======== Structs, Enums & Global Variables ==========
//=====================================================

const string ClientsFileName = "Clients.txt";
const string TransactionsFileName = "Transactions.txt";
const string UsersFileName = "Users.txt";
const string LogFileName = "SystemLog.txt";
const string Separator = "#//#";

const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";

enum TransactionType {
    DEPOSIT = 1,
    WITHDRAWAL = 2,
    TRANSFER = 3
};
enum MainMenuOption {
    ShowClientList = 1,
    AddNewClient = 2,
    DeleteClient = 3,
    UpdateClient = 4,
    FindClient = 5,
    Transactions = 6,
    ManageUsers = 7,
    Logout = 8,
    Exit = 9
};
enum TransactionsOption {
    Deposit = 1,
    Withdraw = 2,
    Transfer = 3,
    ShowTotalBalance = 4,
    ShowTransactionsHistory = 5,
    ShowMainMenu = 6
};
enum UserManagementOption {
    ListUser = 1,
    AddNewUser = 2,
    DeleteUser = 3,
    UpdateUser = 4,
    FindUser = 5,
    MainMenu = 6
};
enum Permission {
    pListClients = 1,
    pAddClient = 2,
    pDeleteClient = 4,
    pUpdateClient = 8,
    pFindClient = 16,
    pTransactions = 32,
    pManageUsers = 64,
    pAll = 127  // Full Access
};
enum LogLevel {
    INFO,
    WARNING,
    ERROR_LOG,
    CRITICAL
};

struct Transaction {
    string          TransactionID;
    TransactionType Type = TransactionType::DEPOSIT;
    string          FromAccount;
    string          ToAccount;
    double          Amount = 0.0;
    double          Fees = 0.0;
    string          Timestamp;
    string          Description;
};
struct strClient {
    string              AccountNumber;
    string              PinCode;
    string              Name;
    string              Phone;
    double              AccountBalance = 0.0;
    bool                MarkForDelete = false;
    vector<Transaction> Transactions;
};
struct strUser {
    string UserName = "";
    string Password = "";
    int    Permissions = -1;
    bool   MarkForDelete = false;
};

extern strUser CurrentUser;

//=====================================================
//=============== Forward Declarations ================
//=====================================================

// Utilities
string formatDouble(double value, int precision = 2);
string formatInt(int value);
string formatCurrency(double value);
string trim(const string& str);
string getCurrentTimestamp();
void   clearScreen();
void   drawLine(int length = 60, char symbol = '-', string color = RESET);
void   showLine(int length = 60, char symbol = '-', string color = RESET);
void   showBorderLine(int length = 60, char symbol = '-', string color = RESET);
void   showSuccessMessage(string message);
void   showErrorMessage(string message);
void   showScreenHeader(const string& title);
void   showOptions(const vector<string>& options);
void   showBackOrExit(bool isMain = false);
void   waitForEnter();
void   pressEnterToContinue();
void   backToMenu();

// Session & Encryption
vector<unsigned char> getEncryptionKey();
string serializeUserData(const strUser& user);
string encryptData(const string& plaintext, const vector<unsigned char>& key);
string decryptData(const string& encryptedData, const vector<unsigned char>& key);
strUser deserializeUserData(const string& data);

// File & Append
void appendLineToFile(const string& FileName, const string& stDataLine);

// Logging
string logLevelToString(LogLevel level);
void logMessage(const string& message, LogLevel level = INFO);
void logTransaction(const Transaction& txn);
void logLoginAttempt(const string& username, bool success);
void logUserAction(const string& action, const string& details = "");

// User & Auth
string hashPassword(const string& password);
int    readUserPermissions();
bool   verifyUserPassword(const string& password, strUser* user);

// Menus
void showMainMenu(vector<strClient>& vClients);
void showManageUsersMenu(vector<strUser>& vUsers);
void showExitScreen();
void showManageUsersScreen();
