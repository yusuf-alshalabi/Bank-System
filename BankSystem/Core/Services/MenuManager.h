#pragma once
#include <string>
#include <vector>

#include "../User.h"

namespace Bank::UI {

	// Keys mirror Screens/MainScreen.h enMainMenueOptions so the screen can
	// route a dynamic selection straight into its existing switch statement.
	enum class MainMenuKey : int
	{
		ListClients = 1,
		AddNewClient = 2,
		DeleteClient = 3,
		UpdateClient = 4,
		FindClient = 5,
		Transactions = 6,
		ManageUsers = 7,
		LoginRegister = 8,
		CurrencyExchange = 9,
		Logout = 10
	};

	struct MainMenuEntry
	{
		int Key;
		std::string Label;
	};

	// Builds the main menu from the acting user's permission bits - migration
	// of LastVersion/MenuManager::buildMainMenuOptions. Only authorized entries
	// are shown, each with a dynamic display index.
	class MenuManager
	{
	public:
		static std::vector<MainMenuEntry> BuildMainMenu(const User& user)
		{
			std::vector<MainMenuEntry> entries;
			auto can = [&user](User::enPermissions permission)
			{
				return user.CheckAccessPermission(permission);
			};

			if (can(User::enPermissions::pListClients))
				entries.push_back({ static_cast<int>(MainMenuKey::ListClients), "Show Client List" });
			if (can(User::enPermissions::pAddNewClient))
				entries.push_back({ static_cast<int>(MainMenuKey::AddNewClient), "Add New Client" });
			if (can(User::enPermissions::pDeleteClient))
				entries.push_back({ static_cast<int>(MainMenuKey::DeleteClient), "Delete Client" });
			if (can(User::enPermissions::pUpdateClients))
				entries.push_back({ static_cast<int>(MainMenuKey::UpdateClient), "Update Client Info" });
			if (can(User::enPermissions::pFindClient))
				entries.push_back({ static_cast<int>(MainMenuKey::FindClient), "Find Client" });
			if (can(User::enPermissions::pTranactions))
				entries.push_back({ static_cast<int>(MainMenuKey::Transactions), "Transactions" });
			if (can(User::enPermissions::pManageUsers))
				entries.push_back({ static_cast<int>(MainMenuKey::ManageUsers), "Manage Users" });
			if (can(User::enPermissions::pLoginRegister))
				entries.push_back({ static_cast<int>(MainMenuKey::LoginRegister), "Login Register" });

			// Currency Exchange has no dedicated permission bit yet, so it stays
			// visible to every signed-in user (matching the v2.0.0 behaviour).
			entries.push_back({ static_cast<int>(MainMenuKey::CurrencyExchange), "Currency Exchange" });

			entries.push_back({ static_cast<int>(MainMenuKey::Logout), "Logout" });
			return entries;
		}
	};

} // namespace Bank::UI