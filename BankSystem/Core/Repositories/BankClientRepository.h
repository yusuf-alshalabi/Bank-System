#pragma once
#include <cstddef>
#include <string>
#include <vector>

#include "../Infrastructure/AtomicFileStore.h"
#include "../BankClient.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/String.h"

namespace Bank::Data {

	using Bank::Persistence::AtomicFileStore;

	// File-backed repository for BankClient records (Clients.txt) - migration
	// of the file I/O formerly owned by Core/BankClient.h. Owns the record
	// format and all storage concerns (atomic saves, malformed-line skipping).
	class BankClientRepository
	{
	public:
		static constexpr const char* FilePath = "Clients.txt";
		static constexpr std::size_t FieldCount = 7;

		struct LoadResult
		{
			std::vector<BankClient> Records;
			std::size_t MalformedLines = 0;
		};

		static std::string ToLine(const BankClient& client)
		{
			std::string line;
			line += client.GetFirstName() + AtomicFileStore::Separator;
			line += client.GetLastName() + AtomicFileStore::Separator;
			line += client.GetEmail() + AtomicFileStore::Separator;
			line += client.GetPhone() + AtomicFileStore::Separator;
			line += client.GetAccountNumber() + AtomicFileStore::Separator;
			line += client.GetPinCode() + AtomicFileStore::Separator;
			line += std::to_string(client.GetAccountBalance());
			return line;
		}

		// Empty BankClient (IsEmpty() == true) when the line is malformed.
		static BankClient FromLine(const std::string& line)
		{
			std::vector<std::string> fields = AtomicFileStore::SplitPreserve(line, AtomicFileStore::Separator);
			if (fields.size() < FieldCount)
				return _Empty();

			try
			{
				return BankClient(BankClient::enMode::UpdateMode, fields[0], fields[1], fields[2],
					fields[3], fields[4], fields[5], std::stod(fields[6]));
			}
			catch (const std::exception&)
			{
				return _Empty();
			}
		}

		static LoadResult LoadAll()
		{
			LoadResult result;
			for (const std::string& line : AtomicFileStore::LoadLines(FilePath))
			{
				BankClient client = FromLine(line);
				if (client.IsEmpty())
				{
					++result.MalformedLines;
					continue;
				}
				result.Records.push_back(std::move(client));
			}
			return result;
		}

		// Atomically persists the list, skipping records marked for deletion.
		static bool SaveAll(const std::vector<BankClient>& clients)
		{
			std::vector<std::string> lines;
			lines.reserve(clients.size());
			for (const BankClient& client : clients)
			{
				if (!client.MarkedForDeleted())
					lines.push_back(ToLine(client));
			}
			return AtomicFileStore::SaveLines(FilePath, lines);
		}

		static BankClient FindByAccountNumber(const std::string& accountNumber)
		{
			for (const std::string& line : AtomicFileStore::LoadLines(FilePath))
			{
				BankClient client = FromLine(line);
				if (!client.IsEmpty() && client.GetAccountNumber() == accountNumber)
					return client;
			}
			return _Empty();
		}

		static BankClient FindByAccountNumberAndPin(const std::string& accountNumber, const std::string& pinCode)
		{
			for (const std::string& line : AtomicFileStore::LoadLines(FilePath))
			{
				BankClient client = FromLine(line);
				if (!client.IsEmpty() && client.GetAccountNumber() == accountNumber
					&& client.GetPinCode() == pinCode)
					return client;
			}
			return _Empty();
		}

		static bool Exists(const std::string& accountNumber)
		{
			return !FindByAccountNumber(accountNumber).IsEmpty();
		}

	private:
		static BankClient _Empty()
		{
			return BankClient(BankClient::enMode::EmptyMode, "", "", "", "", "", "", 0);
		}
	};

} // namespace Bank::Data