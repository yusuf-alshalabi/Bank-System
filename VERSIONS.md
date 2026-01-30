# 📋 Version History - CRUD Bank System

Complete changelog and feature details for all versions.

---

## Version 1.4.0 — Transaction Management System (January 2026)  [v1.4.0][v140]

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
- Transaction ID format: `TXN` + counter + Unix timestamp
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

#### New Functions
- `generateTransactionID()` – Creates unique IDs
- `createDepositTransaction()` – Logs deposit operations
- `createWithdrawTransaction()` – Logs withdrawal operations
- `createTransferTransaction()` – Logs transfer operations
- `saveTransactionToFile()` – Persists transactions
- `loadTransactionsFromFile()` – Retrieves transaction history
- `convertTransactionRecordToLine()` – Serialization
- `convertLineToTransactionRecord()` – Deserialization
- `showTransactionsHistory()` – Display transaction history
- `validateTransferAccounts()` – Account validation for transfers
- `validateTransferAmount()` – Amount and balance validation
- `executeTransfer()` – Atomic transfer execution
- `showTransferScreen()` – Transfer UI with confirmations

#### File Structure Updates
- New file: `Transactions.txt`
- Transaction record format: `ID#//#Type#//#FromAcc#//#ToAcc#//#Amount#//#Fees#//#Timestamp#//#Desc`

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
- `getCurrentUsernameSafe()` – Safe OS username detection
- `getLocalAppDataPath()` – Platform-aware app data path
- `getSessionPath()` / `getSessionFolder()` – Session file locations
- `createSessionFolder()` – Directory creation with permissions
- `generateEncryptionKey()` – Cryptographic key generation
- `getEncryptionKey()` – Key retrieval or creation
- `encryptData()` – XChaCha20-Poly1305 encryption
- `decryptData()` – Decryption with integrity check
- `saveCurrentUserSession()` – Encrypt and save session
- `loadCurrentUserSession()` – Load and decrypt session
- `clearCurrentUserSession()` – Secure session cleanup

#### Dependencies Added
- **Libsodium** library (version 1.0.18+)
- Platform-specific headers (`windows.h` for Windows)

#### File Structure
- Session files: `.bsess` format (encrypted binary)
- Encryption keys: Hidden system files
- Platform-aware directory structure

### 📈 Improvements
- Eliminated plain-text session storage
- Enhanced security with industry-standard encryption
- Cross-platform session management
- Better user experience with auto-resume

---

## Version 1.2.0 — User Management & RBAC (October 2025)  [v1.2.0][v120]

### 🎯 Main Focus
Complete **User Management System** with Role-Based Access Control (RBAC).

### ✨ New Features

#### 👥 User Management
- **User CRUD Operations:**
  - Add new users with custom permissions
  - Delete existing users
  - Update user information
  - Find users by username
  - List all system users
- Default admin account auto-created on first run
  - Username: `Admin`
  - Password: `1234`

#### 🔐 Password Security
- Password hashing using `std::hash`
- Minimum 4-character password requirement
- No plain-text password storage
- Hexadecimal hash representation

#### 🛡 Role-Based Access Control (RBAC)
- **Bitwise permission system** for granular control
- 7 permission levels + admin:
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
- `convertUserRecordToLine()` / `convertLineToUserRecord()` – Serialization
- `findUserByUserName()` – User lookup
- `checkUserPassword()` – Password verification
- `readPermissionsToSet()` – Permission configuration
- `showAddUserScreen()` / `showDeleteUserScreen()` / `showUpdateUserScreen()` / `showFindUserScreen()` – User management UI
- `showManageUsersScreen()` – User management menu
- `checkAccessPermission()` – Permission validation
- `buildMainMenuOptions()` – Dynamic menu builder
- `login()` – Authentication system
- `createDefaultAdmin()` – Default admin creation

#### File Structure
- New file: `Users.txt`
- User record format: `Username#//#HashedPassword#//#Permissions`

### 📈 Improvements
- Multi-user support with individual access levels
- Enhanced security with password hashing
- Flexible permission combinations
- Professional access control system

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
- Summary statistics

#### 🎛 Transactions Menu
- Dedicated interface for financial operations
- Clear navigation between operations
- Return to main menu option

### 🔧 Technical Additions

#### New Functions
- `depositBalanceToClient()` – Deposit operation
- `showDepositScreen()` – Deposit UI
- `withdrawBalanceToClient()` – Withdrawal with validation
- `showWithdrawScreen()` – Withdrawal UI
- `showTotalBalancesReport()` – Balance reporting
- `ManageTransactions()` – Transaction menu loop

#### Validations Added
- Positive amount validation
- Balance sufficiency checks
- Prevent overdraft scenarios

### 📈 Improvements
- Enhanced user interface
- Currency formatting (2 decimal places)
- Better error handling
- Clear financial operation flow

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

enum MainMenuOption {
    ShowClientList = 1,
    AddNewClient = 2,
    DeleteClient = 3,
    UpdateClient = 4,
    FindClient = 5,
    Exit = 6
};
```

#### Core Functions
- `splitString()` – String tokenization
- `convertClientRecordToLine()` / `convertLineToClientRecord()` – Serialization
- `loadClientsDataFromFile()` / `saveClientsToFile()` – File I/O
- `appendLineToFile()` – Append operation
- `readClientData()` – Client input
- `readPositiveDouble()` / `readNonEmptyString()` – Input validation
- `findClientByAccountNumber()` – Search (returns pointer)
- `markClientForDelete()` – Soft delete
- `showAllClientsReport()` – Table display
- `addClient()` / `deleteClient()` / `updateClientByAccountNumber()` – CRUD
- `showScreenHeader()` – UI headers
- `confirm()` – User confirmations
- `readOption()` – Menu input
- `clearScreen()` / `customPause()` – UI utilities

#### Architecture Principles
- **Procedural Programming** approach
- **Single Responsibility Principle** – one function, one task
- **Pass-by-reference** for efficiency
- **Pointer-based search** for in-memory modification

### 📈 Foundation Established
- Clean code structure
- Efficient memory management
- Robust input handling
- Professional UI design

---

## 🎯 Feature Evolution Summary

| Version | Major Addition | Impact |
|:--------|:--------------|:-------|
| **v1.0.0** | Client CRUD | Foundation established |
| **v1.1.0** | Financial Transactions | Banking operations enabled |
| **v1.2.0** | User Management + RBAC | Multi-user security |
| **v1.3.0** | Session Encryption | Enterprise-grade security |
| **v1.4.0** | Transaction Management | Complete audit trail |

---

## 📊 Cumulative Feature Count

- **Total Structs:** 3 (strClient, strUser, Transaction)
- **Total Enums:** 4 (MainMenuOption, TransactionsOptions, ManageUsersOptions, Permissions, TransactionType)
- **Total Functions:** 80+ (across all modules)
- **Data Files:** 3 (Clients.txt, Users.txt, Transactions.txt)
- **External Dependencies:** 1 (Libsodium)

---

## 🚀 Future Roadmap

### 📅 Next Release: Version 2.0.0 — The OOP Transformation

- ​After mastering the procedural logic and security foundations in the 1.x series, the next major milestone will be a complete Architectural Refactor:
- **​Paradigm Shift:** Moving from Procedural to Object-Oriented Programming (OOP).
- **​Class Hierarchy:** Implementing a robust inheritance system (e.g., clsPerson → clsUser & clsClient).
- ​**Encapsulation:** Protecting sensitive financial data using private members and public getters/setters.
- ​**Abstraction & Polymorphism:** Streamlining bank operations through abstract classes and interfaces.
- ​**Advanced Memory Management:** Utilizing Smart Pointers (std::unique_ptr, std::shared_ptr) for modern C++ standards.

---

**Project:** CRUD Bank System  
**Developer:** Yusuf Zakaria Alshalabi  
**Part of:** Programming Advices Roadmap – Course 7

[v140]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.4.0
[v130]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.3.0
[v120]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.2.0
[v110]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.1.0
[v100]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.0.0