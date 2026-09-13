#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "../../Core/Currency.h"

class CurrencyCalculatorScreen :protected Screen

{
private:

    static double _ReadAmount()
    {
        return Core::InputValidate::ReadNumber<double>("\nEnter Amount to Exchange: ");
    }

    static Currency _GetCurrency(std::string Message)
    {

        std::string CurrencyCode = Core::InputValidate::ReadString(Message);

        while (!Currency::IsCurrencyExist(CurrencyCode))
        {
            CurrencyCode = Core::InputValidate::ReadString("\nCurrency is not found, choose another one: ");
        }

        Currency Currency = Currency::FindByCode(CurrencyCode);
        return Currency;

    }


    static  void _PrintCurrencyCard(Currency Currency, std::string Title = "Currency Card:")
    {

       std::cout << "\n" << Title << "\n";
       std::cout << "_____________________________\n";
       std::cout << "\nCountry       : " << Currency.Country();
       std::cout << "\nCode          : " << Currency.CurrencyCode();
       std::cout << "\nName          : " << Currency.CurrencyName();
       std::cout << "\nRate(1$) =    : " << Currency.Rate();
       std::cout << "\n_____________________________\n\n";

    }

    static void _PrintCalculationsResults(double Amount, Currency Currency1, Currency Currency2)
    {

        _PrintCurrencyCard(Currency1, "Convert From:");

        double AmountInUSD = Currency1.ConvertToUSD(Amount);

        std::cout << Amount << " " << Currency1.CurrencyCode()
            << " = " << AmountInUSD << " USD\n";

        if (Currency2.CurrencyCode() == "USD")
        {
            return;
        }

        std::cout << "\nConverting from USD to:\n";

        _PrintCurrencyCard(Currency2, "To:");

        double AmountInCurrrency2 = Currency1.ConvertToOtherCurrency(Amount, Currency2);

        std::cout << Amount << " " << Currency1.CurrencyCode()
            << " = " << AmountInCurrrency2 << " " << Currency2.CurrencyCode();

    }


public:

    static void ShowCurrencyCalculatorScreen()
    {
        do
        {
            std::system("cls");

            _DrawScreenHeader("\tUpdate Currency Screen");

            Currency CurrencyFrom = _GetCurrency("\nPlease Enter Currency1 Code: ");
            Currency CurrencyTo = _GetCurrency("\nPlease Enter Currency2 Code: ");
            double Amount = _ReadAmount();

            _PrintCalculationsResults(Amount, CurrencyFrom, CurrencyTo);

        } while (Core::InputValidate::ReadYesNoOption("\n\nDo you want to perform another calculation? y/n ? "));
    }
};

