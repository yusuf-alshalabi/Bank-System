#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/Person.h"
#include "../../Core/BankClient.h"
#include "../../Core/User.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/Util.h"
#include <vector>

class TotalBalancesScreen : protected Screen
{

private:

    static void PrintClientRecordBalanceLine(const BankClient& Client)
    {
        std::cout << CYAN<< "| " << RESET << std::setw(15) << std::left << Client.GetAccountNumber();
        std::cout << CYAN<< "| " << RESET << std::setw(40) << std::left << Client.FullName();
        std::cout << CYAN<< "| " << RESET << std::setw(14) << std::left
            << std::fixed << std::setprecision(2)
            << Client.GetAccountBalance();
        std::cout << CYAN << "|" << RESET;
    }

public:

    static void ShowTotalBalances()
    {
        if (!CheckAccessRights(User::enPermissions::pTranactions))
        {
            return;
        }

        std::vector<BankClient> vClients = BankClient::GetClientsList();
        double TotalBalances = BankClient::GetTotalBalances();

        _DrawScreenHeader("Balances List Screen", "(" + std::to_string(vClients.size()) + ") Client(s).");

        _ShowTableBorder(74);

        std::cout << CYAN << "| " << RESET << std::left << std::setw(15) << "Acc. Number";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(40) << "Client Name";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(14) << "Balance";
        std::cout << CYAN << "|\n" << RESET;

        _ShowTableBorder(74);

        if (vClients.size() == 0)
        {
            std::cout << "\n  No Clients Available In the System!\n";
        }
        else
        {
            for (const BankClient& Client : vClients)
            {
                PrintClientRecordBalanceLine(Client);
                std::cout << "\n";
            }
        }

        _ShowTableBorder(74);

        std::cout << "\n  Total Balances = " << std::fixed << std::setprecision(2) << YELLOW << TotalBalances << RESET << "\n";
        std::cout << YELLOW << "  ( " << Core::Util::NumberToText(TotalBalances) << ")\n" << RESET;
    }

};