#include <iostream> 
#include "../BankSystem/Screens/LoginScreen.h"

int main()
{
    while (true) {
        if (!LoginScreen::ShowLoginScreen()) {
            break;
        }
    }
    system("pause>0");
    return 0;
}