#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include <iomanip>

class UsersListScreen :protected Screen
{

private:
    static void _PrintUserRecordLine(User User)
    {

        std::cout << std::setw(8) << std::left << "" << "| " << std::setw(12) << std::left << User.GetUserName();
        std::cout << "| " << std::setw(25) << std::left << User.FullName();
        std::cout << "| " << std::setw(12) << std::left << User.GetPhone();
        std::cout << "| " << std::setw(20) << std::left << User.GetEmail();
        std::cout << "| " << std::setw(10) << std::left << User.GetPassword();
        std::cout << "| " << std::setw(12) << std::left << User.GetPermissions();

    }

public:

    static void ShowUsersList()
    {
        vector <User> vUsers = User::GetUsersList();

        string Title = "\t  User List Screen";
        string SubTitle = "\t    (" + to_string(vUsers.size()) + ") User(s).";

        _DrawScreenHeader(Title, SubTitle);

        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "______________________________________________\n" << endl;

        std::cout << std::setw(8) << std::left << "" << "| " << std::left << std::setw(12) << "UserName";
        std::cout << "| " << std::left << std::setw(25) << "Full Name";
        std::cout << "| " << std::left << std::setw(12) << "Phone";
        std::cout << "| " << std::left << std::setw(20) << "Email";
        std::cout << "| " << std::left << std::setw(10) << "Password";
        std::cout << "| " << std::left << std::setw(12) << "Permissions";
        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "______________________________________________\n" << endl;

        if (vUsers.size() == 0)
            std::cout << "\t\t\t\tNo Users Available In the System!";
        else

            for (User User : vUsers)
            {

                _PrintUserRecordLine(User);
                std::cout << endl;
            }

        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "______________________________________________\n" << endl;
    }

};

