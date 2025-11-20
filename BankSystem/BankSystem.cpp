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
void goBackToTransactionsMenu(vector<strClient>& vClients) {
	cout << "\n\nPress any key to go back to Transactions Menu...";
	customPause();
	ManageTransactions(vClients);
}
void goBackToMainMenu() {
	cout << "\n\nPress any key to go back to Main Menu...";
	customPause();
}
void goBackToManageUsers() {
	cout << "\n\nPress any key to go back to Manage Users...";
	customPause();
	showManageUsersScreen();
}
// Split string into tokens using delimiter
vector<string> splitString(string S1, string delim) {
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
// Convert a client struct to a single line for file
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
strClient convertLinetoRecord(const string& Line, const string& seperator) {
	strClient Client;
	vector<string> vClientData;
	vClientData = splitString(Line, seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);//cast string to double
	return Client;
}
// Convert a User struct to a single line for file
string convertUserRecordToLine(const strUser& userInfo, const string& separator = "#//#") {
	string line = "";
	line += userInfo.UserName + separator;
	line += userInfo.Password + separator;
	line += to_string(userInfo.Permissions);
	return line;
}

// Convert a line from file into a User struct
strUser convertLinetoUserInfo(string Line, const string& seperator) {
	strUser userInfo;
	vector<string> vUsersData;
	vUsersData = splitString(Line, seperator);
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
			Client = convertLinetoRecord(Line, Separator);
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
			User = convertLinetoUserInfo(Line, Separator);
			vUsers.push_back(User);
		}
		myFile.close();
	}
	return vUsers;
}
// Save all clients to file, skip those marked for deletion
void SaveClientsDataToFile(string FileName, const vector<strClient>& vClients) {
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
void SaveUsersDataToFile(string FileName, const vector<strUser>& vUsers) {
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
void addDataLineToFile(const string& FileName, const string& stDataLine) {
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open()) {
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

// Read a non-empty string input
string readLine(string s) {
	string line;
	do {
		cout << s;
		getline(cin >> ws, line);
	} while (line.empty());
	return line;
}
// Read a positive double input
double readPositiveDouble(string s) {
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
strClient changeClientRecord(const string& AccountNumber) {
	strClient Client;
	Client.AccountNumber = AccountNumber;
	Client.PinCode = readLine("Enter PinCode? ");
	Client.Name = readLine("Enter Name? ");
	Client.Phone = readLine("Enter Phone? ");
	Client.AccountBalance = readPositiveDouble("Enter AccountBalance? ");
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
	ShowMainMenue = 4
};
enum ManageUsersOptions {
	ListUser = 1,
	AddNewUser = 2,
	DeleteUser = 3,
	UpdateUser = 4,
	FindUser = 5,
	MainMenue = 6
};
enum enPermissions {
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

// Search for a client by account number, return pointer to client if found
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
bool areYouSure(string s) {
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
void ShowAllClientsScreen(const vector<strClient>& vClients) {
	clearScreen();
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_";
	cout << "_\n" << endl;

	cout << "|" << setw(15) << left << "AccountNumber";
	cout << "|" << setw(10) << left << "PinCode";
	cout << "|" << setw(40) << left << "Name";
	cout << "|" << setw(12) << left << "Phone";
	cout << "|" << setw(12) << left << "AccountBalance";
	cout << "\n_";
	cout << "_\n" << endl;
	for (const strClient& Client : vClients) {
		printClientRecordLine(Client);
		cout << endl;
	}
	cout << "\n_";
	cout << "_\n" << endl;
}

// Add client with unique account number
void addnewClient(vector<strClient>& vClients) {
	strClient newClient;
	string accountNumber = readLine("\nPlease enter AccountNumber? ");
	strClient* findClient = findClientByAccountNumber(accountNumber, vClients);

	while (findClient != nullptr) {
		cout << "Client with accountNumber [" << accountNumber << "] already Exits, enter another account number?\n";
		accountNumber = readLine("\nPlease enter AccountNumber? ");
		findClient = findClientByAccountNumber(accountNumber, vClients);
	}

	newClient = changeClientRecord(accountNumber);
	vClients.push_back(newClient);
	addDataLineToFile(ClientsFileName, convertClientRecordToLine(newClient, Separator));
	cout << "Client Added Successfully!";
}
// Show Add New Client screen
void showAddNewClient(vector <strClient>& vClients) {
	showScreenHeader("Add New Client Screen");
	cout << "Adding New Client :\n";

	char AddMore = 'Y';
	do {
		addnewClient(vClients);
		cout << " ,do you want to add more clients? Y/N? ";

		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');

}

// Delete a client by account number
bool deleteClientByAccountNumber(const string& accountNumber, vector<strClient>& vClients) {
	strClient* client = findClientByAccountNumber(accountNumber, vClients);
	if (!client) {
		cout << "\nClient with Account Number (" << accountNumber << ") is not Found!";
		return false;
	}

	printClientCard(*client);
	if (areYouSure("Are you sure you want delete this client ?")) {
		markClientForDelete(client);
		SaveClientsDataToFile(ClientsFileName, vClients);
		vClients = loadClientsDataFromFile(ClientsFileName);
		cout << "\n\nClient Deleted Successfully.";
		return true;
	}
	return false;
}
// Show Delete Client screen
void showDeleteClientScreen(vector<strClient>& vClients) {
	showScreenHeader("Delete Client Screen");
	string accountNumber = readLine("\nPlease enter AccountNumber? ");
	deleteClientByAccountNumber(accountNumber, vClients);
}

// Update client information by account number
bool updateClientByAccountNumber(const string& accountNumber, vector<strClient>& vClients) {
	strClient* client = findClientByAccountNumber(accountNumber, vClients);
	if (!client) {
		cout << "\nClient with Account Number (" << accountNumber << ") is not Found!";
		return false;
	}

	printClientCard(*client);
	if (areYouSure("Are you sure you want update this client ?")) {
		*client = changeClientRecord(accountNumber);
		SaveClientsDataToFile(ClientsFileName, vClients);
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
		string accountNumber = readLine("\nPlease enter AccountNumber? ");
		success = updateClientByAccountNumber(accountNumber, vClients);
	} while (!success);

}

// Show Find Client screen and display client details
void ShowFindClientScreen(vector<strClient>& vClients) {
	showScreenHeader("Find Client Screen");

	string accountNumber = readLine("\nPlease enter AccountNumber? ");
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


// Read choice from menu (form "from" to "to")
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
	string accountNumber = readLine("\nPlease enter AccountNumber? ");
	strClient* client = findClientByAccountNumber(accountNumber, vClients);
	if (!client) {
		cout << "\nClient with Account Number (" << accountNumber << ") is not Found!";
		return;
	}
	printClientCard(*client);
	double depositAmount = readPositiveDouble("Enter Deposit Amount: ");
	if (depositBalanceToClient(client, depositAmount)) {
		SaveClientsDataToFile(ClientsFileName, vClients);
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
	string accountNumber = readLine("\nPlease enter AccountNumber? ");
	strClient* client = findClientByAccountNumber(accountNumber, vClients);
	if (!client) {
		cout << "\nClient with Account Number (" << accountNumber << ") is not Found!";
		return;
	}
	printClientCard(*client);
	double withdrawAmount = readPositiveDouble("Enter Withdraw Amount: ");
	while (withdrawAmount > client->AccountBalance) {
		cout << "Amount Exceed the balance, you can Withdraw up to " << client->AccountBalance << ".\n";
		withdrawAmount = readPositiveDouble("Please enter another withdraw amount?");
	}

	if (withdrawBalanceToClient(client, withdrawAmount)) {
		SaveClientsDataToFile(ClientsFileName, vClients);
		vClients = loadClientsDataFromFile(ClientsFileName);
	}
}

void printBalanceClientLine(const strClient& client) {
	cout << "| " << setw(15) << left << client.AccountNumber;
	cout << "| " << setw(40) << left << client.Name;
	cout << "| " << setw(35) << left << client.AccountBalance;
}
void ShowTotalBalancesScreen(const vector <strClient>& vClients) {
	clearScreen();
	double totalBalance = 0;
	cout << "\n\t\t\t\t\t Balance List (" << vClients.size() << ") Client(s).";
	cout << "\n_";
	cout << "_\n" << endl;

	cout << "| " << setw(15) << left << "AccountNumber";
	cout << "| " << setw(40) << left << "Name";
	cout << "| " << setw(12) << left << "AccountBalance";
	cout << "\n_";
	cout << "_\n" << endl;

	if (vClients.size() == 0) {
		cout << "\t\t\t\t No Client Available In The System!.\n";
	}

	for (const strClient& Client : vClients) {
		totalBalance += Client.AccountBalance;
		printBalanceClientLine(Client);
		cout << endl;
	}
	cout << "\n_";
	cout << "_\n" << endl;
	cout << "\n\t\t\t\t\tTotal Balance = " << totalBalance << endl;

}

void performTransactionsMenuOption(TransactionsOptions TransactionMenueOption, vector<strClient>& vClients)
{
	switch (TransactionMenueOption)
	{
	case TransactionsOptions::Deposit:
	{
		clearScreen();
		showDepositScreen(vClients);
		goBackToTransactionsMenu(vClients);
		break;
	}

	case TransactionsOptions::Withdraw:
	{
		clearScreen();
		showWithdrawScreen(vClients);
		goBackToTransactionsMenu(vClients);
		break;
	}


	case TransactionsOptions::ShowTotalBalance:
	{
		clearScreen();
		ShowTotalBalancesScreen(vClients);
		goBackToTransactionsMenu(vClients);
		break;
	}


	case TransactionsOptions::ShowMainMenue:
	{

		break;
	}
	}

}
void showTransactionsMenuScreen() {
	clearScreen();
	showScreenHeader("Transactions Menue Screen");
	vector<string> options = { "Deposit","Withdraw","Total Balances","Main Menue" };
	showOptions(options);
}
void ManageTransactions(vector<strClient>& vClients) {
	TransactionsOptions Choice;
	do {
		showTransactionsMenuScreen();
		Choice = (TransactionsOptions)readOption(1, 4);
		performTransactionsMenuOption(Choice, vClients);
	} while (Choice != TransactionsOptions::ShowMainMenue);
}
//========================================================================


int readPermissionsToSet()
{
	int Permissions = 0;

	if (areYouSure("Do you want to give full access? "))
		return enPermissions::pAll;
	cout << "\nDo you want to give access to : \n ";
	if (areYouSure("Show Client List? "))
		Permissions += enPermissions::pListClients;
	if (areYouSure("Add New Client? "))
		Permissions += enPermissions::pAddClient;
	if (areYouSure("Delete Client? "))
		Permissions += enPermissions::pDeleteClient;
	if (areYouSure("Update Client? "))
		Permissions += enPermissions::pUpdateClient;
	if (areYouSure("Find Client? "))
		Permissions += enPermissions::pFindClient;
	if (areYouSure("Transactions? "))
		Permissions += enPermissions::pTransactions;
	if (areYouSure("Manage Users? "))
		Permissions += enPermissions::pManageUsers;

	if (Permissions == enPermissions::pAllPermissions)
		return enPermissions::pAll;

	return Permissions;

}

strUser* findUserByUserName(const string& userName, vector<strUser>& vUsers) {
	for (auto& user : vUsers) {
		if (user.UserName == userName)
			return &user;
	}
	return nullptr;
}
strUser changeUserInfo(const string& userName) {
	strUser user;
	user.UserName = userName;
	user.Password = readLine("Please Enter Password? ");
	user.Permissions = readPermissionsToSet();
	return user;
}
bool findUsersByUserNameAndPassword(const string& password, strUser* User) {
	if (User == nullptr)
		return false;
	else
		return (User->Password == password);
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
	cout << "\n_";
	cout << "_\n" << endl;
	cout << "|" << setw(25) << left << "UserName";
	cout << "|" << setw(15) << left << "Password";
	cout << "|" << setw(15) << left << "Permissions";
	cout << "\n_";
	cout << "_\n" << endl;
	for (const strUser& user : vUsers) {
		printUserLine(user);
		cout << endl;
	}
	cout << "\n_";
	cout << "_\n" << endl;
}

void addNewUser(vector<strUser>& vUsers) {
	clearScreen();
	showScreenHeader("Add New User Screen");

	string name = readLine("Please Enter UserName?");
	strUser* existingUser = findUserByUserName(name, vUsers);

	while (existingUser != nullptr) {
		cout << "User with name [" << name << "] already Exits, enter a different username?\n";
		name = readLine("Please Enter UserName?");
		existingUser = findUserByUserName(name, vUsers);
	}

	strUser newUser = changeUserInfo(name);
	vUsers.push_back(newUser);
	addDataLineToFile(UsersFileName, convertUserRecordToLine(newUser, Separator));

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

	if (findUsersByUserNameAndPassword(password, user)) {
		printUserCard(user);
		if (areYouSure("Are you sure you want delete this user ?")) {
			markUserForDelete(user);
			SaveUsersDataToFile(UsersFileName, vUsers);
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

	string name = readLine("Please Enter UserName?");
	string password = readLine("Please Enter Password? ");
	deleteUserByNameAndPassword(name, password, vUsers);
}

void showFindUserScreen(vector<strUser>& vUsers) {
	clearScreen();
	showScreenHeader("Find User Screen");

	string name = readLine("Please Enter UserName?");
	strUser* user = findUserByUserName(name, vUsers);
	string password = readLine("Please Enter Password? ");
	if (findUsersByUserNameAndPassword(password, user)) {
		printUserCard(user);
	}
	else {
		cout << "\nUser with name (" + name + ") and password (" + password + ") is not found!.\n";
	}
}


bool updateUserByNameAndPassword(const string& userName, const string& password, vector<strUser>& vUsers) {
	strUser* user = findUserByUserName(userName, vUsers);
	if (findUsersByUserNameAndPassword(password, user)) {
		printUserCard(user);

		if (areYouSure("Are you sure you want update this User ?")) {
			for (strUser& u : vUsers) {
				if (u.UserName == userName && u.Password == password) {
					u = changeUserInfo(userName);
					break;
				}
			}
			SaveUsersDataToFile(UsersFileName, vUsers);
			cout << "\n\nUser Updated Successfully.";
			return true;
		}
		return false;
	}
	else {
		cout << "\nUser with name (" + userName + ") and password (" + password + ") is not found!.\n";
		return false;
	}
}

void showUpdateUserScreen(vector<strUser>& vUsers) {
	clearScreen();
	showScreenHeader("Update User Screen");

	bool success = false;
	do {
		string name = readLine("Please Enter UserName?");
		string password = readLine("Please Enter Password? ");
		success = updateUserByNameAndPassword(name, password, vUsers);
	} while (!success);
}

void performManageUsersOption(ManageUsersOptions manageUsersOptions, vector <strUser>& vUsers) {
	switch (manageUsersOptions) {
	case ManageUsersOptions::ListUser:
		ShowAllUsersScreen(vUsers);
		goBackToManageUsers();
		break;
	case ManageUsersOptions::AddNewUser: {
		showAddNewUser(vUsers);
		goBackToManageUsers();
		break;
	}
	case ManageUsersOptions::DeleteUser:
		showDeleteUserScreen(vUsers);
		goBackToManageUsers();
		break;
	case ManageUsersOptions::FindUser:
		showFindUserScreen(vUsers);
		goBackToManageUsers();
		break;
	case ManageUsersOptions::UpdateUser:
		showUpdateUserScreen(vUsers);
		goBackToManageUsers();
		break;
	case ManageUsersOptions::MainMenue:
		goBackToMainMenu();
		return;


	}
}

strUser CurrentUser;
bool checkAccessPermission(enPermissions permission) {
	if (CurrentUser.Permissions == enPermissions::pAll)
		return true;

	return (CurrentUser.Permissions & permission) == permission;
}

void manageUsersMenu(vector<strUser>& vUsers) {
	ManageUsersOptions choice;
	do
	{
		showManageUsersScreen();
		choice = (ManageUsersOptions)readOption(1, 6);
		if (choice != ManageUsersOptions::MainMenue)
			performManageUsersOption(choice, vUsers);

	} while (choice != ManageUsersOptions::MainMenue);
}
// Execute main menu option


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
	if (checkAccessPermission(enPermissions::pAll) || checkAccessPermission(enPermissions::pAllPermissions))
		options = { "Show Client List","Add New Client","Delete Client","Update Client","Find Client","Transactions","Manage Users" };
	else {
		if (checkAccessPermission(enPermissions::pListClients))
			options.push_back("Show Client List");
		if (checkAccessPermission(enPermissions::pAddClient))
			options.push_back("Add New Client");
		if (checkAccessPermission(enPermissions::pDeleteClient))
			options.push_back("Delete Client");
		if (checkAccessPermission(enPermissions::pUpdateClient))
			options.push_back("Update Client");
		if (checkAccessPermission(enPermissions::pFindClient))
			options.push_back("Find Client");
		if (checkAccessPermission(enPermissions::pTransactions))
			options.push_back("Transactions");
		if (checkAccessPermission(enPermissions::pManageUsers))
			options.push_back("Manage Users");
	}
	options.push_back("Logout");
	options.push_back("Exit");

	return options;
}

void performMainMenuOption(MainMenuOption MainMenuOption, vector<strClient>& vClients) {
	clearScreen();
	bool hasPermission = true;
	switch (MainMenuOption) {
	case MainMenuOption::ShowClientList:
		hasPermission = checkAccessPermission(enPermissions::pListClients);
		if (hasPermission) {
			ShowAllClientsScreen(vClients);
			goBackToMainMenu();
		}
		break;
	case MainMenuOption::AddNewClient:
		hasPermission = checkAccessPermission(enPermissions::pAddClient);
		if (hasPermission) {
			showAddNewClient(vClients);
			goBackToMainMenu();
		}
		break;
	case MainMenuOption::DeleteClient:
		hasPermission = checkAccessPermission(enPermissions::pDeleteClient);
		if (hasPermission) {
			showDeleteClientScreen(vClients);
			goBackToMainMenu();
		}
		break;
	case MainMenuOption::UpdateClient:
		hasPermission = checkAccessPermission(enPermissions::pUpdateClient);
		if (hasPermission) {
			showUpdateClientScreen(vClients);
			goBackToMainMenu();
		}
		break;
	case MainMenuOption::FindClient:
		hasPermission = checkAccessPermission(enPermissions::pFindClient);
		if (hasPermission) {
			ShowFindClientScreen(vClients);
			goBackToMainMenu();
		}
		break;
	case MainMenuOption::Transactions:
		hasPermission = checkAccessPermission(enPermissions::pTransactions);
		if (hasPermission) {
			ManageTransactions(vClients);
		}
		break;
	case MainMenuOption::ManageUsers: {
		hasPermission = checkAccessPermission(enPermissions::pManageUsers);
		if (hasPermission) {
			vector<strUser> vUsers = loadUsersDataFromFile(UsersFileName);
			manageUsersMenu(vUsers);
			SaveUsersDataToFile(UsersFileName, vUsers);
			goBackToMainMenu();
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
		goBackToMainMenu();
	}
}
// Main loop: show menu, execute options, repeat until exit
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
		performMainMenuOption(Choice, vClients);

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
		string name = readLine("Please Enter UserName?");
		string password = readLine("Please Enter Password? ");

		strUser* user = findUserByUserName(name, vUsers);
		found = findUsersByUserNameAndPassword(password, user);

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
