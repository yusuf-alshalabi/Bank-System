#pragma once
#include <iostream>
#include <string>
#include"../Core/User.h"
#include "../../Libs/Cpp-Library-Collection/Lib/Date.h"
#include "Global.h"
class Screen
{
protected:
    static void _DrawScreenHeader(std::string Title, std::string SubTitle = "")
    {
        std::cout << "\t\t\t\t\t______________________________________";
        std::cout << "\n\n\t\t\t\t\t  " << Title;
        if (SubTitle != "")
        {
            std::cout << "\n\t\t\t\t\t  " << SubTitle;
        }
        std::cout << "\n\t\t\t\t\t______________________________________\n";
        std::cout << "\n\t\t\t\t\tUser: " << CurrentUser.UserName << "\n";
        std::cout << "\t\t\t\t\tDate: " << Core::Date::ToString(Core::Date())
            << "\n\n";
    }

    static bool CheckAccessRights(User::enPermissions Permission)
    {

        if (!CurrentUser.CheckAccessPermission(Permission))
        {
            std::cout << "\t\t\t\t\t______________________________________";
            std::cout << "\n\n\t\t\t\t\t  Access Denied! Contact your Admin.";
            std::cout << "\n\t\t\t\t\t______________________________________\n\n";
            return false;
        }
        else
        {
            return true;
        }

    }

};