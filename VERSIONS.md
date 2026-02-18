# 📋 Version History - CRUD Bank System

Complete changelog and feature details for all versions.

---

## Version 1.4.1 — Modular Header Split (February 2026)  [v1.4.1][v141]

### 🎯 Main Focus
**Pure structural refactor** — splitting the monolithic `BankSystem.cpp` into modular header files as a direct bridge toward the upcoming **v2.0.0 OOP refactor**.

> ⚠️ **No logic changes.** All features, behavior, and output are identical to v1.4.0.  
> This version is architecture-only.

### ✨ What Changed

#### 🗂 Modular File Structure (NEW)
The single `BankSystem.cpp` file has been split into 14 files — one header per logical section:

| File | Section | Future Class (v2.0.0) |
|:-----|:--------|:----------------------|
| `Main.cpp` | Entry point + ordered includes | `main()` |
| `Globals.h` | Structs, Enums, Constants, Forward Declarations | Shared types |
| `Utilities.h` | Formatting, UI helpers, screen control | `clsUtilities` |
| `Crypto.h` | Encryption & Decryption (libsodium) | `clsCrypto` |
| `Session.h` | Session save / load / clear | `clsSession` |
| `Logger.h` | Logging system | `clsLogger` |
| `FileManager.h` | File I/O, Serialization, Atomic save | `clsFileManager` |
| `InputManager.h` | Input reading & validation | `clsInputManager` |
| `PermissionManager.h` | Permission checks | `clsPermissionManager` |
| `ClientManager.h` | Client CRUD operations | `clsClientManager` |
| `TransactionManager.h` | Deposit / Withdraw / Transfer | `clsTransactionManager` |
| `UserManager.h` | User CRUD operations | `clsUserManager` |
| `AuthManager.h` | Login, Password hashing, Admin setup | `clsAuthManager` |
| `MenuManager.h` | All menus and navigation | `clsMenuManager` |

#### 🔧 Compilation Command Updated
```bash
# v1.4.0
g++ -o BankSystem BankSystem.cpp -std=c++11 -lsodium

# v1.4.1
g++ -o BankSystem Main.cpp -std=c++11 -lsodium
```

#### 📌 Design Decisions
- Header file names match future OOP class names to minimize friction in v2.0.0 migration
- `#pragma once` used in all headers to prevent duplicate inclusion
- `CurrentUser` declared `extern` in `Globals.h` and defined once in `Main.cpp`
- Forward declarations placed in `Globals.h` to resolve cross-file dependencies
- Include order in `Main.cpp` is explicit and deterministic

### 📈 Benefits
- Clear separation of concerns — each file has one responsibility
- Easier navigation and code review
- Directly mirrors the planned v2.0.0 class hierarchy
- Paves the way for OOP transformation with zero re-learning of logic

---

## Version 1.4.0 — Transaction Management System (February 2026)  [v1.4.0][v140]

### 🎯 Main Focus
Complete **Transaction Management System** with money transfers, audit trail, and transaction history.

### ✨ New Features

#### 🔄 Money Transfer System
- Transfer funds between any two client accounts
- Automatic fee calculation (default: 1% of transfer amount)
- Dual balance update (debit from sender, credit to receiver)
- Complete validation:
  - Source account existence
  - Destination account existence
  - Prevent self-transfers
  - Sufficient balance check (including fees)
- Detailed confirmation screens with fee breakdown

#### 📝 Transaction Logging System
- Every financial operation receives a unique Transaction ID
- Transaction ID format: `TXN` + timestamp + random component
- Complete audit trail for all operations
- Persistent storage in `Transactions.txt`
- Support for three transaction types:
  - **Deposit (Type 1)** – Add funds to account
  - **Withdrawal (Type 2)** – Remove funds from account
  - **Transfer (Type 3)** – Move funds between accounts

#### 📊 Transaction History
- View complete transaction history for any account
- Detailed report columns:
  - Transaction ID
  - Type (Deposit/Withdrawal/Transfer)
  - From Account
  - To Account
  - Amount
  - Fees
  - Timestamp (YYYY-MM-DD HH:MM:SS)
  - Description
- Professional table formatting
- Filter by account number

#### 💡 Enhanced Transaction Screens
- **Deposit Screen:**
  - Now creates transaction records
  - Displays Transaction ID upon completion
  - Shows before/after balance
- **Withdrawal Screen:**
  - Enhanced confirmation messages
  - Transaction logging
  - Detailed success feedback
- **New Transfer Screen:**
  - Step-by-step transfer process
  - Fee calculation display
  - Both account updates shown
  - Complete transaction details

### 🔧 Technical Additions

#### New Data Structures
```cpp
struct Transaction {
    string TransactionID;
    TransactionType Type;
    string FromAccount;
    string ToAccount;
    double Amount;
    double Fees;
    string Timestamp;
    string Description;
};

enum TransactionType {
    DEPOSIT = 1,
    WITHDRAWAL = 2,
    TRANSFER = 3
};
```
#### 📝 Logging System
- Introduced `SystemLog.txt` for complete audit trail
- Functions:
  - `logMessage()` – General logging
  - `logTransaction()` – Logs financial operations
  - `logLoginAttempt()` – Records login success/failure
  - `logUserAction()` – Tracks user actions
- Supports multiple log levels (INFO, WARNING, ERROR, CRITICAL)
- Silent fail design (logging errors do not disrupt program flow)

#### 🎨 UI Enhancements
- ANSI color codes for success/error/info messages
- `showLine()`, `showBorderLine()`, `printCentered()` for improved console formatting
- Dynamic console width detection with `getConsoleWidth()`
- Clear navigation prompts for smoother user experience

#### 🛡 Defensive Programming & Error Handling
- Added validation for file size and format before reading
- Prevents loading of corrupted or tampered session files
- Secure 3-pass overwrite of session files before deletion
- Logging system designed with silent fail (errors do not disrupt program flow)
- Improved input validation for transactions and user actions

#### 🛡 Data Backup System
- Introduced Atomic Save for `Clients.txt` and `Users.txt`
- Backup files (`.bak`) created automatically before overwriting
- Prevents data loss and supports recovery in case of file corruption

#### 🔐 First-Time Login (Updated)
- Removed default Admin account creation
- On first run, system forces user to create an administrator account
- Password stored securely using Libsodium Argon2id hashing
- Strong password policy enforced: minimum 8 characters, must include uppercase, lowercase, and a digit
- Enhances security by eliminating weak default credentials

#### New Functions

##### 🆕 Transaction Management System (10 functions)
- `generateTransactionID()` – Creates unique transaction IDs (TXN + timestamp + random)
- `createDepositTransaction()` – Creates deposit transaction record
- `createWithdrawTransaction()` – Creates withdrawal transaction record
- `createTransferTransaction()` – Creates transfer transaction record
- `validateTransferAmount()` – Validates transfer amount and fees
- `showTransferScreen()` – Display transfer UI and process transfer
- `showTransactionsHistory()` – Display complete transaction history for account
- `loadTransactionsFromFile()` – Load all transactions from Transactions.txt
- `saveTransactionToFile()` – Save single transaction to file
- `deserializeTransactionRecord()` – Convert file line to Transaction struct

##### 📜 Logging System (5 functions)
- `logLevelToString()` – Convert LogLevel enum to string
- `logMessage()` – Log message with timestamp, level, and user context
- `logTransaction()` – Log transaction details to SystemLog.txt
- `logLoginAttempt()` – Log authentication attempts (success/failure)
- `logUserAction()` – Log general user actions with details

##### 💾 Atomic File Operations (3 functions)
- `saveClientsToFileAtomic()` – Atomic save with backup (.tmp → .bak → rename)
- `saveUsersToFileAtomic()` – Atomic save for users with backup
- `validateFileBeforeLoad()` – Validate file exists, size, and format before loading

##### 🎨 UI Enhancements (11 functions)
- `formatDouble()` – Format double with precision
- `formatInt()` – Convert integer to string safely
- `formatCurrency()` – Format currency with $ sign
- `drawLine()` – Draw line without newlines
- `showLine()` – Separator line with newlines
- `showBorderLine()` – Bordered line (+ at start/end)
- `showBackOrExit()` – Display back/exit options dynamically
- `waitForEnter()` – Wait for Enter key
- `backToMenu()` – Return to menu prompt
- `getCurrentTimestamp()` – Get formatted date/time string (YYYY-MM-DD HH:MM:SS)
- `trim()` – Remove leading/trailing whitespace

##### ✅ Input Validation (4 functions)
- `isValidAccountNumber()` – Validate account number format
- `isValidPhoneNumber()` – Validate phone number format
- `readValidatedAccountNumber()` – Read and validate account number
- `readValidatedPhoneNumber()` – Read and validate phone number

##### 🔐 Authentication & Security (3 functions)
- `verifyPassword()` – Verify password against Argon2id hash
- `startSession()` – Initialize user session and save
- `countFullAccessUsers()` – Count users with pAll permission

#### File Structure Updates
- New file: `Transactions.txt`
- Transaction record format: `ID#//#Type#//#FromAcc#//#ToAcc#//#Amount#//#Fees#//#Timestamp#//#Desc`

#### 🔐 Security Enhancements
- Password hashing upgraded from `std::hash` (used in v1.2.0–v1.3.0) to **Libsodium Argon2id** in v1.4.0, providing modern, memory-hard, and secure password storage
- Permission constant `pAll` standardized: previously defined as `-1` in v1.2–v1.3, now set to `127` (binary 1111111) for consistency

### 📈 Improvements
- Real-time transaction logging
- Enhanced error messages for financial operations
- Atomic transfer operations (both accounts updated together)
- Comprehensive input validation for transfers
- Better user feedback with transaction confirmations

---

## Version 1.3.0 — Binary Session Encryption (December 2025)  [v1.3.0][v130]

### 🎯 Main Focus
Enterprise-grade **Session Management** with binary encryption using Libsodium.

### ✨ New Features

#### 🔒 Binary Session Encryption
- **Algorithm:** XChaCha20-Poly1305 (military-grade encryption)
- **Library:** Libsodium integration
- Encrypted session files stored as `.bsess` (binary format)
- Non-reversible encryption for session data

#### 🔑 Secure Key Management
- Cryptographically secure random key generation (32 bytes)
- OS-protected key storage
- Platform-specific key locations:
  - Windows: `%LOCALAPPDATA%\BankSystem\encryption_key.bsess`
  - Linux/macOS: `~/.config/BankSystem/.encryption_key.bsess`
- Hidden file attributes (Windows: Hidden + System)
- Restricted permissions (chmod 600 on Unix)

#### ⏱ Session Persistence
- Automatic session detection on startup
- Resume previous session option
- Seamless user experience
- Session data includes user credentials and permissions

#### 🛡 Secure Logout
- 3-pass random data overwrite
- Complete session file deletion
- Secure memory cleanup
- Prevents data recovery

### 🔧 Technical Additions

#### New Functions
- `getEncryptionKey()` – Load or generate encryption key
- `generateEncryptionKey()` – Create new random key
- `encryptData()` – XChaCha20-Poly1305 encryption
- `decryptData()` – Decryption with authentication
- `getSessionPath()` – Platform-specific session file path
- `getSessionFolder()` – Session folder path
- `createSessionFolder()` – Create session directory
- `saveCurrentUserSession()` – Encrypted session save
- `loadCurrentUserSession()` – Encrypted session load
- `clearCurrentUserSession()` – Secure session wipe
- `serializeUserData()` – Serialize user struct for storage
- `deserializeUserData()` – Deserialize user from storage

---

## Version 1.2.0 — User Management + RBAC (October 2025)  [v1.2.0][v120]

### 🎯 Main Focus
Multi-user support with **Role-Based Access Control (RBAC)**.

### ✨ New Features

#### 👥 User Management System
- Add, Delete, Update, Find system users
- Persistent storage in `Users.txt`
- Username-based lookup

#### 🔐 Authentication System
- Login screen on startup
- Password hashing (std::hash — upgraded in v1.4.0)
- Default admin: Username `Admin`, Password `1234`

#### 🛡 RBAC Permission System
- Bitwise permission flags:
  - `pListClients` (1) – View client list
  - `pAddClient` (2) – Add new clients
  - `pDeleteClient` (4) – Delete clients
  - `pUpdateClient` (8) – Update client info
  - `pFindClient` (16) – Search clients
  - `pTransactions` (32) – Financial operations
  - `pManageUsers` (64) – User management
  - `pAll` (-1) – Full admin access

#### 🎨 Dynamic Menu System
- Main menu adapts to user permissions
- Users only see options they can access
- Permission checks before each operation
- "Access Denied" messages for unauthorized actions

### 🔧 Technical Additions

#### New Data Structures
```cpp
struct strUser {
    string UserName;
    string Password;        // Hashed
    int Permissions;
    bool MarkForDelete;
};
```

#### New Functions
- `readPassword()` – Secure password input
- `hashPassword()` – Password hashing
- `loadUsersDataFromFile()` / `saveUsersToFile()` – User persistence
- `findUserByUserName()` – User lookup
- `checkUserPassword()` – Password verification
- `readPermissionsToSet()` – Permission configuration
- `showManageUsersScreen()` – User management menu
- `checkAccessPermission()` – Permission validation
- `buildMainMenuOptions()` – Dynamic menu builder
- `login()` – Authentication system
- `createDefaultAdmin()` – Default admin creation

#### File Structure
- New file: `Users.txt`
- User record format: `Username#//#HashedPassword#//#Permissions`

---

## Version 1.1.0 — Financial Transactions (August 2025)  [v1.1.0][v110]

### 🎯 Main Focus
Core **Financial Operations** for the banking system.

### ✨ New Features

#### 💰 Deposit System
- Add funds to client accounts
- Positive amount validation
- Real-time balance updates
- Confirmation messages with new balance

#### 💸 Withdrawal System
- Withdraw funds from accounts
- **Overdraft protection** – prevents negative balances
- Balance validation before withdrawal
- Detailed error messages for insufficient funds

#### 📊 Total Balances Report
- Display all client balances in table format
- Calculate and show grand total
- Professional formatting

### 🔧 Technical Additions

#### New Functions
- `depositBalanceToClient()` – Deposit operation
- `showDepositScreen()` – Deposit UI
- `withdrawBalanceToClient()` – Withdrawal with validation
- `showWithdrawScreen()` – Withdrawal UI
- `showTotalBalancesReport()` – Balance reporting
- `ManageTransactions()` – Transaction menu loop

---

## Version 1.0.0 — Initial Release (June 2025)  [v1.0.0][v100]

### 🎯 Main Focus
Foundation with **CRUD Operations** for client management.

### ✨ Core Features

#### 🏦 Client CRUD Operations
- **Create:** Add new clients with unique account numbers
- **Read:** View all clients in formatted table
- **Update:** Modify existing client information
- **Delete:** Remove clients with confirmation
- **Find:** Search clients by account number

#### 💾 Data Persistence
- File-based storage using `Clients.txt`
- Custom separator: `#//#`
- Real-time file updates
- Automatic file creation

### 🔧 Technical Foundation

#### Data Structures
```cpp
struct strClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete;
};
```

#### Core Functions
- `splitString()` – String tokenization
- `loadClientsDataFromFile()` / `saveClientsToFile()` – File I/O
- `appendLineToFile()` – Append operation
- `readClientData()` – Client input
- `findClientByAccountNumber()` – Search (returns pointer)
- `markClientForDelete()` – Soft delete
- `showAllClientsReport()` – Table display

---

## 🎯 Feature Evolution Summary

| Version | Major Addition | Impact |
|:--------|:--------------|:-------|
| **v1.0.0** | Client CRUD | Foundation established |
| **v1.1.0** | Financial Transactions | Banking operations enabled |
| **v1.2.0** | User Management + RBAC | Multi-user security |
| **v1.3.0** | Session Encryption | Enterprise-grade security |
| **v1.4.0** | Transaction Management | Complete audit trail |
| **v1.4.1** | Modular Header Split | Pre-OOP bridge |

---

## 📊 Cumulative Feature Count

- **Total Structs:** 3 (strClient, strUser, Transaction)
- **Total Enums:** 6 (TransactionType, MainMenuOption, TransactionsOption, UserManagementOption, Permission, LogLevel)
- **Total Functions:** 80+ (across all modules)
- **Source Files:** 14 (1 `.cpp` + 13 `.h`)
- **Data Files:** 3 (Clients.txt, Users.txt, Transactions.txt)
- **External Dependencies:** 1 (Libsodium)

---

## 🚀 Future Roadmap

### 📅 Next Release: Version 2.0.0 — The OOP Transformation

After mastering the procedural logic and security foundations in the 1.x series, the next major milestone will be a complete Architectural Refactor:

- **Paradigm Shift:** Moving from Procedural to Object-Oriented Programming (OOP)
- **Class Hierarchy:** Implementing a robust inheritance system (e.g., `clsPerson` → `clsUser` & `clsClient`)
- **Encapsulation:** Protecting sensitive financial data using private members and public getters/setters
- **Abstraction & Polymorphism:** Streamlining bank operations through abstract classes and interfaces
- **Advanced Memory Management:** Utilizing Smart Pointers (`std::unique_ptr`, `std::shared_ptr`) for modern C++ standards

> v1.4.1 header files map 1-to-1 with planned v2.0.0 classes — migration will be a direct conversion.

---

**Project:** CRUD Bank System  
**Developer:** Yusuf Zakaria Alshalabi  
**Part of:** Programming Advices Roadmap – Course 7

[v141]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.4.1
[v140]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.4.0
[v130]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.3.0
[v120]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.2.0
[v110]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.1.0
[v100]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.0.0
