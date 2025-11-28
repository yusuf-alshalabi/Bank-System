# 🏦 CRUD Bank System (V1.3.0)

A professional-grade console-based Bank Management System built in modern C++.  
This major release introduces Binary Session Encryption (Libsodium) and a fully secured Session Management System, bringing the project to real-world security standards.

---

## 🚀 Quick Start

### Prerequisites
- C++ compiler (g++ recommended)  
- C++11 standard or later  
- *Libsodium* library installed and linked

### Build & Run

```bash
# Compile with Libsodium
g++ -o BankSystem BankSystem.cpp -std=c++11 -lsodium

# Run the executable
./BankSystem        # Linux / macOS
BankSystem.exe      # Windows
```
## 📦 Libsodium Installation

For detailed setup instructions, see the full guide here:  
[Libsodium Setup Instructions](LIBSODIUM_SETUP.md)


## 🔑 First-Time Setup

At first launch, the system automatically creates a default administrator:

- Username: Admin  
- Password: 1234  

⚠ Important: Change this password immediately for security.

---

## ✨ Features Overview (V1.3.0)

### 🔒 Advanced Cryptographic Security
- Binary session encryption using XChaCha20-Poly1305 (Libsodium)
- Secure key generation & storage (OS-protected hidden file)
- Data integrity validation on load
- Non-reversible password hashing using std::hash

### ⏱ Session Management System
- Automatic session resume (decrypts previous session)
- Secure logout with multi-pass random overwrite
- Platform-aware paths (AppData/Local on Windows, ~/.config on Linux/macOS)

### 👥 User & Access Control (RBAC)
- Bitwise permission flags for fast, scalable access control
- Dynamic main menu based on user privileges
- Full administrative User CRUD (Add / Delete / Update / Find)

### 🏦 Bank Core Operations
- Client CRUD (Add, Update, Delete, Find)
- Deposits & Withdrawals with overdraft protection
- Comprehensive balance reports with total aggregation

---

### 🔐 Security Architecture Details

| Feature Area | Implementation Details |
| :--- | :--- |
| *Password Security* | One-way hashing (std::hash), minimum 4-char enforcement, no plain-text storage. |
| *Session Security* | Encryption (Libsodium), OS-protected key storage, 3-pass secure overwrite on logout. |
| *Data Protection* | Input validation, bounds checking, and file corruption detection. |

### 🛡 Bitwise Permissions

```cpp
enum Permissions {
    pListClients    = 1,    // 0000001
    pAddClient      = 2,    // 0000010  
    pDeleteClient   = 4,    // 0000100
    pUpdateClient   = 8,    // 0001000
    pFindClient     = 16,   // 0010000
    pTransactions   = 32,   // 0100000
    pManageUsers    = 64,   // 1000000
    pAllPermissions = 127,  // 1111111 
    pAll            = -1    // 1111111 (Admin) 
};
```

### Encrypted Session Flow
1. User logs in or previous session is detected  
2. Session data is serialized  
3. Encrypted using Libsodium (XChaCha20-Poly1305)  
4. Stored as a hidden binary file (*.bsess)  
5. Key stored securely in a protected OS-specific directory  

---

## 🏗 System Architecture (High-Level)
- 🔐 Authentication & Security
- 👥 User Management + RBAC
- 🏦 Client Management
- 💰 Transaction Processing
- 💾 File-Based Persistence
- 🔐 Encrypted Session Storage

---

## 🏆 Version History
- v1.0 - Basic Client CRUD  
- v1.1 - Financial Transactions  
- v1.2 - User Management + RBAC  
- v1.3.0 - Binary Encryption + Secure Session Management  

---

## 📜 License
[MIT License](LICENSE.txt)
