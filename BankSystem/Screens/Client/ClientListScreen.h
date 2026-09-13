#pragma once

#include <iostream>
#include "Screen.h"
#include "../Core/BankClient.h"
#include "../Core/User.h"
#include <iomanip>
#include <vector>

class ClientListScreen :protected Screen
{
private:
    static void PrintClientRecordLine(const BankClient& Client)
    {

        std::cout << std::setw(8) << std::left << "" << "| " << std::setw(15) << std::left << Client.GetAccountNumber();
        std::cout << "| " << std::setw(20) << std::left << Client.FullName();
        std::cout << "| " << std::setw(12) << std::left << Client.GetPhone();
        std::cout << "| " << std::setw(20) << std::left << Client.GetEmail();
        std::cout << "| " << std::setw(10) << std::left << Client.GetPinCode();
        std::cout << "| " << std::setw(12) << std::left << Client.GetAccountBalance();

    }

public:

    static void ShowClientsList()
    {
        if (!CheckAccessRights(User::enPermissions::pListClients))
        {
            return;// this will exit the function and it will not continue
        }

        std::vector<BankClient> vClients = BankClient::GetClientsList();
        std::string Title = "\t  Client List Screen";
        std::string SubTitle = "\t    (" + std::to_string(vClients.size()) + ") Client(s).";

        _DrawScreenHeader(Title, SubTitle);


        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_________________________________________\n" << std::endl;

        std::cout << std::setw(8) << std::left << "" << "| " << std::left << std::setw(15) << "Accout Number";
        std::cout << "| " << std::left << std::setw(20) << "Client Name";
        std::cout << "| " << std::left << std::setw(12) << "Phone";
        std::cout << "| " << std::left << std::setw(20) << "Email";
        std::cout << "| " << std::left << std::setw(10) << "Pin Code";
        std::cout << "| " << std::left << std::setw(12) << "Balance";
        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_________________________________________\n" << std::endl;

        if (vClients.size() == 0)
            std::cout << "\t\t\t\tNo Clients Available In the System!";
        else

            for (BankClient Client : vClients)
            {

                PrintClientRecordLine(Client);
                std::cout << std::endl;
            }

        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_________________________________________\n" << std::endl;

    }

};

