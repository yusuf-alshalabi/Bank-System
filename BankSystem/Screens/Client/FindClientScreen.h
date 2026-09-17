#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/Person.h"
#include "../../Core/BankClient.h"
#include "../../Core/User.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

class FindClientScreen :protected Screen
{

private:

    static void _PrintClient(const BankClient& Client)
    {
        std::cout << "\nClient Card:";
        _ShowBorderLine(60, '=');
        std::cout << "FirstName   : " << Client.GetFirstName() << "\n";
        std::cout << "LastName    : " << Client.GetLastName() << "\n";
        std::cout << "Full Name   : " << Client.FullName() << "\n";
        std::cout << "Email       : " << Client.GetEmail() << "\n";
        std::cout << "Phone       : " << Client.GetPhone() << "\n";
        std::cout << "Acc. Number : " << Client.GetAccountNumber() << "\n";
        std::cout << "Password    : " << Client.GetPinCode() << "\n";
        std::cout << "Balance     : " << _FormatCurrency(Client.GetAccountBalance()) << "\n";
        _ShowBorderLine(60, '=');
    }

public:

    static void ShowFindClientScreen()
    {
        if (!CheckAccessRights(User::enPermissions::pFindClient))
        {
            return;
        }

        _DrawScreenHeader("Find Client Screen");

        std::string AccountNumber = "";
        AccountNumber = Core::InputValidate::ReadString("\nPlease enter Account Number (or 0 to Back): ");
        if (AccountNumber == "0")
        {
            return;
        }

        while (!BankClient::IsClientExist(AccountNumber))
        {
            AccountNumber = Core::InputValidate::ReadString("\nAccount number is not found, choose another one (or 0 to Back): ");
            if (AccountNumber == "0")
            {
                return;
            }
        }

        BankClient Client1 = BankClient::Find(AccountNumber);

        if (!Client1.IsEmpty())
        {
            std::cout << "\nClient Found.\n";
            _PrintClient(Client1);
        }
        else
        {
            std::cout << "\nClient was not found.\n";
        }
    }

};