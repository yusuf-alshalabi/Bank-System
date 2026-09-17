#pragma once
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "../Core/User.h"
#include "../Core/Infrastructure/SessionManager.h"
#include "../../Libs/Cpp-Library-Collection/Lib/Date.h"

class Screen
{
protected:

    static constexpr const char* RED = "\033[31m";
    static constexpr const char* GREEN = "\033[32m";
    static constexpr const char* YELLOW = "\033[33m";
    static constexpr const char* MAGENTA = "\033[35m";
    static constexpr const char* CYAN = "\033[36m";
    static constexpr const char* RESET = "\033[0m";

    static void _ClearScreen()
    {
        std::system("cls");
    }

    static void _DrawLine(std::size_t Length, char Symbol, const char* Color = CYAN)
    {
        std::cout << Color << std::string(Length, Symbol) << RESET;
    }

    static void _ShowLine(std::size_t Length = 60, char Symbol = '-', const char* Color = CYAN)
    {
        std::cout << "\n";
        _DrawLine(Length, Symbol, Color);
        std::cout << "\n";
    }

    static void _ShowBorderLine(std::size_t Length, char Symbol,char FirstAndFinal = '+', const char* Color = CYAN)
    {
        std::cout << Color << FirstAndFinal << std::string(Length, Symbol) << FirstAndFinal << RESET << "\n";
    }

    static void _ShowTableBorder(std::size_t Length, char Symbol = '-')
    {
        std::cout << CYAN << "+" << std::string(Length, Symbol) << "+" << RESET << "\n";
    }

    static void _DrawScreenHeader(std::string Title, std::string SubTitle = "")
    {
        std::cout << "\n";
        _ShowBorderLine(58, '=');
        _ShowBorderLine(58, ' ', '|');

        std::size_t TitleLength = Title.size();
        std::size_t Padding = TitleLength >= 58 ? 0 : (58 - TitleLength) / 2;
        std::string RightSpaces(58 > Padding + TitleLength ? 58 - Padding - TitleLength : 0, ' ');

        std::cout << CYAN << "|" << std::string(Padding, ' ') << Title << RightSpaces << "|" << RESET << "\n";

        _ShowBorderLine(58, ' ', '|');
        _ShowBorderLine(58, '=');

        std::string User = Bank::Security::SessionManager::Instance().CurrentUserName();
        if (User.empty())
            User = "Not Signed In";

        std::cout << "\n  User:  " << User << "\n";
        std::cout << "  Date:  " << Core::Date::ToString(Core::Date()) << "\n";

        if (SubTitle != "")
        {
            std::cout << "  " << SubTitle << "\n";
        }

        _ShowLine(58, '-');
    }

    static void _ShowOptions(const std::vector<std::string>& Options)
    {
        std::cout << "\n";
        for (std::size_t i = 0; i < Options.size(); ++i)
        {
            std::cout << CYAN << "  [" << (i + 1) << "]  " << RESET
                << YELLOW << Options[i] << RESET << ".\n";
        }
    }

    static void _ShowBackOption(bool IsMain = false)
    {
        std::string Label = IsMain ? "Exit App" : "Back";
        std::cout << "\n" << CYAN << "  [0]  " << RESET << YELLOW << Label << RESET << ".\n";
    }

    static void _ShowSuccessMessage(const std::string& Message)
    {
        _ShowLine(60, '=', GREEN);
        std::cout << GREEN << "   SUCCESS: " << Message << RESET;
        _ShowLine(60, '=', GREEN);
        std::cout << "\n";
    }

    static void _ShowErrorMessage(const std::string& Message)
    {
        _ShowLine(60, '=', RED);
        std::cout << RED << "   ERROR: " << Message << RESET;
        _ShowLine(60, '=', RED);
        std::cout << "\n";
    }

    static void _ShowAccessDeniedMessage()
    {
        _ShowLine(60, '=', RED);
        std::cout << RED << "   ACCESS DENIED!\n   You don't have permission to perform this operation.\n   Please contact your Admin." << RESET;
        _ShowLine(60, '=', RED);
        std::cout << "\n";
    }

    static void _PressEnterToContinue()
    {
        std::cout << "\n\n" << CYAN << "Press Enter to continue..." << RESET;
        std::string dummy;
        std::getline(std::cin, dummy);
    }

    static short _ReadMenuOption(short From, short To, bool IsMain = false)
    {
        std::string ZeroLabel = IsMain ? "Exit" : "Back";
        short Choice = 0;
        while (true)
        {
            std::cout << "\nChoose option [0 for " << ZeroLabel << ", " << From << " to " << To << "] ? ";
            if (!(std::cin >> Choice))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                _ShowErrorMessage("Invalid choice: enter 0 or a number from " + std::to_string(From) + " to " + std::to_string(To) + ".");
                continue;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (Choice == 0 || (Choice >= From && Choice <= To))
                return Choice;

            _ShowErrorMessage("Invalid choice: enter 0 or a number from " + std::to_string(From) + " to " + std::to_string(To) + ".");
        }
    }

    static std::string _CurrencyText(double Amount)
    {
        std::ostringstream Stream;
        Stream << std::fixed << std::setprecision(2) << Amount;
        return "$" + Stream.str();
    }

    static std::string _FormatCurrency(double Amount)
    {
        return std::string(GREEN) + _CurrencyText(Amount) + RESET;
    }

    static std::string _FormatPasswordForDisplay(const std::string& Password)
    {
        if (Password.size() <= 25)
            return Password;
        return "******** (argon2id hash)";
    }

    static bool CheckAccessRights(User::enPermissions Permission)
    {
        if (!Bank::Security::SessionManager::Instance().CanAccess(Permission))
        {
            _ShowAccessDeniedMessage();
            return false;
        }
        return true;
    }

    static bool CheckActiveSession()
    {
        if (!Bank::Security::SessionManager::Instance().IsActive())
        {
            _ShowAccessDeniedMessage();
            return false;
        }
        return true;
    }

};