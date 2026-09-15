#pragma once
#include <iostream>
#include <iomanip>
#include "Screen.h"
#include "Client/ClientListScreen.h"
#include "Client/AddNewClientScreen.h"
#include "Client/DeleteClientScreen.h"
#include "Client/UpdateClientScreen.h"
#include "Client/FindClientScreen.h"
#include "Auth/LoginRegisterScreen.h"
#include "Transaction/TransactionsScreen.h"
#include "User/ManageUsersScreen.h"
#include "Currency/CurrencyExchangeMainScreen.h"
#include "../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "../Core/Infrastructure/SessionManager.h"
#include "../Core/Infrastructure/Logger.h"
#include "../Core/Services/MenuManager.h"

class MainScreen : protected Screen
{
private:
    enum enMainMenueOptions {
        eListClients = 1,
        eAddNewClient = 2,
        eDeleteClient = 3,
        eUpdateClient = 4,
        eFindClient = 5,
        eShowTransactionsMenue = 6,
        eManageUsers = 7,
		eLoginRegister = 8,
		eCurrencyExchange = 9,
        eExit = 10
    };

    static short _ReadMainMenueOption(short MaxOption)
    {
        std::cout << std::setw(37) << std::left << "" << "Choose what do you want to do? [" << 1 << " to " << MaxOption << "]? ";
        short Choice = Core::InputValidate::ReadNumberBetween<short>(1, MaxOption, "Enter Number between 1 and " + std::to_string(MaxOption) + "? ");
        return Choice;
    }

    static void _GoBackToMainMenue()
    {
        std::cout << "\n\nPress any key to go back to Main Menue...";
        system("pause>0");
        ShowMainMenue();
    }

    static void _ShowAllClientsScreen()
    {
       // std::cout << "\nClient List Screen Will be here...\n";
        ClientListScreen::ShowClientsList();
    }

    static void _ShowAddNewClientsScreen()
    {
        // std::cout << "\nAdd New Client Screen Will be here...\n";
        AddNewClientScreen::ShowAddNewClientScreen();
    }

    static void _ShowDeleteClientScreen()
    {
       // DeleteClientScreen::ShowDeleteClientScreen();
        DeleteClientScreen::ShowDeleteClientScreen();
    }

    static void _ShowUpdateClientScreen()
    {
       // std::cout << "\nUpdate Client Screen Will be here...\n";
        UpdateClientScreen::ShowUpdateClientScreen();
    }

    static void _ShowFindClientScreen()
    {
        // std::cout << "\nFind Client Screen Will be here...\n";
        FindClientScreen::ShowFindClientScreen();
    }

    static void _ShowTransactionsMenue()
    {
       // std::cout << "\nTransactions Menue Will be here...\n";
        TransactionsScreen::ShowTransactionsMenue();
    }

    static void _ShowManageUsersMenue()
    {
       // std::cout << "\nManage Users Menue Will be here...\n";
        ManageUsersScreen::ShowManageUsersMenue();
    }

	static void _ShowLoginRegisterScreen()
	{
		// std::cout << "\nLogin Register Screen Will be here...\n";
		LoginRegisterScreen::ShowLoginRegisterScreen();
	}

	static void _ShowCurrencyExchangeScreen()
	{
		// std::cout << "\nCurrency Exchange Screen Will be here...\n";
		CurrencyExchangeMainScreen::ShowCurrenciesMenue();
	}

    static void _Logout()
    {
        Bank::Diagnostics::Logger::Instance().LogUserAction("LOGOUT");
        // secure session wipe + back to the login flow
        Bank::Security::SessionManager::Instance().End();
    }

    static void _PerformMainMenueOption(enMainMenueOptions MainMenueOption)
    {
        switch (MainMenueOption)
        {
        case enMainMenueOptions::eListClients:
            std::system("cls");
            _ShowAllClientsScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eAddNewClient:
            std::system("cls");
            _ShowAddNewClientsScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eDeleteClient:
            std::system("cls");
            _ShowDeleteClientScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eUpdateClient:
            std::system("cls");
            _ShowUpdateClientScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eFindClient:
            std::system("cls");
            _ShowFindClientScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eShowTransactionsMenue:
            system("cls");
            _ShowTransactionsMenue();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eManageUsers:
            std::system("cls");
            _ShowManageUsersMenue();
            _GoBackToMainMenue();
            break;
        case enMainMenueOptions::eLoginRegister:
            std::system("cls");
			_ShowLoginRegisterScreen();
			_GoBackToMainMenue();
            break;
        case enMainMenueOptions::eCurrencyExchange:
            std::system("cls");
			_ShowCurrencyExchangeScreen();
			_GoBackToMainMenue();
            break;
        case enMainMenueOptions::eExit:
            std::system("cls");
            _Logout();
            break;
        }
    }

public:
    static void ShowMainMenue()
    {
        std::system("cls");
        _DrawScreenHeader("\t\tMain Screen");

        // Dynamic menu: only entries the acting user is permitted to use.
        const User& CurrentUser = Bank::Security::SessionManager::Instance().Current();
        std::vector<Bank::UI::MainMenuEntry> Entries = Bank::UI::MenuManager::BuildMainMenu(CurrentUser);

        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t\tMain Menue\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        for (std::size_t i = 0; i < Entries.size(); ++i)
        {
            std::cout << std::setw(37) << std::left << "" << "\t[" << (i + 1) << "] " << Entries[i].Label << ".\n";
        }
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";

        short Choice = _ReadMainMenueOption(static_cast<short>(Entries.size()));
        _PerformMainMenueOption((enMainMenueOptions)Entries[Choice - 1].Key);
    }
};