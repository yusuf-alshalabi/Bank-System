#pragma once


#include <iostream>
#include "Screen.h"
#include "../Core/BankClient.h"
#include "../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include <vector>

class AddNewClientScreen : protected Screen
{
private:
    static void _ReadClientInfo(BankClient& Client)
    {
        std::cout << "\nEnter FirstName: ";
        Client.FirstName = Core::InputValidate::ReadString();

        std::cout << "\nEnter LastName: ";
        Client.LastName = Core::InputValidate::ReadString();

        std::cout << "\nEnter Email: ";
        Client.Email = Core::InputValidate::ReadString();

        std::cout << "\nEnter Phone: ";
        Client.Phone = Core::InputValidate::ReadString();

        std::cout << "\nEnter PinCode: ";
        Client.PinCode = Core::InputValidate::ReadString();

        std::cout << "\nEnter Account Balance: ";
        Client.AccountBalance = Core::InputValidate::ReadNumber<double>();
    }

    static void _PrintClient(BankClient Client)
    {
        std::cout << "\nClient Card:";
        std::cout << "\n___________________";
        std::cout << "\nFirstName   : " << Client.GetFirstName();
        std::cout << "\nLastName    : " << Client.GetLastName();
        std::cout << "\nFull Name   : " << Client.FullName();
        std::cout << "\nEmail       : " << Client.GetEmail();
        std::cout << "\nPhone       : " << Client.GetPhone();
        std::cout << "\nAcc. Number : " << Client.GetAccountNumber();
        std::cout << "\nPassword    : " << Client.GetPinCode();
        std::cout << "\nBalance     : " << Client.GetAccountBalance();
        std::cout << "\n___________________\n";

    }

public:

    static void ShowAddNewClientScreen()
    {

        _DrawScreenHeader("\t  Add New Client Screen");

        std::string AccountNumber = "";

        std::cout << "\nPlease Enter Account Number: ";
        AccountNumber = Core::InputValidate::ReadString();
        while (BankClient::IsClientExist(AccountNumber))
        {
            std::cout << "\nAccount Number Is Already Used, Choose another one: ";
            AccountNumber = Core::InputValidate::ReadString();
        }

        BankClient NewClient = BankClient::GetAddNewClientObject(AccountNumber);


        _ReadClientInfo(NewClient);

        BankClient::enSaveResults SaveResult;

        SaveResult = NewClient.Save();

        switch (SaveResult)
        {
        case  BankClient::enSaveResults::svSucceeded:
        {
            std::cout << "\nAccount Addeded Successfully :-)\n";
            _PrintClient(NewClient);
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



};

