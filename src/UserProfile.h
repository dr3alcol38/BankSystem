#pragma once
#include <string>
#include <vector>

class BankAccount;
struct BankFile;

class UserProfile
{
	private:
		std::string _firstName;
		std::string _middleName;
		std::string _lastName;
		uint64_t _userID;
		std::vector<BankAccount*> _bankAccounts;

		std::string _filePath;
		std::vector<std::string> _accountsDataFileNames;

	public:
		UserProfile(std::string const& firstName, std::string const& middleName, std::string const& lastName, uint64_t const& userID);
		~UserProfile();

		std::string GetFirstName()const;
		std::string GetLastName()const;
		std::string GetFullName()const;
		void AddBankAccount(uint64_t const& accountID, double const& initialBalance);
		BankAccount* GetBankAccountByID(uint64_t const& index) const;
		uint64_t GetAccountsCount()const;
		std::string GetAllAccountsIDs()const;
		
		void SaveBankProfileData();
		void SaveBankProfileData_Version_0(BankFile & file);
		
		void LoadBankProfileData();
		void LoadBankProfileData_Version_0(BankFile & file);
};