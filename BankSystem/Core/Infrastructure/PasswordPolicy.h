#pragma once
#include <cctype>
#include <cstddef>
#include <string>

#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"

namespace Bank::Security {

	// Strong password policy - migration of the legacy InputManager::readPassword
	// rules: minimum 8 characters containing uppercase, lowercase and a digit.
	// Runs when creating or updating user accounts.
	class PasswordPolicy
	{
	public:
		static constexpr std::size_t MinLength = 8;

		struct ValidationResult
		{
			bool Valid = true;
			std::string Error;
		};

		static ValidationResult Validate(const std::string& password)
		{
			if (password.length() < MinLength)
				return { false, "Password must be at least " + std::to_string(MinLength) + " characters!" };

			bool hasUpper = false, hasLower = false, hasDigit = false;
			for (unsigned char c : password)
			{
				if (std::isupper(c))       hasUpper = true;
				else if (std::islower(c))  hasLower = true;
				else if (std::isdigit(c))  hasDigit = true;
			}

			if (!hasUpper || !hasLower || !hasDigit)
				return { false, "Password must contain uppercase, lowercase, and a digit!" };

			return {};
		}

		// Re-prompts until a policy-compliant password is entered.
		static std::string ReadPassword(const std::string& prompt = "Please Enter Password: ")
		{
			while (true)
			{
				std::string password = Core::InputValidate::ReadString(prompt);
				ValidationResult result = Validate(password);
				if (result.Valid)
					return password;
				std::cout << "\n" << result.Error << "\n";
			}
		}
	};

} // namespace Bank::Security