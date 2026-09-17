#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include "UsersListScreen.h"
#include "AddNewUserScreen.h"
#include "DeleteUserScreen.h"
#include "UpdateUserScreen.h"
#include "FindUserScreen.h"

class ManageUsersScreen :protected Screen
{

private:

    enum enManageUsersMenueOptions {
        eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3,
        eUpdateUser = 4, eFindUser = 5
    };

    static void _GoBackToManageUsersMenue()
    {
        _PressEnterToContinue();
    }

    static void _ShowListUsersScreen()
    {
        UsersListScreen::ShowUsersList();
    }

    static void _ShowAddNewUserScreen()
    {
        AddNewUserScreen::ShowAddNewUserScreen();
    }

    static void _ShowDeleteUserScreen()
    {
        DeleteUserScreen::ShowDeleteUserScreen();
    }

    static void _ShowUpdateUserScreen()
    {
        UpdateUserScreen::ShowUpdateUserScreen();
    }

    static void _ShowFindUserScreen()
    {
        FindUserScreen::ShowFindUserScreen();
    }

    static void _PerformManageUsersMenueOption(enManageUsersMenueOptions ManageUsersMenueOption)
    {
        switch (ManageUsersMenueOption)
        {
        case enManageUsersMenueOptions::eListUsers:
            _ClearScreen();
            _ShowListUsersScreen();
            _GoBackToManageUsersMenue();
            break;

        case enManageUsersMenueOptions::eAddNewUser:
            _ClearScreen();
            _ShowAddNewUserScreen();
            _GoBackToManageUsersMenue();
            break;

        case enManageUsersMenueOptions::eDeleteUser:
            _ClearScreen();
            _ShowDeleteUserScreen();
            _GoBackToManageUsersMenue();
            break;

        case enManageUsersMenueOptions::eUpdateUser:
            _ClearScreen();
            _ShowUpdateUserScreen();
            _GoBackToManageUsersMenue();
            break;

        case enManageUsersMenueOptions::eFindUser:
            _ClearScreen();
            _ShowFindUserScreen();
            _GoBackToManageUsersMenue();
            break;
        }
    }

public:

    static void ShowManageUsersMenue()
    {
        if (!CheckAccessRights(User::enPermissions::pManageUsers))
        {
            return;
        }

        while (true)
        {
            _ClearScreen();
            _DrawScreenHeader("Manage Users Menu Screen");

            _ShowOptions({ "List Users", "Add New User", "Delete User", "Update User", "Find User" });
            _ShowBackOption(false);
            _ShowLine(60, '-');

            short Choice = _ReadMenuOption(1, 5);

            if (Choice == 0)
            {
                return;
            }

            _PerformManageUsersMenueOption(static_cast<enManageUsersMenueOptions>(Choice));
        }
    }

};