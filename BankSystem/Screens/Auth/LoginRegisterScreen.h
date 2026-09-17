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
		std::cout << "| " << std::setw(35) << std::left << LoginRegisterRecord.DateTime;
		std::cout << "| " << std::setw(20) << std::left << LoginRegisterRecord.UserName;
		std::cout << "| " << std::setw(20) << std::left << _FormatPasswordForDisplay(LoginRegisterRecord.Password);
		std::cout << "| " << std::setw(10) << std::left << LoginRegisterRecord.Permissions;
		std::cout << "|";
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

		_ShowTableBorder(94);

		std::cout << "| " << std::left << std::setw(35) << "Date/Time";
		std::cout << "| " << std::left << std::setw(20) << "UserName";
		std::cout << "| " << std::left << std::setw(20) << "Password";
		std::cout << "| " << std::left << std::setw(10) << "Permissions";
		std::cout << "|";

		_ShowTableBorder(94);

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

		_ShowTableBorder(94);
	}

};