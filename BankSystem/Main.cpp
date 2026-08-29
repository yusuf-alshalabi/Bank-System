#include <iostream>
#include "Core/BankClient.h"
#include "../Libs/Cpp-Library-Collection/Lib/InputValidate.h" 

using namespace std;

void ReadClientInfo(BankClient& Client)
{
    Client.FirstName = Core::InputValidate::ReadString("\nEnter FirstName: ");

    Client.LastName = Core::InputValidate::ReadString("\nEnter LastName: ");

    Client.Email = Core::InputValidate::ReadString("\nEnter Email: ");

    Client.Phone = Core::InputValidate::ReadString("\nEnter Phone: ");

    Client.PinCode = Core::InputValidate::ReadString("\nEnter PinCode: ");

    Client.AccountBalance = Core::InputValidate::ReadNumber<double>("\nEnter Account Balance: ");
}

void UpdateClient()
{
    string AccountNumber = "";

    cout << "\nPlease Enter client Account Number: ";
    AccountNumber = Core::InputValidate::ReadString();

    while (!BankClient::IsClientExist(AccountNumber))
    {
        cout << "\nAccount number is not found, choose another one: ";
        AccountNumber = Core::InputValidate::ReadString();
    }

    BankClient Client1 = BankClient::Find(AccountNumber);
    Client1.Print();

    cout << "\n\nUpdate Client Info:";
    cout << "\n____________________\n";

    ReadClientInfo(Client1);

    BankClient::enSaveResults SaveResult;
    SaveResult = Client1.Save();

    switch (SaveResult)
    {
    case BankClient::enSaveResults::svSucceeded:
    {
        cout << "\nAccount Updated Successfully :-)\n";
        Client1.Print();
        break;
    }
    case BankClient::enSaveResults::svFailedEmptyObject:
    {
        cout << "\nError account was not saved because it's Empty\n";
        break;
    }
    }
}

void AddNewClient()
{
    string AccountNumber = "";

    AccountNumber = Core::InputValidate::ReadString("\nPlease Enter Account Number: ");
    while (BankClient::IsClientExist(AccountNumber))
    {
        AccountNumber = Core::InputValidate::ReadString("\nAccount Number Is Already Used, Choose another one: ");
    }

    BankClient NewClient = BankClient::GetAddNewClientObject(AccountNumber);


    ReadClientInfo(NewClient);

    BankClient::enSaveResults SaveResult;

    SaveResult = NewClient.Save();

    switch (SaveResult)
    {
    case  BankClient::enSaveResults::svSucceeded:
    {
        cout << "\nAccount Addeded Successfully :-)\n";
        NewClient.Print();
        break;
    }
    case BankClient::enSaveResults::svFailedEmptyObject:
    {
        cout << "\nError account was not saved because it's Empty";
        break;

    }
    case BankClient::enSaveResults::svFaildAccountNumberExists:
    {
        cout << "\nError account was not saved because account number is used!\n";
        break;

    }
    }
}

void DeleteClient()
{
    string AccountNumber = "";

    AccountNumber = Core::InputValidate::ReadString("\nPlease Enter Account Number: ");
    while (!BankClient::IsClientExist(AccountNumber))
    {
        AccountNumber = Core::InputValidate::ReadString("\nAccount number is not found, choose another one: ");
    }

    BankClient Client1 = BankClient::Find(AccountNumber);
    Client1.Print();

    if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to delete this client?"))
    {
        if (Client1.Delete())
        {
            cout << "\nClient Deleted Successfully :-)\n";
            Client1.Print();
        }
        else
        {
            cout << "\nError Client Was not Deleted\n";
        }
    }
}

void PrintClientRecordLine(const BankClient& Client)
{

    cout << "| " << setw(15) << left << Client.GetAccountNumber();
    cout << "| " << setw(20) << left << Client.FullName();
    cout << "| " << setw(12) << left << Client.GetPhone();
    cout << "| " << setw(20) << left << Client.GetEmail();
    cout << "| " << setw(10) << left << Client.GetPinCode();
    cout << "| " << setw(12) << left << Client.GetAccountBalance();

}

void ShowClientsList()
{

    std::vector<BankClient> vClients = BankClient::GetClientsList();

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << std::endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(20) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(20) << "Email";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << std::endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (BankClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            std::cout << endl;
        }

    std::cout << "\n_______________________________________________________";
    std::cout << "_________________________________________\n" << endl;

}

int main()
{
    ShowClientsList();

    system("pause>0");
    return 0;
}