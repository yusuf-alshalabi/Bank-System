#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "../../Core/Currency.h"

class CurrencyCalculatorScreen :protected Screen

{
private:

    static std::string _ReadCurrencyCode(const std::string& Message)
    {
        std::string CurrencyCode = Core::InputValidate::ReadString(Message);
        if (CurrencyCode == "0")
        {
            return "0";
        }

        while (!Currency::IsCurrencyExist(CurrencyCode))
        {
            CurrencyCode = Core::InputValidate::ReadString("\nCurrency is not found, choose another one (or 0 to Back): ");
            if (CurrencyCode == "0")
            {
                return "0";
            }
        }

        return CurrencyCode;
    }

    static void _PrintCurrencyCard(Currency Currency, std::string Title = "Currency Card:")
    {
        std::cout << "\n" << Title << "\n";
        _ShowBorderLine(50, '=');
        std::cout << "Country       : " << Currency.Country() << "\n";
        std::cout << "Code          : " << Currency.CurrencyCode() << "\n";
        std::cout << "Name          : " << Currency.CurrencyName() << "\n";
        std::cout << "Rate (1$) =   : " << Currency.Rate() << "\n";
        _ShowBorderLine(50, '=');
    }

    static void _PrintCalculationsResults(double Amount, Currency Currency1, Currency Currency2)
    {
        _PrintCurrencyCard(Currency1, "Convert From:");

        double AmountInUSD = Currency1.ConvertToUSD(Amount);

        std::cout << "\n" << Amount << " " << Currency1.CurrencyCode()
            << " = " << AmountInUSD << " USD\n";

        if (Currency2.CurrencyCode() == "USD")
        {
            return;
        }

        std::cout << "\nConverting from USD to:\n";

        _PrintCurrencyCard(Currency2, "To:");

        double AmountInCurrrency2 = Currency1.ConvertToOtherCurrency(Amount, Currency2);

        std::cout << "\n" << Amount << " " << Currency1.CurrencyCode()
            << " = " << AmountInCurrrency2 << " " << Currency2.CurrencyCode() << "\n";
    }

public:

    static void ShowCurrencyCalculatorScreen()
    {
        if (!CheckActiveSession())
        {
            return;
        }

        do
        {
            _ClearScreen();

            _DrawScreenHeader("Currency Calculator Screen");

            std::string Currency1Code = _ReadCurrencyCode("\nPlease Enter Currency1 Code (or 0 to Back): ");
            if (Currency1Code == "0")
            {
                return;
            }

            std::string Currency2Code = _ReadCurrencyCode("\nPlease Enter Currency2 Code (or 0 to Back): ");
            if (Currency2Code == "0")
            {
                return;
            }

            Currency CurrencyFrom = Currency::FindByCode(Currency1Code);
            Currency CurrencyTo = Currency::FindByCode(Currency2Code);

            double Amount = Core::InputValidate::ReadNumber<double>("\nEnter Amount to Exchange: ");

            _PrintCalculationsResults(Amount, CurrencyFrom, CurrencyTo);

            _ShowLine(58, '-');

        } while (Core::InputValidate::ReadYesNoOption("\nDo you want to perform another calculation? "));
    }
};