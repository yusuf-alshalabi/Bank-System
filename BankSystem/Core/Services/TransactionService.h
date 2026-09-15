#pragma once
#include <chrono>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <sodium.h>

#include "../Infrastructure/AtomicFileStore.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/Date.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/String.h"

namespace Bank::Transactions {

	using Bank::Persistence::AtomicFileStore;

	enum class TransactionType
	{
		Deposit = 1,
		Withdrawal = 2,
		Transfer = 3
	};

	// Domain model for a single financial transaction - migration of the
	// legacy Transaction struct from LastVersion/Globals.h.
	struct TransactionEntry
	{
		std::string TransactionID;
		int Type = static_cast<int>(TransactionType::Deposit);
		std::string FromAccount;
		std::string ToAccount;
		double Amount = 0.0;
		double Fees = 0.0;
		std::string Timestamp;
		std::string Description;
	};

	// Financial transaction service - migration of LastVersion/TransactionManager.h.
	//
	// Generates unique TXN ids, applies the 1% transfer fee and provides
	// append-only storage of every bank operation plus per-account history
	// reports. Balance mutation itself is performed by BankClient using the
	// repositories so that a transfer is committed as one atomic file save.
	class TransactionService
	{
	public:
		static constexpr double TransferFeeRate = 0.01;

		static constexpr const char* FilePath = "Transactions.txt";

		// "TXN" + microsecond timestamp + 8 hex random digits (legacy format).
		static std::string GenerateTransactionId()
		{
			auto now = std::chrono::high_resolution_clock::now();
			auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
				now.time_since_epoch()).count();

			uint32_t randomNumber = randombytes_random();

			std::ostringstream ss;
			ss << "TXN" << std::setw(16) << std::setfill('0') << microseconds << std::hex << std::setw(8) << std::setfill('0') << randomNumber;
			return ss.str();
		}

		static std::string ToLine(const TransactionEntry& entry)
		{
			std::string line;
			line += entry.TransactionID + AtomicFileStore::Separator;
			line += std::to_string(entry.Type) + AtomicFileStore::Separator;
			line += entry.FromAccount + AtomicFileStore::Separator;
			line += entry.ToAccount + AtomicFileStore::Separator;
			line += _FormatAmount(entry.Amount) + AtomicFileStore::Separator;
			line += _FormatAmount(entry.Fees) + AtomicFileStore::Separator;
			line += entry.Timestamp + AtomicFileStore::Separator;
			line += entry.Description;
			return line;
		}

		static TransactionEntry FromLine(const std::string& line)
		{
			TransactionEntry entry;
			std::vector<std::string> fields = AtomicFileStore::SplitPreserve(line, AtomicFileStore::Separator);
			if (fields.size() < 8)
				return entry;

			try
			{
				entry.TransactionID = fields[0];
				entry.Type = std::stoi(fields[1]);
				entry.FromAccount = fields[2];
				entry.ToAccount = fields[3];
				entry.Amount = std::stod(fields[4]);
				entry.Fees = std::stod(fields[5]);
				entry.Timestamp = fields[6];
				entry.Description = fields[7];
				return entry;
			}
			catch (const std::exception&)
			{
				return TransactionEntry();
			}
		}

		// Appends a new transaction record and returns it (empty ID on failure).
		static TransactionEntry Record(int type, const std::string& fromAccount,
			const std::string& toAccount, double amount, double fees,
			const std::string& description)
		{
			TransactionEntry entry;
			entry.TransactionID = GenerateTransactionId();
			entry.Type = type;
			entry.FromAccount = fromAccount;
			entry.ToAccount = toAccount;
			entry.Amount = amount;
			entry.Fees = fees;
			entry.Timestamp = Core::Date::GetSystemDateTime();
			entry.Description = description;

			if (!Append(entry))
				return TransactionEntry();
			return entry;
		}

		static bool Append(const TransactionEntry& entry)
		{
			if (entry.TransactionID.empty())
				return false;
			return AtomicFileStore::AppendLine(FilePath, ToLine(entry));
		}

		static std::vector<TransactionEntry> LoadAll()
		{
			std::vector<TransactionEntry> entries;
			for (const std::string& line : AtomicFileStore::LoadLines(FilePath))
			{
				TransactionEntry entry = FromLine(line);
				if (entry.TransactionID.empty())
					continue; // skip malformed records
				entries.push_back(std::move(entry));
			}
			return entries;
		}

		// Every transaction touching the account, newest first.
		static std::vector<TransactionEntry> GetAccountHistory(const std::string& accountNumber)
		{
			std::vector<TransactionEntry> history = LoadAll();
			std::vector<TransactionEntry> result;

			for (std::size_t i = history.size(); i > 0; --i)
			{
				const TransactionEntry& entry = history[i - 1];
				if (entry.FromAccount == accountNumber || entry.ToAccount == accountNumber)
					result.push_back(entry);
			}

			return result;
		}

	private:
		static std::string _FormatAmount(double value)
		{
			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << value;
			return ss.str();
		}
	};

} // namespace Bank::Transactions