#pragma once
#include "BankAccount.h"

class UserProfile;

class SavingsAccount : public BankAccount
{
	private:
		float _interestRate;

	public:
		SavingsAccount(UserProfile* const& userProfile, int const& accountID, float const& initialBalance, float const& interestRate, std::string const& filePath);
		~SavingsAccount();

		void ApplyInterest();
};