#include <iostream> 
#include "../BankSystem/Screens/Auth/LoginScreen.h"

int main()
{
    while (true) {
        if (!LoginScreen::ShowLoginScreen()) {
            break;
        }
    }

    return 0;
}