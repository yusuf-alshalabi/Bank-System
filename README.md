# 🏦 CRUD Bank System — Version 1.1.0

A console-based Bank Management System built in C++ as part of the Programming Advices Roadmap (Course 7).

This version includes full CRUD operations, financial transactions, real-time data persistence, and an enhanced user interface.

---

## ✨ Features

### 🔧 Client Management
- View All Clients – Display all clients in a formatted table
- Add New Client – Add a client with a unique account number
- Delete Client – Remove a client by account number
- Update Client Info – Modify client details
- Find Client – Search for a client and display full information

### 💰 Financial Transactions
- Deposit – Add funds to a client's account
- Withdraw – Withdraw funds with full balance validation
- Total Balances – Display all balances with a grand total
- Transactions Menu – Dedicated interface for financial operations

### 💾 Data Management
- Data Persistence – All client data stored in Clients.txt
- Real-Time Updates – Instant file updates after modifications
- Automatic File Creation – Generates data file if not exists

---

## ⚙ How to Run

1. Clone the repository (or save the code into BankSystem.cpp)

2. Compile with g++:
   g++ -o BankSystem BankSystem.cpp

3. Run the executable:
   ./BankSystem
   On Windows: BankSystem.exe

4. The system starts automatically with main menu and loads/creates Clients.txt

---

## 💻 Code Quality & Architecture

This project follows Procedural Programming with emphasis on clean code, efficiency, and maintainability.

### 🏗 Procedural Architecture & Clean Code
- Modular Design – Clear separation between client management and transactions
- Single Responsibility Principle – Each function has one specific task
- Readable Code – Meaningful names, constants, intuitive menus
- Separation of Concerns – UI, file handling, and data processing isolated

### 🔧 Professional C++ Usage
- Structs – strClient organizes related data
- Enums – MainMenuOption, TransactionsOptions eliminate magic numbers
- Pointers – strClient* enables direct in-memory editing
- Input Validation – Prevents negative amounts, overdrafts, invalid input

### ⚡ Performance & Efficiency
- Pointer-based retrieval avoids data copying
- Pass-by-reference for large structures
- Real-time data sync ensures consistency
- Balance protection prevents overdrafts

### 🎯 User Experience
- Dual menu system for clients and transactions
- Intuitive navigation with clear screen flow
- Financial reporting with total balances
- Dedicated operation screens for each task

---

## 🚀 What's New in Version 1.1.0

- Full financial transaction system (Deposit & Withdraw)
- Dedicated Transactions Menu
- Real-time balance calculations
- Enhanced UI with clearer navigation
- Total Balance Report with aggregated sums

---

## 📅 Roadmap Context

This project represents Version 2.0 of the CRUD Bank System, reflecting advanced topics from Course 7 of the Programming Advices Roadmap.

---

## 🤝 Contribution

Contributions are welcome!
Feel free to fork the repository, submit pull requests, or open issues.

---

## 📜 License

MIT License
