#pragma once
#include <iostream>
#include "../Screen.h"
#include "../../Core/Person.h"
#include "../../Core/BankClient.h"
#include "../../Core/User.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/InputValidate.h"
#include "DepositScreen.h"
#include "WithdrawScreen.h"
#include "TotalBalancesScreen.h"
#include "TransferScreen.h"
#include "TransferLogScreen.h"

class TransactionsScreen :protected Screen
{


private:
    enum enTransactionsMenueOptions {
        eDeposit = 1, eWithdraw = 2,
        eShowTotalBalance = 3, eTransfer = 4,eTransferLog=5, eShowMainMenue = 6
    };

    static short ReadTransactionsMenueOption()
    {
        std::cout << std::setw(37) << std::left << "" << "Choose what do you want to do? [1 to 6]? ";
        short Choice = Core::InputValidate::ReadNumberBetween<short>(1, 6, "Enter Number between 1 to 6? ");
        return Choice;
    }


    static void _ShowDepositScreen()
    {
        //cout << "\n Deposit Screen will be here.\n";
        DepositScreen::ShowDepositScreen();

    }

    static void _ShowWithdrawScreen()
    {
       // cout << "\n Withdraw Screen will be here.\n";
		WithdrawScreen::ShowWithdrawScreen();
    }

    static void _ShowTotalBalancesScreen()
    {
       // cout << "\n Balances Screen will be here.\n";
        		TotalBalancesScreen::ShowTotalBalances();
    }

	static void _ShowTransferScreen()
	{
		// cout << "\n Transfer Screen will be here.\n";
		TransferScreen::ShowTransferScreen();
	}

	static void _ShowTransferLogScreen()
	{
		// cout << "\n Transfer Log Screen will be here.\n";
		TransferLogScreen::ShowTransferLogScreen();
	}

    static void _GoBackToTransactionsMenue()
    {
        std::cout << "\n\nPress any key to go back to Transactions Menue...";
        system("pause>0");
        ShowTransactionsMenue();

    }

    static void _PerformTransactionsMenueOption(enTransactionsMenueOptions TransactionsMenueOption)
    {
        switch (TransactionsMenueOption)
        {
        case enTransactionsMenueOptions::eDeposit:
        {
            std::system("cls");
            _ShowDepositScreen();
            _GoBackToTransactionsMenue();
            break;
        }

        case enTransactionsMenueOptions::eWithdraw:
        {
            std::system("cls");
            _ShowWithdrawScreen();
            _GoBackToTransactionsMenue();
            break;
        }

        case enTransactionsMenueOptions::eShowTotalBalance:
        {
            std::system("cls");
            _ShowTotalBalancesScreen();
            _GoBackToTransactionsMenue();
            break;
        }

		case enTransactionsMenueOptions::eTransfer:
		{
			std::system("cls");
			_ShowTransferScreen();
			_GoBackToTransactionsMenue();
			break;
		}

		case enTransactionsMenueOptions::eTransferLog:
		{
			std::system("cls");
			_ShowTransferLogScreen();
			_GoBackToTransactionsMenue();
			break;
		}

        case enTransactionsMenueOptions::eShowMainMenue:
        {
            //do nothing here the main screen will handle it :-) ;
        }
        }


    }



public:


    static void ShowTransactionsMenue()
    {
        if (!CheckAccessRights(User::enPermissions::pTranactions))
        {
            return;// this will exit the function and it will not continue
        }

        std::system("cls");
        _DrawScreenHeader("\t  Transactions Screen");

        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t\t  Transactions Menue\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t[1] Deposit.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[2] Withdraw.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[3] Total Balances.\n";
		std::cout << std::setw(37) << std::left << "" << "\t[4] Transfer.\n";
		std::cout << std::setw(37) << std::left << "" << "\t[5] Transfer Log.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[6] Main Menue.\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";

        _PerformTransactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
    }

};

