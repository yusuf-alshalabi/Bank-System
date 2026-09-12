#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include <iomanip>
#include "CurrenciesListScreen.h"
#include "FindCurrencyScreen.h"
#include "UpdateCurrencyRateScreen.h"

class CurrencyExchangeMainScreen :protected Screen
{

private:
    enum enCurrenciesMainMenueOptions {
        eListCurrencies = 1, eFindCurrency = 2, eUpdateCurrencyRate = 3,
        eCurrencyCalculator = 4, eMainMenue = 5
    };

    static short ReadCurrenciesMainMenueOptions()
    {
        std::cout << std::setw(37) << std::left << "" << "Choose what do you want to do? [1 to 5]? ";
        short Choice = Core::InputValidate::ReadNumberBetween<short>(1, 5, "Enter Number between 1 to 5? ");
        return Choice;
    }

    static void _GoBackToCurrenciesMenue()
    {
        std::cout << "\n\nPress any key to go back to Currencies Menue...";
        system("pause>0");
        ShowCurrenciesMenue();
    }

    static void _ShowCurrenciesListScreen()
    {
       // std::cout << "\nCurriencies List Screen Will Be Here.\n";
        CurrenciesListScreen::ShowCurrenciesListScreen();
    }

    static void _ShowFindCurrencyScreen()
    {
        // std::cout << "\nFind Currency Screen Will Be Here.\n";
        FindCurrencyScreen::ShowFindCurrencyScreen();
    }

    static void _ShowUpdateCurrencyRateScreen()
    {
        // std::cout << "\nUpdate Currency Rate Screen Will Be Here.\n";
        UpdateCurrencyRateScreen::ShowUpdateCurrencyRateScreen();
    }

    static void _ShowCurrencyCalculatorScreen()
    {
        std::cout << "\nCurrency Calculator Screen Will Be Here.\n";

    }

    static void _PerformCurrenciesMainMenueOptions(enCurrenciesMainMenueOptions CurrenciesMainMenueOptions)
    {

        switch (CurrenciesMainMenueOptions)
        {
        case enCurrenciesMainMenueOptions::eListCurrencies:
        {
            std::system("cls");
            _ShowCurrenciesListScreen();
            _GoBackToCurrenciesMenue();
            break;
        }

        case enCurrenciesMainMenueOptions::eFindCurrency:
        {
            std::system("cls");
            _ShowFindCurrencyScreen();
            _GoBackToCurrenciesMenue();
            break;
        }

        case enCurrenciesMainMenueOptions::eUpdateCurrencyRate:
        {
            std::system("cls");
            _ShowUpdateCurrencyRateScreen();
            _GoBackToCurrenciesMenue();
            break;
        }

        case enCurrenciesMainMenueOptions::eCurrencyCalculator:
        {
            std::system("cls");
            _ShowCurrencyCalculatorScreen();
            _GoBackToCurrenciesMenue();
            break;
        }

        case enCurrenciesMainMenueOptions::eMainMenue:
        {
            //do nothing here the main screen will handle it :-) ;
        }
        }

    }

public:

    static void ShowCurrenciesMenue()
    {

        system("cls");
        _DrawScreenHeader("    Currancy Exhange Main Screen");

        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t\t  Currency Exhange Menue\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t[1] List Currencies.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[2] Find Currency.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[3] Update Rate.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[4] Currency Calculator.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[5] Main Menue.\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";

        _PerformCurrenciesMainMenueOptions((enCurrenciesMainMenueOptions)ReadCurrenciesMainMenueOptions());
    }

};

