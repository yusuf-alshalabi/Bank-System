#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "PasswordHasher.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/String.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/Util.h"

namespace Bank::Data {

	// One-time legacy data migration for v2.0.0: Users.txt records written by the
	// previous OOP version (or their v1.4 seed) stored passwords using the weak
	// reversible Core::Util::EncryptText(Password, 2) Caesar cipher.
	//
	// The migrator decrypts the legacy field, re-hashes it with Argon2id and
	// rewrites the file atomically (Users.tmp -> backup Users.bak -> live file).
	// It is idempotent: records whose password field already carries an Argon2
	// hash are left untouched, so it is safe to run on every startup.
	class DataMigrator
	{
	public:
		static bool MigrateLegacyUsersPasswordStorage(const std::string& usersFile = "Users.txt")
		{
			if (!std::filesystem::exists(usersFile))
				return true; // nothing to migrate

			std::vector<std::string> vLines;
			{
				std::ifstream stream(usersFile);
				if (!stream.is_open())
					return false;

				std::string line;
				while (std::getline(stream, line))
					vLines.push_back(line);
			}

			bool changed = false;
			std::vector<std::string> vMigrated(vLines.size());

			for (std::size_t i = 0; i < vLines.size(); ++i)
			{
				std::vector<std::string> vParts = Core::String::Split(vLines[i], "#//#");
				if (vParts.size() < 7)
				{
					vMigrated[i] = vLines[i]; // malformed record: keep untouched
					continue;
				}

				std::string storedPassword = vParts[5];
				if (Bank::Security::PasswordHasher::IsHashed(storedPassword))
				{
					vMigrated[i] = vLines[i];
					continue;
				}

				// Legacy Caesar cipher -> plaintext -> Argon2id hash.
				std::string rawPassword = Core::Util::DecryptText(storedPassword, 2);
				vParts[5] = Bank::Security::PasswordHasher::Hash(rawPassword);

				std::string rebuilt;
				for (std::size_t p = 0; p < vParts.size(); ++p)
				{
					if (p > 0)
						rebuilt += "#//#";
					rebuilt += vParts[p];
				}
				vMigrated[i] = rebuilt;
				changed = true;
			}

			if (!changed)
				return true;

			const std::filesystem::path tmpFile = std::filesystem::path(usersFile).string() + ".tmp";
			const std::filesystem::path bakFile = std::filesystem::path(usersFile).string() + ".bak";

			{
				std::ofstream out(tmpFile, std::ios::out | std::ios::trunc);
				if (!out.is_open())
					return false;
				for (const std::string& line : vMigrated)
					out << line << '\n';
			}

			// Atomic-ish swap: tmp becomes the live file, the previous live file
			// becomes the backup.
			std::error_code ec;
			std::filesystem::remove(bakFile, ec);
			std::error_code ecMove;
			std::filesystem::rename(usersFile, bakFile, ecMove);
			std::error_code ecCommit;
			std::filesystem::rename(tmpFile, usersFile, ecCommit);

			return !ecCommit;
		}
	};

} // namespace Bank::Data