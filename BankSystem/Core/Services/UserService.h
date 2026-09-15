#pragma once
#include <string>

#include "../User.h"
#include "../Repositories/UserRepository.h"

namespace Bank::Users {

	// User-management policy helpers - migration of the legacy gates from
	// LastVersion/UserManager.h (countFullAccessUsers, verifyUserPassword).
	class UserService
	{
	public:
		static int CountFullAccessUsers()
		{
			return Bank::Data::UserRepository::CountFullAccess();
		}

		// True when the given username is a full-access admin AND the only one
		// left with full access, i.e. it cannot be deleted or downgraded.
		static bool IsLastFullAccessAdmin(const std::string& userName)
		{
			User target = Bank::Data::UserRepository::FindByUserName(userName);
			return !target.IsEmpty()
				&& target.GetPermissions() == User::enPermissions::eAll
				&& CountFullAccessUsers() <= 1;
		}

		// Credential gate for sensitive operations: the operator must re-enter
		// the target user's password, mirrored from the legacy
		// deleteUserWithCredentials / updateUserWithCredentials flows.
		static bool VerifyUserPassword(const User& user, const std::string& rawPassword)
		{
			return !Bank::Data::UserRepository::FindByUserNameAndPassword(
				user.GetUserName(), rawPassword).IsEmpty();
		}
	};

} // namespace Bank::Users