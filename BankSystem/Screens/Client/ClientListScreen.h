#pragma once

#include <iostream>
#include "../Screen.h"
#include "../../Core/BankClient.h"
#include "../../Core/User.h"
#include <iomanip>
#include <vector>

class ClientListScreen :protected Screen
{
private:
    static void PrintClientRecordLine(const BankClient& Client)
    {
        std::cout << CYAN << "| " << RESET<< std::setw(15) << std::left << Client.GetAccountNumber();
        std::cout << CYAN << "| " << RESET<< std::setw(20) << std::left << Client.FullName();
        std::cout << CYAN << "| " << RESET<< std::setw(12) << std::left << Client.GetPhone();
        std::cout << CYAN << "| " << RESET<< std::setw(20) << std::left << Client.GetEmail();
        std::cout << CYAN << "| " << RESET<< std::setw(10) << std::left << Client.GetPinCode();
        std::cout << CYAN << "| " << RESET << GREEN << std::setw(14) << std::left << _CurrencyText(Client.GetAccountBalance()) << RESET;
        std::cout << CYAN << "|" << RESET;
    }

public:

    static void ShowClientsList()
    {
        if (!CheckAccessRights(User::enPermissions::pListClients))
        {
            return;
        }

        std::vector<BankClient> vClients = BankClient::GetClientsList();

        _DrawScreenHeader("Client List Screen", "(" + std::to_string(vClients.size()) + ") Client(s).");

        _ShowTableBorder(102);

        std::cout << CYAN << "| " << RESET << std::left << std::setw(15) << "Acc. Number";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(20) << "Client Name";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(12) << "Phone";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(20) << "Email";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(10) << "Pin Code";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(14) << "Balance";
        std::cout << CYAN << "|\n" << RESET;

        _ShowTableBorder(102);

        if (vClients.size() == 0)
        {
            std::cout << "\n  No Clients Available In the System!\n";
        }
        else
        {
            for (const BankClient& Client : vClients)
            {
                PrintClientRecordLine(Client);
                std::cout << "\n";
            }
        }

        _ShowTableBorder(102);
    }

};