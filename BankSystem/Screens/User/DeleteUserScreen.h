#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include "../../Core/Services/UserService.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include <iomanip>

class DeleteUserScreen :protected Screen
{

private:
    static void _PrintUser(const User& User)
    {
        std::cout << "\nUser Card:";
        std::cout << "\n___________________";
        std::cout << "\nFirstName   : " << User.GetFirstName();
        std::cout << "\nLastName    : " << User.GetLastName();
        std::cout << "\nFull Name   : " << User.FullName();
        std::cout << "\nEmail       : " << User.GetEmail();
        std::cout << "\nPhone       : " << User.GetPhone();
        std::cout << "\nUser Name   : " << User.GetUserName();
        std::cout << "\nPassword    : " << _FormatPasswordForDisplay(User.GetPassword());
        std::cout << "\nPermissions : " << User.GetPermissions();
        std::cout << "\n___________________\n";

    }

public:
    static void ShowDeleteUserScreen()
    {

        _DrawScreenHeader("\tDelete User Screen");

        string UserName = "";

        UserName = Core::InputValidate::ReadString("\nPlease Enter UserName: ");
        while (!User::IsUserExist(UserName))
        {
            UserName = Core::InputValidate::ReadString("\nUser is not found, choose another one: ");
        }

        User User1 = User::Find(UserName);

        // --- credential gate: operator must know the target user's password ---
        string password = Core::InputValidate::ReadString("\nEnter password to authorize this operation: ");
        if (!Bank::Users::UserService::VerifyUserPassword(User1, password))
        {
            std::cout << "\nInvalid password. Operation denied.\n";
            return;
        }

        // --- last admin protection: cannot delete the sole full-access user ---
        if (User1.GetPermissions() == User::enPermissions::eAll
            && Bank::Users::UserService::IsLastFullAccessAdmin(User1.GetUserName()))
        {
            std::cout << "\nYou cannot delete the last full-access Admin user.\n";
            return;
        }

        _PrintUser(User1);

        if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to delete this User y/n? "))
        {

            if (User1.Delete())
            {
                std::cout << "\nUser Deleted Successfully :-)\n";
            }
            else
            {
                std::cout << "\nError User Was not Deleted\n";
            }
        }
    }

};
