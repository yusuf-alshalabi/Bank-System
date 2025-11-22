
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <limits>

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

vector<string> buildMainMenuOptions();
void showManageUsersScreen();
void ManageTransactions(vector<strClient>& vClients);
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
void returnToTransactionsMenu(vector<strClient>& vClients) {
	cout << "\n\nPress any key to go back to Transactions Menu...";
	customPause();
	ManageTransactions(vClients);
}
void returnToMainMenu() {
	cout << "\n\nPress any key to go back to Main Menu...";
	customPause();
}
void returnToManageUsers() {
	cout << "\n\nPress any key to go back to Manage Users...";
	customPause();
	showManageUsersScreen();
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
		cout << "Invalid Number, Enter a valid one: " << endl;
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
}

// Add client with unique account number
void addClient(vector<strClient>& vClients) {
	strClient newClient;
	string accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
	strClient* findClient = findClientByAccountNumber(accountNumber, vClients);

	while (findClient != nullptr) {
		cout << "Client with accountNumber [" << accountNumber << "] already Exits, enter another account number?\n";
		accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
		findClient = findClientByAccountNumber(accountNumber, vClients);
	}

	newClient = readClientData(accountNumber);
	vClients.push_back(newClient);
	appendLineToFile(ClientsFileName, convertClientRecordToLine(newClient, Separator));
	cout << "Client Added Successfully!";
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
		cout << "\nClient with Account Number (" << accountNumber << ") is not Found!";
		return false;
	}

	printClientCard(*client);
	if (confirm("Are you sure you want delete this client ?")) {
		markClientForDelete(client);
		saveClientsToFile(ClientsFileName, vClients);
		vClients = loadClientsDataFromFile(ClientsFileName);
		cout << "\n\nClient Deleted Successfully.";
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
		cout << "\nClient with Account Number (" << accountNumber << ") is not Found!";
		return false;
	}

	printClientCard(*client);
	if (confirm("Are you sure you want update this client ?")) {
		*client = readClientData(accountNumber);
		saveClientsToFile(ClientsFileName, vClients);
		vClients = loadClientsDataFromFile(ClientsFileName);
		cout << "\n\nClient Updated Successfully.";
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
		cout << "Client with account Number [" << accountNumber << "] is not found!\n";
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
		while (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Invalid Number, Enter a valid one:  [" << from << " to " << to << "] ? " << endl;
			cin >> choice;
		}
	} while ((choice < from) || (choice > to));

	return choice;
}

string encryptText(const string& text, int key = 5) {
	string encrypted = "";
	for (char c : text) {
		encrypted += char(c + key);
	}
	return encrypted;
}

string decryptText(const string& encryptedText, int key = 5) {
	string decrypted = "";
	for (char c : encryptedText) {
		decrypted += char(c - key);
	}
	return decrypted;
}

string readPassword() {
	string password;
	do {
		password = readNonEmptyString("Please Enter Password? ");
		if (password.length() < 4) {
			cout << "Password must be at least 4 characters!\n";
		}
	} while (password.length() < 4);
	return encryptText(password);
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
	cout << "\nDone Successfully . New Balance is : " << client->AccountBalance << endl;
	return true;
}
void showDepositScreen(vector<strClient>& vClients) {
	showScreenHeader("Deposit Screen");
	string accountNumber = readNonEmptyString("\nPlease enter AccountNumber? ");
	strClient* client = findClientByAccountNumber(accountNumber, vClients);
	if (!client) {
		cout << "\nClient with Account Number (" << accountNumber << ") is not Found!";
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
		cout << "\nDone Successfully . New Balance is : " << client->AccountBalance << endl;
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
		cout << "\nClient with Account Number (" << accountNumber << ") is not Found!";
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

}

void executeTransactionOption(TransactionsOptions TransactionMenuOption, vector<strClient>& vClients)
{
	switch (TransactionMenuOption)
	{
	case TransactionsOptions::Deposit:
	{
		clearScreen();
		showDepositScreen(vClients);
		returnToTransactionsMenu(vClients);
		break;
	}

	case TransactionsOptions::Withdraw:
	{
		clearScreen();
		showWithdrawScreen(vClients);
		returnToTransactionsMenu(vClients);
		break;
	}


	case TransactionsOptions::ShowTotalBalance:
	{
		clearScreen();
		showTotalBalancesReport(vClients);
		returnToTransactionsMenu(vClients);
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

	string decryptedStoredPassword = decryptText(User->Password);
	return (decryptedStoredPassword == inputPassword);
}
void printUserCard(strUser* User) {
	cout << "\nThe following are the User details:\n";
	cout << "-----------------------------------";
	cout << "\nUser Name   : " << User->UserName;
	cout << "\nPassword    : " << User->Password;
	cout << "\nPermissions  : " << to_string(User->Permissions);
	cout << "\n-----------------------------------\n";

}

void printUserLine(const strUser& user) {
	cout << "|" << setw(25) << left << user.UserName;
	cout << "|" << setw(15) << left << user.Password;
	cout << "|" << setw(15) << left << user.Permissions;
}
void ShowAllUsersScreen(const vector<strUser>& vUsers) {
	clearScreen();
	cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;
	cout << "|" << setw(25) << left << "UserName";
	cout << "|" << setw(15) << left << "Password";
	cout << "|" << setw(15) << left << "Permissions";
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;
	for (const strUser& user : vUsers) {
		printUserLine(user);
		cout << endl;
	}
	cout << "\n________________________________________________";
	cout << "_______________________________________________\n" << endl;
}

void addNewUser(vector<strUser>& vUsers) {
	clearScreen();
	showScreenHeader("Add New User Screen");

	string name = readNonEmptyString("Please Enter UserName? ");
	strUser* existingUser = findUserByUserName(name, vUsers);

	while (existingUser != nullptr) {
		cout << "User with name [" << name << "] already Exits, enter a different username?\n";
		name = readNonEmptyString("Please Enter UserName? ");
		existingUser = findUserByUserName(name, vUsers);
	}

	strUser newUser = readUserData(name);
	vUsers.push_back(newUser);
	appendLineToFile(UsersFileName, convertUserRecordToLine(newUser, Separator));

	cout << "User Added Successfully!";
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
		cout << "\n\nYou cannot Delete This User.";
		return false;

	}
	strUser* user = findUserByUserName(userName, vUsers);

	if (checkUserPassword(password, user)) {
		printUserCard(user);
		if (confirm("Are you sure you want delete this user ?")) {
			markUserForDelete(user);
			saveUsersToFile(UsersFileName, vUsers);
			vUsers = loadUsersDataFromFile(UsersFileName);
			cout << "\n\nUser Deleted Successfully.\n";
			return true;
		}
		else {
			return false;
		}
	}
	else {
		cout << "\nUsers with name (" << userName << ") with the password (" + password + ") is not Found!";
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
		cout << "\nUser with name (" + name + ") and password (" + password + ") is not found!.\n";
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
			cout << "\n\nUser Updated Successfully.";
			return true;
		}
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
		returnToManageUsers();
		break;
	case ManageUsersOptions::AddNewUser: {
		showAddNewUser(vUsers);
		returnToManageUsers();
		break;
	}
	case ManageUsersOptions::DeleteUser:
		showDeleteUserScreen(vUsers);
		returnToManageUsers();
		break;
	case ManageUsersOptions::FindUser:
		showFindUserScreen(vUsers);
		returnToManageUsers();
		break;
	case ManageUsersOptions::UpdateUser:
		showUpdateUserScreen(vUsers);
		returnToManageUsers();
		break;
	case ManageUsersOptions::MainMenu:
		returnToMainMenu();
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
			returnToMainMenu();
		}
		break;
	case MainMenuOption::AddNewClient:
		hasPermission = checkAccessPermission(Permissions::pAddClient);
		if (hasPermission) {
			showAddClientScreen(vClients);
			returnToMainMenu();
		}
		break;
	case MainMenuOption::DeleteClient:
		hasPermission = checkAccessPermission(Permissions::pDeleteClient);
		if (hasPermission) {
			showDeleteClientScreen(vClients);
			returnToMainMenu();
		}
		break;
	case MainMenuOption::UpdateClient:
		hasPermission = checkAccessPermission(Permissions::pUpdateClient);
		if (hasPermission) {
			showUpdateClientScreen(vClients);
			returnToMainMenu();
		}
		break;
	case MainMenuOption::FindClient:
		hasPermission = checkAccessPermission(Permissions::pFindClient);
		if (hasPermission) {
			ShowFindClientScreen(vClients);
			returnToMainMenu();
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
			returnToMainMenu();
		}
		break;
	}
	case MainMenuOption::Logout:
		login();
		break;
	case MainMenuOption::Exit:
		showExitClient();
		break;
	default:
		cout << "\nInvalid option.\n";
		break;
	}
	if (!hasPermission) {
		clearScreen();
		cout << "\n------------------------------------\n";
		cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
		cout << "\n------------------------------------\n";
		returnToMainMenu();
	}
}
// Main loop: show menu, execute options, repeat until exit
void ManageMainMenu(vector<strClient>& vClients) {
	do {
		clearScreen();
		showScreenHeader("Main Menu Screen");
		vector<string> options = buildMainMenuOptions();
		showOptions(options);

		int choiceNum = readOption(1, options.size());
		MainMenuOption Choice = convertChoiceToMainMenuOption(choiceNum, options);

		if (Choice == MainMenuOption::Exit) {
			showExitClient();
			break;
		}
		executeMainMenuOption(Choice, vClients);

	} while (true);
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
	bool found = false;
	vector<strUser> vUsers = loadUsersDataFromFile(UsersFileName);

	do {
		string name = readNonEmptyString("Please Enter UserName? ");
		string password = readNonEmptyString("Please Enter Password? ");

		strUser* user = findUserByUserName(name, vUsers);
		found = checkUserPassword(password, user);

		if (found) {
			CurrentUser = *user;
		}
		else {
			cout << "\nInvalid username or password, try again.\n";
		}
	} while (!found);

	cout << "\nWelcome back, " << CurrentUser.UserName << "!\n";
	customPause();
	vector<strClient> vClients = loadClientsDataFromFile(ClientsFileName);
	ManageMainMenu(vClients);
}

int main()
{
	cout << fixed << setprecision(2);
	login();

	customPause();
	return 0;

}
