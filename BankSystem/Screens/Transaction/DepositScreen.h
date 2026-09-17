#pragma once

#include <iostream>
#include "../Screen.h"
#include "../../Core/Person.h"
#include "../../Core/BankClient.h"
#include "../../Core/User.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

class DepositScreen : protected Screen
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

    static void ShowDepositScreen()
    {
        if (!CheckAccessRights(User::enPermissions::pTranactions))
        {
            return;
        }

        _DrawScreenHeader("Deposit Screen");

        std::string AccountNumber = Core::InputValidate::ReadString("\nPlease enter Account Number (or 0 to Back): ");
        if (AccountNumber == "0")
        {
            return;
        }

        while (!BankClient::IsClientExist(AccountNumber))
        {
            AccountNumber = Core::InputValidate::ReadString("\nClient with [" + AccountNumber + "] does not exist (or 0 to Back): ");
            if (AccountNumber == "0")
            {
                return;
            }
        }

        BankClient Client1 = BankClient::Find(AccountNumber);
        _PrintClient(Client1);

        double Amount = Core::InputValidate::ReadNumber<double>("\nPlease enter deposit amount (or 0 to cancel): ");
        if (Amount == 0)
        {
            _ShowErrorMessage("Operation was cancelled.");
            return;
        }

        if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to perform this transaction? "))
        {
            if (Client1.Deposit(Amount))
            {
                _ShowSuccessMessage("Amount deposited successfully.");
                std::cout << "New Balance Is: " << _FormatCurrency(Client1.GetAccountBalance()) << "\n";
            }
            else
            {
                _ShowErrorMessage("Deposit failed: amount must be greater than zero.");
            }
        }
        else
        {
            _ShowErrorMessage("Operation was cancelled.");
        }
    }

};