#pragma once

#include <iostream>
#include "../Screen.h"
#include "../../Core/Currency.h"
#include <iomanip>
#include <vector>
#include <string>

class CurrenciesListScreen :protected Screen
{

private:
    static void PrintCurrencyRecordLine(const Currency& Currency)
    {
        std::cout << CYAN << "| " << RESET << std::setw(30) << std::left << Currency.Country();
        std::cout << CYAN << "| " << RESET << std::setw(8) << std::left << Currency.CurrencyCode();
        std::cout << CYAN << "| " << RESET << std::setw(45) << std::left << Currency.CurrencyName();
        std::cout << CYAN << "| " << RESET << std::setw(12) << std::left << Currency.Rate();
        std::cout << CYAN << "|" << RESET;
    }

public:

    static void ShowCurrenciesListScreen()
    {
        if (!CheckActiveSession())
        {
            return;
        }

        std::vector<Currency> vCurrencies = Currency::GetCurrenciesList();

        _DrawScreenHeader("Currencies List Screen", "(" + std::to_string(vCurrencies.size()) + ") Currency.");

        _ShowTableBorder(102);

        std::cout << CYAN << "| " << RESET << std::left << std::setw(30) << "Country";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(8) << "Code";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(45) << "Name";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(12) << "Rate/(1$)";
        std::cout << CYAN << "|\n" << RESET;

        _ShowTableBorder(102);

        if (vCurrencies.size() == 0)
        {
            std::cout << "\n  No Currencies Available In the System!\n";
        }
        else
        {
            for (const Currency& Currency : vCurrencies)
            {
                PrintCurrencyRecordLine(Currency);
                std::cout << "\n";
            }
        }

        _ShowTableBorder(102);
    }

};