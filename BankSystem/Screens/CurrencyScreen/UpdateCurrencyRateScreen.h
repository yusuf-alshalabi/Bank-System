#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "../../Core/Currency.h"

class UpdateCurrencyRateScreen :protected Screen
{
private:

    static float _ReadRate()
    {
        std::cout << "\nEnter New Rate: ";
        float NewRate = 0;

        NewRate = Core::InputValidate::ReadNumber<float>();
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

        std::cout << "\nPlease Enter Currency Code: ";
        CurrencyCode = Core::InputValidate::ReadString();

        while (!Currency::IsCurrencyExist(CurrencyCode))
        {
            std::cout << "\nCurrency is not found, choose another one: ";
            CurrencyCode = Core::InputValidate::ReadString();
        }

        Currency Currency = Currency::FindByCode(CurrencyCode);
        _PrintCurrency(Currency);

        std::cout << "\nAre you sure you want to update the rate of this Currency y/n? ";

        char Answer = 'n';
        std::cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {

            std::cout << "\n\nUpdate Currency Rate:";
            std::cout << "\n____________________\n";

            Currency.UpdateRate(_ReadRate());

            std::cout << "\nCurrency Rate Updated Successfully :-)\n";
            _PrintCurrency(Currency);


        }

    }
};

