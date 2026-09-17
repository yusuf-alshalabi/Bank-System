#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include "../../Core/Infrastructure/PasswordPolicy.h"
#include "../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

class AddNewUserScreen : protected Screen
{
private:

    static void _ReadUserInfo(User& User)
    {
        User.FirstName = Core::InputValidate::ReadString("\nEnter First Name: ");

        User.LastName = Core::InputValidate::ReadString("\nEnter Last Name: ");

        User.Email = Core::InputValidate::ReadString("\nEnter Email: ");

        User.Phone = Core::InputValidate::ReadString("\nEnter Phone: ");

        User.Password = Bank::Security::PasswordPolicy::ReadPassword("\nEnter Password: ");

        User.Permissions = _ReadPermissionsToSet();
    }

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

    static int _ReadPermissionsToSet()
    {
        int Permissions = 0;

        if (Core::InputValidate::ReadYesNoOption("\nDo you want to give full access? "))
        {
            return -1;
        }

        std::cout << "\nDo you want to give access to: ";

        if (Core::InputValidate::ReadYesNoOption("\nShow Client List? "))
        {
            Permissions += User::enPermissions::pListClients;
        }

        if (Core::InputValidate::ReadYesNoOption("\nAdd New Client? "))
        {
            Permissions += User::enPermissions::pAddNewClient;
        }

        if (Core::InputValidate::ReadYesNoOption("\nDelete Client? "))
        {
            Permissions += User::enPermissions::pDeleteClient;
        }

        if (Core::InputValidate::ReadYesNoOption("\nUpdate Client? "))
        {
            Permissions += User::enPermissions::pUpdateClients;
        }

        if (Core::InputValidate::ReadYesNoOption("\nFind Client? "))
        {
            Permissions += User::enPermissions::pFindClient;
        }

        if (Core::InputValidate::ReadYesNoOption("\nTransactions? "))
        {
            Permissions += User::enPermissions::pTranactions;
        }

        if (Core::InputValidate::ReadYesNoOption("\nManage Users? "))
        {
            Permissions += User::enPermissions::pManageUsers;
        }

        if (Core::InputValidate::ReadYesNoOption("\nLogin/Register? "))
        {
            Permissions += User::enPermissions::pLoginRegister;
        }

        return Permissions;
    }

public:

    static void ShowAddNewUserScreen()
    {
        if (!CheckAccessRights(User::enPermissions::pManageUsers))
        {
            return;
        }

        _DrawScreenHeader("Add New User Screen");

        std::string UserName = "";
        UserName = Core::InputValidate::ReadString("\nPlease enter UserName (or 0 to Back): ");
        if (UserName == "0")
        {
            return;
        }

        while (User::IsUserExist(UserName))
        {
            UserName = Core::InputValidate::ReadString("\nUserName is already used, choose another one (or 0 to Back): ");
            if (UserName == "0")
            {
                return;
            }
        }

        User NewUser = User::GetAddNewUserObject(UserName);

        _ReadUserInfo(NewUser);

        User::enSaveResults SaveResult = NewUser.Save();

        switch (SaveResult)
        {
        case User::enSaveResults::svSucceeded:
            _ShowSuccessMessage("User added successfully.");
            _PrintUser(NewUser);
            break;

        case User::enSaveResults::svFaildEmptyObject:
            _ShowErrorMessage("User was not saved because it's empty.");
            break;

        case User::enSaveResults::svFaildUserExists:
            _ShowErrorMessage("User was not saved because the UserName is already used.");
            break;
        }
    }

};