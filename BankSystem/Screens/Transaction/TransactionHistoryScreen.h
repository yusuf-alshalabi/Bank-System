#pragma once

#include <iostream>
#include "../Screen.h"
#include "../../Core/User.h"
#include <iomanip>
#include <vector>
#include <string>

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
		std::cout << CYAN << "| " << RESET << std::setw(28) << std::left << Entry.TransactionID;
		std::cout << CYAN << "| " << RESET << std::setw(10) << std::left << _TypeName(Entry.Type);
		std::cout << CYAN << "| " << RESET << std::setw(12) << std::left << Entry.FromAccount;
		std::cout << CYAN << "| " << RESET << std::setw(12) << std::left << Entry.ToAccount;
		std::cout << CYAN << "| " << RESET << GREEN << std::setw(10) << std::left << _CurrencyText(Entry.Amount) << RESET;
		std::cout << CYAN << "| " << RESET << GREEN << std::setw(8) << std::left << _CurrencyText(Entry.Fees) << RESET;
		std::cout << CYAN << "| " << RESET << std::setw(20) << std::left << Entry.Timestamp;
		std::cout << CYAN << "| " << RESET << std::setw(27) << std::left << Entry.Description;
		std::cout << CYAN << "|" << RESET;
	}

public:

	static void ShowTransactionHistoryScreen()
	{
		if (!CheckAccessRights(User::enPermissions::pTranactions))
		{
			return;
		}

		std::string AccountNumber = Core::InputValidate::ReadString("\nPlease enter Account Number (or 0 to Back): ");
		if (AccountNumber == "0")
		{
			return;
		}

		std::vector<Bank::Transactions::TransactionEntry> vTransactions =
			Bank::Transactions::TransactionService::GetAccountHistory(AccountNumber);

		_DrawScreenHeader("Transaction History Screen", "Account: " + AccountNumber + " (" + std::to_string(vTransactions.size()) + ") Transaction(s).");

		_ShowTableBorder(142);

		std::cout << CYAN << "| " << RESET << std::left << std::setw(28) << "Transaction ID";
		std::cout << CYAN << "| " << RESET << std::left << std::setw(10) << "Type";
		std::cout << CYAN << "| " << RESET << std::left << std::setw(12) << "From";
		std::cout << CYAN << "| " << RESET << std::left << std::setw(12) << "To";
		std::cout << CYAN << "| " << RESET << std::left << std::setw(10) << "Amount";
		std::cout << CYAN << "| " << RESET << std::left << std::setw(8) << "Fees";
		std::cout << CYAN << "| " << RESET << std::left << std::setw(20) << "DateTime";
		std::cout << CYAN << "| " << RESET << std::left << std::setw(27) << "Description";
		std::cout << CYAN << "|\n" << RESET;

		_ShowTableBorder(142);

		if (vTransactions.empty())
		{
			std::cout << "\n  No transactions found for this account.\n";
		}
		else
		{
			for (const Bank::Transactions::TransactionEntry& Record : vTransactions)
			{
				_PrintTransactionRecordLine(Record);
				std::cout << "\n";
			}
		}

		_ShowTableBorder(142);
	}

};