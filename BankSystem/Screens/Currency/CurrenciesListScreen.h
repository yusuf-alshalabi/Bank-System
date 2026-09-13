#pragma once

#include <iostream>
#include "../Screen.h"
#include "../../Core/Currency.h"
#include <iomanip>

class CurrenciesListScreen :protected Screen
{

private:
    static void PrintCurrencyRecordLine(Currency Currency)
    {

        std::cout << std::setw(8) << std::left << "" << "| " << std::setw(30) << std::left << Currency.Country();
        std::cout << "| " << std::setw(8) << std::left << Currency.CurrencyCode();
        std::cout << "| " << std::setw(45) << std::left << Currency.CurrencyName();
        std::cout << "| " << std::setw(10) << std::left << Currency.Rate();

    }

public:


    static void ShowCurrenciesListScreen()
    {


        vector <Currency> vCurrencys = Currency::GetCurrenciesList();
        string Title = "\t  Currencies List Screen";
        string SubTitle = "\t    (" + to_string(vCurrencys.size()) + ") Currency.";

        _DrawScreenHeader(Title, SubTitle);
        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_______________________________________________\n" << std::endl;

        std::cout << std::setw(8) << std::left << "" << "| " << std::left << std::setw(30) << "Country";
        std::cout << "| " << std::left << std::setw(8) << "Code";
        std::cout << "| " << std::left << std::setw(45) << "Name";
        std::cout << "| " << std::left << std::setw(10) << "Rate/(1$)";
        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_______________________________________________\n" << std::endl;

        if (vCurrencys.size() == 0)
            std::cout << "\t\t\t\tNo Currencies Available In the System!";
        else

            for (Currency Currency : vCurrencys)
            {

                PrintCurrencyRecordLine(Currency);
                std::cout << std::endl;
            }

        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_______________________________________________\n" << std::endl;

    }

};

