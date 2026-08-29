#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: Session.h                                        ||
//  || Section: Session Management System                     ||
//  || Functions to manage user sessions: save, load, clear,  ||
//  || and handle session paths/folders.                      ||
//  ||========================================================||

#include "Globals.h"
#include "Utilities.h"
#include "Crypto.h"

//=====================================================
//============= Session Management System =============
//=====================================================

// Get current system username safely
string getCurrentUsernameSafe() {
    string username = "default_user";

#ifdef _WIN32
    FILE* pipe = _popen("whoami", "r");
    if (pipe) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            username = buffer;
            size_t backslashPos = username.find_last_of("\\");
            if (backslashPos != string::npos) {
                username = username.substr(backslashPos + 1);
            }
            username.erase(username.find_last_not_of(" \n\r\t") + 1);
        }
        _pclose(pipe);
    }
#else
    FILE* pipe = popen("whoami", "r");
    if (pipe) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            username = buffer;
            username.erase(username.find_last_not_of(" \n\r\t") + 1);
        }
        pclose(pipe);
    }
#endif

    return username;
}
// Get local application data path
string getLocalAppDataPath() {
    string localAppDataPath;

#ifdef _WIN32
    FILE* pipe = _popen("echo %LOCALAPPDATA%", "r");
    if (pipe) {
        char buffer[512];
        if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            localAppDataPath = buffer;
            localAppDataPath.erase(localAppDataPath.find_last_not_of(" \n\r\t") + 1);
        }
        _pclose(pipe);
    }

    if (localAppDataPath.empty() || localAppDataPath == "%LOCALAPPDATA%") {
        localAppDataPath = "C:\\Users\\" + getCurrentUsernameSafe() + "\\AppData\\Local";
    }
#else
    localAppDataPath = getenv("HOME") ? string(getenv("HOME")) + "/.config" : "/tmp";
#endif

    return localAppDataPath;
}

#ifdef _WIN32
// Build full path for session file
string getSessionPath() {
    string localAppData = getLocalAppDataPath();
    string username = getCurrentUsernameSafe();
    return localAppData + "\\BankSystem\\session_" + username + ".bsess";
}
// Get folder path for session files
string getSessionFolder() {
    string localAppData = getLocalAppDataPath();
    return localAppData + "\\BankSystem\\";
}
#else
string getSessionPath() {
    string username = getCurrentUsernameSafe();
    return "/home/" + username + "/.config/BankSystem/session.bsess";
}
string getSessionFolder() {
    string username = getCurrentUsernameSafe();
    return "/home/" + username + "/.config/BankSystem/";
}
#endif

// Create session folder if not exists
void createSessionFolder() {
    string folder = getSessionFolder();
    string command;

#ifdef _WIN32
    command = "mkdir \"" + folder + "\" 2>nul";
#else
    command = "mkdir -p \"" + folder + "\" 2>/dev/null";
#endif
    system(command.c_str());
}
// Save current user session encrypted to file
void saveCurrentUserSession(const strUser& user) {
    createSessionFolder();
    string sessionPath = getSessionPath();

    try {
        vector<unsigned char> key = getEncryptionKey();
        string userData = serializeUserData(user);
        string encryptedData = encryptData(userData, key);

        ofstream file(sessionPath, ios::binary);
        if (file.is_open()) {
            size_t dataSize = encryptedData.size();
            file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
            file.write(encryptedData.c_str(), dataSize);
            file.close();
        }
    }
    catch (const exception& e) {
        showErrorMessage("Error: " + string(e.what()));
    }
}
// Load and decrypt current user session from file
bool loadCurrentUserSession(strUser& user) {
    string sessionPath = getSessionPath();

    ifstream file(sessionPath, ios::binary | ios::ate);
    if (!file.is_open()) {
        return false;
    }

    streamsize fileSize = file.tellg();
    if (fileSize < sizeof(size_t) || fileSize > 10 * 1024 * 1024) {
        file.close();
        return false;
    }

    file.seekg(0);

    try {
        vector<unsigned char> key = getEncryptionKey();

        size_t dataSize;
        file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));

        if (dataSize > fileSize - sizeof(size_t) || dataSize < crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES) {
            file.close();
            return false;
        }

        string encryptedData(dataSize, '\0');
        file.read(&encryptedData[0], dataSize);
        file.close();

        string decryptedData = decryptData(encryptedData, key);

        if (decryptedData.empty() || decryptedData.find('\n') == string::npos) {
            return false;
        }

        user = deserializeUserData(decryptedData);

        if (user.UserName.empty() || user.Password.empty()) {
            return false;
        }

        return true;
    }
    catch (const exception& e) {
        if (file.is_open()) file.close();
        return false;
    }
}
// Securely clear and remove session file
void clearCurrentUserSession() {
    string sessionPath = getSessionPath();

    fstream file(sessionPath, ios::binary | ios::out | ios::in);
    if (file.is_open()) {
        file.seekp(0);
        streamsize fileSize = file.tellg();
        file.seekp(0, ios::beg);

        for (int pass = 0; pass < 3; pass++) {
            vector<char> randomData(fileSize);
            if (pass == 0) {
                fill(randomData.begin(), randomData.end(), 0);
            }
            else if (pass == 1) {
                fill(randomData.begin(), randomData.end(), 0xFF);
            }
            else {
                randombytes_buf(randomData.data(), randomData.size());
            }
            file.write(randomData.data(), randomData.size());
            file.flush();
        }
        file.close();
    }

    if (remove(sessionPath.c_str()) != 0) {
        ofstream truncateFile(sessionPath, ios::trunc);
        if (truncateFile.is_open()) {
            truncateFile.close();
        }
    }

    CurrentUser = strUser();
}
// Convert user struct to string for session storage
string serializeUserData(const strUser& user) {
    string data;
    data += user.UserName + "\n";
    data += user.Password + "\n";
    data += formatInt(user.Permissions);
    return data;
}
// Convert string back to user struct
strUser deserializeUserData(const string& data) {
    strUser user;
    stringstream ss(data);
    string line;

    getline(ss, user.UserName);
    getline(ss, user.Password);

    getline(ss, line);
    user.Permissions = stoi(line);

    return user;
}
