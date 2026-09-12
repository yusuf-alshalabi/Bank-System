#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "../../Core/Currency.h"

class UpdateCurrencyRateScreen :protected Screen
{
private:

    static double _ReadRate()
    {
        double NewRate = 0;

        NewRate = Core::InputValidate::ReadNumber<double>("\nEnter New Rate: ");
        return NewRate;
    }

    static void _PrintCurrency(Currency Currency)
    {
        std::cout << "\nCurrency Card:\n";
        std::cout << "_____________________________\n";
        std::cout << "\nCountry    : " << Currency.Country();
        std::cout << "\nCode       : " << Currency.CurrencyCode();
        std::cout << "\nName       : " << Currency.CurrencyName();
        std::cout << "\nRate(1$) = : " << Currency.Rate();
        std::cout << "\n_____________________________\n";
    }

public:

    static void ShowUpdateCurrencyRateScreen()
    {
        _DrawScreenHeader("\tUpdate Currency Screen");

        std::string CurrencyCode = "";

        CurrencyCode = Core::InputValidate::ReadString("\nPlease Enter Currency Code: ");

        while (!Currency::IsCurrencyExist(CurrencyCode))
        {
            CurrencyCode = Core::InputValidate::ReadString("\nCurrency is not found, choose another one: ");
        }

        Currency Currency = Currency::FindByCode(CurrencyCode);
        _PrintCurrency(Currency);

   
        if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to update the rate of this Currency y/n? "))
        {
            std::cout << "\n\nUpdate Currency Rate:";
            std::cout << "\n____________________\n";

            Currency.UpdateRate(_ReadRate());

            std::cout << "\nCurrency Rate Updated Successfully :-)\n";
            _PrintCurrency(Currency);
        }

    }
};

