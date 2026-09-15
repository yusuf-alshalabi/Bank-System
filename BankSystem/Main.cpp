#include <iostream> 
#include "../BankSystem/Screens/Auth/LoginScreen.h"
#include "../BankSystem/Screens/MainScreen.h"
#include "../BankSystem/Core/Infrastructure/SessionManager.h"
#include "../BankSystem/Core/Infrastructure/DataMigrator.h"
#include "../BankSystem/Core/Services/AuthService.h"

int main()
{
    // One-time upgrade path: re-hash any legacy Caesar-cipher passwords to Argon2id.
    Bank::Data::DataMigrator::MigrateLegacyUsersPasswordStorage();

    // First-run: bootstrap an initial full-access admin when no users exist.
    Bank::Auth::AuthService::EnsureFirstRunAdmin();

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