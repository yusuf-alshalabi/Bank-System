#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include "../../Core/Services/UserService.h"
#include "../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

class DeleteUserScreen :protected Screen
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
        std::cout << "User Name   : " << User.GetUserName() << "\n";
        std::cout << "Password    : " << _FormatPasswordForDisplay(User.GetPassword()) << "\n";
        std::cout << "Permissions : " << User.GetPermissions() << "\n";
        _ShowBorderLine(60, '=');
    }

public:

    static void ShowDeleteUserScreen()
    {
        if (!CheckAccessRights(User::enPermissions::pManageUsers))
        {
            return;
        }

        _DrawScreenHeader("Delete User Screen");

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

        User User1 = User::Find(UserName);

        std::string password = Core::InputValidate::ReadString("\nEnter password to authorize this operation: ");
        if (!Bank::Users::UserService::VerifyUserPassword(User1, password))
        {
            _ShowErrorMessage("Invalid password. Operation denied.");
            return;
        }

        if (User1.GetPermissions() == User::enPermissions::eAll
            && Bank::Users::UserService::IsLastFullAccessAdmin(User1.GetUserName()))
        {
            _ShowErrorMessage("You cannot delete the last full-access Admin user.");
            return;
        }

        _PrintUser(User1);

        if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to delete this User? "))
        {
            if (User1.Delete())
            {
                _ShowSuccessMessage("User deleted successfully.");
            }
            else
            {
                _ShowErrorMessage("User was not deleted.");
            }
        }
    }

};