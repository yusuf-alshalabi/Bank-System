#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Person.h"
#include "../../Libs/Cpp-Library-Collection/Lib/String.h"
#include "Infrastructure/AtomicFileStore.h"
#include <vector>

class BankClient : public Person
{
public:
	enum enMode { EmptyMode = 0, UpdateMode = 1, AddNewMode = 2 };

	struct stTrnsferLogRecord
	{
		string DateTime;
		string SourceAccountNumber;
		string DestinationAccountNumber;
		double Amount;
		double srcBalanceAfter;
		double destBalanceAfter;
		string UserName;

	};

private:
	inline static const std::string _TransferLogFileName = "Database/TransferLog.txt";

	static BankClient _GetEmptyClientObject()
	{
		return BankClient(enMode::EmptyMode, "", "", "", "", "", "", 0);
	}

	static stTrnsferLogRecord _ConvertTransferLogLineToRecord(string Line, string Seperator = "#//#")
	{
		stTrnsferLogRecord TrnsferLogRecord;

		vector <string> vTrnsferLogRecordLine = Bank::Persistence::AtomicFileStore::SplitPreserve(Line, Seperator);

		// skip malformed records instead of indexing out of bounds
		if (vTrnsferLogRecordLine.size() < 7)
			return TrnsferLogRecord;

		TrnsferLogRecord.DateTime = vTrnsferLogRecordLine[0];
		TrnsferLogRecord.SourceAccountNumber = vTrnsferLogRecordLine[1];
		TrnsferLogRecord.DestinationAccountNumber = vTrnsferLogRecordLine[2];
		TrnsferLogRecord.Amount = stod(vTrnsferLogRecordLine[3]);
		TrnsferLogRecord.srcBalanceAfter = stod(vTrnsferLogRecordLine[4]);
		TrnsferLogRecord.destBalanceAfter = stod(vTrnsferLogRecordLine[5]);
		TrnsferLogRecord.UserName = vTrnsferLogRecordLine[6];

		return TrnsferLogRecord;

	}


	string _PrepareTransferLogRecord(float Amount, BankClient DestinationClient,
		string UserName, string Seperator = "#//#")
	{
		string TransferLogRecord = "";
		TransferLogRecord += Core::Date::GetSystemDateTime() + Seperator;
		TransferLogRecord += GetAccountNumber() + Seperator;
		TransferLogRecord += DestinationClient.GetAccountNumber() + Seperator;
		TransferLogRecord += std::to_string(Amount) + Seperator;
		TransferLogRecord += std::to_string(GetAccountBalance()) + Seperator;
		TransferLogRecord += std::to_string(DestinationClient.GetAccountBalance()) + Seperator;
		TransferLogRecord += UserName;
		return TransferLogRecord;
	}

void _RegisterTransferLog(float Amount, BankClient DestinationClient, string UserName)
	{
		string stDataLine = _PrepareTransferLogRecord(Amount, DestinationClient, UserName);
		Bank::Persistence::AtomicFileStore::AppendLine(_TransferLogFileName, stDataLine);
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

	// Property Read-Only: MarkedForDeleted
	bool MarkedForDeleted() const
	{
		return _MarkedForDelete;
	}
	__declspec(property(get = MarkedForDeleted)) bool MarkedForDelete;


	static BankClient Find(const std::string& AccountNumber);
	static BankClient Find(const std::string& AccountNumber, const std::string& PinCode);

	enum enSaveResults { svFailedEmptyObject = 0, svSucceeded = 1, svFaildAccountNumberExists = 2 };

	enSaveResults Save();

	static bool IsClientExist(const std::string& AccountNumber);

	bool Delete();

	static BankClient GetAddNewClientObject(const std::string& AccountNumber)
	{
		return BankClient(enMode::AddNewMode, "", "", "", "", AccountNumber, "", 0);
	}

	static std::vector<BankClient> GetClientsList();


	static double GetTotalBalances()
	{
		std::vector<BankClient> vClients = BankClient::GetClientsList();

		double TotalBalances = 0;

		for (const BankClient& Client : vClients)
		{

			TotalBalances += Client.GetAccountBalance();
		}

		return TotalBalances;

	}

	bool Deposit(double Amount);
	bool Withdraw(double Amount);
	bool Transfer(double Amount, BankClient& DestinationClient, std::string CurrentUser);

	static  vector <stTrnsferLogRecord> GetTransfersLogList()
	{
		vector <stTrnsferLogRecord> vTransferLogRecord;

		for (const string& Line : Bank::Persistence::AtomicFileStore::LoadLines(_TransferLogFileName))
		{
			stTrnsferLogRecord TransferRecord = _ConvertTransferLogLineToRecord(Line);

			// skip malformed records
			if (TransferRecord.DateTime.empty())
				continue;

			vTransferLogRecord.push_back(TransferRecord);
		}

		return vTransferLogRecord;

	}

private:
	enMode _Mode;
	std::string _AccountNumber;
	std::string _PinCode;
	double _AccountBalance;
	bool _MarkedForDelete = false;
};

// Persistence and financial logic are delegated to the repository/service
// headers below. They are included after the class so the include graph has
// no cycles: BankClientRepository.h/TransactionService.h only consume the
// fully-defined BankClient type.
#include "Repositories/BankClientRepository.h"
#include "Services/TransactionService.h"
#include "Infrastructure/Logger.h"

inline BankClient BankClient::Find(const std::string& AccountNumber)
{
	return Bank::Data::BankClientRepository::FindByAccountNumber(AccountNumber);
}

inline BankClient BankClient::Find(const std::string& AccountNumber, const std::string& PinCode)
{
	return Bank::Data::BankClientRepository::FindByAccountNumberAndPin(AccountNumber, PinCode);
}

inline BankClient::enSaveResults BankClient::Save()
{
	switch (_Mode)
	{
	case enMode::EmptyMode:
		return enSaveResults::svFailedEmptyObject;

	case enMode::UpdateMode:
	{
		std::vector<BankClient> clients = Bank::Data::BankClientRepository::LoadAll().Records;
		bool updated = false;
		for (BankClient& client : clients)
		{
			if (client.GetAccountNumber() == _AccountNumber)
			{
				client = *this;
				updated = true;
				break;
			}
		}
		if (!updated)
			return enSaveResults::svFailedEmptyObject;

		return Bank::Data::BankClientRepository::SaveAll(clients)
			? enSaveResults::svSucceeded : enSaveResults::svFailedEmptyObject;
	}

	case enMode::AddNewMode:
	{
		if (Bank::Data::BankClientRepository::Exists(_AccountNumber))
			return enSaveResults::svFaildAccountNumberExists;

		std::vector<BankClient> clients = Bank::Data::BankClientRepository::LoadAll().Records;
		clients.push_back(*this);

		if (!Bank::Data::BankClientRepository::SaveAll(clients))
			return enSaveResults::svFailedEmptyObject;

		_Mode = enMode::UpdateMode;
		return enSaveResults::svSucceeded;
	}
	}

	return enSaveResults::svFailedEmptyObject;
}

inline bool BankClient::IsClientExist(const std::string& AccountNumber)
{
	return Bank::Data::BankClientRepository::Exists(AccountNumber);
}

inline bool BankClient::Delete()
{
	if (!Bank::Data::BankClientRepository::Exists(_AccountNumber))
		return false;

	std::vector<BankClient> clients = Bank::Data::BankClientRepository::LoadAll().Records;
	bool deleted = false;
	for (BankClient& client : clients)
	{
		if (client.GetAccountNumber() == _AccountNumber)
		{
			client._MarkedForDelete = true;
			deleted = true;
			break;
		}
	}

	if (!deleted || !Bank::Data::BankClientRepository::SaveAll(clients))
		return false;

	*this = _GetEmptyClientObject();
	return true;
}

inline std::vector<BankClient> BankClient::GetClientsList()
{
	return Bank::Data::BankClientRepository::LoadAll().Records;
}

inline bool BankClient::Deposit(double Amount)
{
	if (Amount <= 0)
		return false;

	_AccountBalance += Amount;

	std::vector<BankClient> clients = Bank::Data::BankClientRepository::LoadAll().Records;
	bool updated = false;
	for (BankClient& client : clients)
	{
		if (client.GetAccountNumber() == _AccountNumber)
		{
			client = *this;
			updated = true;
			break;
		}
	}
	if (!updated)
		return false;

	// one atomic write commits the balance change
	if (!Bank::Data::BankClientRepository::SaveAll(clients))
		return false;

	Bank::Transactions::TransactionEntry entry = Bank::Transactions::TransactionService::Record(
		static_cast<int>(Bank::Transactions::TransactionType::Deposit),
		GetAccountNumber(), GetAccountNumber(), Amount, 0.0, "Deposit operation");

	Bank::Diagnostics::Logger::Instance().LogTransaction(
		entry.TransactionID, "Deposit", Amount, 0.0, GetAccountNumber(), GetAccountNumber());

	return true;
}

inline bool BankClient::Withdraw(double Amount)
{
	if (Amount <= 0)
		return false;
	if (Amount > _AccountBalance)
		return false;

	_AccountBalance -= Amount;

	std::vector<BankClient> clients = Bank::Data::BankClientRepository::LoadAll().Records;
	bool updated = false;
	for (BankClient& client : clients)
	{
		if (client.GetAccountNumber() == _AccountNumber)
		{
			client = *this;
			updated = true;
			break;
		}
	}
	if (!updated)
		return false;

	// one atomic write commits the balance change
	if (!Bank::Data::BankClientRepository::SaveAll(clients))
		return false;

	Bank::Transactions::TransactionEntry entry = Bank::Transactions::TransactionService::Record(
		static_cast<int>(Bank::Transactions::TransactionType::Withdrawal),
		GetAccountNumber(), GetAccountNumber(), Amount, 0.0, "Withdrawal operation");

	Bank::Diagnostics::Logger::Instance().LogTransaction(
		entry.TransactionID, "Withdraw", Amount, 0.0, GetAccountNumber(), GetAccountNumber());

	return true;
}

inline bool BankClient::Transfer(double Amount, BankClient& DestinationClient, std::string CurrentUser)
{
	if (Amount <= 0)
		return false;
	if (GetAccountNumber() == DestinationClient.GetAccountNumber())
		return false;

	double TransferFee = Amount * Bank::Transactions::TransactionService::TransferFeeRate;
	if (_AccountBalance < Amount + TransferFee)
		return false;

	// Both balances are mutated in memory and committed with ONE atomic
	// repository save, so a transfer never leaves a partially written pair.
	_AccountBalance -= (Amount + TransferFee);
	DestinationClient._AccountBalance += Amount;

	std::vector<BankClient> clients = Bank::Data::BankClientRepository::LoadAll().Records;
	bool sourceUpdated = false;
	bool destinationUpdated = false;
	for (BankClient& client : clients)
	{
		if (client.GetAccountNumber() == _AccountNumber && !sourceUpdated)
		{
			client = *this;
			sourceUpdated = true;
		}
		else if (client.GetAccountNumber() == DestinationClient.GetAccountNumber() && !destinationUpdated)
		{
			client = DestinationClient;
			destinationUpdated = true;
		}
	}
	if (!sourceUpdated || !destinationUpdated)
		return false;

	if (!Bank::Data::BankClientRepository::SaveAll(clients))
		return false;

	Bank::Transactions::TransactionEntry entry = Bank::Transactions::TransactionService::Record(
		static_cast<int>(Bank::Transactions::TransactionType::Transfer),
		GetAccountNumber(), DestinationClient.GetAccountNumber(), Amount, TransferFee,
		"Transfer to " + DestinationClient.FullName());

	Bank::Diagnostics::Logger::Instance().LogTransaction(
		entry.TransactionID, "Transfer", Amount, TransferFee, GetAccountNumber(), DestinationClient.GetAccountNumber());

	_RegisterTransferLog(static_cast<float>(Amount), DestinationClient, CurrentUser);

	return true;
}