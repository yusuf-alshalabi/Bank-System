#pragma once
#include <cstddef>
#include <string>
#include <vector>

#include "../Infrastructure/AtomicFileStore.h"
#include "../User.h"
#include "../Infrastructure/PasswordHasher.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/String.h"

namespace Bank::Data {

	using Bank::Persistence::AtomicFileStore;

	// File-backed repository for User records (Users.txt) - migration of the
	// file I/O formerly owned by Core/User.h. Owns the record format and all
	// storage concerns (atomic saves, malformed-line skipping) while User
	// keeps the domain logic.
	class UserRepository
	{
	public:
		static constexpr const char* FilePath = "Users.txt";
		static constexpr std::size_t FieldCount = 7;

		struct LoadResult
		{
			std::vector<User> Records;
			std::size_t MalformedLines = 0;
		};

		static std::string ToLine(const User& user)
		{
			std::string line;
			line += user.GetFirstName() + AtomicFileStore::Separator;
			line += user.GetLastName() + AtomicFileStore::Separator;
			line += user.GetEmail() + AtomicFileStore::Separator;
			line += user.GetPhone() + AtomicFileStore::Separator;
			line += user.GetUserName() + AtomicFileStore::Separator;
			line += Bank::Security::PasswordHasher::HashIfNeeded(user.GetPassword()) + AtomicFileStore::Separator;
			line += std::to_string(user.GetPermissions());
			return line;
		}

		// Empty User (IsEmpty() == true) when the line is malformed.
		static User FromLine(const std::string& line)
		{
			std::vector<std::string> fields = AtomicFileStore::SplitPreserve(line, AtomicFileStore::Separator);
			if (fields.size() < FieldCount)
				return _Empty();

			try
			{
				return User(User::enMode::UpdateMode, fields[0], fields[1], fields[2],
					fields[3], fields[4], fields[5], std::stoi(fields[6]));
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
				User user = FromLine(line);
				if (user.IsEmpty())
				{
					++result.MalformedLines;
					continue;
				}
				result.Records.push_back(std::move(user));
			}
			return result;
		}

		// Atomically persists the list, skipping records marked for deletion.
		static bool SaveAll(const std::vector<User>& users)
		{
			std::vector<std::string> lines;
			lines.reserve(users.size());
			for (const User& user : users)
			{
				if (!user.MarkedForDeleted())
					lines.push_back(ToLine(user));
			}
			return AtomicFileStore::SaveLines(FilePath, lines);
		}

		static User FindByUserName(const std::string& userName)
		{
			for (const std::string& line : AtomicFileStore::LoadLines(FilePath))
			{
				User user = FromLine(line);
				if (!user.IsEmpty() && user.GetUserName() == userName)
					return user;
			}
			return _Empty();
		}

		static User FindByUserNameAndPassword(const std::string& userName, const std::string& password)
		{
			for (const std::string& line : AtomicFileStore::LoadLines(FilePath))
			{
				User user = FromLine(line);
				if (!user.IsEmpty() && user.GetUserName() == userName
					&& Bank::Security::PasswordHasher::Verify(password, user.GetPassword()))
					return user;
			}
			return _Empty();
		}

		static bool Exists(const std::string& userName)
		{
			return !FindByUserName(userName).IsEmpty();
		}

		static int CountFullAccess()
		{
			int count = 0;
			for (const User& user : LoadAll().Records)
			{
				if (user.GetPermissions() == User::enPermissions::eAll)
					++count;
			}
			return count;
		}

	private:
		static User _Empty()
		{
			return User(User::enMode::EmptyMode, "", "", "", "", "", "", 0);
		}
	};

} // namespace Bank::Data