
#pragma once
#include <iostream>
#include "Screen.h"
#include "../Core/BankClient.h"
#include "../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"


class UpdateClientScreen :protected Screen
{
private:

	static void _ReadClientInfo(BankClient& Client)
	{
		Client.FirstName = Core::InputValidate::ReadString("\nEnter FirstName: ");

		Client.LastName = Core::InputValidate::ReadString("\nEnter LastName: ");

		Client.Email = Core::InputValidate::ReadString("\nEnter Email: ");

		Client.Phone = Core::InputValidate::ReadString("\nEnter Phone: ");

		Client.PinCode = Core::InputValidate::ReadString("\nEnter PinCode: ");

		Client.AccountBalance = Core::InputValidate::ReadNumber<double>();
	}

	static void _PrintClient(const BankClient& Client)
	{
		std::cout << "\nClient Card:";
		std::cout << "\n___________________";
		std::cout << "\nFirstName   : " << Client.GetFirstName();
		std::cout << "\nLastName    : " << Client.GetLastName();
		std::cout << "\nFull Name   : " << Client.FullName();
		std::cout << "\nEmail       : " << Client.GetEmail();
		std::cout << "\nPhone       : " << Client.GetPhone();
		std::cout << "\nAcc. Number : " << Client.GetAccountNumber();
		std::cout << "\nPassword    : " << Client.GetPinCode();
		std::cout << "\nBalance     : " << Client.GetAccountBalance();
		std::cout << "\n___________________\n";

	}

public:

	static void ShowUpdateClientScreen()
	{

		_DrawScreenHeader("\tUpdate Client Screen");

		std::string AccountNumber = "";

		AccountNumber = Core::InputValidate::ReadString("\nPlease Enter client Account Number: ");

		while (!BankClient::IsClientExist(AccountNumber))
		{
			std::cout << "\nAccount number is not found, choose another one: ";
			AccountNumber = Core::InputValidate::ReadString();
		}

		BankClient Client1 = BankClient::Find(AccountNumber);

		_PrintClient(Client1);

		if (Core::InputValidate::ReadYesNoOption("\nAre you sure you want to update this client y/n? "))
		{

			std::cout << "\n\nUpdate Client Info:";
			std::cout << "\n____________________\n";


			_ReadClientInfo(Client1);

			BankClient::enSaveResults SaveResult;

			SaveResult = Client1.Save();

			switch (SaveResult)
			{
			case  BankClient::enSaveResults::svSucceeded:
			{
				std::cout << "\nAccount Updated Successfully :-)\n";

				_PrintClient(Client1);
				break;
			}
			case BankClient::enSaveResults::svFailedEmptyObject:
			{
				std::cout << "\nError account was not saved because it's Empty";
				break;

			}

			}

		}
	}

};