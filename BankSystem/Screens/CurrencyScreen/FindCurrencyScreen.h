#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include <iomanip>
#include "../../Core/Currency.h"

class FindCurrencyScreen :protected Screen
{

private:
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

	static void _ShowResults(Currency Currency)
	{
		if (!Currency.IsEmpty())
		{
			std::cout << "\nCurrency Found :-)\n";
			_PrintCurrency(Currency);
		}
		else
		{
			std::cout << "\nCurrency Was not Found :-(\n";
		}
	}

public:

	static void ShowFindCurrencyScreen()
	{

		_DrawScreenHeader("\t  Find Currency Screen");

		short Answer = Core::InputValidate::ReadNumberBetween<short>(1, 2, "\nFind By: [1] Code or [2] Country ?");

		if (Answer == 1)
		{
			std::string CurrencyCode;
			std::cout << "\nPlease Enter CurrencyCode: ";
			CurrencyCode = Core::InputValidate::ReadString();
			Currency Currency = Currency::FindByCode(CurrencyCode);
			_ShowResults(Currency);
		}
		else
		{
			std::string Country;
			std::cout << "\nPlease Enter Country Name: ";
			Country = Core::InputValidate::ReadString();
			Currency Currency = Currency::FindByCountry(Country);
			_ShowResults(Currency);
		}
	}
};

