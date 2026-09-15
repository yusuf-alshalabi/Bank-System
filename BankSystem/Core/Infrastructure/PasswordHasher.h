#pragma once
#include <string>

#include "CryptoEngine.h"

namespace Bank::Security {

	// Argon2id password hashing (crypto_pwhash) - migration of the legacy
	// AuthManager::hashPassword / UserManager::verifyPassword logic using the
	// libsodium INTERACTIVE cost parameters (OPSLIMIT=2, MEMLIMIT=64 MiB) that
	// the legacy hash strings in Users.txt.bak were produced with.
	class PasswordHasher
	{
	public:
		static std::string Hash(const std::string& rawPassword)
		{
			SodiumRaiiGuard guard;

			char hash[crypto_pwhash_STRBYTES];
			if (crypto_pwhash_str(hash, rawPassword.c_str(), rawPassword.size(),
				crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0)
				throw std::runtime_error("PasswordHasher: password hashing failed");

			return std::string(hash);
		}

		static bool Verify(const std::string& rawPassword, const std::string& storedHash)
		{
			if (storedHash.empty())
				return false;

			SodiumRaiiGuard guard;
			return crypto_pwhash_str_verify(storedHash.c_str(), rawPassword.c_str(), rawPassword.size()) == 0;
		}

		// True if storedPassword already looks like an Argon2 encoded hash
		// ("$argon2id$..." or "$argon2i$..."), i.e. it must not be hashed again.
		static bool IsHashed(const std::string& storedHash)
		{
			return storedHash.rfind("$argon2", 0) == 0;
		}

		// Stores plaintext values as hashes but keeps already-hashed values intact.
		// This lets the persistence layer use one code path for both file records
		// and freshly captured passwords.
		static std::string HashIfNeeded(const std::string& value)
		{
			return IsHashed(value) ? value : Hash(value);
		}
	};

} // namespace Bank::Security