#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: Logger.h                                         ||
//  || Section: Logging System                                ||
//  || Functions to log system activities, transactions,      ||
//  || and errors for audit trail and debugging.              ||
//  ||========================================================||

#include "Globals.h"
#include "Utilities.h"

// Forward declare appendLineToFile (defined in FileManager.h)
void appendLineToFile(const string& FileName, const string& stDataLine);

//=====================================================
//================== Logging System ===================
//=====================================================

// Convert log level enum to readable string
string logLevelToString(LogLevel level) {
    switch (level) {
    case INFO:      return "INFO";
    case WARNING:   return "WARNING";
    case ERROR_LOG: return "ERROR";
    case CRITICAL:  return "CRITICAL";
    default:        return "UNKNOWN";
    }
}
// Log message to file with timestamp, level, and user context
void logMessage(const string& message, LogLevel level) {
    try {
        string logEntry = "[" + getCurrentTimestamp() + "] " +
            "[" + logLevelToString(level) + "] " +
            "[User: " + (CurrentUser.UserName.empty() ? "SYSTEM" : CurrentUser.UserName) + "] " +
            message;

        appendLineToFile(LogFileName, logEntry);
    }
    catch (const exception& e) {
        cerr << "[LOGGING ERROR] " << e.what() << endl;
    }
}
// Log transaction activity with full details
void logTransaction(const Transaction& txn) {
    string typeStr;
    switch (txn.Type) {
    case DEPOSIT:    typeStr = "DEPOSIT";    break;
    case WITHDRAWAL: typeStr = "WITHDRAWAL"; break;
    case TRANSFER:   typeStr = "TRANSFER";   break;
    default:         typeStr = "UNKNOWN";    break;
    }

    string message = "Transaction " + txn.TransactionID +
        " - Type: " + typeStr +
        " - Amount: " + formatDouble(txn.Amount) +
        " - From: " + txn.FromAccount +
        " - To: " + txn.ToAccount +
        " - Fees: " + formatDouble(txn.Fees);

    logMessage(message, INFO);
}
// Log authentication attempts (successful or failed)
void logLoginAttempt(const string& username, bool success) {
    string message = "Login attempt for user '" + username + "' - " +
        (success ? "SUCCESS" : "FAILED");
    logMessage(message, success ? INFO : WARNING);
}
// Log general user actions with optional details
void logUserAction(const string& action, const string& details) {
    string message = "Action: " + action;
    if (!details.empty()) {
        message += " - Details: " + details;
    }
    logMessage(message, INFO);
}
