#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: FileManager.h                                    ||
//  || Section: File Manager                                  ||
//  || Functions to serialize/deserialize Clients, Users,     ||
//  || Transactions, and handle all file I/O operations.      ||
//  ||========================================================||

#include "Globals.h"
#include "Utilities.h"
#include "Logger.h"

//=====================================================
//==================== File Manager ===================
//=====================================================

// Split string into tokens using delimiter
vector<string> splitStringByDelimiter(string S1, string delim = Separator) {
    vector<string> split;
    size_t pos = 0;
    string sWord;

    while ((pos = S1.find(delim)) != string::npos) {
        sWord = S1.substr(0, pos);
        if (sWord != "") {
            split.push_back(sWord);
        }
        S1.erase(0, pos + delim.length());
    }
    if (S1 != "") {
        split.push_back(S1);
    }
    return split;
}
// Convert a client struct to file line
string serializeClientRecord(const strClient& clientData, const string& seperator = Separator) {
    string Line = "";
    Line += clientData.AccountNumber + seperator;
    Line += clientData.PinCode + seperator;
    Line += clientData.Name + seperator;
    Line += clientData.Phone + seperator;
    Line += formatDouble(clientData.AccountBalance);
    return Line;
}
// Convert file line to Client struct
strClient deserializeClientRecord(const string& Line, const string& seperator = Separator) {
    strClient Client;
    vector<string> vClientData = splitStringByDelimiter(Line, seperator);

    if (vClientData.size() < 5) {
        logMessage("Invalid client record: expected 5 fields, got " +
            formatInt(vClientData.size()), ERROR_LOG);
        Client.MarkForDelete = true;
        return Client;
    }

    try {
        Client.AccountNumber = trim(vClientData[0]);
        Client.PinCode = trim(vClientData[1]);
        Client.Name = trim(vClientData[2]);
        Client.Phone = trim(vClientData[3]);
        Client.AccountBalance = stod(vClientData[4]);
        Client.MarkForDelete = false;
        return Client;
    }
    catch (const invalid_argument& e) {
        logMessage("Invalid number format in balance field: " + vClientData[4], ERROR_LOG);
        Client.MarkForDelete = true;
        return Client;
    }
    catch (const out_of_range& e) {
        logMessage("Balance value out of range: " + vClientData[4], ERROR_LOG);
        Client.MarkForDelete = true;
        return Client;
    }
}
// Convert User struct to file line
string serializeUserRecord(const strUser& userInfo, const string& separator = Separator) {
    string line = "";
    line += userInfo.UserName + separator;
    line += userInfo.Password + separator;
    line += formatInt(userInfo.Permissions);
    return line;
}
// Convert file line to User struct
strUser deserializeUserRecord(string Line, const string& seperator = Separator) {
    strUser userInfo;
    vector<string> vUsersData = splitStringByDelimiter(Line, seperator);

    if (vUsersData.size() < 3) {
        logMessage("Invalid user record: expected 3 fields, got " +
            formatInt(vUsersData.size()), ERROR_LOG);
        userInfo.MarkForDelete = true;
        return userInfo;
    }

    try {
        userInfo.UserName = trim(vUsersData[0]);
        userInfo.Password = trim(vUsersData[1]);
        userInfo.Permissions = stoi(vUsersData[2]);
        userInfo.MarkForDelete = false;
        return userInfo;
    }
    catch (const invalid_argument& e) {
        logMessage("Invalid permissions format: " + vUsersData[2], ERROR_LOG);
        userInfo.MarkForDelete = true;
        return userInfo;
    }
    catch (const out_of_range& e) {
        logMessage("Permissions value out of range: " + vUsersData[2], ERROR_LOG);
        userInfo.MarkForDelete = true;
        return userInfo;
    }
}
// Convert file line to Transaction struct
Transaction deserializeTransactionRecord(const string& line, const string& separator = Separator) {
    Transaction txn;
    vector<string> vTxnData = splitStringByDelimiter(line, separator);

    if (vTxnData.size() < 8) {
        logMessage("Invalid transaction record: expected 8 fields, got " +
            formatInt(vTxnData.size()), ERROR_LOG);
        return txn;
    }

    try {
        txn.TransactionID = trim(vTxnData[0]);
        txn.Type = static_cast<TransactionType>(stoi(vTxnData[1]));
        txn.FromAccount = trim(vTxnData[2]);
        txn.ToAccount = trim(vTxnData[3]);
        txn.Amount = stod(vTxnData[4]);
        txn.Fees = stod(vTxnData[5]);
        txn.Timestamp = trim(vTxnData[6]);
        txn.Description = trim(vTxnData[7]);
        return txn;
    }
    catch (const invalid_argument& e) {
        logMessage("Invalid transaction data format: " + string(e.what()), ERROR_LOG);
        return txn;
    }
    catch (const out_of_range& e) {
        logMessage("Transaction value out of range: " + string(e.what()), ERROR_LOG);
        return txn;
    }
}

//=====================================================
//============ Atomic File Save Functions =============
// These functions prevent data loss by:
// 1. Writing to temporary file first
// 2. Creating backup of original
// 3. Atomically renaming temp to actual file
//=====================================================

// Atomic save for Clients (prevents data loss)
bool saveClientsToFileAtomic(const string& fileName, const vector<strClient>& vClients) {
    string tempFile = fileName + ".tmp";
    string backupFile = fileName + ".bak";

    try {
        ofstream tempFileStream(tempFile);
        if (!tempFileStream.is_open()) {
            logMessage("Failed to open temp file for writing: " + tempFile, ERROR_LOG);
            return false;
        }

        for (const strClient& c : vClients) {
            if (!c.MarkForDelete) {
                string dataLine = serializeClientRecord(c, Separator);
                tempFileStream << dataLine << endl;
            }
        }
        tempFileStream.close();

        ifstream originalFile(fileName);
        if (originalFile.good()) {
            originalFile.close();
            ifstream src(fileName, ios::binary);
            ofstream dst(backupFile, ios::binary);
            if (src.is_open() && dst.is_open()) {
                dst << src.rdbuf();
                src.close();
                dst.close();
                logMessage("Backup created: " + backupFile, INFO);
            }
        }

#ifdef _WIN32
        remove(fileName.c_str());
#endif

        if (rename(tempFile.c_str(), fileName.c_str()) != 0) {
            logMessage("Failed to rename temp file to actual file", ERROR_LOG);
            return false;
        }

        logMessage("Clients saved successfully (" + formatInt(vClients.size()) + " records)", INFO);
        return true;
    }
    catch (const exception& e) {
        logMessage("Exception during file save: " + string(e.what()), ERROR_LOG);
        remove(tempFile.c_str());
        return false;
    }
}
// Atomic save for Users (prevents data loss)
bool saveUsersToFileAtomic(const string& fileName, const vector<strUser>& vUsers) {
    string tempFile = fileName + ".tmp";
    string backupFile = fileName + ".bak";

    try {
        ofstream tempFileStream(tempFile);
        if (!tempFileStream.is_open()) {
            logMessage("Failed to open temp file for writing: " + tempFile, ERROR_LOG);
            return false;
        }

        for (const strUser& u : vUsers) {
            if (!u.MarkForDelete) {
                string dataLine = serializeUserRecord(u, Separator);
                tempFileStream << dataLine << endl;
            }
        }
        tempFileStream.close();

        ifstream originalFile(fileName);
        if (originalFile.good()) {
            originalFile.close();
            ifstream src(fileName, ios::binary);
            ofstream dst(backupFile, ios::binary);
            if (src.is_open() && dst.is_open()) {
                dst << src.rdbuf();
                src.close();
                dst.close();
                logMessage("Backup created: " + backupFile, INFO);
            }
        }

#ifdef _WIN32
        remove(fileName.c_str());
#endif

        if (rename(tempFile.c_str(), fileName.c_str()) != 0) {
            logMessage("Failed to rename temp file to actual file", ERROR_LOG);
            return false;
        }

        logMessage("Users saved successfully (" + formatInt(vUsers.size()) + " records)", INFO);
        return true;
    }
    catch (const exception& e) {
        logMessage("Exception during file save: " + string(e.what()), ERROR_LOG);
        remove(tempFile.c_str());
        return false;
    }
}
// Validate file exists and is readable
bool validateFileBeforeLoad(const string& fileName, const string& fileType) {
    ifstream file(fileName);
    if (!file.good()) {
        logMessage(fileType + " file not found: " + fileName + " (will create new)", INFO);
        return false;
    }

    file.seekg(0, ios::end);
    streamsize fileSize = file.tellg();
    file.seekg(0, ios::beg);

    if (fileSize == 0) {
        logMessage(fileType + " file is empty (will initialize)", INFO);
        file.close();
        return false;
    }

    const streamsize MAX_FILE_SIZE = 100 * 1024 * 1024; // 100MB
    if (fileSize > MAX_FILE_SIZE) {
        logMessage(fileType + " file suspiciously large: " + formatInt(fileSize) + " bytes", WARNING);
        file.close();
        return false;
    }

    file.close();
    return true;
}
// Load all clients from file, return vector of clients
vector<strClient> loadClientsDataFromFile(const string& fileName) {
    vector<strClient> vClients;

    if (!validateFileBeforeLoad(fileName, "Clients")) {
        return vClients;
    }

    fstream myFile;
    myFile.open(fileName, ios::in);

    if (myFile.is_open()) {
        string Line;
        strClient Client;
        int lineNumber = 0;
        int validRecords = 0;
        int skippedRecords = 0;

        while (getline(myFile, Line)) {
            lineNumber++;
            if (trim(Line).empty()) continue;

            try {
                Client = deserializeClientRecord(Line, Separator);
                if (!Client.MarkForDelete || !Client.AccountNumber.empty()) {
                    vClients.push_back(Client);
                    validRecords++;
                }
                else {
                    skippedRecords++;
                    logMessage("Skipped invalid client record at line " + formatInt(lineNumber), WARNING);
                }
            }
            catch (const exception& e) {
                skippedRecords++;
                logMessage("Error parsing line " + formatInt(lineNumber) + ": " + string(e.what()), ERROR_LOG);
            }
        }
        myFile.close();

        logMessage("Loaded " + formatInt(validRecords) + " clients (" +
            formatInt(skippedRecords) + " skipped)", INFO);
    }

    return vClients;
}
// Save all clients to file (skip those marked for deletion)
void saveClientsToFile(string FileName, const vector<strClient>& vClients) {
    if (!saveClientsToFileAtomic(FileName, vClients)) {
        showErrorMessage("Failed to save clients data. Check system log for details.");
        logMessage("saveClientsToFile failed for: " + FileName, CRITICAL);
    }
}
// Load all Users from file, return vector of Users
vector<strUser> loadUsersDataFromFile(const string& fileName) {
    vector<strUser> vUsers;

    if (!validateFileBeforeLoad(fileName, "Users")) {
        return vUsers;
    }

    fstream myFile;
    myFile.open(fileName, ios::in);

    if (myFile.is_open()) {
        string Line;
        strUser User;
        int lineNumber = 0;
        int validRecords = 0;
        int skippedRecords = 0;

        while (getline(myFile, Line)) {
            lineNumber++;
            if (trim(Line).empty()) continue;

            try {
                User = deserializeUserRecord(Line, Separator);
                if (!User.MarkForDelete || !User.UserName.empty()) {
                    vUsers.push_back(User);
                    validRecords++;
                }
                else {
                    skippedRecords++;
                    logMessage("Skipped invalid user record at line " + formatInt(lineNumber), WARNING);
                }
            }
            catch (const exception& e) {
                skippedRecords++;
                logMessage("Error parsing user line " + formatInt(lineNumber) + ": " + string(e.what()), ERROR_LOG);
            }
        }
        myFile.close();

        logMessage("Loaded " + formatInt(validRecords) + " users (" +
            formatInt(skippedRecords) + " skipped)", INFO);
    }

    return vUsers;
}
// Save all Users to file (skip those marked for deletion)
void saveUsersToFile(string FileName, const vector<strUser>& vUsers) {
    if (!saveUsersToFileAtomic(FileName, vUsers)) {
        showErrorMessage("Failed to save users data. Check system log for details.");
        logMessage("saveUsersToFile failed for: " + FileName, CRITICAL);
    }
}
// Load all Transactions from file, return vector of Transactions
vector<Transaction> loadTransactionsFromFile(const string& fileName) {
    vector<Transaction> transactions;
    fstream file(fileName, ios::in);

    if (!file.is_open()) {
        return transactions;
    }

    try {
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                Transaction txn = deserializeTransactionRecord(line);
                transactions.push_back(txn);
            }
        }
        file.close();
    }
    catch (const exception& e) {
        file.close();
        throw runtime_error(string("Error loading transactions: ") + e.what());
    }

    return transactions;
}
// Save a single transaction to file
void saveTransactionToFile(const Transaction& transaction) {
    string transactionLine = transaction.TransactionID + Separator +
        formatInt(transaction.Type) + Separator +
        transaction.FromAccount + Separator +
        transaction.ToAccount + Separator +
        formatDouble(transaction.Amount) + Separator +
        formatDouble(transaction.Fees) + Separator +
        transaction.Timestamp + Separator +
        transaction.Description;

    try {
        appendLineToFile(TransactionsFileName, transactionLine);
    }
    catch (const exception& e) {
        throw runtime_error(string("Error saving transaction: ") + e.what());
    }
}
// Append line to file
void appendLineToFile(const string& FileName, const string& stDataLine) {
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (!MyFile.is_open()) {
        throw runtime_error("Cannot open file: " + FileName);
    }

    MyFile << stDataLine << endl;

    if (MyFile.fail()) {
        MyFile.close();
        throw runtime_error("Failed to write to file: " + FileName);
    }

    MyFile.close();
}
