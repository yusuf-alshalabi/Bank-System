#pragma once
#include <iostream>
#include "Screen.h"
#include "../Core/BankClient.h"
#include "../Core/User.h"
#include "../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include <vector>

class DeleteClientScreen :protected Screen
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
    static void ShowDeleteClientScreen()
    {
        if (!CheckAccessRights(User::enPermissions::pDeleteClient))
        {
            return;// this will exit the function and it will not continue
        }

        _DrawScreenHeader("\tDelete Client Screen");

        std::string AccountNumber = "";

        AccountNumber = Core::InputValidate::ReadString("\nPlease Enter Account Number: ");
        while (!BankClient::IsClientExist(AccountNumber))
        {
            std::cout << "\nAccount number is not found, choose another one: ";
            AccountNumber = Core::InputValidate::ReadString();
        }

        BankClient Client1 = BankClient::Find(AccountNumber);
        _PrintClient(Client1);

        if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to delete this client y/n? "))
        {


            if (Client1.Delete())
            {
                std::cout << "\nClient Deleted Successfully :-)\n";
                _PrintClient(Client1);
            }
            else
            {
                std::cout << "\nError Client Was not Deleted\n";
            }
        }
    }

};

