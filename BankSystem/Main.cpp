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

int main()
{
    UpdateClient();

    system("pause>0");
    return 0;
}