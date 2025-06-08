#include "SavingsAccount.h"

SavingsAccount::SavingsAccount(UserProfile* const& userProfile, int const& accountID, float const& initialBalance, float const& interestRate, std::string const& filePath) : BankAccount(userProfile, accountID, initialBalance, filePath), _interestRate(interestRate)
{

}

SavingsAccount::~SavingsAccount() 
{

}

void SavingsAccount::ApplyInterest() 
{

}