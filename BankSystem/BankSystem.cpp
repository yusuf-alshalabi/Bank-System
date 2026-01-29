
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>
#include <functional>
#include <sodium.h>

#ifdef _WIN32
#define NOMINMAX 
#include <windows.h>
#endif

using namespace std;

const string ClientsFileName = "Clients.txt";
const string TransactionsFileName = "Transactions.txt";
const string UsersFileName = "Users.txt";
const string Separator = "#//#";
enum TransactionType {
	DEPOSIT = 1,
	WITHDRAWAL = 2,
	TRANSFER = 3
};

struct Transaction {
	string TransactionID;
	TransactionType Type;
	string FromAccount;
	string ToAccount;
	double Amount;
	double Fees;
	string Timestamp;
	string Description;
};

struct strClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete;
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

//Pause until user presses a key
void customPause() {
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cin.get();
}
//Clear console screen
void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void showSuccessMessage(string message) {
	cout << "\n" << string(50, '=') << "\n";
	cout << "   SUCCESS: " << message << "\n";
	cout << string(50, '=') << "\n\n";
}

void showErrorMessage(string message) {
	cout << "\n" << string(50, '-') << "\n";
	cout << "   ERROR: " << message << "\n";
	cout << string(50, '-') << "\n\n";
}

string trim(const string& str) {
	size_t start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == string::npos) return "";

	size_t end = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(start, end - start + 1);
}
//=====================================================
//============= Session Management System =============
//=====================================================
// Smart function to get current username (without getenv)
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
// Smart function to get LOCALAPPDATA path safely
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
string getSessionPath() {
	string localAppData = getLocalAppDataPath();
	string username = getCurrentUsernameSafe();
	return localAppData + "\\BankSystem\\session_" + username + ".bsess";
}
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
//=====================================================
//===============Encryption & Decryption===============
//=====================================================
// Generate encryption key (should be stored securely)
vector<unsigned char> generateEncryptionKey() {
	vector<unsigned char> key(crypto_secretbox_KEYBYTES);
	randombytes_buf(key.data(), key.size());
	return key;
}

// Get or create encryption key
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


// Encrypt data with better error handling
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
// Decrypt data
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

// Serialize user data to string
string serializeUserData(const strUser& user) {
	string data;
	data += user.UserName + "\n";
	data += user.Password + "\n";
	data += to_string(user.Permissions);
	return data;
}

// Deserialize user data from string
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


//========================================================================
// Save current user session to encrypted binary file
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
		// showErrorMessage("Failed to encrypt session data: " + string(e.what()));
	}
}

// Enhanced session loading with integrity checks
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

// Enhanced secure session clearing
void clearCurrentUserSession() {
	string sessionPath = getSessionPath();

	fstream file(sessionPath, ios::binary | ios::out | ios::in);
	if (file.is_open()) {
		file.seekp(0);
		for (int pass = 0; pass < 3; pass++) {
			vector<char> randomData(1024);
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

//========================================================================

vector<string> buildMainMenuOptions();
void showManageUsersScreen();
void ManageTransactions(vector<strClient>& vClients);
void ManageMainMenu(vector<strClient>& vClients);
void login();

// Split string into tokens using delimiter
vector<string> splitStringByDelimiter(string S1, string delim) {
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
// Convert a client struct to a single line for file
string convertClientRecordToLine(const strClient& clientData, const string& seperator) {
	string Line = "";
	Line += clientData.AccountNumber + seperator;
	Line += clientData.PinCode + seperator;
	Line += clientData.Name + seperator;
	Line += clientData.Phone + seperator;
	Line += to_string(clientData.AccountBalance);

	return Line;

}
// Convert a line from file into a Client struct
strClient convertLineToClientRecord(const string& Line, const string& seperator) {
	strClient Client;
	vector<string> vClientData = splitStringByDelimiter(Line, seperator);

	if (vClientData.size() >= 5) {
		Client.AccountNumber = vClientData[0];
		Client.PinCode = vClientData[1];
		Client.Name = vClientData[2];
		Client.Phone = vClientData[3];
		Client.AccountBalance = stod(vClientData[4]);
	}
	else {
		throw runtime_error("Invalid client data format");
	}
	return Client;
}
// Convert aline from file into a Transaction struct
Transaction convertLineToTransaction(const string& line, const string& separator) {
	Transaction txn;
	vector<string> parts = splitStringByDelimiter(line, separator);

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
// Convert a User struct to a single line for file
string convertUserRecordToLine(const strUser& userInfo, const string& separator = "#//#") {
	string line = "";
	line += userInfo.UserName + separator;
	line += userInfo.Password + separator;
	line += to_string(userInfo.Permissions);
	return line;
}

// Convert a line from file into a User struct
strUser convertLineToUserRecord(string Line, const string& seperator) {
	strUser userInfo;
	vector<string> vUsersData;
	vUsersData = splitStringByDelimiter(Line, seperator);
	userInfo.UserName = vUsersData[0];
	userInfo.Password = vUsersData[1];
	userInfo.Permissions = stoi(vUsersData[2]);

	return userInfo;
}

// Load all clients from file, return vector of clients
vector<strClient> loadClientsDataFromFile(const string& fileName) {
	vector <strClient> vClients;
	fstream myFile;
	myFile.open(fileName, ios::in); //read only.
	if (myFile.is_open()) {
		string Line;
		strClient Client;
		while (getline(myFile, Line)) {
			Client = convertLineToClientRecord(Line, Separator);
			vClients.push_back(Client);
		}
		myFile.close();
	}
	return vClients;
}
// Load all Transactions from file, return vector of Transactions
vector<Transaction> loadTransactionsFromFile(const string& fileName) {
	vector<Transaction> transactions;
	fstream file(fileName, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			transactions.push_back(convertLineToTransaction(line, Separator));
		}
		file.close();
	}
	return transactions;
}
// Load all Users from file, return vector of Users
vector<strUser> loadUsersDataFromFile(const string& fileName) {
	vector <strUser> vUsers;
	fstream myFile;
	myFile.open(fileName, ios::in); //read only.
	if (myFile.is_open()) {
		string Line;
		strUser User;
		while (getline(myFile, Line)) {
			User = convertLineToUserRecord(Line, Separator);
			vUsers.push_back(User);
		}
		myFile.close();
	}
	return vUsers;
}
// Save all clients to file, skip those marked for deletion
void saveClientsToFile(string FileName, const vector<strClient>& vClients) {
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::trunc);
	string DataLine;
	if (MyFile.is_open()) {
		for (const strClient& c : vClients) {
			if (!c.MarkForDelete) {
				DataLine = convertClientRecordToLine(c, Separator);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
}
void saveUsersToFile(string FileName, const vector<strUser>& vUsers) {
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::trunc); 

	if (MyFile.is_open()) {
		for (const strUser& user : vUsers) { 
			if (!user.MarkForDelete) {
				string DataLine = convertUserRecordToLine(user, Separator);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	else {
		showErrorMessage("Could not open file for saving users!");
	}
}

// Append a client line to file
void appendLineToFile(const string& FileName, const string& stDataLine) {
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open()) {
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

void saveTransactionToFile(const Transaction& transaction) {
	string transactionLine = transaction.TransactionID + Separator +
		to_string(transaction.Type) + Separator +
		transaction.FromAccount + Separator +
		transaction.ToAccount + Separator +
		to_string(transaction.Amount) + Separator +
		to_string(transaction.Fees) + Separator +
		transaction.Timestamp + Separator +
		transaction.Description;

	appendLineToFile(TransactionsFileName, transactionLine);
}

// Read a non-empty string input
string readNonEmptyString(string s) {
	string line;
	do {
		cout << s;
		getline(cin >> ws, line);
		line = trim(line);
	} while (line.empty());
	return line;
}
// Read a positive double input
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

// Read client data from user except AccountNumber ; it is passed as parameter
strClient readClientData(const string& AccountNumber) {
	strClient Client;
	Client.AccountNumber = AccountNumber;
	Client.PinCode = readNonEmptyString("Enter PinCode? ");
	Client.Name = readNonEmptyString("Enter Name? ");
	Client.Phone = readNonEmptyString("Enter Phone? ");
	Client.AccountBalance = readPositiveNumber("Enter AccountBalance? ");
	return Client;
}

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
enum TransactionsOptions {
	Deposit = 1,
	Withdraw = 2,
	Transfer = 3,
	ShowTotalBalance = 4,
	ShowTransactionsHistory = 5,
	ShowMainMenu = 6
};
enum ManageUsersOptions {
	ListUser = 1,
	AddNewUser = 2,
	DeleteUser = 3,
	UpdateUser = 4,
	FindUser = 5,
	MainMenu = 6
};
enum Permissions {
	pListClients = 1,
	pAddClient = 2,
	pDeleteClient = 4,
	pUpdateClient = 8,
	pFindClient = 16,
	pTransactions = 32,
	pManageUsers = 64,
	pAll = -1,
	pAllPermissions = 127
};

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

void showScreenHeader(const string& title) {
	cout << "\n";
	cout << "+" << string(58, '=') << "+\n";
	cout << "|" << string(58, ' ') << "|\n";

	int padding = (58 - title.length()) / 2;
	cout << "|" << string(padding, ' ') << title
		<< string(58 - padding - title.length(), ' ') << "|\n";

	cout << "|" << string(58, ' ') << "|\n";
	cout << "+" << string(58, '=') << "+\n\n";
}
void showOptions(const vector<string>& options) {
	cout << "\n";
	for (size_t i = 0; i < options.size(); i++) {
		cout << "  [" << (i + 1) << "]  " << options[i] << ".\n";
	}
	cout << "\n" << string(60, '-') << "\n";
}

void printClientCard(const strClient& client) {
	cout << "\n+" << string(58, '=') << "+\n";
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
		<< client.AccountBalance << "|\n";

	cout << "+" << string(58, '=') << "+\n";
}

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
// Mark User for deletion using pointer
bool markUserForDelete(strUser* user) {
	if (user == nullptr)
		return false;

	user->MarkForDelete = true;
	return true;
}

// Ask user for confirmation (y/n)
bool confirm(string s) {
	char c;
	do {
		cout << s << " y/n?\n";
		cin >> c;
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');
	if (c == 'Y' || c == 'y') {
		return true;
	}
	else return false;
}

//- Display all clients in table
void showAllClientsReport(const vector<strClient>& vClients) {
	clearScreen();
	showScreenHeader("Clients List");

	cout << "Total Clients: " << vClients.size() << "\n\n";

	if (vClients.size() == 0) {
		showErrorMessage("No clients available in the system!");
		customPause();
		return;
	}

	// Table header
	cout << "+" << string(105, '-') << "+\n";
	cout << "| " << left << setw(18) << "Account Number"
		<< "| " << setw(12) << "PIN Code"
		<< "| " << setw(30) << "Client Name"
		<< "| " << setw(15) << "Phone"
		<< "| " << setw(21) << "Balance" << "|\n";
	cout << "+" << string(105, '-') << "+\n";

	// Print each client
	for (const strClient& Client : vClients) {
		cout << "| " << left << setw(18) << Client.AccountNumber
			<< "| " << setw(12) << Client.PinCode
			<< "| " << setw(30) << Client.Name
			<< "| " << setw(15) << Client.Phone
			<< "| " << setw(21) << fixed << setprecision(2) << Client.AccountBalance << "|\n";
	}

	cout << "+" << string(105, '-') << "+\n\n";

	customPause();
}

// Add client with unique account number
void addClient(vector<strClient>& vClients) {
	strClient newClient;
	string accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
	strClient* findClient = findClientByAccountNumber(accountNumber, vClients);

	while (findClient != nullptr) {
		showErrorMessage("Client with accountNumber [" + accountNumber + "] already Exits, enter another account number?");
		accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
		findClient = findClientByAccountNumber(accountNumber, vClients);
	}

	newClient = readClientData(accountNumber);
	vClients.push_back(newClient);
	appendLineToFile(ClientsFileName, convertClientRecordToLine(newClient, Separator));
	showSuccessMessage("Client Added Successfully!");
}
// Show Add New Client screen
void showAddClientScreen(vector <strClient>& vClients) {
	showScreenHeader("Add New Client Screen");
	cout << "Adding New Client :\n";

	char AddMore = 'Y';
	do {
		addClient(vClients);
		customPause();
		cout << " ,do you want to add more clients? Y/N? ";

		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');

}

// Delete a client by account number
bool deleteClient(const string& accountNumber, vector<strClient>& vClients) {
	strClient* client = findClientByAccountNumber(accountNumber, vClients);
	if (!client) {
		showErrorMessage("Client with Account Number (" + accountNumber + ") is not Found!");
		return false;
	}

	printClientCard(*client);
	if (confirm("Are you sure you want delete this client ?")) {
		markClientForDelete(client);
		saveClientsToFile(ClientsFileName, vClients);
		vClients = loadClientsDataFromFile(ClientsFileName);
		showSuccessMessage("Client Deleted Successfully.");
		return true;
	}
	return false;
}
// Show Delete Client screen
void showDeleteClientScreen(vector<strClient>& vClients) {
	showScreenHeader("Delete Client Screen");
	string accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
	deleteClient(accountNumber, vClients);
	customPause();
}

// Update client information by account number
bool updateClientByAccountNumber(const string& accountNumber, vector<strClient>& vClients) {
	strClient* client = findClientByAccountNumber(accountNumber, vClients);
	if (!client) {
		showErrorMessage("Client with Account Number (" + accountNumber + ") is not Found!");
		return false;
	}

	printClientCard(*client);
	if (confirm("Are you sure you want update this client ?")) {
		*client = readClientData(accountNumber);
		saveClientsToFile(ClientsFileName, vClients);
		vClients = loadClientsDataFromFile(ClientsFileName);
		showSuccessMessage("Client Updated Successfully.");
		return true;
	}
	return false;
}
// Show Update Client screen
void showUpdateClientScreen(vector<strClient>& vClients) {
	showScreenHeader("Update Info Client Screen");

	bool success = false;
	do {
		string accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
		success = updateClientByAccountNumber(accountNumber, vClients);
		customPause();
	} while (!success);

}

// Show Find Client screen and display client details
void ShowFindClientScreen(vector<strClient>& vClients) {
	showScreenHeader("Find Client Screen");

	string accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
	strClient* client = findClientByAccountNumber(accountNumber, vClients);
	if (!client) {
		showErrorMessage("Client with account Number [" + accountNumber +"] is not found!");
	}
	else {
		printClientCard(*client);
	}
	customPause();
}

// Show exit Client screen
void showExitClient() {
	clearScreen();
	showScreenHeader("Program Ends :-)");
	customPause();
}


// Read choice from menu (form "from" to "to")
int readOption(int from, int to) {
	int choice;
	do {
		cout << "Choose what do you want to do? [" << from << " to " << to << "] ? ";
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			showErrorMessage("Invalid input! Please enter a number between " + to_string(from) + " and " + to_string(to)+".");
			continue;
		}

		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	} while ((choice < from) || (choice > to));

	return choice;
}

string hashPassword(const string& password) {
	size_t hashed = hash<string>{}(password);
	stringstream ss;
	ss << hex << setw(16) << setfill('0') << hashed;
	return ss.str();
}

string readPassword() {
	string password;
	do {
		password = readNonEmptyString("Please Enter Password? ");
		if (password.length() < 4) {
			showErrorMessage("Password must be at least 4 characters!");
		}
	} while (password.length() < 4);
	return hashPassword(password);
}

// ==================================================
// TRANSACTIONS MANAGEMENT SYSTEM
// Handles deposits, withdrawals, and balance reports
// ==================================================
string generateTransactionID() {
	static int counter = 1000;
	return "TXN" + to_string(++counter) + to_string(time(0));
}

bool depositBalanceToClient(strClient* client, double depositAmount) {
	if (client == nullptr) {
		return false;
	}
	client->AccountBalance += depositAmount;
	showSuccessMessage("Done Successfully . New Balance is : " + to_string(client->AccountBalance));
	return true;
}
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
void showDepositScreen(vector<strClient>& vClients) {
	showScreenHeader("Deposit Screen");
	string accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
	strClient* client = findClientByAccountNumber(accountNumber, vClients);

	if (!client) {
		showErrorMessage("Account " + accountNumber + " not found. Please check the account number.");
		customPause();
		return;
	}

	printClientCard(*client);
	double depositAmount = readPositiveNumber("Enter Deposit Amount: ");

	if (!confirm("Confirm deposit of " + to_string(depositAmount) + "?")) {
		showErrorMessage("Deposit cancelled");
		customPause();
		return;
	}

	double originalBalance = client->AccountBalance;

	if (depositBalanceToClient(client, depositAmount)) {
		Transaction txn = createDepositTransaction(client->AccountNumber, depositAmount);
		saveTransactionToFile(txn);
		saveClientsToFile(ClientsFileName, vClients);

		string successMessage = string("Transaction completed successfully!\n") +
			"Transaction ID: " + txn.TransactionID + "\n" +
			"Deposited Amount: " + to_string(depositAmount) + "\n" +
			"Previous Balance: " + to_string(originalBalance) + "\n" +
			"New Balance: " + to_string(originalBalance + depositAmount);

		showSuccessMessage(successMessage);
		customPause();
	}
}

bool withdrawBalanceToClient(strClient* client, double withdrawAmount) {
	if (client == nullptr) {
		showErrorMessage("Client not found!");
		customPause();
		return false;
	}

	if (withdrawAmount <= 0) {
		showErrorMessage("Invalid amount! Please enter a positive value.");
		customPause();
		return false;
	}

	if (withdrawAmount > client->AccountBalance) {
		showErrorMessage("Insufficient funds! Available balance: " +
			to_string(client->AccountBalance));
		customPause();
		return false;
	}

	client->AccountBalance -= withdrawAmount;
	showSuccessMessage("Withdrawal successful! Remaining balance: " +
		to_string(client->AccountBalance));
	customPause();
	return true;
}
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
void showWithdrawScreen(vector<strClient>& vClients) {
	showScreenHeader("Withdraw Screen");
	string accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
	strClient* client = findClientByAccountNumber(accountNumber, vClients);

	if (!client) {
		showErrorMessage("Account " + accountNumber + " not found. Please check the account number.");
		customPause();
		return;
	}

	printClientCard(*client);
	double withdrawAmount = readPositiveNumber("Enter Withdraw Amount: ");

	while (withdrawAmount > client->AccountBalance) {
		showErrorMessage("Amount exceeds balance! Available: " +
			to_string(client->AccountBalance));

		if (!confirm("Enter a different amount?")) {
			showErrorMessage("Withdrawal cancelled");
			customPause();
			return;
		}

		withdrawAmount = readPositiveNumber("Enter Withdraw Amount: ");
	}

	if (!confirm("Confirm withdrawal of " + to_string(withdrawAmount) + "?")) {
		showErrorMessage("Withdrawal cancelled");
		customPause();
		return;
	}

	double originalBalance = client->AccountBalance;

	if (withdrawBalanceToClient(client, withdrawAmount)) {
		Transaction txn = createWithdrawTransaction(client->AccountNumber, withdrawAmount);
		saveTransactionToFile(txn);
		saveClientsToFile(ClientsFileName, vClients);

		string successMessage = string("Transaction completed successfully!\n") +
			"Transaction ID: " + txn.TransactionID + "\n" +
			"Withdrawn Amount: " + to_string(withdrawAmount) + "\n" +
			"Previous Balance: " + to_string(originalBalance) + "\n" +
			"New Balance: " + to_string(originalBalance - withdrawAmount);

		showSuccessMessage(successMessage);
	}
	customPause();
}

void showTotalBalancesReport(const vector <strClient>& vClients) {
	clearScreen();
	showScreenHeader("Total Balances Report");

	double totalBalance = 0;

	cout << "Total Clients: " << vClients.size() << "\n\n";

	if (vClients.size() == 0) {
		showErrorMessage("No clients available in the system!");
		customPause();
		return;
	}

	// Table header
	cout << "+" << string(80, '-') << "+\n";
	cout << "| " << left << setw(18) << "Account Number"
		<< "| " << setw(35) << "Client Name"
		<< "| " << setw(22) << "Balance" << "|\n";
	cout << "+" << string(80, '-') << "+\n";

	// Print each client
	for (const strClient& Client : vClients) {
		totalBalance += Client.AccountBalance;
		cout << "| " << left << setw(18) << Client.AccountNumber
			<< "| " << setw(35) << Client.Name
			<< "| " << setw(22) << fixed << setprecision(2) << Client.AccountBalance << "|\n";
	}

	cout << "+" << string(80, '-') << "+\n";
	cout << "| " << left << setw(55) << "TOTAL BALANCE"
		<< "| " << setw(22) << fixed << setprecision(2) << totalBalance << "|\n";
	cout << "+" << string(80, '=') << "+\n\n";

	customPause();
}

void showTransactionsHistory(const string& accountNumber) {
	vector<Transaction> transactions = loadTransactionsFromFile("Transactions.txt");

	clearScreen();
	showScreenHeader("Transaction History");

	cout << "Account Number: " << accountNumber << "\n\n";

	// Table header
	cout << "+" << string(140, '-') << "+\n";
	cout << "| " << left << setw(18) << "Transaction ID"
		<< "| " << setw(12) << "Type"
		<< "| " << setw(15) << "From Account"
		<< "| " << setw(15) << "To Account"
		<< "| " << setw(12) << "Amount"
		<< "| " << setw(8) << "Fees"
		<< "| " << setw(20) << "Timestamp"
		<< "| " << setw(25) << "Description" << "|\n";
	cout << "+" << string(140, '-') << "+\n";

	bool found = false;
	for (const Transaction& txn : transactions) {
		if (txn.FromAccount == accountNumber || txn.ToAccount == accountNumber) {
			found = true;
			string type = (txn.Type == DEPOSIT ? "Deposit" :
				txn.Type == WITHDRAWAL ? "Withdraw" : "Transfer");

			cout << "| " << left << setw(18) << txn.TransactionID
				<< "| " << setw(12) << type
				<< "| " << setw(15) << txn.FromAccount
				<< "| " << setw(15) << txn.ToAccount
				<< "| " << setw(12) << fixed << setprecision(2) << txn.Amount
				<< "| " << setw(8) << txn.Fees
				<< "| " << setw(20) << txn.Timestamp
				<< "| " << setw(25) << txn.Description << "|\n";
		}
	}

	cout << "+" << string(140, '-') << "+\n";

	if (!found) {
		cout << "\n";
		showErrorMessage("No transactions found for this account.");
	}

	customPause();
}

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

bool validateTransferAmount(double transferAmount, double transferFee, strClient* fromClient) {
	if (fromClient->AccountBalance < (transferAmount + transferFee)) {
		showErrorMessage("Insufficient balance! Total required: " +
			to_string(transferAmount + transferFee));
		return false;
	}
	return true;
}

bool executeTransfer(strClient* fromClient, strClient* toClient,
	double transferAmount, double transferFee) {
	fromClient->AccountBalance -= (transferAmount + transferFee);
	toClient->AccountBalance += transferAmount;
	return true;
}

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

void showTransferScreen(vector<strClient>& vClients) {
	showScreenHeader("Transfer Screen");

	string fromAccount = readNonEmptyString("Enter Your Account Number: ");
	string toAccount = readNonEmptyString("Enter Recipient Account Number: ");

	strClient* fromClient = nullptr;
	strClient* toClient = nullptr;
	if (!validateTransferAccounts(fromAccount, toAccount, fromClient, toClient, vClients)) {
		return;
	}

	double transferAmount = readPositiveNumber("Enter Transfer Amount: ");
	double transferFee = transferAmount * 0.01;

	while (!validateTransferAmount(transferAmount, transferFee, fromClient)) {
		if (!confirm("Do you want to enter a different amount?")) {
			showErrorMessage("Transfer cancelled.");
			return;
		}
		transferAmount = readPositiveNumber("Enter Transfer Amount: ");
		transferFee = transferAmount * 0.01;
	}

	showTransferConfirmation(fromClient, toClient, fromAccount, toAccount,
		transferAmount, transferFee);

	if (!confirm("Confirm transfer?")) {
		showErrorMessage("Transfer cancelled.");
		return;
	}

	double originalBalance = fromClient->AccountBalance;

	if (!executeTransfer(fromClient, toClient, transferAmount, transferFee)) {
		showErrorMessage("Transfer failed!");
		return;
	}

	string description = "Transfer to " + toClient->Name;
	Transaction transferTransaction = createTransferTransaction(fromAccount, toAccount,
		transferAmount, transferFee, description);
	saveTransactionToFile(transferTransaction);

	saveClientsToFile(ClientsFileName, vClients);
	vClients = loadClientsDataFromFile(ClientsFileName);

	string successMessage = string("Transfer completed successfully!\n") +
		"Transaction ID: " + transferTransaction.TransactionID + "\n" +
		"Transferred Amount: " + to_string(transferAmount) + "\n" +
		"Fee: " + to_string(transferFee) + "\n" +
		"Previous Balance: " + to_string(originalBalance) + "\n" +
		"New Balance: " + to_string(fromClient->AccountBalance);

	showSuccessMessage(successMessage);
	customPause();
}

void executeTransactionOption(TransactionsOptions TransactionMenuOption, vector<strClient>& vClients)
{
	switch (TransactionMenuOption)
	{
	case TransactionsOptions::Deposit:
	{
		clearScreen();
		showDepositScreen(vClients);
		break;
	}

	case TransactionsOptions::Withdraw:
	{
		clearScreen();
		showWithdrawScreen(vClients);
		break;
	}

	case TransactionsOptions::Transfer: {
		clearScreen();
		showTransferScreen(vClients);
		break;
	}

	case TransactionsOptions::ShowTotalBalance:
	{
		clearScreen();
		showTotalBalancesReport(vClients);
		break;
	}

	case TransactionsOptions::ShowTransactionsHistory: {
		string accountNumber = readNonEmptyString("Enter Account Number: ");
		showTransactionsHistory(accountNumber);
		break;
	}

	case TransactionsOptions::ShowMainMenu:
	{

		break;
	}
	}

}
void showTransactionsMenuScreen() {
	clearScreen();
	showScreenHeader("Transactions Menu Screen");
	vector<string> options = { "Deposit","Withdraw","Transfer","Total Balances","Transactions History","Main Menu"};
	showOptions(options);
}
void ManageTransactions(vector<strClient>& vClients) {
	TransactionsOptions Choice;
	do {
		showTransactionsMenuScreen();
		Choice = (TransactionsOptions)readOption(1, 6);
		if (Choice != TransactionsOptions::ShowMainMenu) {
			executeTransactionOption(Choice, vClients);
			vClients = loadClientsDataFromFile(ClientsFileName);
		}
	} while (Choice != TransactionsOptions::ShowMainMenu);
}
//========================================================================


int readPermissionsToSet()
{
	int Permissions = 0;

	if (confirm("Do you want to give full access? "))
		return Permissions::pAll;
	cout << "\nDo you want to give access to : \n ";
	if (confirm("Show Client List? "))
		Permissions += Permissions::pListClients;
	if (confirm("Add New Client? "))
		Permissions += Permissions::pAddClient;
	if (confirm("Delete Client? "))
		Permissions += Permissions::pDeleteClient;
	if (confirm("Update Client? "))
		Permissions += Permissions::pUpdateClient;
	if (confirm("Find Client? "))
		Permissions += Permissions::pFindClient;
	if (confirm("Transactions? "))
		Permissions += Permissions::pTransactions;
	if (confirm("Manage Users? "))
		Permissions += Permissions::pManageUsers;

	if (Permissions == Permissions::pAllPermissions)
		return Permissions::pAll;

	return Permissions;

}

strUser* findUserByUserName(const string& userName, vector<strUser>& vUsers) {
	for (auto& user : vUsers) {
		if (user.UserName == userName)
			return &user;
	}
	return nullptr;
}
strUser readUserData(const string& userName) {
	strUser user;
	user.UserName = userName;
	user.Password = readPassword();
	user.Permissions = readPermissionsToSet();
	return user;
}
bool checkUserPassword(const string& inputPassword, strUser* User) {
	if (User == nullptr)
		return false;

	string hashInputPassword = hashPassword(inputPassword);
	return (User->Password == hashInputPassword);
}

string getPermissionsAsString(int permissions) {
	if (permissions == Permissions::pAll)
		return "Full Access";

	vector<string> permissionList;

	if (permissions & Permissions::pListClients)
		permissionList.push_back("List Clients");
	if (permissions & Permissions::pAddClient)
		permissionList.push_back("Add Clients");
	if (permissions & Permissions::pDeleteClient)
		permissionList.push_back("Delete Clients");
	if (permissions & Permissions::pUpdateClient)
		permissionList.push_back("Update Clients");
	if (permissions & Permissions::pFindClient)
		permissionList.push_back("Find Clients");
	if (permissions & Permissions::pTransactions)
		permissionList.push_back("Transactions");
	if (permissions & Permissions::pManageUsers)
		permissionList.push_back("Manage Users");

	if (permissionList.empty())
		return "No Permissions";

	string result;
	for (size_t i = 0; i < permissionList.size(); i++) {
		result += permissionList[i];
		if (i < permissionList.size() - 1)
			result += ", ";
	}
	return result;
}


void printUserCard(strUser* User) {
	cout << "\nThe following are the User details:\n";
	cout << "-----------------------------------";
	cout << "\nUser Name   : " << User->UserName;
	cout << "\nPermissions : " << getPermissionsAsString(User->Permissions); 
	cout << "\n-----------------------------------\n";
}

void printUserLine(const strUser& user) {
	cout << "|" << setw(20) << left << user.UserName;
	cout << "|" << setw(40) << left << getPermissionsAsString(user.Permissions);
}
void ShowAllUsersScreen(const vector<strUser>& vUsers) {
	clearScreen();
	cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;

	cout << "|" << setw(20) << left << "User Name";
	cout << "|" << setw(40) << left << "Permissions";
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;

	for (const strUser& user : vUsers) {
		printUserLine(user);
		cout << endl;
	}
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;
	customPause();
}

void addNewUser(vector<strUser>& vUsers) {
	clearScreen();
	showScreenHeader("Add New User Screen");

	string name = readNonEmptyString("Please Enter UserName? ");
	strUser* existingUser = findUserByUserName(name, vUsers);

	while (existingUser != nullptr) {
		showErrorMessage("User with name [" + name + "] already Exits, enter a different username?");
		name = readNonEmptyString("Please Enter UserName? ");
		existingUser = findUserByUserName(name, vUsers);
	}

	strUser newUser = readUserData(name);
	vUsers.push_back(newUser);
	appendLineToFile(UsersFileName, convertUserRecordToLine(newUser, Separator));

	showSuccessMessage("User Added Successfully!");
}
void showAddNewUser(vector<strUser>& vUsers) {

	char AddMore = 'Y';
	do
	{
		addNewUser(vUsers);
		customPause();
		cout << " ,do you want to add more Users? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

bool deleteUserByNameAndPassword(const string& userName, const string& password, vector<strUser>& vUsers) {
	if (userName == "Admin")
	{
		showErrorMessage("You cannot Delete This User.");
		return false;

	}
	strUser* user = findUserByUserName(userName, vUsers);

	if (checkUserPassword(password, user)) {
		printUserCard(user);
		if (confirm("Are you sure you want delete this user ?")) {
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
		showErrorMessage("Users with name (" + userName + ") with the password (" + password + ") is not Found!");
		return false;
	}
}

void showDeleteUserScreen(vector<strUser>& vUsers) {
	clearScreen();
	showScreenHeader("Delete User Screen");

	string name = readNonEmptyString("Please Enter UserName? ");
	string password = readNonEmptyString("Please Enter Password? ");
	deleteUserByNameAndPassword(name, password, vUsers);
	customPause();
}

void showFindUserScreen(vector<strUser>& vUsers) {
	clearScreen();
	showScreenHeader("Find User Screen");

	string name = readNonEmptyString("Please Enter UserName? ");
	strUser* user = findUserByUserName(name, vUsers);
	string password = readNonEmptyString("Please Enter Password? ");
	if (checkUserPassword(password, user)) {
		printUserCard(user);
	}
	else {
		showErrorMessage("User with name (" + name + ") and password (" + password + ") is not found!.");
	}
	customPause();
}

bool updateUserByNameAndPassword(const string& userName, const string& password, vector<strUser>& vUsers) {
	strUser* user = findUserByUserName(userName, vUsers);
	if (checkUserPassword(password, user)) {
		printUserCard(user);
		if (confirm("Are you sure you want update this User ?")) {
			for (strUser& u : vUsers) {
				if (u.UserName == userName) {
					cout << "Enter new user data:\n";
					u = readUserData(userName);
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



void showUpdateUserScreen(vector<strUser>& vUsers) {
	clearScreen();
	showScreenHeader("Update User Screen");

	bool success = false;
	do {
		string name = readNonEmptyString("Please Enter UserName? ");
		string password = readNonEmptyString("Please Enter Password? ");
		success = updateUserByNameAndPassword(name, password, vUsers);
		customPause();
	} while (!success);
}

void executeUserOption(ManageUsersOptions manageUsersOptions, vector <strUser>& vUsers) {
	switch (manageUsersOptions) {
	case ManageUsersOptions::ListUser:
		ShowAllUsersScreen(vUsers);
		break;
	case ManageUsersOptions::AddNewUser: {
		showAddNewUser(vUsers);
		break;
	}
	case ManageUsersOptions::DeleteUser:
		showDeleteUserScreen(vUsers);
		break;
	case ManageUsersOptions::FindUser:
		showFindUserScreen(vUsers);
		break;
	case ManageUsersOptions::UpdateUser:
		showUpdateUserScreen(vUsers);
		break;
	case ManageUsersOptions::MainMenu:
		return;


	}
}

bool checkAccessPermission(Permissions permission) {
	if (CurrentUser.Permissions == Permissions::pAll)
		return true;

	return (CurrentUser.Permissions & permission) == permission;
}


void manageUsersMenu(vector<strUser>& vUsers) {
	ManageUsersOptions choice;
	do
	{
		showManageUsersScreen();
		choice = (ManageUsersOptions)readOption(1, 6);
		if (choice != ManageUsersOptions::MainMenu)
			executeUserOption(choice, vUsers);

	} while (choice != ManageUsersOptions::MainMenu);
}
// Execute main menu option


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

vector<string> buildMainMenuOptions() {
	vector<string> options;
	if (checkAccessPermission(Permissions::pAll) || checkAccessPermission(Permissions::pAllPermissions))
		options = { "Show Client List","Add New Client","Delete Client","Update Client","Find Client","Transactions","Manage Users" };
	else {
		if (checkAccessPermission(Permissions::pListClients))
			options.push_back("Show Client List");
		if (checkAccessPermission(Permissions::pAddClient))
			options.push_back("Add New Client");
		if (checkAccessPermission(Permissions::pDeleteClient))
			options.push_back("Delete Client");
		if (checkAccessPermission(Permissions::pUpdateClient))
			options.push_back("Update Client");
		if (checkAccessPermission(Permissions::pFindClient))
			options.push_back("Find Client");
		if (checkAccessPermission(Permissions::pTransactions))
			options.push_back("Transactions");
		if (checkAccessPermission(Permissions::pManageUsers))
			options.push_back("Manage Users");
	}
	options.push_back("Logout");
	options.push_back("Exit");

	return options;
}

void executeMainMenuOption(MainMenuOption MainMenuOption, vector<strClient>& vClients) {
	clearScreen();
	bool hasPermission = true;
	switch (MainMenuOption) {
	case MainMenuOption::ShowClientList:
		hasPermission = checkAccessPermission(Permissions::pListClients);
		if (hasPermission) {
			showAllClientsReport(vClients);
		}
		break;
	case MainMenuOption::AddNewClient:
		hasPermission = checkAccessPermission(Permissions::pAddClient);
		if (hasPermission) {
			showAddClientScreen(vClients);
			vClients = loadClientsDataFromFile(ClientsFileName);
		}
		break;
	case MainMenuOption::DeleteClient:
		hasPermission = checkAccessPermission(Permissions::pDeleteClient);
		if (hasPermission) {
			showDeleteClientScreen(vClients);
		}
		break;
	case MainMenuOption::UpdateClient:
		hasPermission = checkAccessPermission(Permissions::pUpdateClient);
		if (hasPermission) {
			showUpdateClientScreen(vClients);
			vClients = loadClientsDataFromFile(ClientsFileName);
		}
		break;
	case MainMenuOption::FindClient:
		hasPermission = checkAccessPermission(Permissions::pFindClient);
		if (hasPermission) {
			ShowFindClientScreen(vClients);
		}
		break;
	case MainMenuOption::Transactions:
		hasPermission = checkAccessPermission(Permissions::pTransactions);
		if (hasPermission) {
			ManageTransactions(vClients);
			vClients = loadClientsDataFromFile(ClientsFileName);
		}
		break;
	case MainMenuOption::ManageUsers: {
		hasPermission = checkAccessPermission(Permissions::pManageUsers);
		if (hasPermission) {
			vector<strUser> vUsers = loadUsersDataFromFile(UsersFileName);
			manageUsersMenu(vUsers);
			saveUsersToFile(UsersFileName, vUsers);
		}
		break;
	}
	case MainMenuOption::Logout:
		if (confirm("Are you sure you want to logout?")) {
			clearCurrentUserSession();
			showSuccessMessage("You have been logged out successfully. Session cleared.");
			customPause();
			login();
		}
		break;
	case MainMenuOption::Exit:
		showExitClient();
		break;
	default:
		showErrorMessage("Invalid option.");
		break;
	}
	if (!hasPermission) {
		clearScreen();
		cout << "\n------------------------------------\n";
		showErrorMessage("Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.");
		cout << "\n------------------------------------\n";
	}
}
// Main loop: show menu, execute options, repeat until exit
void ManageMainMenu(vector<strClient>& vClients) {
	MainMenuOption Choice;

	do {
		clearScreen();
		showScreenHeader("Main Menu Screen");
		vector<string> options = buildMainMenuOptions();
		showOptions(options);

		int choiceNum = readOption(1, options.size());
		Choice = convertChoiceToMainMenuOption(choiceNum, options);

		if (Choice == MainMenuOption::Exit) {
			showExitClient();
			break;
		}
		executeMainMenuOption(Choice, vClients);

	} while (Choice != MainMenuOption::Exit);
}

// Display Manage Users menu options
void showManageUsersScreen() {
	clearScreen();
	showScreenHeader("Manage Users Menu Screen");
	vector<string> options = { "List Users","Add New User","Delete User","Update User","Find User","Main Menu" };
	showOptions(options);
}

void login() {
	clearScreen();
	showScreenHeader("Login Screen");

	strUser sessionUser;
	if (loadCurrentUserSession(sessionUser)) {
	
		cout << "Welcome back, " << sessionUser.UserName << "!" << endl;

		if (confirm("Do you want to continue with your previous session?")) {
			CurrentUser = sessionUser;
			showSuccessMessage("Welcome back, " + CurrentUser.UserName + "!");
			customPause();
			vector<strClient> vClients = loadClientsDataFromFile(ClientsFileName);
			ManageMainMenu(vClients);
			return;
		}
	}

	bool found = false;
	vector<strUser> vUsers = loadUsersDataFromFile(UsersFileName);

	do {
		string name = readNonEmptyString("Please Enter UserName? ");
		string password = readNonEmptyString("Please Enter Password? ");

		strUser* user = findUserByUserName(name, vUsers);
		found = checkUserPassword(password, user);

		if (found) {
			CurrentUser = *user;
			saveCurrentUserSession(CurrentUser); 
			showSuccessMessage("Login successful! Welcome, " + CurrentUser.UserName + "!");
			customPause();
		}
		else {
			showErrorMessage("Invalid username or password, try again.");
			customPause();
		}
	} while (!found);

	customPause();
	vector<strClient> vClients = loadClientsDataFromFile(ClientsFileName);
	ManageMainMenu(vClients);
}

void createDefaultAdmin() {
	vector<strUser> vUsers = loadUsersDataFromFile(UsersFileName);

	if (vUsers.empty()) {
		strUser adminUser;
		adminUser.UserName = "Admin";
		adminUser.Password = hashPassword("1234");  
		adminUser.Permissions = Permissions::pAll;

		vUsers.push_back(adminUser);
		saveUsersToFile(UsersFileName, vUsers);

		cout << "Default admin user created:\n";
		cout << "Username: Admin\n";
		cout << "Password: 1234\n";
		cout << "Please change the password after first login!\n";
		customPause();
	}
}
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

	customPause();
	return 0;
}
