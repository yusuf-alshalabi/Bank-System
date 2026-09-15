#include <iostream> 
#include "../BankSystem/Screens/Auth/LoginScreen.h"
#include "../BankSystem/Screens/MainScreen.h"
#include "../BankSystem/Core/Infrastructure/SessionManager.h"
#include "../BankSystem/Core/Infrastructure/DataMigrator.h"

int main()
{
    // One-time upgrade path: re-hash any legacy Caesar-cipher passwords to Argon2id.
    Bank::Data::DataMigrator::MigrateLegacyUsersPasswordStorage();

    while (true) {
        // Resume the previous encrypted session when one is still alive.
        if (Bank::Security::SessionManager::Instance().TryResume()) {
            MainScreen::ShowMainMenue();
        }
        else if (!LoginScreen::ShowLoginScreen()) {
            break;
        }
    }

    return 0;
}