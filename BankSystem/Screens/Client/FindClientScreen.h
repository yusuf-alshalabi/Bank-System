#pragma once
#include <iostream>
#include "Screen.h"
#include "../Core/Person.h"
#include "../Core/BankClient.h"
#include "../Core/User.h"
#include "../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

class FindClientScreen :protected Screen
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

    static void ShowFindClientScreen()
    {
        if (!CheckAccessRights(User::enPermissions::pFindClient))
        {
            return;// this will exit the function and it will not continue
        }

        _DrawScreenHeader("\tFind Client Screen");

        std::string AccountNumber = Core::InputValidate::ReadString("\nPlease Enter Account Number: ");
        while (!BankClient::IsClientExist(AccountNumber))
        {
            AccountNumber = Core::InputValidate::ReadString("\nAccount number is not found, choose another one: ");
        }

        BankClient Client1 = BankClient::Find(AccountNumber);

        if (!Client1.IsEmpty())
        {
            std::cout << "\nClient Found :-)\n";
        }
        else
        {
            std::cout << "\nClient Was not Found :-(\n";
        }

        _PrintClient(Client1);

    }

};

