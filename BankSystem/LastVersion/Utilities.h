#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: Utilities.h                                      ||
//  || Section: Utilities                                     ||
//  || Helper functions for formatting, trimming, screen      ||
//  || control, and displaying messages.                      ||
//  ||========================================================||

#include "Globals.h"

//=====================================================
//==================== Utilities ======================
//=====================================================

// Format double with fixed precision
string formatDouble(double value, int precision) {
    ostringstream out;
    out << fixed << setprecision(precision) << value;
    return out.str();
}
// Convert integer safely to string
string formatInt(int value) {
    return to_string(value);
}
// Format currency with $ sign
string formatCurrency(double value) {
    ostringstream out;
    out << "$" << fixed << setprecision(2) << value;
    return out.str();
}
// Remove leading and trailing whitespace from string
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == string::npos) return "";

    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}
// Get current date and time as formatted string
string getCurrentTimestamp() {
    time_t now = time(0);

#ifdef _WIN32
    tm localTime;
    localtime_s(&localTime, &now);
#else
    tm* localTime = localtime(&now);
#endif

    stringstream ss;
#ifdef _WIN32
    ss << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
#else
    ss << put_time(localTime, "%Y-%m-%d %H:%M:%S");
#endif
    return ss.str();
}
// Clear console screen (Windows/Linux)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
// Draw a line with given length, symbol, and color (no newlines)
void drawLine(int length, char symbol, string color) {
    cout << color << string(length, symbol) << RESET;
}
// Show a separator line with newlines before and after
void showLine(int length, char symbol, string color) {
    cout << "\n";
    drawLine(length, symbol, color);
    cout << "\n";
}
// Show a bordered line (+ at start and end)
void showBorderLine(int length, char symbol, string color) {
    cout << color << "+";
    drawLine(length, symbol, color);
    cout << color << "+" << RESET << "\n";
}
// Display formatted success message
void showSuccessMessage(string message) {
    showLine(60, '=', GREEN);
    cout << GREEN << "   SUCCESS: " << message << RESET;
    showLine(60, '=', GREEN);
    cout << "\n";
}
// Display formatted error message
void showErrorMessage(string message) {
    showLine(60, '=', RED);
    cout << RED << "   ERROR: " << message << RESET;
    showLine(60, '=', RED);
    cout << "\n";
}
// Display centered header with borders
void showScreenHeader(const string& title) {
    cout << "\n";
    showBorderLine(58, '=', CYAN);
    cout << CYAN << "|" << string(58, ' ') << "|\n";

    int padding = (58 - title.length()) / 2;
    cout << "|" << string(padding, ' ') << title
        << string(58 - padding - title.length(), ' ') << "|\n";

    cout << "|" << string(58, ' ') << "|\n";
    showBorderLine(58, '=', CYAN);
}
// Display numbered list of options
void showOptions(const vector<string>& options) {
    cout << "\n";
    for (size_t i = 0; i < options.size(); i++) {
        cout << CYAN << "  [" << (i + 1) << "]  " << RESET
            << YELLOW << options[i] << RESET << ".\n";
    }
}
// Display Back or Exit option
void showBackOrExit(bool isMain) {
    string back = (isMain ? "Exit" : "Back");
    cout << "\n" << CYAN << "  [0]  " << RESET << YELLOW << back << RESET << ".\n";
}
// Wait until user presses Enter once
void waitForEnter() {
    string dummy;
    getline(cin, dummy);
}
// Show message and wait for Enter to continue
void pressEnterToContinue() {
    cout << "\n\n" << CYAN << "Press Enter to continue..." << RESET;
    waitForEnter();
}
// Show message and wait for Enter to return to main menu
void backToMenu() {
    cout << "\n\n" << YELLOW << "Press Enter to return to the main menu..." << RESET;
    waitForEnter();
}
