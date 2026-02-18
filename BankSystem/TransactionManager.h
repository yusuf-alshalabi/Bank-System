#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: TransactionManager.h                             ||
//  || Section: Transactions Manager                          ||
//  || Functions to handle deposits, withdrawals, transfers,  ||
//  || balance reports, and transaction history.              ||
//  ||========================================================||

#include "Globals.h"
#include "Utilities.h"
#include "InputManager.h"
#include "FileManager.h"
#include "Logger.h"
#include "ClientManager.h"

//=====================================================
//=============== Transactions Manager ================
//=====================================================

// Generate unique transaction ID
string generateTransactionID() {
    auto now = chrono::high_resolution_clock::now();
    auto timestamp = chrono::duration_cast<chrono::microseconds>(
        now.time_since_epoch()).count();

    uint32_t randomNum = randombytes_random();

    stringstream ss;
    ss << "TXN" << timestamp << hex << setw(8) << setfill('0') << randomNum;
    return ss.str();
}
// Deposit amount to client account
bool depositToClientAccount(strClient* client, double depositAmount) {
    if (client == nullptr) {
        return false;
    }
    client->AccountBalance += depositAmount;
    showSuccessMessage("Done Successfully . New Balance is : " + formatDouble(client->AccountBalance));
    return true;
}
// Create deposit transaction record
Transaction createDepositTransaction(const string& account, double amount, const string& description = "Deposit operation") {
    Transaction txn;
    txn.TransactionID = generateTransactionID();
    txn.Type = DEPOSIT;
    txn.FromAccount = account;
    txn.ToAccount = account;
    txn.Amount = amount;
    txn.Fees = 0;
    txn.Timestamp = getCurrentTimestamp();
    txn.Description = description;
    return txn;
}
// Show deposit screen and process transaction
void showDepositScreen(vector<strClient>& vClients) {
    clearScreen();
    showScreenHeader("Deposit Screen");
    showBackOrExit();
    showLine();

    string accountNumber = readNonEmptyString("\nPlease enter AccountNumber (or 0 to Back)? ");
    if (accountNumber == "0") return;

    strClient* client = findClientByAccountNumber(accountNumber, vClients);

    if (!client) {
        showErrorMessage("Account " + accountNumber + " not found. Please check the account number.");
        logUserAction("DEPOSIT_FAILED", "Account not found: " + accountNumber);
        backToMenu();
        return;
    }

    showClientCard(*client);
    double depositAmount = readPositiveNumber("Enter Deposit Amount: ");

    if (!confirmAction("Confirm deposit of " + formatDouble(depositAmount) + "?")) {
        showErrorMessage("Deposit cancelled");
        logUserAction("DEPOSIT_CANCELLED", "Account: " + accountNumber);
        backToMenu();
        return;
    }

    double originalBalance = client->AccountBalance;

    if (depositToClientAccount(client, depositAmount)) {
        Transaction depositTransaction = createDepositTransaction(client->AccountNumber, depositAmount);
        saveTransactionToFile(depositTransaction);
        saveClientsToFile(ClientsFileName, vClients);

        logTransaction(depositTransaction);
        logUserAction("DEPOSIT", "Account: " + accountNumber + " - Amount: " + formatDouble(depositAmount));

        string successMessage = string("Transaction completed successfully!\n") +
            "Transaction ID: " + depositTransaction.TransactionID + "\n" +
            "Deposited Amount: " + formatDouble(depositAmount) + "\n" +
            "Previous Balance: " + formatDouble(originalBalance) + "\n" +
            "New Balance: " + formatDouble(originalBalance + depositAmount);

        showSuccessMessage(successMessage);
        backToMenu();
    }
}
// Withdraw amount from client account
bool withdrawToClientAccount(strClient* client, double withdrawAmount) {
    if (client == nullptr) {
        showErrorMessage("Client not found!");
        backToMenu();
        return false;
    }

    if (withdrawAmount <= 0) {
        showErrorMessage("Invalid amount! Please enter a positive value.");
        backToMenu();
        return false;
    }

    if (withdrawAmount > client->AccountBalance) {
        showErrorMessage("Insufficient funds! Available balance: " +
            formatDouble(client->AccountBalance));
        backToMenu();
        return false;
    }

    client->AccountBalance -= withdrawAmount;
    showSuccessMessage("Withdrawal successful! Remaining balance: " +
        formatDouble(client->AccountBalance));
    return true;
}
// Create withdrawal transaction record
Transaction createWithdrawTransaction(const string& account, double amount, const string& description = "Withdrawal operation") {
    Transaction txn;
    txn.TransactionID = generateTransactionID();
    txn.Type = WITHDRAWAL;
    txn.FromAccount = account;
    txn.ToAccount = account;
    txn.Amount = amount;
    txn.Fees = 0;
    txn.Timestamp = getCurrentTimestamp();
    txn.Description = description;
    return txn;
}
// Show withdraw screen and process transaction
void showWithdrawScreen(vector<strClient>& vClients) {
    clearScreen();
    showScreenHeader("Withdraw Screen");
    showBackOrExit();
    showLine();

    string accountNumber = readNonEmptyString("\nPlease enter AccountNumber (or 0 to Back)? ");
    if (accountNumber == "0") return;

    strClient* client = findClientByAccountNumber(accountNumber, vClients);

    if (!client) {
        showErrorMessage("Account " + accountNumber + " not found. Please check the account number.");
        logUserAction("WITHDRAWAL_FAILED", "Account not found: " + accountNumber);
        backToMenu();
        return;
    }

    showClientCard(*client);
    double withdrawAmount = readPositiveNumber("Enter Withdraw Amount: ");

    while (withdrawAmount > client->AccountBalance) {
        showErrorMessage("Amount exceeds balance! Available: " + formatCurrency(client->AccountBalance));
        logUserAction("WITHDRAWAL_FAILED", "Insufficient funds - Account: " + accountNumber);
        if (!confirmAction("Enter a different amount?")) {
            showErrorMessage("Withdrawal cancelled");
            backToMenu();
            return;
        }
        withdrawAmount = readPositiveNumber("Enter Withdraw Amount: ");
    }

    if (!confirmAction("Confirm withdrawal of " + formatDouble(withdrawAmount) + "?")) {
        showErrorMessage("Withdrawal cancelled");
        logUserAction("WITHDRAWAL_CANCELLED", "Account: " + accountNumber);
        backToMenu();
        return;
    }

    double originalBalance = client->AccountBalance;

    if (withdrawToClientAccount(client, withdrawAmount)) {
        Transaction withdrawalTransaction = createWithdrawTransaction(client->AccountNumber, withdrawAmount);
        saveTransactionToFile(withdrawalTransaction);
        saveClientsToFile(ClientsFileName, vClients);

        logTransaction(withdrawalTransaction);
        logUserAction("WITHDRAWAL", "Account: " + accountNumber + " - Amount: " + formatCurrency(withdrawalTransaction.Amount));

        string successMessage = string("Transaction completed successfully!\n") +
            "Transaction ID: " + withdrawalTransaction.TransactionID + "\n" +
            "Withdrawn Amount: " + formatDouble(withdrawAmount) + "\n" +
            "Previous Balance: " + formatDouble(originalBalance) + "\n" +
            "New Balance: " + formatDouble(originalBalance - withdrawAmount);

        showSuccessMessage(successMessage);
        backToMenu();
    }
}
// Validate source and destination accounts
bool validateTransferAccounts(const string& fromAccount, const string& toAccount,
    strClient*& fromClient, strClient*& toClient,
    vector<strClient>& vClients) {

    fromClient = findClientByAccountNumber(fromAccount, vClients);
    if (!fromClient) {
        showErrorMessage("Your account not found!");
        return false;
    }

    toClient = findClientByAccountNumber(toAccount, vClients);
    if (!toClient) {
        showErrorMessage("Recipient account not found!");
        return false;
    }

    if (fromAccount == toAccount) {
        showErrorMessage("Cannot transfer to the same account!");
        return false;
    }

    return true;
}
// Validate transfer amount and fees
bool validateTransferAmount(double transferAmount, double transferFee, strClient* fromClient) {
    if (fromClient->AccountBalance < (transferAmount + transferFee)) {
        showErrorMessage("Insufficient balance! Total required: " +
            formatDouble(transferAmount + transferFee));
        return false;
    }
    return true;
}
// Execute transfer between accounts
bool executeTransfer(strClient* fromClient, strClient* toClient,
    double transferAmount, double transferFee) {
    fromClient->AccountBalance -= (transferAmount + transferFee);
    toClient->AccountBalance += transferAmount;
    return true;
}
// Display transfer details before confirm
void showTransferConfirmation(strClient* fromClient, strClient* toClient,
    const string& fromAccount, const string& toAccount,
    double transferAmount, double transferFee) {

    cout << "\nTransfer Details:\n";
    cout << "From: " << fromClient->Name << " (" << fromAccount << ")\n";
    cout << "To: " << toClient->Name << " (" << toAccount << ")\n";
    cout << "Amount: " << transferAmount << "\n";
    cout << "Fee: " << transferFee << "\n";
    cout << "Total: " << (transferAmount + transferFee) << "\n";
}
// Create transfer transaction record
Transaction createTransferTransaction(const string& fromAccount,
    const string& toAccount,
    double transferAmount,
    double transferFee,
    const string& description) {
    Transaction transaction;
    transaction.TransactionID = generateTransactionID();
    transaction.Type = TRANSFER;
    transaction.FromAccount = fromAccount;
    transaction.ToAccount = toAccount;
    transaction.Amount = transferAmount;
    transaction.Fees = transferFee;
    transaction.Timestamp = getCurrentTimestamp();
    transaction.Description = description;
    return transaction;
}
// Show transfer screen and process transaction
void showTransferScreen(vector<strClient>& vClients) {
    clearScreen();
    showScreenHeader("Transfer Screen");
    showBackOrExit();
    showLine();

    string fromAccount = readNonEmptyString("Enter Your Account Number (or 0 to Back)? ");
    if (fromAccount == "0") return;

    string toAccount = readNonEmptyString("Enter Recipient Account Number (or 0 to Back)? ");
    if (toAccount == "0") return;

    strClient* fromClient = nullptr;
    strClient* toClient = nullptr;
    if (!validateTransferAccounts(fromAccount, toAccount, fromClient, toClient, vClients)) {
        backToMenu();
        return;
    }

    double transferAmount = readPositiveNumber("Enter Transfer Amount: ");
    double transferFee = transferAmount * 0.01;

    while (!validateTransferAmount(transferAmount, transferFee, fromClient)) {
        if (!confirmAction("Do you want to enter a different amount?")) {
            showErrorMessage("Transfer cancelled.");
            backToMenu();
            return;
        }
        transferAmount = readPositiveNumber("Enter Transfer Amount: ");
        transferFee = transferAmount * 0.01;
    }

    showTransferConfirmation(fromClient, toClient, fromAccount, toAccount, transferAmount, transferFee);

    if (!confirmAction("Confirm transfer?")) {
        showErrorMessage("Transfer cancelled.");
        backToMenu();
        return;
    }

    double originalBalance = fromClient->AccountBalance;
    executeTransfer(fromClient, toClient, transferAmount, transferFee);

    string description = "Transfer to " + toClient->Name;
    Transaction transferTransaction = createTransferTransaction(fromAccount, toAccount,
        transferAmount, transferFee, description);
    saveTransactionToFile(transferTransaction);
    saveClientsToFile(ClientsFileName, vClients);

    logTransaction(transferTransaction);
    logUserAction("TRANSFER", "From: " + fromAccount + " To: " + toAccount + " - Amount: " + formatCurrency(transferAmount));

    string successMessage = string("Transfer completed successfully!\n") +
        "Transaction ID: " + transferTransaction.TransactionID + "\n" +
        "Transferred Amount: " + formatDouble(transferAmount) + "\n" +
        "Fee: " + formatDouble(transferFee) + "\n" +
        "Previous Balance: " + formatDouble(originalBalance) + "\n" +
        "New Balance: " + formatDouble(fromClient->AccountBalance);

    showSuccessMessage(successMessage);

    vClients = loadClientsDataFromFile(ClientsFileName);
    backToMenu();
}
// Show report of total balances for all clients
void showTotalBalancesReport(const vector<strClient>& vClients) {
    clearScreen();
    showScreenHeader("Total Balances Report");

    double totalBalance = 0;

    cout << "Total Clients: " << vClients.size() << "\n\n";

    if (vClients.size() == 0) {
        showErrorMessage("No clients available in the system!");
        backToMenu();
        return;
    }

    showBorderLine(80, '-', CYAN);
    cout << CYAN << "| " << left << setw(18) << "Account Number"
        << "| " << setw(35) << "Client Name"
        << "| " << setw(22) << "Balance" << "|\n";
    showBorderLine(80, '-', CYAN);

    for (const strClient& Client : vClients) {
        totalBalance += Client.AccountBalance;
        string balanceColor = (Client.AccountBalance >= 0) ? GREEN : RED;
        cout << CYAN << "| " << RESET << left << setw(18) << Client.AccountNumber
            << CYAN << "| " << RESET << setw(35) << Client.Name
            << CYAN << "| " << RESET << balanceColor << setw(22)
            << formatCurrency(Client.AccountBalance) << CYAN << "|\n" << RESET;
    }
    showBorderLine(80, '-', CYAN);
    cout << CYAN << "| " << left << setw(55) << "TOTAL BALANCE"
        << "| " << YELLOW << setw(22) << formatCurrency(totalBalance) << CYAN << "|\n";
    showBorderLine(80, '-', CYAN);

    backToMenu();
}
// Show transaction history for account
void showTransactionsHistory() {
    vector<Transaction> transactions = loadTransactionsFromFile(TransactionsFileName);

    clearScreen();
    showScreenHeader("Transaction History");
    showBackOrExit(false);
    showLine();

    string accountNumber = readNonEmptyString("Enter Account Number (or 0 to Back)? ");
    if (accountNumber == "0") return;

    cout << "Account Number: " << accountNumber << "\n\n";

    showBorderLine(150, '-', CYAN);
    cout << CYAN << "| " << left << setw(28) << "Transaction ID"
        << "| " << setw(12) << "Type"
        << "| " << setw(15) << "From Account"
        << "| " << setw(15) << "To Account"
        << "| " << setw(12) << "Amount"
        << "| " << setw(8) << "Fees"
        << "| " << setw(20) << "Timestamp"
        << "| " << setw(25) << "Description" << "|\n";
    showBorderLine(150, '-', CYAN);

    bool found = false;
    for (const Transaction& txn : transactions) {
        if (txn.FromAccount == accountNumber || txn.ToAccount == accountNumber) {
            found = true;
            string typeColor = (txn.Type == DEPOSIT) ? GREEN : (txn.Type == WITHDRAWAL) ? RED : YELLOW;
            string type = (txn.Type == DEPOSIT ? "Deposit" : txn.Type == WITHDRAWAL ? "Withdraw" : "Transfer");

            cout << CYAN << "| " << RESET << left << setw(28) << txn.TransactionID
                << CYAN << "| " << RESET << typeColor << setw(12) << type << RESET
                << CYAN << "| " << RESET << setw(15) << txn.FromAccount
                << CYAN << "| " << RESET << setw(15) << txn.ToAccount
                << CYAN << "| " << RESET << GREEN << setw(12) << fixed << setprecision(2) << formatCurrency(txn.Amount) << RESET
                << CYAN << "| " << RESET << YELLOW << setw(8) << txn.Fees << RESET
                << CYAN << "| " << RESET << setw(20) << txn.Timestamp
                << CYAN << "| " << RESET << setw(25) << txn.Description << CYAN << "|\n" << RESET;
        }
    }

    showBorderLine(150, '-', CYAN);

    if (!found) {
        cout << "\n";
        showErrorMessage("No transactions found for this account.");
    }

    backToMenu();
}
// Execute selected transaction option
void executeTransactionOption(TransactionsOption TransactionMenuOption, vector<strClient>& vClients) {
    switch (TransactionMenuOption) {
    case TransactionsOption::Deposit:
        clearScreen();
        showDepositScreen(vClients);
        break;

    case TransactionsOption::Withdraw:
        clearScreen();
        showWithdrawScreen(vClients);
        break;

    case TransactionsOption::Transfer:
        clearScreen();
        showTransferScreen(vClients);
        break;

    case TransactionsOption::ShowTotalBalance:
        clearScreen();
        showTotalBalancesReport(vClients);
        break;

    case TransactionsOption::ShowTransactionsHistory:
        showTransactionsHistory();
        break;

    case TransactionsOption::ShowMainMenu:
        break;
    }
}
// Show transactions menu options
void showTransactionsMenuScreen() {
    clearScreen();
    showScreenHeader("Transactions Menu Screen");
    vector<string> options = { "Deposit","Withdraw","Transfer","Total Balances","Transactions History" };
    showOptions(options);
    showBackOrExit();
    showLine(60, '-', CYAN);
}
// Manage transactions menu loop
void manageTransactions(vector<strClient>& vClients) {
    int choice;
    do {
        showTransactionsMenuScreen();
        choice = readMenuOption(1, 5);
        if (choice == 0) break;
        executeTransactionOption((TransactionsOption)choice, vClients);
        vClients = loadClientsDataFromFile(ClientsFileName);
    } while (choice != 0);
}
