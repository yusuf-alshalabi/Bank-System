#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "../../Core/Currency.h"

class UpdateCurrencyRateScreen :protected Screen
{
private:

    static void _PrintCurrency(Currency Currency)
    {
        std::cout << "\nCurrency Card:";
        _ShowBorderLine(50, '=');
        std::cout << "Country       : " << Currency.Country() << "\n";
        std::cout << "Code          : " << Currency.CurrencyCode() << "\n";
        std::cout << "Name          : " << Currency.CurrencyName() << "\n";
        std::cout << "Rate (1$) =   : " << Currency.Rate() << "\n";
        _ShowBorderLine(50, '=');
    }

public:

    static void ShowUpdateCurrencyRateScreen()
    {
        if (!CheckActiveSession())
        {
            return;
        }

        _DrawScreenHeader("Update Currency Rate Screen");

        std::string CurrencyCode = Core::InputValidate::ReadString("\nPlease Enter Currency Code (or 0 to Back): ");
        if (CurrencyCode == "0")
        {
            return;
        }

        while (!Currency::IsCurrencyExist(CurrencyCode))
        {
            CurrencyCode = Core::InputValidate::ReadString("\nCurrency is not found, choose another one (or 0 to Back): ");
            if (CurrencyCode == "0")
            {
                return;
            }
        }

        Currency Currency = Currency::FindByCode(CurrencyCode);
        _PrintCurrency(Currency);

        if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to update the rate of this currency? "))
        {
            std::cout << "\nUpdate Currency Rate:";
            _ShowBorderLine(50, '=');

            Currency.UpdateRate(static_cast<float>(Core::InputValidate::ReadNumber<double>("\nEnter New Rate: ")));

            _ShowSuccessMessage("Currency rate updated successfully.");
            _PrintCurrency(Currency);
        }
    }
};