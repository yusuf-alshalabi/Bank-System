#pragma once

#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include <iomanip>
#include <vector>
#include <string>

class LoginRegisterScreen :protected Screen
{

private:

	static void PrintLoginRegisterRecordLine(const User::stLoginRegisterRecord& LoginRegisterRecord)
	{
		std::cout << CYAN << "| " << RESET << std::setw(25) << std::left << LoginRegisterRecord.DateTime;
		std::cout << CYAN << "| " << RESET << std::setw(10) << std::left << LoginRegisterRecord.UserName;
		std::cout << CYAN << "| " << RESET << std::setw(10) << std::left << _FormatPasswordForDisplay(LoginRegisterRecord.Password);
		std::cout << CYAN << "| " << RESET << std::setw(120) << std::left << User::FormatPermissions(LoginRegisterRecord.Permissions);
		std::cout << CYAN << "|" << RESET;
	}

public:

	static void ShowLoginRegisterScreen()
	{
		if (!CheckAccessRights(User::enPermissions::pLoginRegister))
		{
			return;
		}

		std::vector<User::stLoginRegisterRecord> vLoginRegisterRecord = User::GetLoginRegisterList();

		_DrawScreenHeader("Login Register List Screen", "(" + std::to_string(vLoginRegisterRecord.size()) + ") Record(s).");

		_ShowTableBorder(172);

		std::cout << CYAN << "| " << RESET << std::left << std::setw(25) << "Date/Time";
		std::cout << CYAN << "| " << RESET << std::left << std::setw(10) << "UserName";
		std::cout << CYAN << "| " << RESET << std::left << std::setw(10) << "Password";
		std::cout << CYAN << "| " << RESET << std::left << std::setw(120)<< "Permissions";
		std::cout << CYAN << "|\n" << RESET;

		_ShowTableBorder(172);

		if (vLoginRegisterRecord.size() == 0)
		{
			std::cout << "\n  No Logins Available In the System!\n";
		}
		else
		{
			for (const User::stLoginRegisterRecord& Record : vLoginRegisterRecord)
			{
				PrintLoginRegisterRecordLine(Record);
				std::cout << "\n";
			}
		}

		_ShowTableBorder(172);
	}

};