#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Person.h"
#include "../../Libs/Cpp-Library-Collection/Lib/String.h"

class BankClient : public Person
{
private:
	enum enMode { EmptyMode = 0, UpdateMode = 1 };
	enMode _Mode;
	std::string _AccountNumber;
	std::string _PinCode;
	double _AccountBalance;

	static BankClient _ConvertLinetoClientObject(const std::string& Line, const std::string& Seperator = "#//#")
	{
		std::vector<std::string> vClientData;

		vClientData = Core::String::Split(Line, Seperator);

		return BankClient(enMode::UpdateMode, vClientData[0], vClientData[1], vClientData[2],
			vClientData[3], vClientData[4], vClientData[5], std::stod(vClientData[6]));
	}

	static std::string _ConverClientObjectToLine(const BankClient& Client, const std::string& Seperator = "#//#")
	{
		std::string ClientRecord = "";
		ClientRecord += Client.FirstName + Seperator;
		ClientRecord += Client.LastName + Seperator;
		ClientRecord += Client.Email + Seperator;
		ClientRecord += Client.Phone + Seperator;
		ClientRecord += Client.AccountNumber + Seperator;
		ClientRecord += Client.PinCode + Seperator;
		ClientRecord += std::to_string(Client.AccountBalance);

		return ClientRecord;
	}

	static std::vector<BankClient> _LoadClientsDataFromFile()
	{
		std::vector<BankClient> vClients;

		std::fstream MyFile;
		MyFile.open("Clients.txt", std::ios::in); // Read Mode

		if (MyFile.is_open())
		{
			std::string Line;

			while (getline(MyFile, Line))
			{
				BankClient Client = _ConvertLinetoClientObject(Line);
				vClients.push_back(Client);
			}

			MyFile.close();
		}

		return vClients;
	}

	static void _SaveCleintsDataToFile(const std::vector<BankClient>& vClients)
	{
		std::fstream MyFile;
		MyFile.open("Clients.txt", std::ios::out); // Overwrite mode

		if (MyFile.is_open())
		{
			std::string DataLine;

			for (const BankClient& C : vClients)
			{
				DataLine = _ConverClientObjectToLine(C);
				MyFile << DataLine << "\n";
			}

			MyFile.close();
		}
	}

	void _Update()
	{
		std::vector<BankClient> vClients = _LoadClientsDataFromFile();

		for (BankClient& C : vClients)
		{
			if (C.GetAccountNumber() == _AccountNumber)
			{
				C = *this;
				break;
			}
		}

		_SaveCleintsDataToFile(vClients);
	}

	void _AddDataLineToFile(const std::string& DataLine)
	{
		std::fstream MyFile;
		MyFile.open("Clients.txt", std::ios::out | std::ios::app);

		if (MyFile.is_open())
		{

			MyFile << DataLine << "\n";
			MyFile.close();
		}

	}

	static BankClient _GetEmptyClientObject()
	{
		return BankClient(enMode::EmptyMode, "", "", "", "", "", "", 0);
	}

public:
	BankClient(enMode Mode, const std::string& FirstName, const std::string& LastName,
		const std::string& Email, const std::string& Phone, const std::string& AccountNumber, const std::string& PinCode,
		double AccountBalance) :
		Person(FirstName, LastName, Email, Phone)
	{
		_Mode = Mode;
		_AccountNumber = AccountNumber;
		_PinCode = PinCode;
		_AccountBalance = AccountBalance;
	}

	bool IsEmpty() const
	{
		return (_Mode == enMode::EmptyMode);
	}

	// Property Read-Only: AccountNumber
	std::string GetAccountNumber() const
	{
		return _AccountNumber;
	}
	__declspec(property(get = GetAccountNumber)) std::string AccountNumber;

	// Property Set & Get: PinCode
	void SetPinCode(const std::string& PinCode)
	{
		_PinCode = PinCode;
	}
	std::string GetPinCode() const
	{
		return _PinCode;
	}
	__declspec(property(get = GetPinCode, put = SetPinCode)) std::string PinCode;

	// Property Set & Get: AccountBalance
	void SetAccountBalance(double AccountBalance)
	{
		_AccountBalance = AccountBalance;
	}
	double GetAccountBalance() const
	{
		return _AccountBalance;
	}
	__declspec(property(get = GetAccountBalance, put = SetAccountBalance)) double AccountBalance;

	void Print() const
	{
		std::cout << "\nClient Card:";
		std::cout << "\n___________________";
		std::cout << "\nFirstName   : " << GetFirstName();
		std::cout << "\nLastName    : " << GetLastName();
		std::cout << "\nFull Name   : " << FullName();
		std::cout << "\nEmail       : " << GetEmail();
		std::cout << "\nPhone       : " << GetPhone();
		std::cout << "\nAcc. Number : " << _AccountNumber;
		std::cout << "\nPassword    : " << _PinCode;
		std::cout << "\nBalance     : " << _AccountBalance;
		std::cout << "\n___________________\n";
	}

	static BankClient Find(const std::string& AccountNumber)
	{
		std::fstream MyFile;
		MyFile.open("Clients.txt", std::ios::in);

		if (MyFile.is_open())
		{
			std::string Line;
			while (getline(MyFile, Line))
			{
				BankClient Client = _ConvertLinetoClientObject(Line);
				if (Client.GetAccountNumber() == AccountNumber)
				{
					MyFile.close();
					return Client;
				}
			}
			MyFile.close();
		}
		return _GetEmptyClientObject();
	}

	static BankClient Find(const std::string& AccountNumber, const std::string& PinCode)
	{
		std::fstream MyFile;
		MyFile.open("Clients.txt", std::ios::in);

		if (MyFile.is_open())
		{
			std::string Line;
			while (getline(MyFile, Line))
			{
				BankClient Client = _ConvertLinetoClientObject(Line);
				if (Client.GetAccountNumber() == AccountNumber && Client.PinCode == PinCode)
				{
					MyFile.close();
					return Client;
				}
			}
			MyFile.close();
		}
		return _GetEmptyClientObject();
	}

	enum enSaveResults { svFailedEmptyObject = 0, svSucceeded = 1 };

	enSaveResults Save()
	{
		switch (_Mode)
		{
		case enMode::EmptyMode:
			return enSaveResults::svFailedEmptyObject;

		case enMode::UpdateMode:
			_Update();
			return enSaveResults::svSucceeded;
		}

		return enSaveResults::svFailedEmptyObject;
	}

	static bool IsClientExist(const std::string& AccountNumber)
	{
		BankClient Client1 = BankClient::Find(AccountNumber);
		return (!Client1.IsEmpty());
	}
};