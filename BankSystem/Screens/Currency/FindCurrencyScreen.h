#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "../../Core/Currency.h"

class FindCurrencyScreen :protected Screen
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

	static void _ShowResults(Currency Currency)
	{
		if (!Currency.IsEmpty())
		{
			_ShowSuccessMessage("Currency found.");
			_PrintCurrency(Currency);
		}
		else
		{
			_ShowErrorMessage("Currency was not found.");
		}
	}

public:

	static void ShowFindCurrencyScreen()
	{
		if (!CheckActiveSession())
		{
			return;
		}

		_DrawScreenHeader("Find Currency Screen");

		_ShowOptions({ "Find By Code", "Find By Country" });
		_ShowBackOption(false);
		_ShowLine(46, '-');

		short Answer = _ReadMenuOption(1, 2);
		if (Answer == 0)
		{
			return;
		}

		if (Answer == 1)
		{
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

			_ShowResults(Currency::FindByCode(CurrencyCode));
		}
		else
		{
			std::string Country = Core::InputValidate::ReadString("\nPlease Enter Country Name (or 0 to Back): ");
			if (Country == "0")
			{
				return;
			}

			while (Currency::FindByCountry(Country).IsEmpty())
			{
				Country = Core::InputValidate::ReadString("\nCountry is not found, choose another one (or 0 to Back): ");
				if (Country == "0")
				{
					return;
				}
			}

			_ShowResults(Currency::FindByCountry(Country));
		}
	}
};