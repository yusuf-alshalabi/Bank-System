#pragma once
#include <algorithm>
#include <cstdlib>
#include <iostream>
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
        eLogout = 10
    };

    static void _ShowAllClientsScreen()
    {
        ClientListScreen::ShowClientsList();
    }

    static void _ShowAddNewClientsScreen()
    {
        AddNewClientScreen::ShowAddNewClientScreen();
    }

    static void _ShowDeleteClientScreen()
    {
        DeleteClientScreen::ShowDeleteClientScreen();
    }

    static void _ShowUpdateClientScreen()
    {
        UpdateClientScreen::ShowUpdateClientScreen();
    }

    static void _ShowFindClientScreen()
    {
        FindClientScreen::ShowFindClientScreen();
    }

    static void _ShowTransactionsMenue()
    {
        TransactionsScreen::ShowTransactionsMenue();
    }

    static void _ShowManageUsersMenue()
    {
        ManageUsersScreen::ShowManageUsersMenue();
    }

    static void _ShowLoginRegisterScreen()
    {
        LoginRegisterScreen::ShowLoginRegisterScreen();
    }

    static void _ShowCurrencyExchangeScreen()
    {
        CurrencyExchangeMainScreen::ShowCurrenciesMenue();
    }

    static void _Logout()
    {
        Bank::Diagnostics::Logger::Instance().LogUserAction("LOGOUT");
        Bank::Security::SessionManager::Instance().End();
    }

    static void _GoBackToMainMenue()
    {
        _PressEnterToContinue();
    }

    static void _PerformMainMenueOption(enMainMenueOptions MainMenueOption)
    {
        switch (MainMenueOption)
        {
        case enMainMenueOptions::eListClients:
            _ClearScreen();
            _ShowAllClientsScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eAddNewClient:
            _ClearScreen();
            _ShowAddNewClientsScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eDeleteClient:
            _ClearScreen();
            _ShowDeleteClientScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eUpdateClient:
            _ClearScreen();
            _ShowUpdateClientScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eFindClient:
            _ClearScreen();
            _ShowFindClientScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eShowTransactionsMenue:
            _ShowTransactionsMenue();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eManageUsers:
            _ShowManageUsersMenue();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eLoginRegister:
            _ClearScreen();
            _ShowLoginRegisterScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eCurrencyExchange:
            _ShowCurrencyExchangeScreen();
            _GoBackToMainMenue();
            break;
        }
    }

    static void _ShowExitScreen()
    {
        _ClearScreen();
        _DrawScreenHeader("Program Ends");
        _ShowSuccessMessage("Thank you for using BankSystem. Goodbye!");
    }

public:

    static void ShowMainMenue()
    {
        while (true)
        {
            _ClearScreen();
            _DrawScreenHeader("Main Menu Screen");

            const User& CurrentUser = Bank::Security::SessionManager::Instance().Current();
            std::vector<Bank::UI::MainMenuEntry> Entries = Bank::UI::MenuManager::BuildMainMenu(CurrentUser);

            std::cout << "\n";
            for (const Bank::UI::MainMenuEntry& Entry : Entries)
            {
                std::cout << CYAN << "  [" << Entry.Key << "]  " << RESET
                    << YELLOW << Entry.Label << RESET << ".\n";
            }
            std::cout << CYAN << "\n  [0]  " << RESET << YELLOW << "Exit App" << RESET << ".\n";

            _ShowLine(60, '-');

            short Choice = _ReadMenuOption(1, 10, true);

            if (Choice == 0)
            {
                _ShowExitScreen();
                std::exit(0);
            }

            auto Found = std::find_if(Entries.begin(), Entries.end(),
                [Choice](const Bank::UI::MainMenuEntry& Entry) { return Entry.Key == Choice; });

            if (Found == Entries.end())
            {
                std::cout << "\n";
                _ShowErrorMessage("Invalid option: the feature is not available for your account.");
                _PressEnterToContinue();
                continue;
            }

            if (Found->Key == static_cast<int>(Bank::UI::MainMenuKey::Logout))
            {
                _Logout();
                return;
            }

            _PerformMainMenueOption(static_cast<enMainMenueOptions>(Found->Key));
        }
    }
};