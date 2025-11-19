
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

const string ClientsFileName = "Clients.txt";
const string Separator = "#//#";
struct strClient {
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

void showMainMenuScreen();
void ManageTransactions(vector<strClient>& vCleints);
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
	showMainMenuScreen();
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
	Exit = 7
};

enum TransactionsOptions { 
	Deposit = 1, 
	Withdraw = 2,
	ShowTotalBalance = 3,
	ShowMainMenue = 4 
};

void showScreenHeader(const string& title) {
	clearScreen();
	cout << "\n-------------------------------------------------\n";
	cout << "\t\t" << title << "\n";
	cout << "-------------------------------------------------\n";
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
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "|" << setw(15) << left << "AccountNumber";
	cout << "|" << setw(10) << left << "PinCode";
	cout << "|" << setw(40) << left << "Name";
	cout << "|" << setw(12) << left << "Phone";
	cout << "|" << setw(12) << left << "AccountBalance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (const strClient& Client : vClients) {
		printClientRecordLine(Client);
		cout << endl;
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
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

bool depositBalanceToClient(strClient* client, double depositAmount) {
	if (client == nullptr) {
		return false;
	}
	client->AccountBalance +=  depositAmount;
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
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << setw(15) << left << "AccountNumber";
	cout << "| " << setw(40) << left << "Name";
	cout << "| " << setw(12) << left << "AccountBalance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0) {
		cout << "\t\t\t\t No Client Available In The System!.\n";
	}

	for (const strClient& Client : vClients) {
		totalBalance += Client.AccountBalance;
		printBalanceClientLine(Client);
		cout << endl;
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "\n\t\t\t\t\tTotal Balance = " << totalBalance << endl;

}

void PerfromTranactionsMenuOption(TransactionsOptions TransactionMenueOption, vector<strClient>& vClients)
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
		showMainMenuScreen();
		break;
	}
	}

}
void showTransactionsMenuScreen() {
	clearScreen();
	cout << "===========================================\n";
	cout << "\t\tTransactions Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "===========================================\n";
}
TransactionsOptions readTransactionsMenuOption() {
	showTransactionsMenuScreen();
	int choice;
	do {
		cout << "Choose what do you want to do? [1 to 4]? ";
		cin >> choice;
		while (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Invalid Number, Enter a valid one: [1 to 4]? " << endl;
			cin >> choice;
		}
	} while ((choice < 1) || (choice > 4));

	return (TransactionsOptions)choice;

}
void ManageTransactions(vector<strClient>& vClients) {
	TransactionsOptions Choice;
	do {
		Choice = readTransactionsMenuOption();
		if (Choice != TransactionsOptions::ShowMainMenue) {
			PerfromTranactionsMenuOption(Choice, vClients);
		}
	} while (Choice != TransactionsOptions::ShowMainMenue);
}

// Read user's choice from main menu (1 to 7)
MainMenuOption readMainMenuOption() {
	showMainMenuScreen();
	int choice;
	do {
		cout << "Choose what do you want to do? [1 to 7]? ";
		cin >> choice;
		while (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Invalid Number, Enter a valid one: [1 to 7]? " << endl;
			cin >> choice;
		}
	} while ((choice < 1) || (choice > 7));

	return (MainMenuOption)choice;

}
// Execute main menu option
void performMainMenuOption(MainMenuOption MainMenuOption, vector<strClient>& vClients) {
	clearScreen();
	switch (MainMenuOption) {
	case MainMenuOption::ShowClientList:
		ShowAllClientsScreen(vClients);
		goBackToMainMenu();
		break;
	case MainMenuOption::AddNewClient:
		showAddNewClient(vClients);
		goBackToMainMenu();
		break;
	case MainMenuOption::DeleteClient:
		showDeleteClientScreen(vClients);
		goBackToMainMenu();
		break;
	case MainMenuOption::UpdateClient:
		showUpdateClientScreen(vClients);
		goBackToMainMenu();
		break;
	case MainMenuOption::FindClient:
		ShowFindClientScreen(vClients);
		goBackToMainMenu();
		break;
	case MainMenuOption::Transactions:
		ManageTransactions(vClients);
		break;
	case MainMenuOption::Exit:
		showExitClient();
		break;
	default:
		cout << "\nInvalid option.\n";
		break;
	}
}
// Main loop: show menu, execute options, repeat until exit
void ManageMainMenu(vector<strClient>& vClients) {
	MainMenuOption Choice;
	do {
		Choice = readMainMenuOption();
		if (Choice == MainMenuOption::Exit) {
			showExitClient();
			break;
		}
		performMainMenuOption(Choice, vClients);

	} while (Choice != MainMenuOption::Exit);

}
// Display main menu options
void showMainMenuScreen() {
	clearScreen();
	cout << "\n===========================================\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "===========================================\n";

}

int main()
{
	cout << fixed << setprecision(2);
	vector<strClient> vClients = loadClientsDataFromFile(ClientsFileName);
	ManageMainMenu(vClients);

	customPause();
	return 0;

}
