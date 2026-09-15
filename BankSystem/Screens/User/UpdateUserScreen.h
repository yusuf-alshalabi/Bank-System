#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include "../../Core/Infrastructure/PasswordPolicy.h"
#include "../../Core/Services/UserService.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include <iomanip>

class UpdateUserScreen :protected Screen

{
private:

    static void _ReadUserInfo(User& User)
    {
        User.FirstName = Core::InputValidate::ReadString("\nEnter FirstName: ");

        User.LastName = Core::InputValidate::ReadString("\nEnter LastName: ");

        User.Email = Core::InputValidate::ReadString("\nEnter Email: ");

        User.Phone = Core::InputValidate::ReadString("\nEnter Phone: ");

        User.Password = Bank::Security::PasswordPolicy::ReadPassword("\nEnter Password: ");

        cout << "\nEnter Permission: ";
        User.Permissions = _ReadPermissionsToSet();
    }

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

    static int _ReadPermissionsToSet()
    {

        int Permissions = 0;
        char Answer = 'n';

        if (Core::InputValidate::ReadYesNoOption("\nDo you want to give full access? y/n? "))
        {
            return -1;
        }

        std::cout << "\nDo you want to give access to : \n ";

        if (Core::InputValidate::ReadYesNoOption("\nShow Client List? y/n? "))
        {
            Permissions += User::enPermissions::pListClients;
        }

        if (Core::InputValidate::ReadYesNoOption("\nAdd New Client? y/n? "))
        {
            Permissions += User::enPermissions::pAddNewClient;
        }

        if (Core::InputValidate::ReadYesNoOption("\nDelete Client? y/n? "))
        {
            Permissions += User::enPermissions::pDeleteClient;
        }

        if (Core::InputValidate::ReadYesNoOption("\nUpdate Client? y/n? "))
        {
            Permissions += User::enPermissions::pUpdateClients;
        }

        if (Core::InputValidate::ReadYesNoOption("\nFind Client? y/n? "))
        {
            Permissions += User::enPermissions::pFindClient;
        }

        if (Core::InputValidate::ReadYesNoOption("\nTransactions? y/n? "))
        {
            Permissions += User::enPermissions::pTranactions;
        }

        if (Core::InputValidate::ReadYesNoOption("\nManage Users? y/n? "))
        {
            Permissions += User::enPermissions::pManageUsers;
        }

        return Permissions;

    }

public:

    static void ShowUpdateUserScreen()
    {

        _DrawScreenHeader("\tUpdate User Screen");

        string UserName = "";

        UserName = Core::InputValidate::ReadString("\nPlease Enter User UserName: ");

        while (!User::IsUserExist(UserName))
        {
            UserName = Core::InputValidate::ReadString("\nAccount number is not found, choose another one: ");
        }

        // --- credential gate: operator must know the target user's password ---
        string password = Core::InputValidate::ReadString("\nEnter password to authorize this operation: ");
        if (!Bank::Users::UserService::VerifyUserPassword(User::Find(UserName), password))
        {
            std::cout << "\nInvalid password. Operation denied.\n";
            return;
        }

        User User1 = User::Find(UserName);

        _PrintUser(User1);

        if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to update this User y/n? "))
        {

            // capture full-access status before any change
            bool wasFullAccess = (User1.GetPermissions() == User::enPermissions::eAll);

            std::cout << "\n\nUpdate User Info:";
            std::cout << "\n____________________\n";


            _ReadUserInfo(User1);

            // --- last admin protection: cannot downgrade the sole full-access user ---
            int newPermissions = User1.GetPermissions();
            if (wasFullAccess && newPermissions != User::enPermissions::eAll
                && Bank::Users::UserService::IsLastFullAccessAdmin(User1.GetUserName()))
            {
                std::cout << "\nYou cannot remove full access from the last Admin user.\n";
                return;
            }

            User::enSaveResults SaveResult;

            SaveResult = User1.Save();

            switch (SaveResult)
            {
            case  User::enSaveResults::svSucceeded:
            {
                std::cout << "\nUser Updated Successfully :-)\n";

                _PrintUser(User1);
                break;
            }
            case User::enSaveResults::svFaildEmptyObject:
            {
                std::cout << "\nError User was not saved because it's Empty";
                break;

            }

            }

        }

    }
};
