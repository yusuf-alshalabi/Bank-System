#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include "../../Core/Services/UserService.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include <iomanip>

class FindUserScreen :protected Screen
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
        std::cout << "\nUserName    : " << User.GetUserName();
        std::cout << "\nPassword    : " << _FormatPasswordForDisplay(User.GetPassword());
        std::cout << "\nPermissions : " << User.GetPermissions();
        std::cout << "\n___________________\n";

    }

public:

    static void ShowFindUserScreen()
    {

        _DrawScreenHeader("\t  Find User Screen");

        string UserName;
        UserName = Core::InputValidate::ReadString("\nPlease Enter UserName: ");
        while (!User::IsUserExist(UserName))
        {
            UserName = Core::InputValidate::ReadString("\nUser is not found, choose another one: ");
        }

        // --- credential gate: operator must know the target user's password ---
        string password = Core::InputValidate::ReadString("\nEnter password to authorize this operation: ");

        User User1 = User::Find(UserName);

        if (!User1.IsEmpty() && Bank::Users::UserService::VerifyUserPassword(User1, password))
        {
            std::cout << "\nUser Found :-)\n";
            _PrintUser(User1);
        }
        else
        {
            std::cout << "\nInvalid password. Operation denied.\n";
        }

    }

};
