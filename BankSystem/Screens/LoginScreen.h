#pragma once

#include <iostream>
#include "Screen.h"
#include "../Core/User.h"
#include <iomanip>
#include "MainScreen.h"
#include "Global.h"

class LoginScreen :protected Screen
{

private:

    static  void _Login()
    {
        bool LoginFaild = false;

        string Username, Password;
        do
        {

            if (LoginFaild)
            {
                cout << "\nInvlaid Username/Password!\n\n";
            }

            cout << "Enter Username? ";
            cin >> Username;

            cout << "Enter Password? ";
            cin >> Password;

            CurrentUser = User::Find(Username, Password);

            LoginFaild = CurrentUser.IsEmpty();

        } while (LoginFaild);

        MainScreen::ShowMainMenue();

    }

public:


    static void ShowLoginScreen()
    {
        system("cls");
        _DrawScreenHeader("\t  Login Screen");
        _Login();

    }

};

