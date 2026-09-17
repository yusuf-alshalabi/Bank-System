#pragma once
#include <iostream>
#include "../Screen.h"
#include "CurrenciesListScreen.h"
#include "FindCurrencyScreen.h"
#include "UpdateCurrencyRateScreen.h"
#include "CurrencyCalculatorScreen.h"

class CurrencyExchangeMainScreen :protected Screen
{

private:

    enum enCurrenciesMainMenueOptions {
        eListCurrencies = 1, eFindCurrency = 2, eUpdateCurrencyRate = 3,
        eCurrencyCalculator = 4
    };

    static void _GoBackToCurrenciesMenue()
    {
        _PressEnterToContinue();
    }

    static void _ShowCurrenciesListScreen()
    {
        CurrenciesListScreen::ShowCurrenciesListScreen();
    }

    static void _ShowFindCurrencyScreen()
    {
        FindCurrencyScreen::ShowFindCurrencyScreen();
    }

    static void _ShowUpdateCurrencyRateScreen()
    {
        UpdateCurrencyRateScreen::ShowUpdateCurrencyRateScreen();
    }

    static void _ShowCurrencyCalculatorScreen()
    {
        CurrencyCalculatorScreen::ShowCurrencyCalculatorScreen();
    }

    static void _PerformCurrenciesMainMenueOptions(enCurrenciesMainMenueOptions CurrenciesMainMenueOptions)
    {
        switch (CurrenciesMainMenueOptions)
        {
        case enCurrenciesMainMenueOptions::eListCurrencies:
            _ClearScreen();
            _ShowCurrenciesListScreen();
            _GoBackToCurrenciesMenue();
            break;

        case enCurrenciesMainMenueOptions::eFindCurrency:
            _ClearScreen();
            _ShowFindCurrencyScreen();
            _GoBackToCurrenciesMenue();
            break;

        case enCurrenciesMainMenueOptions::eUpdateCurrencyRate:
            _ClearScreen();
            _ShowUpdateCurrencyRateScreen();
            _GoBackToCurrenciesMenue();
            break;

        case enCurrenciesMainMenueOptions::eCurrencyCalculator:
            _ClearScreen();
            _ShowCurrencyCalculatorScreen();
            _GoBackToCurrenciesMenue();
            break;
        }
    }

public:

    static void ShowCurrenciesMenue()
    {
        if (!CheckActiveSession())
        {
            return;
        }

        while (true)
        {
            _ClearScreen();
            _DrawScreenHeader("Currency Exchange Menu Screen");

            _ShowOptions({ "List Currencies", "Find Currency", "Update Rate", "Currency Calculator" });
            _ShowBackOption(false);
            _ShowLine(60, '-');

            short Choice = _ReadMenuOption(1, 4);

            if (Choice == 0)
            {
                return;
            }

            _PerformCurrenciesMainMenueOptions(static_cast<enCurrenciesMainMenueOptions>(Choice));
        }
    }

};