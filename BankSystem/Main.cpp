

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  ||========================================================||
//  || File: Main.cpp                                         ||
//  || Entry point of the application.                        ||
//  ||                                                        ||
//  || File Structure:                                        ||
//  ||  - Globals.h            : Structs, Enums, Constants,   ||
//  ||                           Forward Declarations         ||
//  ||  - Utilities.h          : Format, UI, Screen helpers   ||
//  ||  - Crypto.h             : Encryption & Decryption      ||
//  ||  - Session.h            : Session Management           ||
//  ||  - Logger.h             : Logging System               ||
//  ||  - FileManager.h        : File I/O & Serialization     ||
//  ||  - InputManager.h       : Input reading & validation   ||
//  ||  - ClientManager.h      : Client CRUD operations       ||
//  ||  - TransactionManager.h : Deposit/Withdraw/Transfer    ||
//  ||  - UserManager.h        : User CRUD operations         ||
//  ||  - PermissionManager.h  : Permission checks            ||
//  ||  - AuthManager.h        : Login, Hashing, Admin setup  ||
//  ||  - MenuManager.h        : All menus and navigation     ||
//  ||========================================================||

// NOTE: Include order matters - each file depends on those above it.
// Globals must be first, MenuManager and AuthManager last.

#include "Globals.h"
#include "Utilities.h"
#include "Crypto.h"
#include "Session.h"
#include "Logger.h"
#include "FileManager.h"
#include "InputManager.h"
#include "PermissionManager.h"
#include "ClientManager.h"
#include "TransactionManager.h"
#include "UserManager.h"
#include "AuthManager.h"
#include "MenuManager.h"

//=====================================================
// Global variable definition (declared extern in Globals.h)
//=====================================================
strUser CurrentUser;

//=====================================================
//==================== Main Function ==================
//=====================================================

// Program entry point: initialize system, create admin, login, run menus
int main()
{
    cout << fixed << setprecision(2);

    if (sodium_init() < 0) {
        showErrorMessage("System initialization failed!");
        return 1;
    }

    try {
        createDefaultAdmin();
        login();
    }
    catch (const exception& e) {
        showErrorMessage("Critical system error: " + string(e.what()));
        return 1;
    }

    return 0;
}
