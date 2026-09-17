#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include "../../Core/Services/UserService.h"
#include "../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

class FindUserScreen :protected Screen
{

private:

    static void _PrintUser(const User& User)
    {
        std::cout << "\nUser Card:";
        _ShowBorderLine(60, '=');
        std::cout << "FirstName   : " << User.GetFirstName() << "\n";
        std::cout << "LastName    : " << User.GetLastName() << "\n";
        std::cout << "Full Name   : " << User.FullName() << "\n";
        std::cout << "Email       : " << User.GetEmail() << "\n";
        std::cout << "Phone       : " << User.GetPhone() << "\n";
        std::cout << "UserName    : " << User.GetUserName() << "\n";
        std::cout << "Password    : " << _FormatPasswordForDisplay(User.GetPassword()) << "\n";
        std::cout << "Permissions : " << User.GetPermissions() << "\n";
        _ShowBorderLine(60, '=');
    }

public:

    static void ShowFindUserScreen()
    {
        if (!CheckAccessRights(User::enPermissions::pManageUsers))
        {
            return;
        }

        _DrawScreenHeader("Find User Screen");

        std::string UserName = "";
        UserName = Core::InputValidate::ReadString("\nPlease enter UserName (or 0 to Back): ");
        if (UserName == "0")
        {
            return;
        }

        while (!User::IsUserExist(UserName))
        {
            UserName = Core::InputValidate::ReadString("\nUser is not found, choose another one (or 0 to Back): ");
            if (UserName == "0")
            {
                return;
            }
        }

        std::string password = Core::InputValidate::ReadString("\nEnter password to authorize this operation: ");

        User User1 = User::Find(UserName);

        if (!User1.IsEmpty() && Bank::Users::UserService::VerifyUserPassword(User1, password))
        {
            std::cout << "\nUser Found.\n";
            _PrintUser(User1);
        }
        else
        {
            _ShowErrorMessage("Invalid password. Operation denied.");
        }
    }

};