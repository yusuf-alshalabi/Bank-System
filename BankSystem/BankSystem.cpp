
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

const string ClientsFileName = "Clients.txt";
const string Seperator = "#//#";
struct strClient {
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

void showMainMenuScreen();
void customPause() {
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cin.get();
}
void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}
void goBackToMainMenu() {
	cout << "\n\nPress any key to go back to Main Menu...";
	customPause();
	showMainMenuScreen();
}
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
string convertClientRecordToLine(const strClient& clientData, const string& seperator) {
	string Line = "";
	Line += clientData.AccountNumber + seperator;
	Line += clientData.PinCode + seperator;
	Line += clientData.Name + seperator;
	Line += clientData.Phone + seperator;
	Line += to_string(clientData.AccountBalance);

	return Line;

}
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
vector<strClient> loadClientsDataFromFile(const string& fileName) {
	vector <strClient> vClients;
	fstream myFile;
	myFile.open(fileName, ios::in); //read only.
	if (myFile.is_open()) {
		string Line;
		strClient Client;
		while (getline(myFile, Line)) {
			Client = convertLinetoRecord(Line, Seperator);
			vClients.push_back(Client);
		}
		myFile.close();
	}
	return vClients;
}
void SaveClientsDataToFile(string FileName, const vector<strClient>& vClients) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);//over write
	string DataLine;
	if (MyFile.is_open()) {
		for (strClient c : vClients) {
			if (c.MarkForDelete == false) {
				//we only write records that are not marked for delete.
				DataLine = convertClientRecordToLine(c, Seperator);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
}
void addDataLineToFile(const string& FileName, const string& stDataLine) {
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open()) {
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}
string readLine(string s) {
	string line;
	do {
		cout << s;
		getline(cin >> ws, line);
	} while (line.empty());
	return line;
}
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

strClient changeClientRecord(const string& AccountNumber) {
	strClient Client;
	Client.AccountNumber = AccountNumber;
	Client.PinCode = readLine("Enter PinCode? ");
	Client.Name = readLine("Enter Name? ");
	Client.Phone = readLine("Enter Phone? ");
	Client.AccountBalance = readPositiveDouble("Enter AccountBalance? ");
	return Client;
}

enum enMainMenuOptions {
	ShowClientList = 1,
	AddNewClient = 2,
	DeleteClient = 3,
	UpdateClient = 4,
	FindClient = 5,
	Exit = 6
};

void printClientRecordLine(const strClient& client) {
	cout << "|" << setw(15) << left << client.AccountNumber;
	cout << "|" << setw(10) << left << client.PinCode;
	cout << "|" << setw(40) << left << client.Name;
	cout << "|" << setw(12) << left << client.Phone;
	cout << "|" << setw(12) << left << client.AccountBalance;
}
void printClientCard(const strClient& Client) {
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccout Number  : " << Client.AccountNumber;
	cout << "\nPin Code       : " << Client.PinCode;
	cout << "\nName           : " << Client.Name;
	cout << "\nPhone          : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";

}

bool findClientByAccountNumber(const string& accountNumber, const vector<strClient>& vClient, strClient& client) {
	for (const strClient& c : vClient) {
		if (c.AccountNumber == accountNumber) {
			client = c;
			return true;
		}
	}
	return false;
}
bool markClientForDeleteByAccountNumber(string AccountNumber, vector <strClient>& vClients) {
	for (strClient& c : vClients) {
		if (AccountNumber == c.AccountNumber) {
			c.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
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

void showScreenHeader(const string& title) {
	clearScreen();
	cout << "\n-------------------------------------------------\n";
	cout << "\t\t" << title << "\n";
	cout << "-------------------------------------------------\n";
}

void addnewClient(vector<strClient>& vClients) {
	strClient newClient;
	string accountNumber = readLine("\nPlease enter AccountNumber? ");

	while (findClientByAccountNumber(accountNumber, vClients, newClient)) {
		cout << "Client with accountNumber [" << accountNumber << "] already Exits, enter another account number?\n";
		accountNumber = readLine("\nPlease enter AccountNumber? ");
	}

	newClient = changeClientRecord(accountNumber);
	vClients.push_back(newClient);
	addDataLineToFile(ClientsFileName, convertClientRecordToLine(newClient, Seperator));
	cout << "Client Added Successfully!";

}
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

bool deleteClientByAccountNumber(const string accountNumber, vector<strClient>& vClients) {
	strClient client;
	if (findClientByAccountNumber(accountNumber, vClients, client)) {
		printClientCard(client);
		if (areYouSure("Are you sure you want delete this client ?")) {
			markClientForDeleteByAccountNumber(accountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);
			vClients = loadClientsDataFromFile(ClientsFileName);
			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
		else {
			return false;
		}
	}
	else {
		cout << "\nClient with Account Number (" << accountNumber << ") is not Found!";
		return false;
	}
}
void showDeleteClientScreen(vector<strClient>& vClients) {
	showScreenHeader("Delete Client Screen");
	string accountNumber = readLine("\nPlease enter AccountNumber? ");
	deleteClientByAccountNumber(accountNumber, vClients);
}

bool updateClientByAccountNumber(string accountNumber, vector<strClient>& vClients) {
	strClient client;
	if (findClientByAccountNumber(accountNumber, vClients, client)) {
		printClientCard(client);

		if (areYouSure("Are you sure you want update this client ?")) {
			for (strClient& C : vClients) {
				if (C.AccountNumber == accountNumber) {
					C = changeClientRecord(accountNumber);
					break;
				}
			}
			SaveClientsDataToFile(ClientsFileName, vClients);
			cout << "\n\nClient Updated Successfully.";
			return true;
		}
		return false;
	}
	else {
		cout << "\nClient with Account Number (" << accountNumber << ") is not Found!";
		return false;
	}

}
void showUpdateClientScreen(vector<strClient>& vClients) {
	showScreenHeader("Update Info Client Screen");

	bool success = false;
	do {
		string accountNumber = readLine("\nPlease enter AccountNumber? ");
		success = updateClientByAccountNumber(accountNumber, vClients);
	} while (!success);

}

void ShowFindClientScreen(const vector <strClient>& vClients) {
	showScreenHeader("Find Client Screen");

	strClient client;
	string accountNumber = readLine("\nPlease enter AccountNumber? ");
	if (findClientByAccountNumber(accountNumber, vClients, client)) {
		printClientCard(client);
	}
	else {
		cout << "Client with account Number [" << accountNumber << "] is not found!\n";
	}

}

void showExitClient() {
	clearScreen();

	cout << "\n-------------------------------------------------\n";
	cout << "\t\tProgram Ends :-)\n";
	cout << "-------------------------------------------------\n";

}
enMainMenuOptions readMainMenuOption() {
	showMainMenuScreen();
	int choice;
	do {
		cout << "Choose what do you want to do? [1 to 6]? ";
		cin >> choice;
		while (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Invalid Number, Enter a valid one: [1 to 6]? " << endl;
			cin >> choice;
		}
	} while ((choice < 1) || (choice > 6));

	return (enMainMenuOptions)choice;

}
void performMainMenuOption(enMainMenuOptions MainMenuOption, vector<strClient>& vClients) {
	clearScreen();
	switch (MainMenuOption) {
	case enMainMenuOptions::ShowClientList:
		ShowAllClientsScreen(vClients);
		goBackToMainMenu();
		break;
	case enMainMenuOptions::AddNewClient:
		showAddNewClient(vClients);
		goBackToMainMenu();
		break;
	case enMainMenuOptions::DeleteClient:
		showDeleteClientScreen(vClients);
		goBackToMainMenu();
		break;
	case enMainMenuOptions::UpdateClient:
		showUpdateClientScreen(vClients);
		goBackToMainMenu();
		break;
	case enMainMenuOptions::FindClient:
		ShowFindClientScreen(vClients);
		goBackToMainMenu();
		break;
	case enMainMenuOptions::Exit:
		showExitClient();
		break;
	default:
		cout << "\nInvalid option.\n";
		break;
	}
}
void ManageMainMenu(vector<strClient>& vClients) {
	enMainMenuOptions Choice;
	do {
		Choice = readMainMenuOption();
		if (Choice == enMainMenuOptions::Exit) {
			showExitClient();
			break;
		}
		performMainMenuOption(Choice, vClients);

	} while (Choice != enMainMenuOptions::Exit);

}
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
	cout << "\t[6] Exit.\n";
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
