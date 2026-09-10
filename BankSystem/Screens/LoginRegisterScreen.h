#pragma once

#include <iostream>
#include "Screen.h"
#include "../Core/User.h"
#include <iomanip>
#include <vector>
#include <fstream>


class LoginRegisterScreen :protected Screen
{

private:

	static void PrintLoginRegisterRecordLine(User::stLoginRegisterRecord LoginRegisterRecord)
	{

		std::cout << std::setw(8) << std::left << "" << "| " << std::setw(35) << std::left << LoginRegisterRecord.DateTime;
		std::cout << "| " << std::setw(20) << std::left << LoginRegisterRecord.UserName;
		std::cout << "| " << std::setw(20) << std::left << LoginRegisterRecord.Password;
		std::cout << "| " << std::setw(10) << std::left << LoginRegisterRecord.Permissions;
	}

public:

	static void ShowLoginRegisterScreen()
	{
		if (!CheckAccessRights(User::enPermissions::pLoginRegister))
		{
			return;// this will exit the function and it will not continue
		}

		vector <User::stLoginRegisterRecord> vLoginRegisterRecord = User::GetLoginRegisterList();

		string Title = "\tLogin Register List Screen";
		string SubTitle = "\t\t(" + to_string(vLoginRegisterRecord.size()) + ") Record(s).";

		_DrawScreenHeader(Title, SubTitle);

		std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
		std::cout << "_________________________________________\n" << endl;

		std::cout << std::setw(8) << std::left << "" << "| " << std::left << std::setw(35) << "Date/Time";
		std::cout << "| " << std::left << std::setw(20) << "UserName";
		std::cout << "| " << std::left << std::setw(20) << "Password";
		std::cout << "| " << std::left << std::setw(10) << "Permissions";
		std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
		std::cout << "_________________________________________\n" << endl;

		if (vLoginRegisterRecord.size() == 0)
			std::cout << "\t\t\t\tNo Logins Available In the System!";
		else

			for (User::stLoginRegisterRecord Record : vLoginRegisterRecord)
			{

				PrintLoginRegisterRecordLine(Record);
				std::cout << endl;
			}

		std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
		std::cout << "_________________________________________\n" << endl;

	}

};
