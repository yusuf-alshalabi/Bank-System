#pragma once

#include <iostream>
#include "../Screen.h"
#include "../../Core/BankClient.h"
#include "../../Core/User.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

class AddNewClientScreen : protected Screen
{
private:

    static void _ReadClientInfo(BankClient& Client)
    {
        Client.FirstName = Core::InputValidate::ReadString("\nEnter First Name: ");

        Client.LastName = Core::InputValidate::ReadString("\nEnter Last Name: ");

        Client.Email = Core::InputValidate::ReadString("\nEnter Email: ");

        Client.Phone = Core::InputValidate::ReadString("\nEnter Phone: ");

        Client.PinCode = Core::InputValidate::ReadString("\nEnter Pin Code: ");

        Client.AccountBalance = Core::InputValidate::ReadNumber<double>("\nEnter Account Balance: ");
    }

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

    static void ShowAddNewClientScreen()
    {
        if (!CheckAccessRights(User::enPermissions::pAddNewClient))
        {
            return;
        }

        _DrawScreenHeader("Add New Client Screen");

        std::string AccountNumber = "";
        AccountNumber = Core::InputValidate::ReadString("\nPlease enter Account Number (or 0 to Back): ");
        if (AccountNumber == "0")
        {
            return;
        }

        while (BankClient::IsClientExist(AccountNumber))
        {
            AccountNumber = Core::InputValidate::ReadString("\nAccount Number is already used, choose another one (or 0 to Back): ");
            if (AccountNumber == "0")
            {
                return;
            }
        }

        BankClient NewClient = BankClient::GetAddNewClientObject(AccountNumber);

        _ReadClientInfo(NewClient);

        BankClient::enSaveResults SaveResult = NewClient.Save();

        switch (SaveResult)
        {
        case BankClient::enSaveResults::svSucceeded:
            _ShowSuccessMessage("Account added successfully.");
            _PrintClient(NewClient);
            break;

        case BankClient::enSaveResults::svFailedEmptyObject:
            _ShowErrorMessage("Account was not saved because it's empty.");
            break;

        case BankClient::enSaveResults::svFaildAccountNumberExists:
            _ShowErrorMessage("Account was not saved because the account number is already used.");
            break;
        }
    }

};