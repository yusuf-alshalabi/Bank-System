
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>
#include <functional>

using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";
const string Separator = "#//#";
struct strClient {
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
};
struct strUser
{
	string UserName = "";
	string Password = "";
	int Permissions = -1;
	bool MarkForDelete = false;
};

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

// Save current user session to binary file
void saveCurrentUserSession(const strUser& user) {
	createSessionFolder();
	string sessionPath = getSessionPath();

	ofstream file(sessionPath, ios::binary);
	if (file.is_open()) {
		// Save username length and content
		size_t usernameSize = user.UserName.size();
		file.write(reinterpret_cast<const char*>(&usernameSize), sizeof(usernameSize));
		file.write(user.UserName.c_str(), usernameSize);

		// Save password length and content
		size_t passwordSize = user.Password.size();
		file.write(reinterpret_cast<const char*>(&passwordSize), sizeof(passwordSize));
		file.write(user.Password.c_str(), passwordSize);

		// Save permissions
		file.write(reinterpret_cast<const char*>(&user.Permissions), sizeof(user.Permissions));

		file.close();
		cout << "Session saved securely to: " << sessionPath << endl;
	}
}

// Load current user session from binary file
bool loadCurrentUserSession(strUser& user) {
	string sessionPath = getSessionPath();
	ifstream file(sessionPath, ios::binary);

	if (file.is_open()) {
		// Read username
		size_t usernameSize;
		file.read(reinterpret_cast<char*>(&usernameSize), sizeof(usernameSize));
		user.UserName.resize(usernameSize);
		file.read(&user.UserName[0], usernameSize);

		// Read password
		size_t passwordSize;
		file.read(reinterpret_cast<char*>(&passwordSize), sizeof(passwordSize));
		user.Password.resize(passwordSize);
		file.read(&user.Password[0], passwordSize);

		// Read permissions
		file.read(reinterpret_cast<char*>(&user.Permissions), sizeof(user.Permissions));

		file.close();
		cout << "Session loaded from: " << sessionPath << endl;
		return true;
	}
	return false;
}

// Clear current user session (on logout)
void clearCurrentUserSession() {
	string sessionPath = getSessionPath();
	if (remove(sessionPath.c_str()) == 0) {
		cout << "Session cleared securely." << endl;
	}
}


//========================================================================

vector<string> buildMainMenuOptions();
void showManageUsersScreen();
void ManageTransactions(vector<strClient>& vClients);
void ManageMainMenu(vector<strClient>& vClients);
void login();

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
	vector<string> vClientData;
	vClientData = splitStringByDelimiter(Line, seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);//cast string to double
	return Client;
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
	MyFile.open(FileName, ios::out);//over write
	string DataLine;
	if (MyFile.is_open()) {
		for (strClient c : vClients) {
			if (c.MarkForDelete == false) {
				//we only write records that are not marked for delete.
				DataLine = convertClientRecordToLine(c, Separator);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
}
void saveUsersToFile(string FileName, const vector<strUser>& vUsers) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;
	if (MyFile.is_open()) {
		for (strUser c : vUsers) {
			if (c.MarkForDelete == false) {
				DataLine = convertUserRecordToLine(c, Separator);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
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

// Read a non-empty string input
string readNonEmptyString(string s) {
	string line;
	do {
		cout << s;
		getline(cin >> ws, line);
	} while (line.empty());
	return line;
}
// Read a positive double input
double readPositiveNumber(string s) {
	double num;
	cout << s;
	cin >> num;
	while (cin.fail() || num < 0) {
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		showErrorMessage("Invalid Number, Enter a valid one: ");
		cin >> num;
	}
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
	ShowTotalBalance = 3,
	ShowMainMenu = 4
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

void showScreenHeader(const string& title) {
	clearScreen();
	cout << "\n-------------------------------------------------\n";
	cout << "\t\t" << title << "\n";
	cout << "-------------------------------------------------\n";
}
void showOptions(vector<string> options) {
	int count = 1;
	for (string& option : options) {
		cout << "\t[" << count++ << "] " << option << ".\n";
	}
	cout << "===========================================\n";
}

void printClientCard(const strClient& Client) {
	showScreenHeader("Client Information");
	cout << "\nAccount Number  : " << Client.AccountNumber;
	cout << "\nPin Code       : " << Client.PinCode;
	cout << "\nName           : " << Client.Name;
	cout << "\nPhone          : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-------------------------------------------------\n";

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

void printClientRecordLine(const strClient& client) {
	cout << "|" << setw(15) << left << client.AccountNumber;
	cout << "|" << setw(10) << left << client.PinCode;
	cout << "|" << setw(40) << left << client.Name;
	cout << "|" << setw(12) << left << client.Phone;
	cout << "|" << setw(12) << left << client.AccountBalance << "|";
}
//- Display all clients in table
void showAllClientsReport(const vector<strClient>& vClients) {
	clearScreen();
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;

	cout << "|" << setw(15) << left << "AccountNumber";
	cout << "|" << setw(10) << left << "PinCode";
	cout << "|" << setw(40) << left << "Name";
	cout << "|" << setw(12) << left << "Phone";
	cout << "|" << setw(12) << left << "AccountBalance";
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;
	for (const strClient& Client : vClients) {
		printClientRecordLine(Client);
		cout << endl;
	}
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;
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
}

// Show exit Client screen
void showExitClient() {
	clearScreen();

	showScreenHeader("Program Ends :-)");
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
bool depositBalanceToClient(strClient* client, double depositAmount) {
	if (client == nullptr) {
		return false;
	}
	client->AccountBalance += depositAmount;
	showSuccessMessage("Done Successfully . New Balance is : " + to_string(client->AccountBalance));
	return true;
}
void showDepositScreen(vector<strClient>& vClients) {
	showScreenHeader("Deposit Screen");
	string accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
	strClient* client = findClientByAccountNumber(accountNumber, vClients);
	if (!client) {
		showErrorMessage("Client with Account Number (" + accountNumber + ") is not Found!");
		return;
	}
	printClientCard(*client);
	double depositAmount = readPositiveNumber("Enter Deposit Amount: ");
	if (depositBalanceToClient(client, depositAmount)) {
		saveClientsToFile(ClientsFileName, vClients);
		vClients = loadClientsDataFromFile(ClientsFileName);
	}
}

bool withdrawBalanceToClient(strClient* client, double withdrawAmount) {
	if (client == nullptr) {
		cout << "Client is null\n";
		return false;
	}
	if (withdrawAmount <= client->AccountBalance) {
		client->AccountBalance -= withdrawAmount;
		showSuccessMessage("Done Successfully . New Balance is : " + to_string(client->AccountBalance));
		return true;
	}
	cout << "Amount Exceed the balance\n";
	return false;
}
void showWithdrawScreen(vector<strClient>& vClients) {
	showScreenHeader("Withdraw Screen");
	string accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
	strClient* client = findClientByAccountNumber(accountNumber, vClients);
	if (!client) {
		showErrorMessage("Client with Account Number (" + accountNumber + ") is not Found!");
		return;
	}
	printClientCard(*client);
	double withdrawAmount = readPositiveNumber("Enter Withdraw Amount: ");
	while (withdrawAmount > client->AccountBalance) {
		cout << "Amount Exceed the balance, you can Withdraw up to " << client->AccountBalance << ".\n";
		withdrawAmount = readPositiveNumber("Please enter another withdraw amount?");
	}

	if (withdrawBalanceToClient(client, withdrawAmount)) {
		saveClientsToFile(ClientsFileName, vClients);
		vClients = loadClientsDataFromFile(ClientsFileName);
	}
}

void printBalanceClientLine(const strClient& client) {
	cout << "| " << setw(15) << left << client.AccountNumber;
	cout << "| " << setw(40) << left << client.Name;
	cout << "| " << setw(35) << left << client.AccountBalance;
}
void showTotalBalancesReport(const vector <strClient>& vClients) {
	clearScreen();
	double totalBalance = 0;
	cout << "\n\t\t\t\t\t Balance List (" << vClients.size() << ") Client(s).";
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;

	cout << "| " << setw(15) << left << "AccountNumber";
	cout << "| " << setw(40) << left << "Name";
	cout << "| " << setw(12) << left << "AccountBalance";
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;

	if (vClients.size() == 0) {
		cout << "\t\t\t\t No Client Available In The System!.\n";
	}

	for (const strClient& Client : vClients) {
		totalBalance += Client.AccountBalance;
		printBalanceClientLine(Client);
		cout << endl;
	}
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;
	cout << "\n\t\t\t\t\tTotal Balance = " << totalBalance << endl;
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


	case TransactionsOptions::ShowTotalBalance:
	{
		clearScreen();
		showTotalBalancesReport(vClients);
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
	vector<string> options = { "Deposit","Withdraw","Total Balances","Main Menu" };
	showOptions(options);
}
void ManageTransactions(vector<strClient>& vClients) {
	TransactionsOptions Choice;
	do {
		showTransactionsMenuScreen();
		Choice = (TransactionsOptions)readOption(1, 4);
		executeTransactionOption(Choice, vClients);
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
void printUserCard(strUser* User) {
	cout << "\nThe following are the User details:\n";
	cout << "-----------------------------------";
	cout << "\nUser Name   : " << User->UserName;
	cout << "\nPassword    : " << User->Password;
	cout << "\nPermissions  : " << to_string(User->Permissions);
	cout << "\n-----------------------------------\n";

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

strUser CurrentUser;
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
		// NEW: Clear session on logout
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

	// Try to load existing BINARY session
	strUser sessionUser;
	if (loadCurrentUserSession(sessionUser)) {
		cout << "Welcome back, " << sessionUser.UserName << "!" << endl;
		cout << "Session loaded from secure binary storage." << endl;

		if (confirm("Do you want to continue with your previous session?")) {
			CurrentUser = sessionUser;
			showSuccessMessage("Welcome back, " + CurrentUser.UserName + "!");
			customPause();
			vector<strClient> vClients = loadClientsDataFromFile(ClientsFileName);
			ManageMainMenu(vClients);
			return;
		}
	}

	// Continue with normal login if no session or user declined
	bool found = false;
	vector<strUser> vUsers = loadUsersDataFromFile(UsersFileName);

	do {
		string name = readNonEmptyString("Please Enter UserName? ");
		string password = readNonEmptyString("Please Enter Password? ");

		strUser* user = findUserByUserName(name, vUsers);
		found = checkUserPassword(password, user);

		if (found) {
			CurrentUser = *user;
			// Save the new session in BINARY format
			saveCurrentUserSession(CurrentUser);
			cout << "New session saved in secure binary format." << endl;
		}
		else {
			showErrorMessage("Invalid username or password, try again.");
		}
	} while (!found);

	showSuccessMessage("Welcome back, " + CurrentUser.UserName + "!");
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
	createDefaultAdmin();
	login();

	customPause();
	return 0;

}
