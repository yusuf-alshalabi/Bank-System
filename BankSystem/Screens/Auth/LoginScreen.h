#pragma once

#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include "../../Core/Infrastructure/SessionManager.h"
#include "../../Core/Infrastructure/Logger.h"
#include "../MainScreen.h"

class LoginScreen :protected Screen
{

private:

    static  bool _Login()
    {
        bool LoginFaild = false;
        int Attempts = 0;

        std::string Username, Password;
        User FoundUser = User::Find("", "");
        do
        {
            if (LoginFaild)
            {
                Attempts++;
                Bank::Diagnostics::Logger::Instance().LogLoginAttempt(Username, false);
                std::cout << "\nInvalid Username/Password!\n";
                std::cout << "You have " << 3 - Attempts << " attempts to login\n\n";
                if (Attempts == 3)
                {
                    Bank::Diagnostics::Logger::Instance().Log(
                        Bank::Diagnostics::LogLevel::Critical,
                        "Account locked after 3 failed login attempts for user '" + Username + "'");
                    std::cout << "\nYou are locked after 3 failed attempts.\n\n";
                    return false;
                }
            }

            std::cout << "Enter Username? ";
            std::cin >> Username;

            std::cout << "Enter Password? ";
            std::cin >> Password;

            FoundUser = User::Find(Username, Password);

            LoginFaild = FoundUser.IsEmpty();

        } while (LoginFaild);

        Bank::Security::SessionManager::Instance().Start(FoundUser);
        Bank::Diagnostics::Logger::Instance().LogLoginAttempt(Username, true);
        FoundUser.RegisterLogIn();
        MainScreen::ShowMainMenue();
        return true;
    }

public:


    static bool ShowLoginScreen()
    {
        _ClearScreen();
        _DrawScreenHeader("Login Screen");
        return _Login();

    }

};

