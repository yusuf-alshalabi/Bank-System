# 🏦 CRUD Bank System — Version 1.2.0

A fully-featured console-based *Bank Management System* built in C++. This release introduces a major upgrade with a robust *User Management and Permissions System*, securing the application and allowing granular control over access rights for different staff members.

---

## ✨ Features

### 🔐 User Management & Security (New)
* *Secure Login:* Dedicated login screen required to access the main menu.
* *User CRUD Operations:* Full capabilities to *Add, Delete, Update, and Find* system users.
* *Role-Based Access Control (RBAC):* Implementation of *Bitwise Permissions* to define precise access levels for each user (e.g., read-only, transactions, full access).
* *Password Hashing:* Passwords are stored securely as *hashed* strings using std::hash.
* *Dynamic Menus:* The Main Menu structure adapts automatically based on the currently logged-in user's permissions.
* *Default Admin:* An initial "Admin" user is created automatically on the first run for setup (Username: Admin, Password: 1234).

### 🔧 Client Management
* *View All Clients:* Display client data in a clear, formatted table.
* *Add New Client:* Register new clients with unique account numbers.
* *Delete Client:* Remove client records.
* *Update Client Info:* Modify existing client details.
* *Find Client:* Search for a client by account number.

### 💰 Financial Transactions
* *Deposit:* Add funds to a client's account.
* *Withdraw:* Debit funds with *overdraft protection* (validation against current balance).
* *Total Balances Report:* Generate a list of all client balances, including the aggregated grand total.

### 💾 Data Persistence
* *File Storage:* Client data is stored in Clients.txt and user credentials/permissions are stored in Users.txt.
* *Real-Time Sync:* Data files are updated instantly after every successful modification.

---

## ⚙ How to Run

1.  Save the code as BankSystem.cpp.
2.  Compile the program using g++:
    bash
    g++ -o BankSystem BankSystem.cpp
    
3.  Run the executable:
    bash
    ./BankSystem
    # On Windows: BankSystem.exe
    
4.  The system will prompt you for login. Use the default admin credentials initially.

---

## 💻 Code Quality & Architecture

This version significantly enhances security and modularity while maintaining the principles of *Clean Procedural Programming*.

### 🛡 Security Architecture
* *Bitwise Permissions:* Permissions are managed efficiently using *Bit Masks*, allowing for combinations of access rights stored as a single integer.
* *Access Checks:* The checkAccessPermission function acts as the central gatekeeper, preventing unauthorized operations.
* *Input Handling:* Robust input validation and dedicated helper functions (readNonEmptyString, readPositiveNumber) ensure data integrity and system stability.

### 🏗 Procedural Design
* *Modularity:* Clear separation of concerns into distinct modules (Client Management, Transactions, User Management, File Handling, and UI utilities).
* *Structs and Enums:* Effective use of strClient, strUser for data organization, and enum types for clear menu and permission constants.
* *Pointers and References:* Continued use of pointers (strClient*, strUser*) for efficient in-memory searching and updates, avoiding unnecessary data copying.

---

## 🚀 What's New in Version 1.2.0

| Feature | Description |
| :--- | :--- |
| *Complete User & Permissions System* | The core focus of the release, introducing multi-level access control. |
| *Password Hashing* | Security enhancement for user data integrity. |
| *Dedicated User Management Menu* | A new section for administrative control over system users. |
| *Dynamic UI* | Main Menu options are filtered and displayed based on the logged-in user's granted permissions. |

---

## 📅 Project Context

This project represents the *1.2.0 release* of the CRUD Bank System, demonstrating proficiency in advanced topics including file security, data hashing, and granular access control.

---

## 🤝 Contribution

Contributions, bug reports, and suggestions for future enhancements are highly welcome. Feel free to fork the repository or open an issue.

---

## 📜 License

MIT License