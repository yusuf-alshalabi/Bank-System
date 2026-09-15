#pragma once
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "../User.h"
#include "CryptoEngine.h"

namespace Bank::Security {

	// Encrypted, per-OS-user session manager - migration of LastVersion/Session.h.
	//
	// On login the active User is kept in memory and an encrypted token
	// (the username) is persisted to:
	//   %LOCALAPPDATA%\BankSystem\session_<osUser>.bsess
	// protected with a libsodium key file stored next to it. On startup the token
	// is decrypted and the user is re-hydrated from Users.txt, which gives the
	// "resume previous session" behaviour of the legacy app without ever storing
	// any password material on disk.
	class SessionManager
	{
	public:
		static SessionManager& Instance()
		{
			static SessionManager instance;
			return instance;
		}

		// Returns true and activates the user if a valid encrypted session exists.
		// Corrupted or tampered session files are wiped and ignored.
		bool TryResume()
		{
			std::filesystem::path file = _SessionFile();
			if (!std::filesystem::exists(file))
				return false;

			try
			{
				std::ifstream stream(file, std::ios::binary);
				if (!stream.is_open())
					return false;

				std::string content((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
				std::string userName = CryptoEngine::DecryptBlob(content, _LoadKey());

				User user = User::Find(userName);
				if (user.IsEmpty())
					return false; // stale session: user no longer exists

				_ActiveUser = user;
				return true;
			}
			catch (const std::exception&)
			{
				End(); // discard the unusable session file
				return false;
			}
		}

		// Activates the user and persists an encrypted session token.
		bool Start(const User& user)
		{
			if (user.IsEmpty())
				return false;

			_ActiveUser = user;
			try
			{
				std::filesystem::path file = _SessionFile();
				std::error_code ec;
				std::filesystem::create_directories(file.parent_path(), ec);

				std::string payload = CryptoEngine::EncryptBlob(user.GetUserName(), _LoadKey());
				std::ofstream stream(file, std::ios::binary | std::ios::trunc);
				stream.write(payload.data(), static_cast<std::streamsize>(payload.size()));
				if (!stream)
					throw std::runtime_error("SessionManager: failed to persist session file");
				return true;
			}
			catch (const std::exception&)
			{
				_ActiveUser.reset();
				return false;
			}
		}

		// Secure logout: 3-pass file wipe then removal (port of Session.h:185).
		void End()
		{
			std::filesystem::path file = _SessionFile();
			_WipeFile(file);
			std::error_code ec;
			std::filesystem::remove(file, ec);
			_ActiveUser.reset();
		}

		bool IsActive() const
		{
			return _ActiveUser.has_value();
		}

		const User& Current() const
		{
			if (!_ActiveUser)
				throw std::runtime_error("SessionManager: no active session");
			return *_ActiveUser;
		}

		// Safe username lookup used by UI headers and the logger; "" when no session.
		std::string CurrentUserName() const
		{
			return _ActiveUser ? _ActiveUser->GetUserName() : std::string();
		}

		bool CanAccess(User::enPermissions permission) const
		{
			return _ActiveUser && _ActiveUser->CheckAccessPermission(permission);
		}

	private:
		SessionManager() = default;
		SessionManager(const SessionManager&) = delete;
		SessionManager& operator=(const SessionManager&) = delete;

		// getenv is flagged by SDL checks; guarded with a NULL check and a
		// scoped warning disable so the warning stays localized to this helper.
		static std::string _EnvironmentVariable(const char* variableName)
		{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
			const char* value = std::getenv(variableName);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
			return value ? std::string(value) : std::string();
		}

		// Resolved from the USERNAME environment variable to keep the header free of
		// windows.h, whose GetUserName macro would collide with User's
		// __declspec(property(get=GetUserName)) declaration (legacy Session.h worked
		// around the same clash by shelling out to whoami).
		static std::string _OsUserName()
		{
			std::string userName = _EnvironmentVariable("USERNAME");
			return userName.empty() ? "UnknownUser" : userName;
		}

		static std::filesystem::path _SessionDir()
		{
			std::string localAppData = _EnvironmentVariable("LOCALAPPDATA");
			std::filesystem::path base = !localAppData.empty()
				? std::filesystem::path(localAppData)
				: std::filesystem::temp_directory_path();
			return base / "BankSystem";
		}

		static std::filesystem::path _SessionFile()
		{
			return _SessionDir() / ("session_" + _OsUserName() + ".bsess");
		}

		static std::filesystem::path _KeyFile()
		{
			return _SessionDir() / "encryption_key.bsess";
		}

		static const SecretKey& _LoadKey()
		{
			static std::unique_ptr<SecretKey> key =
				std::make_unique<SecretKey>(CryptoEngine::LoadOrCreateKey(_KeyFile()));
			return *key;
		}

		static void _WipeFile(const std::filesystem::path& file)
		{
			std::error_code ec;
			std::uintmax_t size = std::filesystem::file_size(file, ec);
			if (ec)
				return;

			std::vector<char> buffer(static_cast<std::size_t>(size));
			std::fstream stream(file, std::ios::binary | std::ios::in | std::ios::out);
			if (!stream.is_open())
				return;

			for (int pass = 0; pass < 3; ++pass)
			{
				if (pass == 0)
					std::fill(buffer.begin(), buffer.end(), '\0');
				else if (pass == 1)
					std::fill(buffer.begin(), buffer.end(), '\xFF');
				else
					randombytes_buf(buffer.data(), buffer.size());

				stream.seekp(0);
				stream.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
				stream.flush();
			}
			stream.close();
		}

		std::optional<User> _ActiveUser;
	};

} // namespace Bank::Security