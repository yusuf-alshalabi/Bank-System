#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/Person.h"
#include "../../Core/BankClient.h"
#include "../../Core/User.h"
#include "DepositScreen.h"
#include "WithdrawScreen.h"
#include "TotalBalancesScreen.h"
#include "TransferScreen.h"
#include "TransferLogScreen.h"
#include "TransactionHistoryScreen.h"

class TransactionsScreen :protected Screen
{

private:

    enum enTransactionsMenueOptions {
        eDeposit = 1, eWithdraw = 2,
        eShowTotalBalance = 3, eTransfer = 4, eTransferLog = 5,
        eHistory = 6
    };

    static void _ShowDepositScreen()
    {
        DepositScreen::ShowDepositScreen();
    }

    static void _ShowWithdrawScreen()
    {
        WithdrawScreen::ShowWithdrawScreen();
    }

    static void _ShowTotalBalancesScreen()
    {
        TotalBalancesScreen::ShowTotalBalances();
    }

    static void _ShowTransferScreen()
    {
        TransferScreen::ShowTransferScreen();
    }

    static void _ShowTransferLogScreen()
    {
        TransferLogScreen::ShowTransferLogScreen();
    }

    static void _ShowHistoryScreen()
    {
        TransactionHistoryScreen::ShowTransactionHistoryScreen();
    }

    static void _GoBackToTransactionsMenue()
    {
        _PressEnterToContinue();
    }

    static void _PerformTransactionsMenueOption(enTransactionsMenueOptions TransactionsMenueOption)
    {
        switch (TransactionsMenueOption)
        {
        case enTransactionsMenueOptions::eDeposit:
            _ClearScreen();
            _ShowDepositScreen();
            _GoBackToTransactionsMenue();
            break;

        case enTransactionsMenueOptions::eWithdraw:
            _ClearScreen();
            _ShowWithdrawScreen();
            _GoBackToTransactionsMenue();
            break;

        case enTransactionsMenueOptions::eShowTotalBalance:
            _ClearScreen();
            _ShowTotalBalancesScreen();
            _GoBackToTransactionsMenue();
            break;

        case enTransactionsMenueOptions::eTransfer:
            _ClearScreen();
            _ShowTransferScreen();
            _GoBackToTransactionsMenue();
            break;

        case enTransactionsMenueOptions::eTransferLog:
            _ClearScreen();
            _ShowTransferLogScreen();
            _GoBackToTransactionsMenue();
            break;

        case enTransactionsMenueOptions::eHistory:
            _ClearScreen();
            _ShowHistoryScreen();
            _GoBackToTransactionsMenue();
            break;
        }
    }

public:

    static void ShowTransactionsMenue()
    {
        if (!CheckAccessRights(User::enPermissions::pTranactions))
        {
            return;
        }

        while (true)
        {
            _ClearScreen();
            _DrawScreenHeader("Transactions Menu Screen");

            _ShowOptions({ "Deposit", "Withdraw", "Total Balances", "Transfer", "Transfer Log", "Transactions History" });
            _ShowBackOption(false);
            _ShowLine(60, '-');

            short Choice = _ReadMenuOption(1, 6);

            if (Choice == 0)
            {
                return;
            }

            _PerformTransactionsMenueOption(static_cast<enTransactionsMenueOptions>(Choice));
        }
    }

};