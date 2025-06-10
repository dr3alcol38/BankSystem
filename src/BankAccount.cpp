#include "BankAccount.h"

#include <fstream>
#include <string>
#include <sstream>

#include "HelperFuncs.h"
#include "UserProfile.h"
#include "FileSystem.h"

BankAccount::BankAccount(UserProfile* const& userProfile, std::string const& filePath) : _userProfile(userProfile), _filePath(filePath)
{
	LoadBankAccountData();
	if (_userProfile->GetBankAccountByID(_accountID) != nullptr) { delete this; }
}

BankAccount::BankAccount(UserProfile* const& userProfile, uint64_t const& accountID, double const& initialBalance, std::string const& filePath) : _userProfile(userProfile), _accountID(accountID), _accountBalance(initialBalance), _filePath(filePath)
{
	HelperFuncs::LogLine("New account created by " + _userProfile->GetFullName() + ". AccountID: " + std::to_string(GetAccountID()) + " file path: " + _filePath);
	PrintBalance();
}

BankAccount::~BankAccount()
{
	SaveBankAccountData();

	HelperFuncs::LogLine("AccountID: " + std::to_string(GetAccountID()) + ", Account deleted");
}

void BankAccount::Deposit(double const& depositAmount) 
{
	if (depositAmount > 0) {
		_accountBalance += depositAmount;
		UpdateTransactionLog("$" + FormatAmount(depositAmount) + " deposited");
	}
	else
	{
		HelperFuncs::LogLine("Be creative");
	}

	PrintBalance();
}

void BankAccount::Withdraw(double const& withdrawAmount)
{
	if (GetBalance() >= withdrawAmount && withdrawAmount > 0) { _accountBalance -= withdrawAmount;	UpdateTransactionLog("$" + FormatAmount(withdrawAmount) + " withdrawn");}
	else if(withdrawAmount <= 0){ HelperFuncs::LogLine("Yes you can definatly withdraw such amount"); }
	else { HelperFuncs::LogLine("AccountID: " + std::to_string(GetAccountID()) + ", Insufficient balance!!!"); }

	PrintBalance();
}

double BankAccount::GetBalance() const
{
	return _accountBalance;
}

std::string BankAccount::GetBalanceToString() const
{
	return std::to_string(GetBalance());
}

void BankAccount::PrintBalance() const
{
	HelperFuncs::LogLine("AccountID: " + std::to_string(GetAccountID()) + ", Current balance $" + std::to_string(GetBalance()));
}

uint64_t BankAccount::GetAccountID() const
{
	return _accountID;
}

void BankAccount::UpdateTransactionLog(std::string const& log) 
{
	_transactionLog.push_back(log);
}

void BankAccount::PrintTransactionLog()const 
{
	HelperFuncs::LogLine("Transaction log:");

	for (std::string const& log : _transactionLog) 
	{
		HelperFuncs::LogLine(log);
	}
}

std::string BankAccount::FormatAmount(double amount)const
{
	std::ostringstream stream;
	stream.precision(2);
	stream << std::fixed << amount;
	return stream.str();
}

void BankAccount::SaveBankAccountData() 
{
	HelperFuncs::LogLine("Saving bank account...");

	std::string fileType = FileSystem::FileTypeToString(FileType::bankAccountData);
	uint64_t fileVersion = 0;

	BankFile file(_filePath);
	if (!file.FileExists()) { file.CreateFile(); }
	else
	{
		//read the previous file type and data
		fileType = file.ReadStringFromFile();
		fileType = fileType == "" ? FileSystem::FileTypeToString(FileType::bankAccountData) : fileType;
		fileVersion = file.ReadUint64_tFromFile();
	}

	if (fileType != FileSystem::FileTypeToString(FileType::bankAccountData)) 
	{
		HelperFuncs::Log("File Mismatch: ");
		HelperFuncs::LogLine(fileType + " is not accepted for " + FileSystem::FileTypeToString(FileType::bankAccountData));
		return; 
	}

	switch (fileVersion)
	{
		default:
			HelperFuncs::Log("No file version found");
			break;

		case 0:
			//clear the old data
			file.ClearFileData();

			//add the file type and version
			file.AddDataToFile(fileType);
			file.AddDataToFile(fileVersion);

			//save the file based on it's version
			SaveBankAccountData_Version_0(file);
			break;
	}

	file.SaveFile();

	HelperFuncs::LogLine("Saving complete");
}

void BankAccount::SaveBankAccountData_Version_0(BankFile & file) 
{
	file.AddDataToFile(_accountID);
	file.AddDataToFile(_accountBalance);

	uint64_t transactionLogSize = _transactionLog.size();
	file.AddDataToFile(transactionLogSize);

	for (std::string& log : _transactionLog)
	{
		file.AddDataToFile(log);
	}
}

void BankAccount::LoadBankAccountData() 
{
	HelperFuncs::LogLine("Loading bank account...");

	BankFile file(_filePath);
	if (!file.FileExists()) { HelperFuncs::LogLine("No data exists"); return; }

	std::string fileType = file.ReadStringFromFile(); //to skip the file title
	uint64_t fileVersion = file.ReadUint64_tFromFile(); //to skip the file version

	if (fileType != FileSystem::FileTypeToString(FileType::bankAccountData))
	{
		HelperFuncs::Log("File Mismatch: ");
		HelperFuncs::LogLine(fileType + " is not accepted for " + FileSystem::FileTypeToString(FileType::bankAccountData));
		return;
	}

	switch (fileVersion)
	{
	default:
		HelperFuncs::Log("No file version found");
		break;

	case 0:
		LoadBankAccountData_Version_0(file);
		break;
	}

	HelperFuncs::LogLine("Loading complete");
}

void BankAccount::LoadBankAccountData_Version_0(BankFile & file)
{
	_accountID = file.ReadUint64_tFromFile();
	_accountBalance = file.ReadDoubleFromFile();

	uint64_t transactionLogSize = file.ReadUint64_tFromFile();
	_transactionLog.resize(transactionLogSize);

	for (uint64_t i = 0; i < transactionLogSize; ++i)
	{
		_transactionLog[i] = file.ReadStringFromFile();
	}
}










//void BankAccount::SaveBankAccountData() const
//{
//	HelperFuncs::LogLine("Saving bank account...");
//
//	uint64_t transactionLogSize = _transactionLog.size();
//
//	std::ofstream out(_filePath, std::ios::binary);
//
//	out.write(reinterpret_cast<char const*>(&_accountID), sizeof(_accountID));
//
//	out.write(reinterpret_cast<char const*>(&_accountBalance), sizeof(_accountBalance));
//
//	out.write(reinterpret_cast<char*>(&transactionLogSize), sizeof(transactionLogSize));
//
//	for (std::string const& log : _transactionLog)
//	{
//		uint64_t logSize = log.length();
//
//		out.write(reinterpret_cast<char*>(&logSize), sizeof(logSize));
//
//		out.write(log.c_str(), logSize);
//	}
//
//	out.close();
//
//	HelperFuncs::LogLine("Saving complete");
//}
//
//void BankAccount::LoadBankAccountData()
//{
//	HelperFuncs::LogLine("Loading bank account...");
//
//	uint64_t accountID;
//	double accountBalance;
//	uint64_t transactionLogSize;
//
//	std::ifstream in(_filePath, std::ios::binary);
//	//Checks if the file/data exists
//	if (!in) { HelperFuncs::LogLine("No data exists"); return; }
//
//	in.read(reinterpret_cast<char*>(&accountID), sizeof(accountID));
//
//	in.read(reinterpret_cast<char*>(&accountBalance), sizeof(accountBalance));
//
//	in.read(reinterpret_cast<char*>(&transactionLogSize), sizeof(transactionLogSize));
//
//	_transactionLog.resize(transactionLogSize);
//
//	for (uint64_t i = 0; i < transactionLogSize; ++i)
//	{
//		uint64_t logSize;
//
//		in.read(reinterpret_cast<char*>(&logSize), sizeof(logSize));
//
//		std::string log(logSize, '\0');
//
//		in.read(&log[0], logSize);
//
//		_transactionLog[i] = log;
//	}
//
//	in.close();
//
//	_accountID = accountID;
//	_accountBalance = accountBalance;
//
//	HelperFuncs::LogLine("Loading complete");
//}