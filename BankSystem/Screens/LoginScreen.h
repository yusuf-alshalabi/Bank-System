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

    static  bool _Login()
    {
        bool LoginFaild = false;
		int Attempts = 0;

        string Username, Password;
        do
        {
            if (LoginFaild)
            {
                Attempts++;
                std::cout << "\nInvlaid Username/Password!\n";
                std::cout << "You have " << 3 - Attempts << " attempts to login\n\n";
                if (Attempts == 3)
                {
                    std::cout << "\nYou are locked after 3 failed attempts.\n\n";
                    return false;
                }
            }

            std::cout << "Enter Username? ";
            std::cin >> Username;

            std::cout << "Enter Password? ";
            std::cin >> Password;

            CurrentUser = User::Find(Username, Password);

            LoginFaild = CurrentUser.IsEmpty();

        } while (LoginFaild);

        CurrentUser.AddLoginRegister();
        MainScreen::ShowMainMenue();
        return true;
    }

public:


    static bool ShowLoginScreen()
    {
        system("cls");
        _DrawScreenHeader("\t  Login Screen");
        return _Login();

    }

};

