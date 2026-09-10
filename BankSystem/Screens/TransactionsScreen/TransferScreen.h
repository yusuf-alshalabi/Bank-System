#pragma once

#include <iostream>
#include "../Screen.h"
#include "../../Core/Person.h"
#include "../../Core/BankClient.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "../Global.h"

class TransferScreen : protected Screen
{

private:

	static void _PrintClient(const BankClient& Client)
	{
		std::cout << "\nClient Card:";
		std::cout << "\n___________________";
		std::cout << "\nFull Name   : " << Client.FullName();
		std::cout << "\nAcc. Number : " << Client.GetAccountNumber();
		std::cout << "\nBalance     : " << Client.GetAccountBalance();
		std::cout << "\n___________________\n";
	}

	static string _ReadAccountNumber(const std::string& Prompt)
	{
		std::string AccountNumber = Core::InputValidate::ReadString(Prompt);
		while (!BankClient::IsClientExist(AccountNumber))
		{
			std::cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
			AccountNumber = Core::InputValidate::ReadString(Prompt);
		}
		return AccountNumber;
	}

	static string _ReadAccountNumber()
	{
		string AccountNumber;
		cout << "\nPlease Enter Account Number to Transfer From: ";
		AccountNumber = Core::InputValidate::ReadString();
		while (!BankClient::IsClientExist(AccountNumber))
		{
			cout << "\nAccount number is not found, choose another one: ";
			AccountNumber = Core::InputValidate::ReadString();
		}
		return AccountNumber;
	}

	static double ReadAmount(const BankClient& SourceClient)
	{
		double Amount = Core::InputValidate::ReadNumber<double>("\nEnter transfer amount? ");

		while (Amount <= 0)
		{
			std::cout << "\nTransfer amount must be greater than zero.\n";
			Amount = Core::InputValidate::ReadNumber<double>("\nEnter transfer amount? ");
		}

		while (SourceClient.GetAccountBalance() < Amount)
		{
			std::cout << "\nAmount Exceeds the available Balance,";
			Amount = Core::InputValidate::ReadNumber<double>("\nEnter another amount : ");
		}
		return Amount;
	}


public:

	static void ShowTransferScreen()
	{
		_DrawScreenHeader("\t   Transfer Screen");

		BankClient SourceClient = BankClient::Find(_ReadAccountNumber("\nPlease enter Account Number to transfer from: "));
		_PrintClient(SourceClient);

		BankClient DestinationClient = BankClient::Find(_ReadAccountNumber("\nPlease enter Account Number to transfer to: "));
		_PrintClient(DestinationClient);

		double Amount = ReadAmount(SourceClient);


		if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to perform this transaction? "))
		{
			SourceClient.Transfer(Amount, DestinationClient, CurrentUser.UserName);
			std::cout << "\nAmount Transferred Successfully.\n";
			_PrintClient(SourceClient);
			_PrintClient(DestinationClient);
		}
		else
		{
			std::cout << "\nOperation was cancelled.\n";
		}
	}
};

