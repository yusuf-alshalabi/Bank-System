#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
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

        User.Password = Core::InputValidate::ReadString("\nEnter Password: ");

        cout << "\nEnter Permission: ";
        User.Permissions = _ReadPermissionsToSet();
    }

    static void _PrintUser(User User)
    {
        cout << "\nUser Card:";
        cout << "\n___________________";
        cout << "\nFirstName   : " << User.GetFirstName();
        cout << "\nLastName    : " << User.GetLastName();
        cout << "\nFull Name   : " << User.FullName();
        cout << "\nEmail       : " << User.GetEmail();
        cout << "\nPhone       : " << User.GetPhone();
        cout << "\nUser Name   : " << User.GetUserName();
        cout << "\nPassword    : " << User.GetPassword();
        cout << "\nPermissions : " << User.GetPermissions();
        cout << "\n___________________\n";

    }

    static int _ReadPermissionsToSet()
    {

        int Permissions = 0;
        char Answer = 'n';

        if (Core::InputValidate::ReadYesNoOption("\nDo you want to give full access? y/n? "))
        {
            return -1;
        }

        cout << "\nDo you want to give access to : \n ";

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

        User User1 = User::Find(UserName);

        _PrintUser(User1);

        if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to update this User y/n? "))
        {

            cout << "\n\nUpdate User Info:";
            cout << "\n____________________\n";


            _ReadUserInfo(User1);

            User::enSaveResults SaveResult;

            SaveResult = User1.Save();

            switch (SaveResult)
            {
            case  User::enSaveResults::svSucceeded:
            {
                cout << "\nUser Updated Successfully :-)\n";

                _PrintUser(User1);
                break;
            }
            case User::enSaveResults::svFaildEmptyObject:
            {
                cout << "\nError User was not saved because it's Empty";
                break;

            }

            }

        }

    }
};

