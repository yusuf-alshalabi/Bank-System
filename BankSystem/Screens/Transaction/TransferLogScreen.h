#pragma once

#include <iostream>
#include "../Screen.h"
#include <iomanip>
#include <fstream>
#include "../../Core/BankClient.h"


class TransferLogScreen :protected Screen
{

private:

    static void PrintTransferLogRecordLine(BankClient::stTrnsferLogRecord TransferLogRecord)
    {

        cout << setw(8) << left << "" << "| " << setw(23) << left << TransferLogRecord.DateTime;
        cout << "| " << setw(8) << left << TransferLogRecord.SourceAccountNumber;
        cout << "| " << setw(8) << left << TransferLogRecord.DestinationAccountNumber;
        cout << "| " << setw(8) << left << TransferLogRecord.Amount;
        cout << "| " << setw(10) << left << TransferLogRecord.srcBalanceAfter;
        cout << "| " << setw(10) << left << TransferLogRecord.destBalanceAfter;
        cout << "| " << setw(8) << left << TransferLogRecord.UserName;


    }

public:

    static void ShowTransferLogScreen()
    {


        vector <BankClient::stTrnsferLogRecord> vTransferLogRecord = BankClient::GetTransfersLogList();

        string Title = "\tTransfer Log List Screen";
        string SubTitle = "\t    (" + to_string(vTransferLogRecord.size()) + ") Record(s).";

        _DrawScreenHeader(Title, SubTitle);

        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_________________________________________\n" << endl;

        std::cout << std::setw(8) << std::left << "" << "| " << std::left << std::setw(23) << "Date/Time";
        std::cout << "| " << std::left << std::setw(8) << "s.Acct";
        std::cout << "| " << std::left << std::setw(8) << "d.Acct";
        std::cout << "| " << std::left << std::setw(8) << "Amount";
        std::cout << "| " << std::left << std::setw(10) << "s.Balance";
        std::cout << "| " << std::left << std::setw(10) << "d.Balance";
        std::cout << "| " << std::left << std::setw(8) << "User";

        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_________________________________________\n" << endl;

        if (vTransferLogRecord.size() == 0)
            std::cout << "\t\t\t\tNo Transfers Available In the System!";
        else

            for (BankClient::stTrnsferLogRecord Record : vTransferLogRecord)
            {

                PrintTransferLogRecordLine(Record);
                std::cout << endl;
            }

        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_________________________________________\n" << endl;

    }

};

