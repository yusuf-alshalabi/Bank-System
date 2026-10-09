# 🏦 CRUD Bank System — Version 2.0.0 (OOP)

![Version](https://img.shields.io/badge/version-2.0.0-blue)
![Language](https://img.shields.io/badge/C%2B%2B-17-blue)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)
![Crypto](https://img.shields.io/badge/crypto-libsodium-green)
![License](https://img.shields.io/badge/license-MIT-brightgreen)

> ⚠️ **PRODUCTION WARNING**  
> This is an **educational project** demonstrating procedural-to-object-oriented refactoring and secure console programming concepts.  
> **NOT RECOMMENDED FOR PRODUCTION USE**.  
> For production banking software, use established frameworks with proper security audits.

---

A professional console-based Bank Management System built in **C++17** as part of the Programming Advices Roadmap (Course 7).

Version **v2.0.0** is a **complete Object-Oriented Programming (OOP) redesign** of the procedural v1.x series. The single-purpose free functions of the past have been replaced by a **layered class architecture** — domain models own their behavior, repositories own persistence, infrastructure owns security and I/O, services own policy, and screens own presentation. All application functionality from v1.4.1 is preserved, re-homed, and extended with a **Currency Exchange** module.

---

## ✨ Features

### 🔧 Client Management
- **View All Clients** – Display all clients in a bordered, color-coded table
- **Add New Client** – Add a client with a unique account number
- **Delete Client** – Remove a client with confirmation
- **Update Client Info** – Modify client details through encapsulated setters
- **Find Client** – Search for a client and display a full client card

### 💰 Financial Transactions
- **Deposit** – Add funds to a client's account
- **Withdraw** – Withdraw funds with full overdraft/balance validation
- **Transfer** – Send money between accounts with automatic **1% fee** and a single atomic commit for both balances
- **Total Balances** – Display all balances with a grand total (plus number-to-text)
- **Transaction History** – Newest-first transaction report per account
- **Transfer Log** – Dedicated human-readable transfer audit table

### 👥 User Management & Access Control
- **User CRUD Operations** – Add, Delete, Update, Find system users
- **Bitwise Role-Based Access Control (RBAC)** – **9 permission flags** for granular access
- **Dynamic Menus** – The main menu is built at runtime from the signed-in user's permission bits
- **Password Hashing** – Secure password storage using Libsodium **Argon2id** (`crypto_pwhash`)
- **Strong Password Policy** – Minimum 8 characters, must include uppercase, lowercase, and a digit
- **First-Time Setup** – The system forces creation of a secure administrator account on first run
- **Last-Admin Protection** – The final full-access admin cannot be deleted or downgraded

### 🔒 Security & Session Management
- **Argon2id Password Hashing** – Memory-hard, salted hashes (`m=65536, t=2, p=1`)
- **XChaCha20-Poly1305 Session Encryption** – Encrypted session token via Libsodium `crypto_secretbox`
- **Auto Session Resume** – Seamless encrypted login resume on startup
- **RAII Key Management** – The 32-byte key is zeroed on destruction (`SecretKey` + `sodium_memzero`), guarded by `SodiumRaiiGuard`
- **Secure Logout** – 3-pass random overwrite (0x00 → 0xFF → random) before deletion
- **No Password on Disk** – Only the username is stored in the encrypted session file
- **Login Lockout** – Account is blocked after 3 failed login attempts
- **Legacy Password Upgrade** – `DataMigrator` re-hashes legacy Caesar-cipher passwords to Argon2id

### 💱 Currency Exchange System *(New in v2.0.0)*
- **Browse Currencies** – Full currency list with USD-based rates
- **Find Currency** – Lookup by currency code or country
- **Update Rate** – Persist live exchange-rate changes
- **Currency Calculator** – Bidirectional conversion between any two currencies through a USD pivot

### 📜 Audit & Resilience
- **Complete Audit Trail** – Every deposit, withdrawal, transfer, and login is logged
- **Structured Logs**
  - `Transactions.txt` – Full financial transaction ledger
  - `TransferLog.txt` – Before/after balances per transfer
  - `LoginRegister.txt` – Successful login registry
  - `SystemLog.txt` – Leveled system log (INFO, WARNING, ERROR, CRITICAL)
- **Unique Transaction IDs** – Format: `TXN` + microsecond timestamp + random hex component
- **AtomicFileStore** – Crash-safe `.tmp → .bak → rename` persistence with automatic backups
- **Malformed-Line Skipping** – Corrupt records are ignored instead of crashing the loader

### 🎨 Enhanced User Interface
- **Color-coded messages** (success, error, info, access denied) using ANSI escape codes
- **Green currency formatting** and **bordered/centered tables** for professional console output
- **Consistent navigation** with `0 to Back / Exit` prompts on every screen

---

## 🗂 File & Directory Architecture (v2.0.0 — Layered OOP)

> **NEW in v2.0.0:** The modular procedural headers of v1.4.1 have been rebuilt as an **object-oriented, layered architecture**. Presentation, domain logic, persistence, security, and policy are now cleanly separated. The old v1.4.1 headers are kept under `LastVersion/` for reference and are no longer compiled.

```text
BankSystem/
├── BankSystem.sln                 # Visual Studio solution
├── Main.cpp                       # Entry point (only .cpp file)
├── BankSystem/
│   ├── Core/                      # Domain + business/security layers
│   │   ├── InterfaceComunication.h   # Abstract communication interface
│   │   ├── Person.h                  # Base identity class
│   │   ├── User.h                    # System operator + RBAC
│   │   ├── BankClient.h              # Bank account + money operations
│   │   ├── Currency.h                # Currency + conversion logic
│   │   ├── Infrastructure/           # Cross-cutting services
│   │   │   ├── AtomicFileStore.h        # Crash-safe file I/O
│   │   │   ├── CryptoEngine.h           # Libsodium encryption (+ RAII guards)
│   │   │   ├── DataMigrator.h           # Legacy password migration
│   │   │   ├── Logger.h                 # Singleton leveled logger
│   │   │   ├── PasswordHasher.h         # Argon2id hashing
│   │   │   ├── PasswordPolicy.h         # Strength policy
│   │   │   └── SessionManager.h         # Encrypted session singleton
│   │   ├── Repositories/            # Persistence layer
│   │   │   ├── BankClientRepository.h
│   │   │   ├── CurrencyRepository.h
│   │   │   └── UserRepository.h
│   │   └── Services/               # Policy & orchestration layer
│   │       ├── AuthService.h
│   │       ├── MenuManager.h
│   │       ├── TransactionService.h
│   │       └── UserService.h
│   ├── Screens/                   # UI layer
│   │   ├── Screen.h                  # Shared UI toolkit (base class)
│   │   ├── MainScreen.h
│   │   ├── Auth/                     # LoginScreen, LoginRegisterScreen
│   │   ├── Client/                   # Add/List/Delete/Update/Find
│   │   ├── Currency/                 # List/Find/UpdateRate/Calculator/Main
│   │   ├── Transaction/              # Deposit/Withdraw/Transfer/...
│   │   └── User/                     # Manage/Add/List/Delete/Update/Find
│   └── Database/                  # Flat-file persistent storage
│       ├── Clients.txt
│       ├── Users.txt
│       ├── Currencies.txt
│       ├── Transactions.txt
│       ├── TransferLog.txt
│       ├── LoginRegister.txt
│       └── SystemLog.txt
└── Libs/Cpp-Library-Collection/    # Git submodule (String, Date, Util, InputValidate)
```

### Layer Responsibilities

| Layer | Location | Responsibility |
|:------|:---------|:---------------|
| **Domain** | `Core/*.h` | Entities owned by an identity: `Person`, `User`, `BankClient`, `Currency` |
| **Infrastructure** | `Core/Infrastructure/` | Crypto, session, logging, atomic file I/O, migration |
| **Repositories** | `Core/Repositories/` | Record format + file persistence for each entity |
| **Services** | `Core/Services/` | Auth bootstrap, transactions, user policy, dynamic menus |
| **UI (Screens)** | `Screens/` | Console presentation, input, navigation, permission gates |

### Data Files
- **Clients.txt** – Client account information
- **Users.txt** – User credentials (Argon2id hashes) and permissions
- **Currencies.txt** – Currency catalogue and rates
- **Transactions.txt** – Complete transaction ledger
- **TransferLog.txt** – Detailed transfer audit
- **LoginRegister.txt** – Login registry
- **SystemLog.txt** – System activity log

### Session Files (Hidden)
- **Windows:** `%LOCALAPPDATA%\BankSystem\session_<osUser>.bsess`
- **Linux/macOS:** `~/.config/BankSystem/session.bsess` (falls back to the temp directory)
- **Encryption Key:** `encryption_key.bsess` stored alongside the session file

---

## 📊 Version Comparison

| Feature | v1.0 | v1.1 | v1.2 | v1.3 | v1.4 | v1.4.1 | **v2.0.0 (OOP)** |
|---------|:----:|:----:|:----:|:----:|:----:|:------:|:----------------:|
| Client CRUD | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| Deposit/Withdraw | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| Money Transfers | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| Total Balances Report | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| Transaction History | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| User Management | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ |
| RBAC Permissions | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ (9 flags) |
| Session Encryption | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ |
| Password Hashing | ❌ | ❌ | ⚠️ Weak | ⚠️ Weak | ✅ Argon2id | ✅ Argon2id | ✅ Argon2id |
| Password Strength Policy | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| Transaction Logging | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| System Logging | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| Color-coded UI | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| Data Loss Protection (Atomic Save) | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| Modular Header Files | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| **Currency Exchange** | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |
| **Object-Oriented Architecture** | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |
| **Layered Repositories & Services** | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |
| **Legacy Password Migration** | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |

**Legend:**  
✅ Fully Implemented | ⚠️ Implemented (Weak/Insecure) | ❌ Not Available

**Security Note:** v1.2.0 and v1.3.0 used `std::hash` for password hashing, which is NOT cryptographically secure. Only v1.4.0+ uses Libsodium Argon2id password hashing. v2.0.0 additionally migrates any surviving legacy passwords automatically.

---

## 🏗 Architecture Overview

```text
v1.0.0 → v1.3.0
Single / early modular procedural source
        │
        ▼
v1.4.1
Modular procedural headers (14 files)
        │
        ▼
v2.0.0
Layered Object-Oriented Architecture
Domain · Infrastructure · Repositories · Services · UI Screens
```

### 🧩 OOP Concepts Applied
- **Inheritance** – `Person` → `User` / `BankClient`; every screen derives from `Screen`
- **Abstraction / Interfaces** – `InterfaceComunication` abstract base
- **Encapsulation** – `private` members exposed via getters/setters and `__declspec(property)`
- **Polymorphism** – Virtual `SendEmail` / `SendFax` / `SendSMS`
- **Composition** – Repositories compose `AtomicFileStore`; `Logger` composes `SessionManager`
- **Singleton** – `Logger::Instance()`, `SessionManager::Instance()`
- **Repository Pattern** – One repository per entity owns record format + persistence
- **Service Layer** – Auth, transaction, user, and menu policy isolated from UI
- **RAII** – `SodiumRaiiGuard` and `SecretKey` manage crypto resources safely
- **Static Utility Classes** – `AtomicFileStore`, `CryptoEngine`, `PasswordHasher`, `PasswordPolicy`, `DataMigrator`

---

## ⚙ How to Run

### Prerequisites
- A **C++17** (or later) compiler: MSVC 2017+/VS2022, GCC 7+, or Clang 5+
- **Libsodium** library installed and linked
- *(Windows)* Visual Studio with the **v143** toolset and **vcpkg** for Libsodium

### Installation & Build

1. **Install Libsodium** (see [LIBSODIUM_SETUP.md](LIBSODIUM_SETUP.md) for details)

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
   g++ -o BankSystem Main.cpp -std=c++17 -lsodium
   ```

3. **Run the executable**
   ```bash
   ./BankSystem        # Linux / macOS
   BankSystem.exe      # Windows
   ```

   > **Windows (Visual Studio):** open `BankSystem.sln`, select **x64**, and build. The project is configured for `stdcpp17` and links `libsodium.lib` from vcpkg.

### 🔐 First-Time Setup
1. `DataMigrator::MigrateLegacyUsersPasswordStorage()` runs automatically — any legacy Caesar-cipher passwords in `Users.txt` are upgraded to Argon2id (idempotent).
2. `AuthService::EnsureFirstRunAdmin()` runs automatically — if no users exist, you are prompted to create the first administrator.
3. Enter a username.
4. Enter a password meeting the strength policy (min 8 chars, uppercase, lowercase, digit).
5. The password is hashed with Argon2id; the account is created with full permissions (`eAll = 255`).
6. Subsequent launches resume an encrypted session or present the login screen.

---

## 🔐 Security Architecture

| Feature Area | Implementation Details |
| :--- | :--- |
| **Password Security** | Libsodium **Argon2id** (`crypto_pwhash`, INTERACTIVE cost), min 8 chars, uppercase + lowercase + digit required |
| **Password Migration** | `DataMigrator` decrypts legacy Caesar-cipher passwords and re-hashes them with Argon2id |
| **Session Security** | **XChaCha20-Poly1305** (`crypto_secretbox`), per-OS-user encrypted key file, 3-pass secure overwrite |
| **Memory Safety** | `SecretKey` zeroes key material on destruction; `SodiumRaiiGuard` guarantees one-time init |
| **Transaction Security** | Unique transaction IDs, timestamp capture, balance verification, single atomic commit for transfers |
| **Access Control** | Bitwise permission flags (8 granular bits + full access `eAll = 255`), dynamic permission-filtered menus |
| **Data Protection** | Atomic saves (`.tmp → .bak → rename`), 100 MB file-size guard, malformed-line skipping |

---

## 🛡 Bitwise Permissions

```cpp
enum enPermissions {
    pListClients   = 1,    // 00000001
    pAddNewClient  = 2,    // 00000010
    pDeleteClient  = 4,    // 00000100
    pUpdateClients = 8,    // 00001000
    pFindClient    = 16,   // 00010000
    pTranactions   = 32,   // 00100000
    pManageUsers   = 64,   // 01000000
    pLoginRegister = 128,  // 10000000  (New in v2.0.0)
    eAll           = 255   // 11111111  (Full Access)
};
```

Permission checks are performed by `User::CheckAccessPermission()` using pure bitwise evaluation:

```cpp
bool CheckAccessPermission(enPermissions Permission) const
{
    return (Permission & Permissions) == Permission;
}
```

---

## 📊 Transaction Structure

```cpp
struct TransactionEntry {
    std::string TransactionID;   // Format: TXN + timestamp + random hex
    int         Type;            // 1 = Deposit | 2 = Withdrawal | 3 = Transfer
    std::string FromAccount;     // Source account number
    std::string ToAccount;       // Destination account number
    double      Amount;          // Transaction amount
    double      Fees;            // Transfer fees (0 for deposit/withdraw)
    std::string Timestamp;       // YYYY-MM-DD HH:MM:SS
    std::string Description;     // Transaction description
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
| [v1.4.1][v141] | Modular Code Organization |
| [v2.0.0][v200] | Complete OOP Redesign + Currency Exchange ⭐ |

---

## 🤝 Contribution

Contributions are welcome. If you find a bug or have an idea for improvement, feel free to open an issue or submit a pull request.

---

## 📜 License

**[MIT License](LICENSE.txt)**

---

**Built with:** C++17 | Libsodium | File-based Persistence  
**Developer:** Yusuf Zakaria Alshalabi  
**Version:** 2.0.0  
**Last Update:** October 2026

[v200]: https://github.com/yusuf-alshalabi/Bank-System/tree/v2.0.0
[v141]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.4.1
[v140]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.4.0
[v130]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.3.0
[v120]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.2.0
[v110]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.1.0
[v100]: https://github.com/yusuf-alshalabi/Bank-System/tree/v1.0.0