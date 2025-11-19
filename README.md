# 🏦 CRUD Bank System

A Console Application for managing bank client data, developed as part of the Programming Advices Roadmap (Course 7).

This project implements *Create, **Read, **Update, and **D*elete (CRUD) operations for client records, with data persistence handled by a text file.

## ✨ Features

* *View All Clients:* Display a list of all bank clients.
* *Add New Client:* Add a new client with a unique account number.
* *Delete Client:* Remove an existing client by account number.
* *Update Client Info:* Modify the details of an existing client.
* *Find Client:* Search for and display a client's details by account number.
* *Data Persistence:* Client data is stored and retrieved from a text file (Clients.txt).

## ⚙ How to Run

1.  *Clone the repository* (or save the provided code as a .cpp file).
2.  *Compile the C++ source code* using a standard C++ compiler (e.g., g++):

    bash
    g++ -o bank_system main.cpp
    

3.  *Run the executable:*

    bash
    ./bank_system
    
    *(On Windows, it might be bank_system.exe)*

4.  The application will start, display the main menu, and automatically create the Clients.txt file if it doesn't exist.

## 💻 Code Quality, Architecture & Performance

This project is built using a *Procedural Programming* approach in C++ and demonstrates high standards of clean code, efficiency, and robustness.

### 1. Procedural Programming & Clean Code (SRP)

* *Procedural Architecture:* The program is structured around a clear sequence of procedures/functions, separating the logic for UI, file handling, and core operations.
* *Single Responsibility Principle (SRP):* Functions are highly granular, each dedicated to a single task (e.g., splitString, convertClientRecordToLine, showDeleteClientScreen). This makes the code easy to test, maintain, and understand.
* *Clarity and Readability:* The use of descriptive function and variable names, along with defined constants (ClientsFileName, Separator), ensures excellent code readability.

### 2. Professional Use of C++ Fundamentals

* *Structs:* strClient effectively bundles related data points into a single, cohesive entity.
* *Enums:* enum MainMenuOption enhances code clarity by replacing "magic numbers" with descriptive names for menu choices, directly benefiting maintainability.
* *Pointers:* Pointers (strClient*) are used professionally in the core search function (findClientByAccountNumber). This allows for *direct in-memory modification* of the client record within the std::vector during Update and Delete operations, avoiding unnecessary data copying.

### 3. High Performance and Efficiency

* *Performance-Optimized CRUD:* By returning a pointer, the system achieves *high performance* for Update and Delete operations, as changes are applied directly to the client object in the memory vector, minimizing I/O operations and avoiding redundant searches.
* *Efficient Memory Management:* Large data structures (the client vector) are consistently passed by **reference (&)** throughout the application (e.g., const vector<strClient>&), preventing costly object copying and ensuring low memory overhead.
* *Robust Input Handling:* Functions like readPositiveDouble include logic to robustly handle input errors (cin.fail()) and buffer issues (cin.ignore), ensuring stability and a smooth user experience.

---

## 📅 Roadmap Context

This project is *Version 1.0 (v 1.0)* of the CRUD Bank System, representing the successful completion of *Course 7* in the Programming Advices Roadmap.

## 🤝 Contribution

Contributions are welcome! Please fork this repository and submit a pull request.

---

## 📜 License

MIT License