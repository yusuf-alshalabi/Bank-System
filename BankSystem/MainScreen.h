#pragma once
#include <iostream>
#include "Screen.h"
#include "../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include <iomanip>


using namespace std;

class MainScreen :protected Screen
{

private:
    enum MainMenueOptions {
        ListClients = 1, AddNewClient = 2, DeleteClient = 3,
        UpdateClient = 4, FindClient = 5, ShowTransactionsMenue = 6,
        ManageUsers = 7, Exit = 8
    };

    static short _ReadMainMenuOption()
    {
        cout << setw(37) << left << "" << "Choose what do you want to do? [1 to 8]? ";
        short Choice = Core::InputValidate::ReadNumberBetween<short>(1, 8, "Enter Number between 1 to 8? ");
        return Choice;
    }

    static  void _GoBackToMainMenue()
    {
        cout << setw(37) << left << "" << "\n\tPress any key to go back to Main Menue...\n";

        system("pause>0");
        ShowMainMenu();
    }

    static void _ShowAllClientsScreen()
    {
        cout << "\nClient List Screen Will be here...\n";


    }

    static void _ShowAddNewClientsScreen()
    {
        cout << "\nAdd New Client Screen Will be here...\n";

    }

    static void _ShowDeleteClientScreen()
    {
        cout << "\nDelete Client Screen Will be here...\n";

    }

    static void _ShowUpdateClientScreen()
    {
        cout << "\nUpdate Client Screen Will be here...\n";

    }

    static void _ShowFindClientScreen()
    {
        cout << "\nFind Client Screen Will be here...\n";

    }

    static void _ShowTransactionsMenue()
    {
        cout << "\nTransactions Menue Will be here...\n";

    }

    static void _ShowManageUsersMenue()
    {
        cout << "\nUsers Menue Will be here...\n";

    }

    static void _ShowEndScreen()
    {
        cout << "\nEnd Screen Will be here...\n";

    }

    static void _PerfromMainMenuOption(MainMenueOptions MainMenueOption)
    {
        switch (MainMenueOption)
        {
        case MainMenueOptions::ListClients:
        {
            system("cls");
            _ShowAllClientsScreen();
            _GoBackToMainMenue();
            break;
        }
        case MainMenueOptions::AddNewClient:
            system("cls");
            _ShowAddNewClientsScreen();
            _GoBackToMainMenue();
            break;

        case MainMenueOptions::DeleteClient:
            system("cls");
            _ShowDeleteClientScreen();
            _GoBackToMainMenue();
            break;

        case MainMenueOptions::UpdateClient:
            system("cls");
            _ShowUpdateClientScreen();
            _GoBackToMainMenue();
            break;

        case MainMenueOptions::FindClient:
            system("cls");
            _ShowFindClientScreen();
            _GoBackToMainMenue();
            break;

        case MainMenueOptions::ShowTransactionsMenue:
            system("cls");
            _ShowTransactionsMenue();
            break;

        case MainMenueOptions::ManageUsers:
            system("cls");
            _ShowManageUsersMenue();
            break;

        case MainMenueOptions::Exit:
            system("cls");
            _ShowEndScreen();
            //Login();

            break;
        }

    }



public:


    static void ShowMainMenu()
    {
        system("cls");
        _DrawScreenHeader("\tMain Menue Screen");

        std::cout << "\t\t\t\t\t[1] Show Client List.\n";
        std::cout << "\t\t\t\t\t[2] Add New Client.\n";
        std::cout << "\t\t\t\t\t[3] Delete Client.\n";
        std::cout << "\t\t\t\t\t[4] Update Client Info.\n";
        std::cout << "\t\t\t\t\t[5] Find Client.\n";
        std::cout << "\t\t\t\t\t[6] Transactions.\n";
        std::cout << "\t\t\t\t\t[7] Manage Users.\n";
        std::cout << "\t\t\t\t\t[8] Logout.\n";
        std::cout << "\t\t\t\t\t===========================================\n";

        _PerfromMainMenuOption((MainMenueOptions)_ReadMainMenuOption());
    }

};

