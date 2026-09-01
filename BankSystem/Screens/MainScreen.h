#pragma once
#include <iostream>
#include <iomanip>
#include "Screen.h"
#include "ClientListScreen.h"
#include "AddNewClientScreen.h"
#include "DeleteClientScreen.h"
#include "UpdateClientScreen.h"
#include "FindClientScreen.h"
#include "TransactionsScreen.h"
#include "../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

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
        eExit = 8
    };

    static short _ReadMainMenueOption()
    {
        std::cout << std::setw(37) << std::left << "" << "Choose what do you want to do? [1 to 8]? ";
        short Choice = Core::InputValidate::ReadNumberBetween<short>(1, 8, "Enter Number between 1 to 8: ");
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
        // TODO: Call ManageUsersScreen::ShowManageUsersMenue();
        std::cout << "\nManage Users Menue Will be here...\n";
    }

    static void _Logout()
    {
        // TODO: Handle session end / logout
        std::cout << "\nUser logged out successfully.\n";
    }

    static void _PerformMainMenueOption(enMainMenueOptions MainMenueOption)
    {
        switch (MainMenueOption)
        {
        case enMainMenueOptions::eListClients:
            system("cls");
            _ShowAllClientsScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eAddNewClient:
            system("cls");
            _ShowAddNewClientsScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eDeleteClient:
            system("cls");
            _ShowDeleteClientScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eUpdateClient:
            system("cls");
            _ShowUpdateClientScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eFindClient:
            system("cls");
            _ShowFindClientScreen();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eShowTransactionsMenue:
            system("cls");
            _ShowTransactionsMenue();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eManageUsers:
            system("cls");
            _ShowManageUsersMenue();
            _GoBackToMainMenue();
            break;

        case enMainMenueOptions::eExit:
            system("cls");
            _Logout();
            break;
        }
    }

public:
    static void ShowMainMenue()
    {
        system("cls");
        _DrawScreenHeader("\t\tMain Screen");

        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t\tMain Menue\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t[1] Show Client List.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[2] Add New Client.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[3] Delete Client.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[4] Update Client Info.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[5] Find Client.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[6] Transactions.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[7] Manage Users.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[8] Logout.\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";

        _PerformMainMenueOption((enMainMenueOptions)_ReadMainMenueOption());
    }
};