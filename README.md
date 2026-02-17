# 🏦 CRUD Bank System — Version 1.4.0

> ⚠️ **PRODUCTION WARNING**  
> This is an **educational project** demonstrating procedural programming concepts.  
> **NOT RECOMMENDED FOR PRODUCTION USE**.  
> For production banking software, use established frameworks with proper security audits.

---

A professional console-based Bank Management System built in C++ as part of the Programming Advices Roadmap (Course 7).

This version introduces a comprehensive **Transaction Management System** with money transfers, complete audit trail, and transaction history tracking.

---

## ✨ Features

### 🔧 Client Management
- **View All Clients** – Display all clients in a formatted table
- **Add New Client** – Add a client with a unique account number
- **Delete Client** – Remove a client by account number
- **Update Client Info** – Modify client details
- **Find Client** – Search for a client and display full information

### 💰 Financial Transactions
- **Deposit** – Add funds to a client's account
- **Withdraw** – Withdraw funds with full balance validation
- **Transfer** – Send money between accounts with automatic fee calculation (1%)
- **Total Balances** – Display all balances with a grand total
- **Transaction History** – View complete transaction log for any account

### 📊 Transaction Management System (NEW in v1.4.0)
- **Complete Audit Trail** – Every deposit, withdrawal, and transfer is logged
- **Unique Transaction IDs** – Format: TXN + timestamp + random component
- **Fee Management** – Automatic 1% fee calculation for transfers
- **Transaction Records** – Stored persistently in `Transactions.txt`
- **History Reports** – Detailed transaction history per account

### 👥 User Management & Access Control
- **User CRUD Operations** – Add, Delete, Update, Find system users
- **Role-Based Access Control (RBAC)** – Bitwise permission flags for granular access
- **Dynamic Menus** – Menu adapts based on user permissions
- **Password Hashing** – Secure password storage using Libsodium Argon2id hashing
- **First-Time Setup** – In v1.4.0, no default credentials are created. The system forces you to create a secure administrator account on first run with a strong password policy (minimum 8 characters, must include uppercase, lowercase, and a digit).

### 🔒 Security & Session Management
- **Binary Session Encryption** – XChaCha20-Poly1305 using Libsodium
- **Secure Key Storage** – OS-protected encryption keys
- **Auto Session Resume** – Seamless login experience
- **Secure Logout** – 3-pass random overwrite before deletion

### 💾 Data Management
- **Data Persistence** – All client data stored in `Clients.txt`
- **User Credentials** – Stored in `Users.txt` with hashed passwords
- **Transaction Logs** – Complete history in `Transactions.txt`
- **Real-Time Updates** – Instant file updates after modifications
- **Automatic File Creation** – Generates data files if not exists

### 📜 Logging System (NEW in v1.4.0)
- Logs all system activities in `SystemLog.txt`
- Levels: **INFO, WARNING, ERROR, CRITICAL**
- Context-aware: includes current user and timestamp
- Transaction logs automatically generated
- Functions:
  - `logMessage()` – General logging
  - `logTransaction()` – Logs financial operations
  - `logLoginAttempt()` – Records login success/failure
  - `logUserAction()` – Tracks user actions

### 🎨 Enhanced User Interface (NEW in v1.4.0)
- **Color-coded messages** (success, error, info) using ANSI escape codes
- **Centered headers** and **bordered separators** for professional console output
- **Dynamic console width detection** with `getConsoleWidth()`
- **Utility functions**:
  - `showLine()` – Separator lines
  - `showBorderLine()` – Bordered lines
  - `printCentered()` – Centered text display
- Clear navigation prompts (`pressEnterToContinue()`, `backToMenu()`)

### 🛡 Defensive Programming & Error Handling (NEW in v1.4.0)
- Validates file size and format before reading
- Prevents corrupted or tampered session files from loading
- Secure overwrite of session files (3-pass random data) before deletion
- Silent fail design in logging (errors don’t disrupt program flow)
- Comprehensive input validation for transactions and user actions

### 🛡 Data Backup & Atomic Save (NEW in v1.4.0)
- Automatic backup files (`Clients.bak`, `Users.bak`) are created before any overwrite.
- Atomic Save process:
  1. Data is first written to a temporary `.tmp` file.
  2. If the original file exists, it is copied to a `.bak` backup.
  3. The `.tmp` file is then renamed to replace the original file atomically.
- This guarantees recovery in case of unexpected errors or crashes and prevents corruption.
- Demonstrates Defensive Programming principles by ensuring data durability and safe recovery.
---

## 📊 Version Comparison

| Feature | v1.0 | v1.1 | v1.2 | v1.3 | v1.4 |
|---------|:----:|:----:|:----:|:----:|:----:|
| Client CRUD | ✅ | ✅ | ✅ | ✅ | ✅ |
| Deposit/Withdraw | ❌ | ✅ | ✅ | ✅ | ✅ |
| Total Balances Report | ❌ | ✅ | ✅ | ✅ | ✅ |
| User Management | ❌ | ❌ | ✅ | ✅ | ✅ |
| RBAC Permissions | ❌ | ❌ | ✅ | ✅ | ✅ |
| Session Encryption | ❌ | ❌ | ❌ | ✅ | ✅ |
| Money Transfers | ❌ | ❌ | ❌ | ❌ | ✅ |
| Transaction History | ❌ | ❌ | ❌ | ❌ | ✅ |
| Password Hashing | ❌ | ❌ | ⚠️ Weak | ⚠️ Weak | ✅ Argon2id |
| Transaction Logging | ❌ | ❌ | ❌ | ❌ | ✅ |
| System Logging | ❌ | ❌ | ❌ | ❌ | ✅ |
| Color-coded UI | ❌ | ❌ | ❌ | ❌ | ✅ |
| Data Loss Protection | ❌ | ❌ | ❌ | ❌ | ✅ |

**Legend:**  
✅ Fully Implemented | ⚠️ Implemented (Weak/Insecure) | ❌ Not Available

**Security Note:** v1.2.0 and v1.3.0 used `std::hash` for password hashing, which is NOT cryptographically secure. Only v1.4.0 uses proper Argon2id hashing.

---

## ⚙ How to Run

### Prerequisites
- C++ compiler (g++ recommended)
- C++11 standard or later
- **Libsodium** library installed and linked

### Installation & Build

1. **Install Libsodium**

   **Ubuntu/Debian:**
   ```bash
   sudo apt update && sudo apt install libsodium-dev
   ```

   **macOS:**
   ```bash
   brew install libsodium
   ```

   **Windows:**
   ```bash
   vcpkg install libsodium
   ```

2. **Compile the program**
   ```bash
   g++ -o BankSystem BankSystem.cpp -std=c++11 -lsodium
   ```

3. **Run the executable**
   ```bash
   ./BankSystem        # Linux / macOS
   BankSystem.exe      # Windows
   ```

4. The system starts automatically with main menu and loads/creates data files

### 📦 Libsodium Installation

For detailed setup instructions, see the full guide here:  
[Libsodium Setup Instructions](LIBSODIUM_SETUP.md)

### 🔐 First-Time Setup

#### Important Version Differences

**v1.4.0 (Current - Secure):**
- ✅ System forces you to create administrator account on first run
- ✅ No default credentials - you choose username and password
- ✅ Password immediately hashed with **Argon2id** (cryptographically secure)
- ✅ Strong password policy: minimum 8 characters, must include uppercase, lowercase, and a digit (recommend adding special characters for production).

**v1.2.0 & v1.3.0 (Previous - Insecure):**
- ❌ Created default admin: Username `Admin`, Password `1234`
- ❌ Used weak hashing (`std::hash` - NOT cryptographically secure)
- ❌ Weak password policy (minimum 4 characters only)
- ❌ **Do NOT use these versions in any real scenario**

#### First Run Process (v1.4.0)

1. Program detects no users exist in `Users.txt`
2. Prompts you to create first administrator account
3. Enter desired username (minimum 4 characters)
4. Enter password (minimum 8 characters, must include uppercase, lowercase, and a digit; special characters recommended for stronger security)
5. Password is immediately hashed using Argon2id
6. Account created with full permissions (pAll = 127)

---

## 💻 Code Quality & Architecture

This project follows **Procedural Programming** with emphasis on clean code, efficiency, and maintainability.

### 🏗 Procedural Architecture & Clean Code
- **Modular Design** – Clear separation between client management, transactions, and user management
- **Single Responsibility Principle** – Each function has one specific task
- **Readable Code** – Meaningful names, constants, intuitive menus
- **Separation of Concerns** – UI, file handling, and data processing isolated

### 🔧 Professional C++ Usage
- **Structs** – `strClient`, `strUser`, `Transaction` organize related data
- **Enums** – Menu options and permission flags eliminate magic numbers
- **Pointers** – `strClient*` enables direct in-memory editing
- **Input Validation** – Prevents negative amounts, overdrafts, invalid input
- **Cross-Platform** – Works on Windows, Linux, and macOS

### ⚡ Performance & Efficiency
- **Pointer-based retrieval** avoids data copying
- **Pass-by-reference** for large structures
- **Real-time data sync** ensures consistency
- **Balance protection** prevents overdrafts
- **Efficient file I/O** with minimal overhead

### 🎯 User Experience
- **Multi-menu system** for clients, transactions, and users
- **Intuitive navigation** with clear screen flow
- **Financial reporting** with total balances
- **Transaction confirmations** with detailed summaries
- **Error handling** with helpful messages

---

## 🚀 What's New in Version 1.4.0

### Major Features Added

#### 🔄 Money Transfer System
- Transfer funds between any two client accounts
- Automatic fee calculation (1% of transfer amount)
- Dual balance update (debit from sender, credit to receiver)
- Complete validation (account existence, sufficient balance)
- Detailed confirmation messages

#### 📝 Transaction Logging
- Every financial operation is logged with unique ID
- Transaction types: Deposit (1), Withdrawal (2), Transfer (3)
- Persistent storage in `Transactions.txt`
- Complete audit trail for compliance

#### 📊 Transaction History
- View all transactions for any account
- Detailed report showing: ID, Type, Accounts, Amount, Fees, Timestamp
- Filter transactions by account number
- Professional table formatting

#### 💡 Enhanced Transaction Screens
- Deposit screen now logs transactions
- Withdrawal screen with enhanced confirmations
- New transfer screen with fee breakdown
- Success messages include transaction IDs

### Technical Improvements
- New `Transaction` struct with 8 fields
- `TransactionType` enum for type safety
- Transaction ID generation algorithm
- File serialization for transactions
- Enhanced error handling for transfers

#### 📜 Logging System
- Introduced `SystemLog.txt` for complete audit trail
- Supports multiple log levels (INFO, WARNING, ERROR, CRITICAL)
- Logs include timestamp and current user context

#### 🎨 UI Enhancements
- Added ANSI color codes for success/error/info messages
- Centered headers and bordered separators for better console formatting
- Dynamic console width detection for adaptive layouts

#### 🛡 Defensive Programming & Error Handling
- Added validation for file size and format before reading
- Prevents loading of corrupted or tampered session files
- Secure 3-pass overwrite of session files before deletion
- Logging system designed with silent fail (errors do not disrupt program flow)
- Improved input validation for transactions and user actions

---

## 📊 Transaction Structure

```cpp
struct Transaction {
    string TransactionID;       // Format: TXN + timestamp + random component
    TransactionType Type;       // DEPOSIT (1) / WITHDRAWAL (2) / TRANSFER (3)
    string FromAccount;         // Source account number
    string ToAccount;           // Destination account number
    double Amount;              // Transaction amount
    double Fees;                // Transfer fees (0 for deposit/withdraw)
    string Timestamp;           // YYYY-MM-DD HH:MM:SS
    string Description;         // Transaction description
};
```

---

## 🔐 Security Architecture

| Feature Area | Implementation Details |
| :--- | :--- |
| **Password Security** | Libsodium Argon2id hashing, strong policy: minimum 8 characters, must include uppercase, lowercase, and a digit; Argon2id chosen for memory-hard resistance against brute-force attacks |
| **Session Security** | Encryption (Libsodium XChaCha20-Poly1305), OS-protected key storage, 3-pass secure overwrite |
| **Transaction Security** | Unique transaction IDs, timestamp validation, balance verification |
| **Access Control** | Bitwise permission flags (7 levels + admin), dynamic menu system |
| **Data Protection** | Input validation, bounds checking, file corruption detection |

---

## 🛡 Bitwise Permissions

```cpp
enum Permissions {
    pListClients    = 1,    // 0000001
    pAddClient      = 2,    // 0000010  
    pDeleteClient   = 4,    // 0000100
    pUpdateClient   = 8,    // 0001000
    pFindClient     = 16,   // 0010000
    pTransactions   = 32,   // 0100000
    pManageUsers    = 64,   // 1000000
    pAll            = 127   // 1111111 (Full Access) 
};
```

---

## 💡 Usage Example: Money Transfer

```
1. Login to the system
2. Main Menu → Select [6] Transactions
3. Transactions Menu → Select [3] Transfer
4. Enter your account number: A150
5. Enter recipient account number: A151
6. Enter transfer amount: 1000

System displays:
- Transfer Amount: $1,000.00
- Transfer Fee (1%): $10.00
- Total Deducted: $1,010.00

7. Confirm: Yes

Result:
✓ Transaction ID: TXN10011738425600
✓ A150 balance: $5,000 → $3,990
✓ A151 balance: $2,000 → $3,000
✓ Transaction logged to Transactions.txt
```

---

## 📂 File Structure

### Data Files
- **Clients.txt** – Client account information
- **Users.txt** – User credentials and permissions
- **Transactions.txt** – Complete transaction history (NEW in v1.4.0)

### Session Files (Hidden)
- **Windows:** `%LOCALAPPDATA%\BankSystem\session_username.bsess`
- **Linux/macOS:** `~/.config/BankSystem/session.bsess`

---

## 📅 Project Context

This project represents **Version 1.4.0** of the CRUD Bank System, demonstrating mastery of:
- Advanced file-based data management
- Financial transaction processing
- Complete audit trail implementation
- Cross-platform C++ development

**Part of:** Programming Advices Roadmap – Course 7

---

## 🏆 Version History

For detailed information about each version and changes, see **[VERSIONS.md](VERSIONS.md)**

| Version | Key Features |
| :--- | :--- |
| [v1.0.0][v100] | Basic Client CRUD operations |
| [v1.1.0][v110] | Financial Transactions (Deposit & Withdraw) |
| [v1.2.0][v120] | User Management + RBAC System |
| [v1.3.0][v130] | Binary Session Encryption (Libsodium) |
| [v1.4.0][v140] | Transaction Management + Money Transfers ⭐ |

---

## 🤝 Contribution

Contributions, bug reports, and suggestions for future enhancements are highly welcome.
Feel free to fork the repository, submit pull requests, or open issues.

---

## 📜 License

**[MIT License](LICENSE.txt)**

---

**Built with:** C++ | Libsodium | File-based Persistence  
**Developer:** Yusuf Zakaria Alshalabi  
**Version:** 1.4.0  
**Last Update:** February 2026

[v140]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.4.0
[v130]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.3.0
[v120]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.2.0
[v110]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.1.0
[v100]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.0.0