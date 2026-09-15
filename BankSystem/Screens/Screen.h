#pragma once
#include <iostream>
#include <string>
#include"../Core/User.h"
#include "../Core/Infrastructure/SessionManager.h"
#include "../../Libs/Cpp-Library-Collection/Lib/Date.h"
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
        std::cout << "\n\t\t\t\t\tUser: " << Bank::Security::SessionManager::Instance().CurrentUserName() << "\n";
        std::cout << "\t\t\t\t\tDate: " << Core::Date::ToString(Core::Date())
            << "\n\n";
    }

    static bool CheckAccessRights(User::enPermissions Permission)
    {

        if (!Bank::Security::SessionManager::Instance().CanAccess(Permission))
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