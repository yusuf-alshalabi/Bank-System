#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include <iomanip>
#include <vector>
#include <string>

class UsersListScreen :protected Screen
{

private:
    static void _PrintUserRecordLine(const User& User)
    {
        std::cout << CYAN << "| "<< RESET << std::setw(12) << std::left << User.GetUserName();
        std::cout << CYAN << "| "<< RESET << std::setw(25) << std::left << User.FullName();
        std::cout << CYAN << "| "<< RESET << std::setw(12) << std::left << User.GetPhone();
        std::cout << CYAN << "| "<< RESET << std::setw(20) << std::left << User.GetEmail();
        std::cout << CYAN << "| "<< RESET << std::setw(25) << std::left << _FormatPasswordForDisplay(User.GetPassword());
        std::cout << CYAN << "| "<< RESET << std::setw(14) << std::left << User.GetPermissions();
        std::cout << CYAN << "|" << RESET;
    }

public:

    static void ShowUsersList()
    {
        if (!CheckAccessRights(User::enPermissions::pManageUsers))
        {
            return;
        }

        std::vector<User> vUsers = User::GetUsersList();

        _DrawScreenHeader("User List Screen", "(" + std::to_string(vUsers.size()) + ") User(s).");

        _ShowTableBorder(119);

        std::cout << CYAN << "| " << RESET << std::left << std::setw(12) << "UserName";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(25) << "Full Name";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(12) << "Phone";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(20) << "Email";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(25) << "Password";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(14) << "Permissions";
        std::cout << CYAN << "|\n"<< RESET ;

        _ShowTableBorder(119);

        if (vUsers.size() == 0)
        {
            std::cout << "\n  No Users Available In the System!\n";
        }
        else
        {
            for (const User& User : vUsers)
            {
                _PrintUserRecordLine(User);
                std::cout << "\n";
            }
        }

        _ShowTableBorder(119);
    }

};