#include <iostream>
#include "Core/BankClient.h"
#include "../Libs/Cpp-Library-Collection/Lib/InputValidate.h" 


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
    std::string AccountNumber = "";

    std::cout << "\nPlease Enter client Account Number: ";
    AccountNumber = Core::InputValidate::ReadString();

    while (!BankClient::IsClientExist(AccountNumber))
    {
        std::cout << "\nAccount number is not found, choose another one: ";
        AccountNumber = Core::InputValidate::ReadString();
    }

    BankClient Client1 = BankClient::Find(AccountNumber);
    Client1.Print();

    std::cout << "\n\nUpdate Client Info:";
    std::cout << "\n____________________\n";

    ReadClientInfo(Client1);

    BankClient::enSaveResults SaveResult;
    SaveResult = Client1.Save();

    switch (SaveResult)
    {
    case BankClient::enSaveResults::svSucceeded:
    {
        std::cout << "\nAccount Updated Successfully :-)\n";
        Client1.Print();
        break;
    }
    case BankClient::enSaveResults::svFailedEmptyObject:
    {
        std::cout << "\nError account was not saved because it's Empty\n";
        break;
    }
    }
}

void AddNewClient()
{
    std::string AccountNumber = "";

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
        std::cout << "\nAccount Addeded Successfully :-)\n";
        NewClient.Print();
        break;
    }
    case BankClient::enSaveResults::svFailedEmptyObject:
    {
        std::cout << "\nError account was not saved because it's Empty";
        break;

    }
    case BankClient::enSaveResults::svFaildAccountNumberExists:
    {
        std::cout << "\nError account was not saved because account number is used!\n";
        break;

    }
    }
}

void DeleteClient()
{
    std::string AccountNumber = "";

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
            std::cout << "\nClient Deleted Successfully :-)\n";
            Client1.Print();
        }
        else
        {
            std::cout << "\nError Client Was not Deleted\n";
        }
    }
}

void PrintClientRecordLine(const BankClient& Client)
{

    std::cout << "| " << std::setw(15) << std::left << Client.GetAccountNumber();
    std::cout << "| " << std::setw(20) << std::left << Client.FullName();
    std::cout << "| " << std::setw(12) << std::left << Client.GetPhone();
    std::cout << "| " << std::setw(20) << std::left << Client.GetEmail();
    std::cout << "| " << std::setw(10) << std::left << Client.GetPinCode();
    std::cout << "| " << std::setw(12) << std::left << Client.GetAccountBalance();

}

void ShowClientsList()
{

    std::vector<BankClient> vClients = BankClient::GetClientsList();

    std::cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    std::cout << "\n_______________________________________________________";
    std::cout << "_________________________________________\n" << std::endl;
    std::cout << "| " << std::left << std::setw(15) << "Accout Number";
    std::cout << "| " << std::left << std::setw(20) << "Client Name";
    std::cout << "| " << std::left << std::setw(12) << "Phone";
    std::cout << "| " << std::left << std::setw(20) << "Email";
    std::cout << "| " << std::left << std::setw(10) << "Pin Code";
    std::cout << "| " << std::left << std::setw(12) << "Balance";
    std::cout << "\n_______________________________________________________";
    std::cout << "_________________________________________\n" << std::endl;

    if (vClients.size() == 0)
        std::cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (const BankClient& Client : vClients)
        {

            PrintClientRecordLine(Client);
            std::cout << std::endl;
        }

    std::cout << "\n_______________________________________________________";
    std::cout << "_________________________________________\n" << std::endl;

}

int main()
{
    ShowClientsList();

    system("pause>0");
    return 0;
}