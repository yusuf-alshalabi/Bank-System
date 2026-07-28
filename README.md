# 🏦 CRUD Bank System — Version 1.4.1

> ⚠️ **PRODUCTION WARNING**  
> This is an **educational project** demonstrating procedural programming concepts.  
> **NOT RECOMMENDED FOR PRODUCTION USE**.  
> For production banking software, use established frameworks with proper security audits.

---

A professional console-based Bank Management System built in C++ as part of the Programming Advices Roadmap (Course 7).

This version is a **structural refactor** of v1.4.0. The codebase has been reorganized into modular header files to improve readability, maintainability, and separation of responsibilities. No application logic was changed.

This modular structure also simplifies the upcoming v2.0.0 OOP refactor while keeping the future object-oriented design flexible as the project evolves. 

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

### 📊 Transaction Management System
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
- **First-Time Setup** – System forces you to create a secure administrator account on first run with a strong password policy (minimum 8 characters, must include uppercase, lowercase, and a digit).

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

### 📜 Logging System
- Logs all system activities in `SystemLog.txt`
- Levels: **INFO, WARNING, ERROR, CRITICAL**
- Context-aware: includes current user and timestamp
- Transaction logs automatically generated

### 🎨 Enhanced User Interface
- **Color-coded messages** (success, error, info) using ANSI escape codes
- **Centered headers** and **bordered separators** for professional console output
- Clear navigation prompts (`pressEnterToContinue()`, `backToMenu()`)

### 🛡 Defensive Programming & Error Handling
- Validates file size and format before reading
- Prevents corrupted or tampered session files from loading
- Secure overwrite of session files (3-pass random data) before deletion
- Silent fail design in logging (errors don't disrupt program flow)
- Comprehensive input validation for transactions and user actions

### 🛡 Data Backup & Atomic Save
- Automatic backup files (`Clients.bak`, `Users.bak`) created before any overwrite
- Atomic Save: write to `.tmp` → backup original to `.bak` → rename `.tmp` to original
- Guarantees recovery in case of unexpected errors or crashes

---

## 🗂 File Structure (v1.4.1 — Modular)

> **NEW in v1.4.1:** The single-file `BankSystem.cpp` has been split into 14 modular header files. This improves code organization, readability, maintainability, and separation of responsibilities, laying the groundwork for the complete object-oriented redesign planned for v2.0.0.
### Source Files

| File | Responsibility |
|:-----|:--------------|
| `Main.cpp` | Entry point — includes all headers in order |
| `Globals.h` | Structs, Enums, Constants, Forward Declarations |
| `Utilities.h` | Formatting, UI helpers, screen control |
| `Crypto.h` | Encryption & Decryption (libsodium) |
| `Session.h` | Session save / load / clear |
| `Logger.h` | Logging system |
| `FileManager.h` | File I/O, Serialization, Atomic save |
| `InputManager.h` | Input reading & validation |
| `PermissionManager.h` | Permission checks |
| `ClientManager.h` | Client CRUD operations |
| `TransactionManager.h` | Deposit / Withdraw / Transfer |
| `UserManager.h` | User CRUD operations |
| `AuthManager.h` | Login, Password hashing, Admin setup |
| `MenuManager.h` | All menus and navigation |

### Data Files
- **Clients.txt** – Client account information
- **Users.txt** – User credentials and permissions
- **Transactions.txt** – Complete transaction history

### Session Files (Hidden)
- **Windows:** `%LOCALAPPDATA%\BankSystem\session_username.bsess`
- **Linux/macOS:** `~/.config/BankSystem/session.bsess`

---

## 📊 Version Comparison

| Feature | v1.0 | v1.1 | v1.2 | v1.3 | v1.4 | v1.4.1 |
|---------|:----:|:----:|:----:|:----:|:----:|:------:|
| Client CRUD | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| Deposit/Withdraw | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ |
| Money Transfers | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| Total Balances Report | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ |
| Transaction History | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| User Management | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ |
| RBAC Permissions | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ |
| Session Encryption | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| Password Hashing | ❌ | ❌ | ⚠️ Weak | ⚠️ Weak | ✅ Argon2id | ✅ Argon2id |
| Transaction Logging | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| System Logging | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| Color-coded UI | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| Data Loss Protection | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| **Modular Header Files** | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |

**Legend:**  
✅ Fully Implemented | ⚠️ Implemented (Weak/Insecure) | ❌ Not Available

**Security Note:** v1.2.0 and v1.3.0 used `std::hash` for password hashing, which is NOT cryptographically secure. Only v1.4.0+ uses Libsodium Argon2id password hashing.

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
   g++ -o BankSystem Main.cpp -std=c++11 -lsodium
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

1. Program detects no users exist in `Users.txt`
2. Prompts you to create first administrator account
3. Enter desired username
4. Enter password (minimum 8 characters, must include uppercase, lowercase, and a digit)
5. Password is immediately hashed using Argon2id
6. Account created with full permissions (pAll = 127)

---

## 💻 Code Quality & Architecture

### 🏗 v1.4.1 — Modular Procedural Architecture

v1.4.1 is a **pure structural refactor** of v1.4.0 with **zero logic changes**. The goal of this release is to improve code organization by separating the application into logical modules with clear responsibilities. This structural refactor makes the codebase easier to maintain, navigate, and extend while making future architectural changes easier, including the transition to an object-oriented design.

```text
v1.4.0
Monolithic source file
        │
        ▼
v1.4.1
Modular source organization
        │
        ▼
v2.0.0
Object-Oriented Architecture
```

### 🔧 Professional C++ Usage
- **Structs** – `strClient`, `strUser`, `Transaction` organize related data
- **Enums** – Menu options and permission flags eliminate magic numbers
- **Pointers** – `strClient*` enables direct in-memory editing
- **Input Validation** – Prevents negative amounts, overdrafts, invalid input
- **Cross-Platform** – Works on Windows, Linux, and macOS
- **Pointers** – Direct in-memory modification of client records without unnecessary copying

---

## 🔐 Security Architecture

| Feature Area | Implementation Details |
| :--- | :--- |
| **Password Security** | Libsodium Argon2id hashing, minimum 8 characters, uppercase + lowercase + digit required |
| **Session Security** | XChaCha20-Poly1305 encryption, OS-protected key storage, 3-pass secure overwrite |
| **Transaction Security** | Unique transaction IDs, timestamp validation, balance verification |
| **Access Control** | Bitwise permission flags (7 levels + admin), dynamic menu system |
| **Data Protection** | Input validation, bounds checking, file corruption detection |

---

## 🛡 Bitwise Permissions

```cpp
enum Permission {
    pListClients  = 1,   // 0000001
    pAddClient    = 2,   // 0000010
    pDeleteClient = 4,   // 0000100
    pUpdateClient = 8,   // 0001000
    pFindClient   = 16,  // 0010000
    pTransactions = 32,  // 0100000
    pManageUsers  = 64,  // 1000000
    pAll          = 127  // 1111111 (Full Access)
};
```

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

## 🏆 Version History

For detailed information about each version and changes, see **[VERSIONS.md](VERSIONS.md)**

| Version | Key Features |
| :--- | :--- |
| [v1.0.0][v100] | Basic Client CRUD operations |
| [v1.1.0][v110] | Financial Transactions (Deposit & Withdraw) |
| [v1.2.0][v120] | User Management + RBAC System |
| [v1.3.0][v130] | Binary Session Encryption (Libsodium) |
| [v1.4.0][v140] | Transaction Management + Money Transfers |
| [v1.4.1][v141] | Modular Code Organization ⭐ |

---

## 🤝 Contribution

Contributions are welcome. If you find a bug or have an idea for improvement, feel free to open an issue or submit a pull request.

---

## 📜 License

**[MIT License](LICENSE.txt)**

---

**Built with:** C++ | Libsodium | File-based Persistence  
**Developer:** Yusuf Zakaria Alshalabi  
**Version:** 1.4.1  
**Last Update:** February 2026

[v141]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.4.1
[v140]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.4.0
[v130]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.3.0
[v120]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.2.0
[v110]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.1.0
[v100]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.0.0
