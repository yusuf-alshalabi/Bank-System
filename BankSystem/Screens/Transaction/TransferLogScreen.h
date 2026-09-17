#pragma once

#include <iostream>
#include "../Screen.h"
#include "../../Core/BankClient.h"
#include "../../Core/User.h"
#include <iomanip>
#include <vector>
#include <string>

class TransferLogScreen :protected Screen
{

private:

    static void PrintTransferLogRecordLine(const BankClient::stTrnsferLogRecord& TransferLogRecord)
    {
        std::cout << CYAN << "| " << RESET << std::setw(23) << std::left << TransferLogRecord.DateTime;
        std::cout << CYAN << "| " << RESET << std::setw(8) << std::left << TransferLogRecord.SourceAccountNumber;
        std::cout << CYAN << "| " << RESET << std::setw(8) << std::left << TransferLogRecord.DestinationAccountNumber;
        std::cout << CYAN << "| " << RESET << GREEN << std::setw(8) << std::left << _CurrencyText(TransferLogRecord.Amount) << RESET;
        std::cout << CYAN << "| " << RESET << GREEN << std::setw(10) << std::left << _CurrencyText(TransferLogRecord.srcBalanceAfter) << RESET;
        std::cout << CYAN << "| " << RESET << GREEN << std::setw(10) << std::left << _CurrencyText(TransferLogRecord.destBalanceAfter) << RESET;
        std::cout << CYAN << "| " << RESET << std::setw(10) << std::left << TransferLogRecord.UserName;
        std::cout << CYAN << "|" << RESET;
    }

public:

    static void ShowTransferLogScreen()
    {
        if (!CheckAccessRights(User::enPermissions::pTranactions))
        {
            return;
        }

        std::vector<BankClient::stTrnsferLogRecord> vTransferLogRecord = BankClient::GetTransfersLogList();

        _DrawScreenHeader("Transfer Log List Screen", "(" + std::to_string(vTransferLogRecord.size()) + ") Record(s).");

        _ShowTableBorder(90);

        std::cout << CYAN << "| " << RESET << std::left << std::setw(23) << "Date/Time";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(8) << "s.Acct";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(8) << "d.Acct";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(8) << "Amount";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(10) << "s.Balance";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(10) << "d.Balance";
        std::cout << CYAN << "| " << RESET << std::left << std::setw(10) << "User";
        std::cout << CYAN << "|\n" << RESET;

        _ShowTableBorder(90);

        if (vTransferLogRecord.size() == 0)
        {
            std::cout << "\n  No Transfers Available In the System!\n";
        }
        else
        {
            for (const BankClient::stTrnsferLogRecord& Record : vTransferLogRecord)
            {
                PrintTransferLogRecordLine(Record);
                std::cout << "\n";
            }
        }

        _ShowTableBorder(90);
    }

};