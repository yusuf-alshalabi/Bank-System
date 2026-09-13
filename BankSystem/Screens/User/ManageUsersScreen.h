#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "UsersListScreen.h"
#include "AddNewUserScreen.h"
#include "DeleteUserScreen.h"
#include "UpdateUserScreen.h"
#include "FindUserScreen.h"
#include <iomanip>

class ManageUsersScreen :protected Screen
{

private:
    enum enManageUsersMenueOptions {
        eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3,
        eUpdateUser = 4, eFindUser = 5, eMainMenue = 6
    };

    static short ReadManageUsersMenueOption()
    {
        std::cout << std::setw(37) << std::left << "" << "Choose what do you want to do? [1 to 6]? ";
        short Choice = Core::InputValidate::ReadNumberBetween<short>(1, 6, "Enter Number between 1 to 6? ");
        return Choice;
    }

    static void _GoBackToManageUsersMenue()
    {
        std::cout << "\n\nPress any key to go back to Manage Users Menue...";
        std::system("pause>0");
        ShowManageUsersMenue();
    }

    static void _ShowListUsersScreen()
    {
       // std::cout << "\nList Users Screen Will Be Here.\n";
		UsersListScreen::ShowUsersList();
    }

    static void _ShowAddNewUserScreen()
    {
       // std::cout << "\nAdd New User Screen Will Be Here.\n";
        AddNewUserScreen::ShowAddNewUserScreen();

    }

    static void _ShowDeleteUserScreen()
    {
        //std::cout << "\nDelete User Screen Will Be Here.\n";
		DeleteUserScreen::ShowDeleteUserScreen();

    }

    static void _ShowUpdateUserScreen()
    {
        //std::cout << "\nUpdate User Screen Will Be Here.\n";
        UpdateUserScreen::ShowUpdateUserScreen();
    }

    static void _ShowFindUserScreen()
    {
      //  std::cout << "\nFind User Screen Will Be Here.\n";
        FindUserScreen::ShowFindUserScreen();
    }


    static void _PerformManageUsersMenueOption(enManageUsersMenueOptions ManageUsersMenueOption)
    {

        switch (ManageUsersMenueOption)
        {
        case enManageUsersMenueOptions::eListUsers:
        {
            std::system("cls");
            _ShowListUsersScreen();
            _GoBackToManageUsersMenue();
            break;
        }

        case enManageUsersMenueOptions::eAddNewUser:
        {
            std::system("cls");
            _ShowAddNewUserScreen();
            _GoBackToManageUsersMenue();
            break;
        }

        case enManageUsersMenueOptions::eDeleteUser:
        {
            std::system("cls");
            _ShowDeleteUserScreen();
            _GoBackToManageUsersMenue();
            break;
        }

        case enManageUsersMenueOptions::eUpdateUser:
        {
            std::system("cls");
            _ShowUpdateUserScreen();
            _GoBackToManageUsersMenue();
            break;
        }

        case enManageUsersMenueOptions::eFindUser:
        {
            std::system("cls");

            _ShowFindUserScreen();
            _GoBackToManageUsersMenue();
            break;
        }

        case enManageUsersMenueOptions::eMainMenue:
        {
            //do nothing here the main screen will handle it :-) ;
        }
        }

    }



public:


    static void ShowManageUsersMenue()
    {
        if (!CheckAccessRights(User::enPermissions::pManageUsers))
        {
            return;// this will exit the function and it will not continue
        }

        std::system("cls");
        _DrawScreenHeader("\t Manage Users Screen");

        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t\t  Manage Users Menue\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t[1] List Users.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[2] Add New User.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[3] Delete User.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[4] Update User.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[5] Find User.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[6] Main Menue.\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";

        _PerformManageUsersMenueOption((enManageUsersMenueOptions)ReadManageUsersMenueOption());
    }

};

