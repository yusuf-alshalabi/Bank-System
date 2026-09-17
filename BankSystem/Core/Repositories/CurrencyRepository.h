#pragma once
#include <cstddef>
#include <string>
#include <vector>

#include "../Infrastructure/AtomicFileStore.h"
#include "../Currency.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/String.h"

namespace Bank::Data {

	using Bank::Persistence::AtomicFileStore;

	// File-backed repository for Currency records (Currencies.txt) - migration
	// of the file I/O formerly owned by Core/Currency.h. Owns the record
	// format and all storage concerns (atomic saves, malformed-line skipping).
	class CurrencyRepository
	{
	public:
		static constexpr const char* FilePath = "Currencies.txt";
		static constexpr std::size_t FieldCount = 4;

		struct LoadResult
		{
			std::vector<Currency> Records;
			std::size_t MalformedLines = 0;
		};

		static std::string ToLine(const Currency& currency)
		{
			std::string line;
			line += currency.Country() + AtomicFileStore::Separator;
			line += currency.CurrencyCode() + AtomicFileStore::Separator;
			line += currency.CurrencyName() + AtomicFileStore::Separator;
			line += std::to_string(currency.Rate());
			return line;
		}

		// Empty Currency (IsEmpty() == true) when the line is malformed.
		static Currency FromLine(const std::string& line)
		{
			std::vector<std::string> fields = AtomicFileStore::SplitPreserve(line, AtomicFileStore::Separator);
			if (fields.size() < FieldCount)
				return _Empty();

			try
			{
				return Currency(Currency::enMode::UpdateMode, fields[0], fields[1], fields[2],
					static_cast<float>(std::stod(fields[3])));
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
				Currency currency = FromLine(line);
				if (currency.IsEmpty())
				{
					++result.MalformedLines;
					continue;
				}
				result.Records.push_back(std::move(currency));
			}
			return result;
		}

		// Atomically persists the whole list.
		static bool SaveAll(const std::vector<Currency>& currencies)
		{
			std::vector<std::string> lines;
			lines.reserve(currencies.size());
			for (const Currency& currency : currencies)
				lines.push_back(ToLine(currency));
			return AtomicFileStore::SaveLines(FilePath, lines);
		}

		static Currency FindByCode(const std::string& currencyCode)
		{
			std::string code = Core::String::UpperAllString(currencyCode);
			for (const std::string& line : AtomicFileStore::LoadLines(FilePath))
			{
				Currency currency = FromLine(line);
				if (!currency.IsEmpty() && currency.CurrencyCode() == code)
					return currency;
			}
			return _Empty();
		}

		static Currency FindByCountry(const std::string& country)
		{
			std::string target = Core::String::UpperAllString(country);
			for (const std::string& line : AtomicFileStore::LoadLines(FilePath))
			{
				Currency currency = FromLine(line);
				if (!currency.IsEmpty() && Core::String::UpperAllString(currency.Country()) == target)
					return currency;
			}
			return _Empty();
		}

		static bool Exists(const std::string& currencyCode)
		{
			return !FindByCode(currencyCode).IsEmpty();
		}

	private:
		static Currency _Empty()
		{
			return Currency(Currency::enMode::EmptyMode, "", "", "", 0);
		}
	};

} // namespace Bank::Data