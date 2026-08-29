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


int main()
{
    DeleteClient();

    system("pause>0");
    return 0;
}