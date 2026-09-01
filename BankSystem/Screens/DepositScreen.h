#pragma once

#include <iostream>
#include "Screen.h"
#include "../Core/Person.h"
#include "../Core/BankClient.h"
#include "../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

class DepositScreen : protected Screen
{

private:

    static void _PrintClient(const BankClient& Client)
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

    static void ShowDepositScreen()
    {
        _DrawScreenHeader("\t   Deposit Screen");

        std::string AccountNumber = Core::InputValidate::ReadString("\nPlease enter Account Number: ");


        while (!BankClient::IsClientExist(AccountNumber))
        {
            std::cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
            AccountNumber = Core::InputValidate::ReadString("\nPlease enter Account Number: ");
        }

        BankClient Client1 = BankClient::Find(AccountNumber);
        _PrintClient(Client1);

        double Amount = Core::InputValidate::ReadNumber<double>("\nPlease enter deposit amount? ");

        if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to perform this transaction? "))
        {
            Client1.Deposit(Amount);
            std::cout << "\nAmount Deposited Successfully.\n";
            std::cout << "\nNew Balance Is: " << Client1.GetAccountBalance();

        }
        else
        {
            std::cout << "\nOperation was cancelled.\n";
        }

    }

};

