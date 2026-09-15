#pragma once
#include <iostream>
#include <string>

#include "../User.h"
#include "../Repositories/UserRepository.h"
#include "../Infrastructure/PasswordHasher.h"
#include "../Infrastructure/PasswordPolicy.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

namespace Bank::Auth {

	// Authentication bootstrap - migration of LastVersion/AuthManager.h.
	// Set up during application startup, before the login loop runs.
	class AuthService
	{
	public:
		// On a first run (no persisted users) interactively creates the initial
		// full-access admin account so the system is never left locked out.
		static void EnsureFirstRunAdmin()
		{
			if (!Bank::Data::UserRepository::LoadAll().Records.empty())
				return;

			std::cout << "\nNo users found in the system - first run detected.\n";
			std::cout << "Please create the first Admin account:\n";

			std::string userName = Core::InputValidate::ReadString("\nEnter Admin Username: ");
			std::string rawPassword = Bank::Security::PasswordPolicy::ReadPassword("\nEnter Admin Password: ");

			User admin = User::GetAddNewUserObject(userName);
			admin.Password = Bank::Security::PasswordHasher::Hash(rawPassword);
			admin.Permissions = User::enPermissions::eAll;

			if (admin.Save() == User::enSaveResults::svSucceeded)
			{
				std::cout << "\nAdmin user '" << userName << "' created successfully with Full Access.\n";
				std::cout << "Please login with your new credentials.\n\n";
			}
			else
			{
				std::cout << "\nFailed to create the admin user. Check the Users file.\n\n";
			}
		}
	};

} // namespace Bank::Auth