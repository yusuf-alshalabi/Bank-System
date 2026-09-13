#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/Person.h"
#include "../../Core/BankClient.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/Util.h"
#include <vector>

class TotalBalancesScreen : protected Screen
{

private:

    static void PrintClientRecordBalanceLine(BankClient Client)
    {
        std::cout << std::setw(25) << std::left << "" << "| " << std::setw(15) << std::left << Client.GetAccountNumber();
        std::cout << "| " << std::setw(40) << std::left << Client.FullName();
        std::cout << "| " << std::setw(12) << std::left
            << std::fixed << std::setprecision(2)
            << Client.GetAccountBalance();
    }

public:

    static void ShowTotalBalances()
    {

        std::vector <BankClient> vClients = BankClient::GetClientsList();

        std::string Title = "\t  Balances List Screen";
        std::string SubTitle = "\t    (" + std::to_string(vClients.size()) + ") Client(s).";

        _DrawScreenHeader(Title, SubTitle);

        std::cout << std::setw(25) << std::left << "" << "\n\t\t_______________________________________________________";
        std::cout << "__________________________\n" << std::endl;

        std::cout << std::setw(25) << std::left << "" << "| " << std::left << std::setw(15) << "Accout Number";
        std::cout << "| " << std::left << std::setw(40) << "Client Name";
        std::cout << "| " << std::left << std::setw(12) << "Balance";
        std::cout << std::setw(25) << std::left << "" << "\t\t_______________________________________________________";
        std::cout << "__________________________\n" << std::endl;

        double TotalBalances = BankClient::GetTotalBalances();

        if (vClients.size() == 0)
            std::cout << "\t\t\t\tNo Clients Available In the System!";
        else

            for (BankClient Client : vClients)
            {
                PrintClientRecordBalanceLine(Client);
                std::cout << std::endl;
            }

        std::cout << std::setw(25) << std::left << "" << "\n\t\t_______________________________________________________";
        std::cout << "__________________________\n" << std::endl;

        std::cout << std::setw(8) << std::left << "" << "\t\t\t\t\t\t\t     Total Balances = " << std::fixed << std::setprecision(2) << TotalBalances << std::endl;
        std::cout << std::setw(8) << std::left << "" << "\t\t\t\t  ( " << Core::Util::NumberToText(TotalBalances) << ")";
    }

};
;
