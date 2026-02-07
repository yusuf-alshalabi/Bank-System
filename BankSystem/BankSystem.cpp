
//  ||========================================================||
//  || BankSystem Project - Version v1.4.0 (Procedural Final) ||
//  ||========================================================||
//  || Table of Contents:									  ||
//  ||  1. Structs, Enums & Global Variables				  ||
//  ||  2. Forward Declarations								  ||
//  ||  3. Utilities										  ||
//  ||  4. Session Management System						  ||
//  ||  5. Encryption & Decryption							  ||
//  ||  6. File Manager										  ||
//  ||  7. Logging System									  ||  
//  ||  8. Input Manager									  ||
//  ||  9. Client Manager									  ||
//  || 10. Transactions Manager								  ||
//  || 11. User Manager										  ||
//  || 12. Auth Manager										  ||
//  || 13. Permission Manager								  ||
//  || 14. Menu Manager										  ||
//  || 15. Main Function									  ||
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
//======== struct & enums & global variables ==========
//=====================================================
#pragma region struct & enums & global variables

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
	pAll = 127   // Full Access
};
enum LogLevel {
	INFO,
	WARNING,
	ERROR_LOG,  
	CRITICAL
};

struct Transaction {
	string TransactionID;
	TransactionType Type = TransactionType::DEPOSIT;  // default
	string FromAccount;
	string ToAccount;
	double Amount = 0.0;   // default
	double Fees = 0.0;	   // default
	string Timestamp;
	string Description;
};
struct strClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance = 0.0;   // default
	bool MarkForDelete = false;
	vector<Transaction> Transactions;
};
struct strUser
{
	string UserName = "";
	string Password = "";
	int Permissions = -1;
	bool MarkForDelete = false;
};

strUser CurrentUser;
#pragma endregion
//=====================================================


//=====================================================
//=============== Forward Declarations ================
//=====================================================
#pragma region Forward Declarations

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
void logMessage(const string& message, LogLevel level);
void logTransaction(const Transaction& txn);
void logLoginAttempt(const string& username, bool success);
void logUserAction(const string& action, const string& details);

// User & Auth
string hashPassword(const string& password);
int readUserPermissions();
bool verifyUserPassword(const string& password, strUser* user);

// Menus
void showMainMenu(vector<strClient>& vClients);
void showManageUsersMenu(vector<strUser>& vUsers);
void showExitScreen();
void showManageUsersScreen();
#pragma endregion
//=====================================================


//=====================================================
//==================== Utilities ======================
// Helper functions for formatting, trimming, screen control,
// and displaying messages.
//=====================================================
#pragma region Utilities

// Format double with fixed precision
string formatDouble(double value, int precision = 2) {
	ostringstream out;
	out << fixed << setprecision(precision) << value;
	return out.str();
}
// Convert integer safely to string
string formatInt(int value) {
	return to_string(value);
}
// Format currency with $ sign
string formatCurrency(double value) {
	ostringstream out;
	out << "$" << fixed << setprecision(2) << value;
	return out.str();
}
// Remove leading and trailing whitespace from string
string trim(const string& str) {
	size_t start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == string::npos) return "";

	size_t end = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(start, end - start + 1);
}
// Get current date and time as formatted string
string getCurrentTimestamp() {
	time_t now = time(0);

#ifdef _WIN32
	tm localTime;
	localtime_s(&localTime, &now);
#else
	tm* localTime = localtime(&now);
#endif

	stringstream ss;
#ifdef _WIN32
	ss << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
#else
	ss << put_time(localTime, "%Y-%m-%d %H:%M:%S");
#endif
	return ss.str();
}

//Clear console screen (Windows/Linux)
void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}
// Draw a line with given length, symbol, and color (no newlines)
void drawLine(int length = 60, char symbol = '-', string color = RESET) {
	cout << color << string(length, symbol) << RESET;
}
// Show a separator line with newlines before and after
void showLine(int length = 60, char symbol = '-', string color = RESET) {
	cout << "\n";
	drawLine(length, symbol, color);
	cout << "\n";
}
// Show a bordered line (+ at start and end)
void showBorderLine(int length = 60, char symbol = '-', string color = RESET) {
	cout << color << "+";
	drawLine(length, symbol, color);
	cout << color << "+" << RESET << "\n";
}
// Display formatted success message
void showSuccessMessage(string message) {
	showLine(60,'=',RESET);
	cout << GREEN << "   SUCCESS: " << message << RESET;
	showLine(60, '=', RESET);
    cout << "\n";
}
// Display formatted error message
void showErrorMessage(string message) {
	showLine();
	cout << RED << "   ERROR: " << message << RESET;
	showLine();
	cout << "\n";
}
// Display centered header with borders
void showScreenHeader(const string& title) {
	cout << "\n";
	showBorderLine(58, '=', CYAN);
	cout << CYAN << "|" << string(58, ' ') << "|\n";

	int padding = (58 - title.length()) / 2;
	cout << "|" << string(padding, ' ') << title
		<< string(58 - padding - title.length(), ' ') << "|\n";

	cout << "|" << string(58, ' ') << "|\n";
	showBorderLine(58, '=', CYAN);
}
// Display numbered list of options
void showOptions(const vector<string>& options) {
	cout << "\n";
	for (size_t i = 0; i < options.size(); i++) {
		cout << CYAN << "  [" << (i + 1) << "]  " << RESET
			<< YELLOW << options[i] << RESET << ".\n";
	}
}
// Display Back or Exit option
void showBackOrExit(bool isMain = false) {
	string back = (isMain ? "Exit" : "Back");
	cout << "\n" << CYAN << "  [0]  " << RESET << YELLOW << back << RESET << ".\n";
}
// Wait until user presses Enter once
void waitForEnter() {
	string dummy;
	getline(cin, dummy);
}
// Show message and wait for Enter to continue
void pressEnterToContinue() {
	cout << "\n\n" << CYAN << "Press Enter to continue..." << RESET;
	waitForEnter();
}
// Show message and wait for Enter to return to main menu
void backToMenu() {
	cout << "\n\n" << YELLOW << "Press Enter to return to the main menu..." << RESET;
	waitForEnter();
}
#pragma endregion
//=====================================================


//=====================================================
//============= Session Management System =============
// Functions to manage user sessions: save, load, clear,
// and handle session paths/folders.
//=====================================================
#pragma region Session Management System

// Get current system username safely
string getCurrentUsernameSafe() {
	string username = "default_user";

#ifdef _WIN32
	FILE* pipe = _popen("whoami", "r");
	if (pipe) {
		char buffer[128];
		if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
			username = buffer;
			// Extract username from result (usually: DOMAIN\username)
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
	// Safe method: using system command to query special folders
	FILE* pipe = _popen("echo %LOCALAPPDATA%", "r");
	if (pipe) {
		char buffer[512];
		if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
			localAppDataPath = buffer;
			localAppDataPath.erase(localAppDataPath.find_last_not_of(" \n\r\t") + 1);
		}
		_pclose(pipe);
	}

	// If first method failed, use default path
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

	if (remove(sessionPath.c_str()) == 0) {
	}
	else {
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
#pragma endregion
//=====================================================


//=====================================================
//=============== Encryption & Decryption =============
// Functions for generating keys, encrypting and decrypting
// sensitive data using libsodium.
//=====================================================
#pragma region Encryption & Decryption

// Generate random encryption key
vector<unsigned char> generateEncryptionKey() {
	vector<unsigned char> key(crypto_secretbox_KEYBYTES);
	randombytes_buf(key.data(), key.size());
	return key;
}
// Load or create encryption key file securely
vector<unsigned char> getEncryptionKey() {
	vector<unsigned char> key(crypto_secretbox_KEYBYTES);
	string keyFolder = getSessionFolder();

#ifdef _WIN32
	string keyFile = keyFolder + "encryption_key.bsess";
#else
	string keyFile = keyFolder + ".encryption_key.bsess";
#endif

	ifstream file(keyFile, ios::binary);
	if (file.is_open()) {
		file.read(reinterpret_cast<char*>(key.data()), key.size());
		file.close();

		return key;
	}

	key = generateEncryptionKey();

	createSessionFolder();
	ofstream outFile(keyFile, ios::binary);
	if (outFile.is_open()) {
		outFile.write(reinterpret_cast<const char*>(key.data()), key.size());
		outFile.close();

#ifdef _WIN32
		DWORD attributes = GetFileAttributesA(keyFile.c_str());
		if (attributes != INVALID_FILE_ATTRIBUTES) {
			SetFileAttributesA(keyFile.c_str(), attributes | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
		}
#endif

#ifdef _WIN32
		string permissionCommand = "icacls \"" + keyFile + "\" /inheritance:r /grant:r \"%USERNAME%:F\"";
		system(permissionCommand.c_str());
#else
		string permissionCommand = "chmod 600 \"" + keyFile + "\"";
		system(permissionCommand.c_str());
#endif

	}

	return key;
}
// Encrypt plaintext using libsodium secretbox
string encryptData(const string& plaintext, const vector<unsigned char>& key) {
	if (key.size() != crypto_secretbox_KEYBYTES) {
		throw runtime_error("Invalid key size for encryption");
	}

	if (plaintext.empty()) {
		throw runtime_error("Cannot encrypt empty data");
	}

	try {
		vector<unsigned char> nonce(crypto_secretbox_NONCEBYTES);
		randombytes_buf(nonce.data(), nonce.size());

		vector<unsigned char> ciphertext(plaintext.size() + crypto_secretbox_MACBYTES);

		if (crypto_secretbox_easy(ciphertext.data(),
			reinterpret_cast<const unsigned char*>(plaintext.c_str()),
			plaintext.size(),
			nonce.data(),
			key.data()) != 0) {
			throw runtime_error("Encryption operation failed");
		}

		// Combine nonce + ciphertext
		string result;
		result.append(reinterpret_cast<const char*>(nonce.data()), nonce.size());
		result.append(reinterpret_cast<const char*>(ciphertext.data()), ciphertext.size());

		return result;
	}
	catch (const exception& e) {
		throw runtime_error(string("Encryption error: ") + e.what());
	}
}
// Decrypt ciphertext using libsudium secretbox
string decryptData(const string& encryptedData, const vector<unsigned char>& key) {
	if (key.size() != crypto_secretbox_KEYBYTES) {
		throw runtime_error("Invalid key size");
	}

	if (encryptedData.size() < crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES) {
		throw runtime_error("Encrypted data too short");
	}

	// Extract nonce and ciphertext
	vector<unsigned char> nonce(crypto_secretbox_NONCEBYTES);
	copy(encryptedData.begin(), encryptedData.begin() + crypto_secretbox_NONCEBYTES, nonce.begin());

	vector<unsigned char> ciphertext(encryptedData.size() - crypto_secretbox_NONCEBYTES);
	copy(encryptedData.begin() + crypto_secretbox_NONCEBYTES, encryptedData.end(), ciphertext.begin());

	vector<unsigned char> plaintext(ciphertext.size() - crypto_secretbox_MACBYTES);

	if (crypto_secretbox_open_easy(plaintext.data(),
		ciphertext.data(),
		ciphertext.size(),
		nonce.data(),
		key.data()) != 0) {
		throw runtime_error("Decryption failed - tampered or corrupted data");
	}

	return string(reinterpret_cast<const char*>(plaintext.data()), plaintext.size());
}
#pragma endregion
//=====================================================


//=====================================================
//==================== File Manager ===================
// Functions to serialize/deserialize Clients, Users, and
// Transactions, and handle file I/O operations.
//=====================================================
#pragma region File Manager

// Split string into tokens using delimiter
vector<string> splitStringByDelimiter(string S1, string delim = Separator) {
	vector <string> split;
	size_t pos = 0;
	string sWord;

	while ((pos = S1.find(delim)) != std::string::npos) {
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
string serializeClientRecord(const strClient& clientData, const string& seperator = "#//#") {
	string Line = "";
	Line += clientData.AccountNumber + seperator;
	Line += clientData.PinCode + seperator;
	Line += clientData.Name + seperator;
	Line += clientData.Phone + seperator;
	Line += formatDouble(clientData.AccountBalance);

	return Line;

}
// Convert file line to Client struct
strClient deserializeClientRecord(const string& Line, const string& seperator = "#//#") {
	strClient Client;
	vector<string> vClientData = splitStringByDelimiter(Line);

	if (vClientData.size() < 5) {
		throw runtime_error("Invalid client data format");
	}

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);
	Client.MarkForDelete = false;

	return Client;
}
// Convert User struct to file line
string serializeUserRecord(const strUser& userInfo, const string& separator = "#//#") {
	string line = "";
	line += userInfo.UserName + separator;
	line += userInfo.Password + separator;
	line += formatInt(userInfo.Permissions);
	return line;
}
// Convert file line to User struct
strUser deserializeUserRecord(string Line, const string& seperator = "#//#") {
	strUser userInfo;
	vector<string> vUsersData = splitStringByDelimiter(Line);

	if (vUsersData.size() < 3) {
		throw runtime_error("Invalid user data format");
	}

	userInfo.UserName = vUsersData[0];
	userInfo.Password = vUsersData[1];
	userInfo.Permissions = stoi(vUsersData[2]);

	return userInfo;
}
// Convert file line to Transaction struct
Transaction deserializeTransactionRecord(const string& line, const string& separator = "#//#") {
	Transaction txn;
	vector<string> parts = splitStringByDelimiter(line);

	if (parts.size() >= 8) {
		txn.TransactionID = parts[0];
		txn.Type = static_cast<TransactionType>(stoi(parts[1]));
		txn.FromAccount = parts[2];
		txn.ToAccount = parts[3];
		txn.Amount = stod(parts[4]);
		txn.Fees = stod(parts[5]);
		txn.Timestamp = parts[6];
		txn.Description = parts[7];
	}
	return txn;
}
// Load all clients from file, return vector of clients
vector<strClient> loadClientsDataFromFile(const string& fileName) {
	vector<strClient> vClients;
	fstream myFile;
	myFile.open(fileName, ios::in);

	if (!myFile.is_open()) {
		return vClients; // File doesn't exist yet
	}

	try {
		string Line;
		while (getline(myFile, Line)) {
			if (!Line.empty()) {
				strClient Client = deserializeClientRecord(Line);
				vClients.push_back(Client);
			}
		}
		myFile.close();
	}
	catch (const exception& e) {
		myFile.close();
		throw runtime_error(string("Error loading clients: ") + e.what());
	}

	return vClients;
}
// Save all clients to file, (skip those marked for deletion)
void saveClientsToFile(string FileName, const vector<strClient>& vClients) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (!MyFile.is_open()) {
		throw runtime_error("Cannot open file for writing: " + FileName);
	}

	try {
		for (const strClient& c : vClients) {
			if (!c.MarkForDelete) {
				MyFile << serializeClientRecord(c) << endl;
				if (MyFile.fail()) {
					throw runtime_error("Failed to write client data");
				}
			}
		}
		MyFile.close();
	}
	catch (const exception& e) {
		MyFile.close();
		throw runtime_error(string("Error saving clients: ") + e.what());
	}
}
// Load all Users from file, return vector of Users
vector<strUser> loadUsersDataFromFile(const string& fileName) {
	vector<strUser> vUsers;
	fstream myFile;
	myFile.open(fileName, ios::in);

	if (!myFile.is_open()) {
		return vUsers;
	}

	try {
		string Line;
		while (getline(myFile, Line)) {
			if (!Line.empty()) {
				strUser User = deserializeUserRecord(Line);
				vUsers.push_back(User);
			}
		}
		myFile.close();
	}
	catch (const exception& e) {
		myFile.close();
		throw runtime_error(string("Error loading users: ") + e.what());
	}

	return vUsers;
}
// Save all Users to file, (skip those marked for deletion)
void saveUsersToFile(string FileName, const vector<strUser>& vUsers) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (!MyFile.is_open()) {
		throw runtime_error("Cannot open file for writing: " + FileName);
	}

	try {
		for (const strUser& user : vUsers) {
			if (!user.MarkForDelete) {
				MyFile << serializeUserRecord(user) << endl;
				if (MyFile.fail()) {
					throw runtime_error("Failed to write user data");
				}
			}
		}
		MyFile.close();
	}
	catch (const exception& e) {
		MyFile.close();
		throw runtime_error(string("Error saving users: ") + e.what());
	}
}
// Load all Transactions from file, return vector of Transactions
vector<Transaction> loadTransactionsFromFile(const string& fileName) {
	vector<Transaction> transactions;
	fstream file(fileName, ios::in);

	if (!file.is_open()) {
		// File doesn't exist yet - not an error
		return transactions;
	}

	try {
		string line;
		while (getline(file, line)) {
			if (!line.empty()) {  // Skip empty lines
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
// Save all transaction to file, (skip those marked for deletion)
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
// Append  line to file
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
#pragma endregion
//=====================================================

//=====================================================
//================== Logging System ===================
// Functions to log system activities, transactions,
// and errors for audit trail and debugging.
//=====================================================
#pragma region Logging System

// Convert log level enum to readable string
string logLevelToString(LogLevel level) {
	switch (level) {
	case INFO: return "INFO";
	case WARNING: return "WARNING";
	case ERROR_LOG: return "ERROR";
	case CRITICAL: return "CRITICAL";
	default: return "UNKNOWN";
	}
}
// Log message to file with timestamp, level, and user context
void logMessage(const string& message, LogLevel level = INFO) {
	try {
		string logEntry = "[" + getCurrentTimestamp() + "] " +
			"[" + logLevelToString(level) + "] " +
			"[User: " + (CurrentUser.UserName.empty() ? "SYSTEM" : CurrentUser.UserName) + "] " +
			message;

		appendLineToFile(LogFileName, logEntry);
	}
	catch (const exception& e) {
		// Silent fail - don't disrupt program flow
		// Optionally could write to stderr for debugging
		cerr << "[LOGGING ERROR] " << e.what() << endl;
	}
}
// Log transaction activity with full details
void logTransaction(const Transaction& txn) {
	string typeStr;
	switch (txn.Type) {
	case DEPOSIT: typeStr = "DEPOSIT"; break;
	case WITHDRAWAL: typeStr = "WITHDRAWAL"; break;
	case TRANSFER: typeStr = "TRANSFER"; break;
	default: typeStr = "UNKNOWN"; break;
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
void logUserAction(const string& action, const string& details = "") {
	string message = "Action: " + action;
	if (!details.empty()) {
		message += " - Details: " + details;
	}
	logMessage(message, INFO);
}

#pragma endregion
//=====================================================

//=====================================================
//==================== Input Manager ==================
// Functions to read and validate user input (strings,
// numbers, passwords, confirmations).
//=====================================================
#pragma region Input Manager

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

	// Check if alphanumeric only
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

	// Allow digits, spaces, +, -, ()
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
// Enhanced readClientData with validation
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
// Read password input (min 8 chars, stronger policy)
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
				password.clear(); // force retry
			}
		}
	} while (password.empty() || password.length() < 8);
	return password;  // Don't hash here - will be hashed when stored
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
#pragma endregion
//=====================================================


//=====================================================
//==================== Client Manager =================
// Functions to manage clients: add, update, delete,
// find, and display client information.
//=====================================================
#pragma region Client Manager

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
	showBorderLine(58,'=');
	cout << "|  " << left << setw(56) << "Client Information" << "|\n";
	cout << "+" << string(58, '=') << "+\n";

	cout << "|  Account Number : " << left << setw(39)
		<< client.AccountNumber << "|\n";
	cout << "|  PIN Code       : " << left << setw(39)
		<< client.PinCode << "|\n";
	cout << "|  Name           : " << left << setw(39)
		<< client.Name << "|\n";
	cout << "|  Phone          : " << left << setw(39)
		<< client.Phone << "|\n";
	cout << "|  Balance        : " << left << setw(39)
		<< formatCurrency(client.AccountBalance) << "|\n";

	showBorderLine(58, '=');
}
//- Display all clients in table format
void showAllClientsReport(const vector<strClient>& vClients) {
	clearScreen();
	showScreenHeader("Clients List");

	cout << "Total Clients: " << vClients.size() << "\n\n";

	if (vClients.size() == 0) {
		showErrorMessage("No clients available in the system!");
		backToMenu();
		return;
	}

	// Table header
	showBorderLine(105, '-', CYAN);
	cout << CYAN << "| " << left << setw(18) << "Account Number"
		<< "| " << setw(12) << "PIN Code"
		<< "| " << setw(30) << "Client Name"
		<< "| " << setw(15) << "Phone"
		<< "| " << setw(21) << "Balance" << "|\n";
	showBorderLine(105, '-', CYAN);

	// Print each client
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
void showAddClientScreen(vector <strClient>& vClients) {
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
#pragma endregion
//=====================================================


//=====================================================
//=============== Transactions Manager ================
// Functions to handle deposits, withdrawals, transfers,
// balances reports, and transaction history.
//=====================================================
#pragma region Transactions Manager

// Generate unique transaction ID
string generateTransactionID() {
	// Use high-resolution timestamp for uniqueness
	auto now = chrono::high_resolution_clock::now();
	auto timestamp = chrono::duration_cast<chrono::microseconds>(
		now.time_since_epoch()).count();

	// Add cryptographically secure random component
	uint32_t randomNum = randombytes_random();

	// Combine timestamp and random number
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
// Display transfer details befor confirm
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
void showTotalBalancesReport(const vector <strClient>& vClients) {
	clearScreen();
	showScreenHeader("Total Balances Report");

	double totalBalance = 0;

	cout << "Total Clients: " << vClients.size() << "\n\n";

	if (vClients.size() == 0) {
		showErrorMessage("No clients available in the system!");
		backToMenu();
		return;
	}

	// Table header
	showBorderLine(80,'-',CYAN);
	cout << CYAN << "| " << left << setw(18) << "Account Number"
		 << "| " << setw(35) << "Client Name"
		 << "| " << setw(22) << "Balance" << "|\n";
	showBorderLine(80, '-', CYAN);

	// Print each client
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

	// Table header
	showBorderLine(150,'-',CYAN);
	cout << CYAN << "| " << left << setw(28) << "Transaction ID"
		 << "| " << setw(12) << "Type"
		 << "| " << setw(15) << "From Account"
		 << "| " << setw(15) << "To Account"
		 << "| " << setw(12) << "Amount"
		 << "| " << setw(8)  << "Fees"
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
				 << CYAN << "| " << RESET << setw(25) << txn.Description <<CYAN << "|\n" << RESET;
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
void executeTransactionOption(TransactionsOption TransactionMenuOption, vector<strClient>& vClients)
{
	switch (TransactionMenuOption)
	{
	case TransactionsOption::Deposit:
	{
		clearScreen();
		showDepositScreen(vClients);
		break;
	}

	case TransactionsOption::Withdraw:
	{
		clearScreen();
		showWithdrawScreen(vClients);
		break;
	}

	case TransactionsOption::Transfer: {
		clearScreen();
		showTransferScreen(vClients);
		break;
	}

	case TransactionsOption::ShowTotalBalance:
	{
		clearScreen();
		showTotalBalancesReport(vClients);
		break;
	}

	case TransactionsOption::ShowTransactionsHistory: {
		showTransactionsHistory();
		break;
	}

	case TransactionsOption::ShowMainMenu:
	{

		break;
	}
	}

}
// Show transactions menu options
void showTransactionsMenuScreen() {
	clearScreen();
	showScreenHeader("Transactions Menu Screen");
	vector<string> options = { "Deposit","Withdraw","Transfer","Total Balances","Transactions History"};
	showOptions(options);
	showBackOrExit();
	showLine();
}
// Manage transactions menu loop
void manageTransactions(vector<strClient>& vClients) {
	int choice;
	do {
		showTransactionsMenuScreen();
		choice = readMenuOption(1, 5); // 0 for Back
		if (choice == 0) break; // Back to main menu
		executeTransactionOption((TransactionsOption)choice, vClients);
		vClients = loadClientsDataFromFile(ClientsFileName);
	} while (choice != 0);
}
#pragma endregion
//=====================================================


//=====================================================
//==================== User Manager ===================
// Functions to manage users: add, update, delete,
// find, list, and display user information.
//=====================================================
#pragma region User Manager

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
	string rawPassword = readPassword();   // get raw password
	user.Password = hashPassword(rawPassword); // hash before storing
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
// Format permissions integer to readable string
string formatPermissions(int permissions) {
	if (permissions == Permission::pAll)
		return "Full Access";

	vector<pair<int, string>> mapping = {
		{Permission::pListClients, "List Clients"},
		{Permission::pAddClient, "Add Clients"},
		{Permission::pDeleteClient, "Delete Clients"},
		{Permission::pUpdateClient, "Update Clients"},
		{Permission::pFindClient, "Find Clients"},
		{Permission::pTransactions, "Transactions"},
		{Permission::pManageUsers, "Manage Users"}
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
	showBorderLine(102,'=');
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

	// Table header
	showBorderLine(120,'-',CYAN);
	cout << CYAN << "| " << left << setw(25) << "Username"
		 << "| " << setw(92) << "Permissions" << "|\n";
	showBorderLine(120, '-', CYAN);

	// Print each user
	for (const strUser& user : vUsers) {
		cout << CYAN << "| " <<RESET << left << setw(25) << user.UserName
			 << CYAN << "| " <<RESET << setw(92) << formatPermissions(user.Permissions) << CYAN << "|\n" << RESET;
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

	// Prevent deleting the last remaining full access user
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

					// Prevent removing full access from the last Admin
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
// Execute selected user managment option
void executeUserOption(UserManagementOption manageUsersOptions, vector <strUser>& vUsers) {
	switch (manageUsersOptions) {
	case UserManagementOption::ListUser:
		showUsersListScreen(vUsers);
		break;
	case UserManagementOption::AddNewUser: {
		showAddUserScreen(vUsers);
		break;
	}
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
#pragma endregion
//=====================================================


//=====================================================
//==================== Auth Manager ===================
// Functions for authentication: hashing, verifying
// passwords, login, and default admin creation.
//=====================================================
#pragma region Auth Manager

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
// Verify user password against stored hash
bool verifyUserPassword(const string& password, strUser* user) {
	if (user == nullptr) {
		return false;
	}
	return verifyPassword(password, user->Password);
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
		string password = readNonEmptyString("Please Enter Password? ");  // Raw password, not hashed

		strUser* user = findUserByUsername(name, vUsers);
		found = verifyUserPassword(password, user);  // Verification happens here

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
		adminUser.Permissions = Permission::pAll; // Full access

		vUsers.push_back(adminUser);
		saveUsersToFile(UsersFileName, vUsers);

		showSuccessMessage("Admin user created successfully!");
		cout << "Username: " << adminUser.UserName << "\n";
		cout << "Please remember your password securely.\n";
		pressEnterToContinue();

		startSession(adminUser);
	}
}
#pragma endregion
//=====================================================


//=====================================================
//================ Permission Manager =================
// Functions to read and check user permissions.
//=====================================================
#pragma region Permission Manager

// Read user permissions interactively
int readUserPermissions()
{
	int Permissions = 0;

	if (confirmAction("Do you want to give full access? "))
		return Permission::pAll;
	cout << "\nDo you want to give access to : \n ";
	if (confirmAction("Show Client List? "))
		Permissions += Permission::pListClients;
	if (confirmAction("Add New Client? "))
		Permissions += Permission::pAddClient;
	if (confirmAction("Delete Client? "))
		Permissions += Permission::pDeleteClient;
	if (confirmAction("Update Client? "))
		Permissions += Permission::pUpdateClient;
	if (confirmAction("Find Client? "))
		Permissions += Permission::pFindClient;
	if (confirmAction("Transactions? "))
		Permissions += Permission::pTransactions;
	if (confirmAction("Manage Users? "))
		Permissions += Permission::pManageUsers;

	return Permissions;

}
// Check if current user has specific permission
bool hasPermission(Permission permission) {
	if (CurrentUser.Permissions == Permission::pAll)
		return true;

	return (CurrentUser.Permissions & permission) == permission;
}
#pragma endregion
//=====================================================


//=====================================================
//==================== Menu Manager ===================
// Functions to build, display, and execute main menu
// and user management menu options.
//=====================================================
#pragma region Menu Manager

// Build main menu options based on permissions
vector<string> buildMainMenuOptions() {
	vector<string> options;
	if (hasPermission(Permission::pAll))
		options = { "Show Client List","Add New Client","Delete Client","Update Client","Find Client","Transactions","Manage Users" };
	else {
		if (hasPermission(Permission::pListClients))
			options.push_back("Show Client List");
		if (hasPermission(Permission::pAddClient))
			options.push_back("Add New Client");
		if (hasPermission(Permission::pDeleteClient))
			options.push_back("Delete Client");
		if (hasPermission(Permission::pUpdateClient))
			options.push_back("Update Client");
		if (hasPermission(Permission::pFindClient))
			options.push_back("Find Client");
		if (hasPermission(Permission::pTransactions))
			options.push_back("Transactions");
		if (hasPermission(Permission::pManageUsers))
			options.push_back("Manage Users");
	}
	options.push_back("Logout");

	return options;
}
// Convert numeric choice to menu option enum
MainMenuOption convertChoiceToMainMenuOption(int choice, const vector<string>& options) {
	string selectedOption = options[choice - 1];

	if (selectedOption == "Show Client List") return MainMenuOption::ShowClientList;
	if (selectedOption == "Add New Client") return MainMenuOption::AddNewClient;
	if (selectedOption == "Delete Client") return MainMenuOption::DeleteClient;
	if (selectedOption == "Update Client") return MainMenuOption::UpdateClient;
	if (selectedOption == "Find Client") return MainMenuOption::FindClient;
	if (selectedOption == "Transactions") return MainMenuOption::Transactions;
	if (selectedOption == "Manage Users") return MainMenuOption::ManageUsers;
	if (selectedOption == "Logout") return MainMenuOption::Logout;
	if (selectedOption == "Exit") return MainMenuOption::Exit;

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
		if (loadCurrentUserSession(sessionUser)) {
			showSuccessMessage("Welcome back, " + sessionUser.UserName + "!");
		}
		showScreenHeader("Main Menu Screen");
		vector<string> options = buildMainMenuOptions();
		showOptions(options);
		showBackOrExit(true); // Exit option
		showLine();

		choiceNum = readMenuOption(1, options.size()); // 0 for Exit
		if (choiceNum == 0) {
			showExitScreen();
			break;
		}// Exit program
		Choice = convertChoiceToMainMenuOption(choiceNum, options);

		executeMainMenuOption(Choice, vClients);

	} while (choiceNum != 0 && Choice != MainMenuOption::Exit);
}
// Show manage users menu loop
void showManageUsersMenu(vector<strUser>& vUsers) {
	int choice;
	do {
		showManageUsersScreen();
		choice = readMenuOption(1, 5); // 0 for Back
		if (choice == 0) break; // Back to main menu
		executeUserOption((UserManagementOption)choice, vUsers);
	} while (choice != 0);
}
// Show manage users menu options
void showManageUsersScreen() {
	clearScreen();
	showScreenHeader("Manage Users Menu Screen");
	vector<string> options = { "List Users","Add New User","Delete User","Update User","Find User"};
	showOptions(options);
	showBackOrExit();
	showLine();
}
// Show exit screen
void showExitScreen() {
	clearScreen();
	showScreenHeader("Program Ends :-)");
	waitForEnter();
}
#pragma endregion
//=====================================================


//=====================================================
//==================== Main Function ==================
//=====================================================

// Program entry point: initialize system, create admin, login, run menus
int main()
{
	cout << fixed << setprecision(2);

	if (sodium_init() < 0) {
		showErrorMessage("System initialization failed!");
		return 1;
	}

	try {
		createDefaultAdmin();
		login();
	}
	catch (const exception& e) {
		showErrorMessage("Critical system error: " + string(e.what()));
		return 1;
	}

	return 0;
}
//=====================================================
