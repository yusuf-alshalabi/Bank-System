#include <iostream> 
#include "../BankSystem/Screens/LoginScreen.h"

int main()
{
    while (true) {
        if (!LoginScreen::ShowLoginScreen()) {
            break;
        }
    }

    return 0;
}