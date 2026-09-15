#pragma once

#include <iostream>
#include "../Screen.h"
#include <iomanip>
#include "../../Core/Services/TransactionService.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

class TransactionHistoryScreen :protected Screen
{

private:

	static const char* _TypeName(int Type)
	{
		if (Type == static_cast<int>(Bank::Transactions::TransactionType::Deposit))
			return "Deposit";
		if (Type == static_cast<int>(Bank::Transactions::TransactionType::Withdrawal))
			return "Withdraw";
		if (Type == static_cast<int>(Bank::Transactions::TransactionType::Transfer))
			return "Transfer";
		return "Unknown";
	}

	static void _PrintTransactionRecordLine(const Bank::Transactions::TransactionEntry& Entry)
	{
		std::cout << std::setw(8) << std::left << "" << "| " << std::setw(28) << std::left << Entry.TransactionID;
		std::cout << "| " << std::setw(10) << std::left << _TypeName(Entry.Type);
		std::cout << "| " << std::setw(12) << std::left << Entry.FromAccount;
		std::cout << "| " << std::setw(12) << std::left << Entry.ToAccount;
		std::cout << "| " << std::setw(10) << std::left << std::fixed << std::setprecision(2) << Entry.Amount;
		std::cout << "| " << std::setw(8) << std::left << std::fixed << std::setprecision(2) << Entry.Fees;
		std::cout << "| " << std::setw(20) << std::left << Entry.Timestamp;
		std::cout << "| " << std::left << Entry.Description;
	}

public:

	static void ShowTransactionHistoryScreen()
	{
		std::string AccountNumber = Core::InputValidate::ReadString("\nPlease enter Account Number (or 0 to Back): ");
		if (AccountNumber == "0")
			return;

		std::vector<Bank::Transactions::TransactionEntry> vTransactions =
			Bank::Transactions::TransactionService::GetAccountHistory(AccountNumber);

		std::string Title = "\tTransaction History Screen";
		std::string SubTitle = "\t    Account: " + AccountNumber + " (" + std::to_string(vTransactions.size()) + ") Transaction(s).";

		_DrawScreenHeader(Title, SubTitle);

		std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
		std::cout << "_________________________________________\n" << std::endl;

		std::cout << std::setw(8) << std::left << "" << "| " << std::left << std::setw(28) << "Transaction ID";
		std::cout << "| " << std::left << std::setw(10) << "Type";
		std::cout << "| " << std::left << std::setw(12) << "From";
		std::cout << "| " << std::left << std::setw(12) << "To";
		std::cout << "| " << std::left << std::setw(10) << "Amount";
		std::cout << "| " << std::left << std::setw(8) << "Fees";
		std::cout << "| " << std::left << std::setw(20) << "DateTime";
		std::cout << "| " << std::left << "Description";

		std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
		std::cout << "_________________________________________\n" << std::endl;

		if (vTransactions.empty())
			std::cout << "\t\t\t\tNo transactions found for this account.";
		else
		{
			for (const Bank::Transactions::TransactionEntry& Record : vTransactions)
			{
				_PrintTransactionRecordLine(Record);
				std::cout << std::endl;
			}
		}

		std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
		std::cout << "_________________________________________\n" << std::endl;
	}

};