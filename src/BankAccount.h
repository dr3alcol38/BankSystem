#pragma once
#include <string>
#include <vector>

class UserProfile;
struct BankFile;

class BankAccount 
{
	private:
		UserProfile* _userProfile;
		uint64_t _accountID = 0;
		double _accountBalance = 0;
		std::vector<std::string> _transactionLog;

		std::string _filePath;

	public:
		BankAccount(UserProfile* const& userProfile, std::string const& filePath);
		BankAccount(UserProfile* const& userProfile, uint64_t const& accountID, double const& initialBalance, std::string const& filePath);
		~BankAccount();

		void Deposit(double const& depositAmount);
		void Withdraw(double const& withdrawAmount);
		double GetBalance() const;
		std::string GetBalanceToString() const;
		void PrintBalance() const;
		uint64_t GetAccountID() const;
		void UpdateTransactionLog(std::string const& log);
		void PrintTransactionLog()const;
		std::string FormatAmount(double amount)const;

		void SaveBankAccountData();
		void SaveBankAccountData_Version_0(BankFile & file);

		void LoadBankAccountData();
		void LoadBankAccountData_Version_0(BankFile & file);
};