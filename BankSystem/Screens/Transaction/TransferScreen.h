#pragma once

#include <iostream>
#include "../Screen.h"
#include "../../Core/Person.h"
#include "../../Core/BankClient.h"
#include "../../Core/User.h"
#include "../../Core/Infrastructure/SessionManager.h"
#include "../../Core/Services/TransactionService.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

class TransferScreen : protected Screen
{

private:

	static void _PrintClient(const BankClient& Client)
	{
		std::cout << "\nClient Card:";
		_ShowBorderLine(60, '=');
		std::cout << "Full Name   : " << Client.FullName() << "\n";
		std::cout << "Acc. Number : " << Client.GetAccountNumber() << "\n";
		std::cout << "Balance     : " << Client.GetAccountBalance() << "\n";
		_ShowBorderLine(60, '=');
	}

	static std::string _ReadAccountNumber(const std::string& Prompt)
	{
		std::string AccountNumber = Core::InputValidate::ReadString(Prompt);
		if (AccountNumber == "0")
		{
			return "0";
		}

		while (!BankClient::IsClientExist(AccountNumber))
		{
			AccountNumber = Core::InputValidate::ReadString("\nClient with [" + AccountNumber + "] does not exist (or 0 to Back): ");
			if (AccountNumber == "0")
			{
				return "0";
			}
		}
		return AccountNumber;
	}

	static double ReadAmount(const BankClient& SourceClient)
	{
		double Amount = Core::InputValidate::ReadNumber<double>("\nEnter transfer amount (or 0 to cancel): ");
		if (Amount == 0)
		{
			return 0;
		}

		while (Amount <= 0)
		{
			std::cout << "\nTransfer amount must be greater than zero.\n";
			Amount = Core::InputValidate::ReadNumber<double>("\nEnter transfer amount (or 0 to cancel): ");
			if (Amount == 0)
			{
				return 0;
			}
		}

		while (SourceClient.GetAccountBalance() < Amount)
		{
			std::cout << "\nAmount exceeds the available balance.\n";
			Amount = Core::InputValidate::ReadNumber<double>("\nEnter another amount (or 0 to cancel): ");
			if (Amount == 0)
			{
				return 0;
			}
		}
		return Amount;
	}

public:

	static void ShowTransferScreen()
	{
		if (!CheckAccessRights(User::enPermissions::pTranactions))
		{
			return;
		}

		_DrawScreenHeader("Transfer Screen");

		std::string SourceAccount = _ReadAccountNumber("\nPlease enter Account Number to transfer from (or 0 to Back): ");
		if (SourceAccount == "0")
		{
			return;
		}

		BankClient SourceClient = BankClient::Find(SourceAccount);
		_PrintClient(SourceClient);

		std::string DestinationAccount = _ReadAccountNumber("\nPlease enter Account Number to transfer to (or 0 to Back): ");
		if (DestinationAccount == "0")
		{
			return;
		}

		BankClient DestinationClient = BankClient::Find(DestinationAccount);
		_PrintClient(DestinationClient);

		double Amount = ReadAmount(SourceClient);
		if (Amount == 0)
		{
			_ShowErrorMessage("Operation was cancelled.");
			return;
		}

		if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to perform this transaction? "))
		{
			double TransferFee = Amount * Bank::Transactions::TransactionService::TransferFeeRate;

			if (SourceClient.Transfer(Amount, DestinationClient, Bank::Security::SessionManager::Instance().CurrentUserName()))
			{
				_ShowSuccessMessage("Amount transferred successfully.");
				std::cout << "Transfer Fee (1%): " << TransferFee << "\n";
				_PrintClient(SourceClient);
				_PrintClient(DestinationClient);
			}
			else
			{
				_ShowErrorMessage("Transfer failed. Insufficient balance (amount + 1% fee) or invalid input.");
			}
		}
		else
		{
			_ShowErrorMessage("Operation was cancelled.");
		}
	}
};