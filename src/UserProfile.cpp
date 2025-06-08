#include "UserProfile.h"

#include <fstream>

#include "HelperFuncs.h"
#include "BankAccount.h"
#include "FileSystem.h"

UserProfile::UserProfile(std::string const& firstName, std::string const& middleName, std::string const& lastName, uint64_t const& userID) : _firstName(firstName), _middleName(middleName), _lastName(lastName), _userID(userID)
{
	_filePath = "C:/Users/Cole/AppData/LocalLow/BankSystem/Saved Data/User Profile/" + _firstName + _lastName + std::to_string(_userID) + ".userprofile";

	LoadBankProfileData();

	HelperFuncs::LogLine("User " + GetFullName() + " -> Active");
}

UserProfile::~UserProfile() 
{
	SaveBankProfileData();

	for (BankAccount* account : _bankAccounts) 
	{
		delete account;
	}
	_bankAccounts.clear();
}

std::string UserProfile::GetFirstName()const
{
	return _firstName;
}

std::string UserProfile::GetLastName()const
{
	return _lastName;
}

std::string UserProfile::GetFullName()const
{
	return _lastName + " " + _middleName + " " + _firstName;
}

void UserProfile::AddBankAccount(uint64_t const& accountID, double const& initialBalance)
{
	if (GetBankAccountByID(accountID) != nullptr) { return; }

	std::string filePath = "C:/Users/Cole/AppData/LocalLow/BankSystem/Saved Data/Bank Accounts/" + GetLastName() + GetFirstName() + std::to_string(accountID) + ".bankaccountdata";

	_bankAccounts.push_back(new BankAccount(this, accountID, initialBalance, filePath));
	_accountsDataFileNames.push_back(filePath);
}

BankAccount* UserProfile::GetBankAccountByID(uint64_t const& index) const 
{
	for (int i = 0; i < GetAccountsCount(); i++)
	{
		if (index == _bankAccounts[i]->GetAccountID()) 
		{
			return _bankAccounts[i];
		}
	}

	return nullptr;
}

uint64_t UserProfile::GetAccountsCount()const
{
	return size(_bankAccounts);
}

std::string UserProfile::GetAllAccountsIDs()const
{
	if (_bankAccounts.empty()) return "No accounts available.";

	std::string returnValue;

	for (int i = 0; i < GetAccountsCount() - 1; ++i)
	{
		returnValue += "Acc ID: " + std::to_string((*_bankAccounts[i]).GetAccountID()) + " at $" + (*_bankAccounts[i]).FormatAmount((*_bankAccounts[i]).GetBalance()) + ", ";
	}

	returnValue += "Acc ID: " + std::to_string((*_bankAccounts[GetAccountsCount() - 1]).GetAccountID()) + " at $" + (*_bankAccounts[GetAccountsCount() - 1]).FormatAmount((*_bankAccounts[GetAccountsCount() - 1]).GetBalance()) + ".";

	return returnValue;
}

void UserProfile::SaveBankProfileData() 
{
	HelperFuncs::LogLine("Saving user profile...");

	BankFile file(_filePath);
	if (!file.FileExists()) { file.CreateFile(FileType::userProfileData); }
	else
	{
		file.ClearFileData(FileType::userProfileData);
	}

	std::string fileType = file.ReadStringFromFile();
	uint64_t fileVersion = file.ReadUint64_tFromFile();

	if (fileType != BankFileSystem::FileTypeToString(FileType::userProfileData))
	{
		HelperFuncs::Log("File Mismatch: ");
		HelperFuncs::LogLine(fileType + " is not accepted for " + BankFileSystem::FileTypeToString(FileType::userProfileData));
		return;
	}

	switch (fileVersion)
	{
	default:
		HelperFuncs::Log("No file version found");
		break;

	case 0:
		SaveBankProfileData_Version_0(file);
		break;
	}

	HelperFuncs::LogLine("Saving complete");
}

void UserProfile::SaveBankProfileData_Version_0(BankFile & file)
{
	file.AddDataToFile(_firstName);
	file.AddDataToFile(_middleName);
	file.AddDataToFile(_lastName);

	//save the string cvector
	uint64_t fileNamesSize = _accountsDataFileNames.size();
	file.AddDataToFile(fileNamesSize);

	for (std::string& str : _accountsDataFileNames)
	{
		file.AddDataToFile(str);
	}
}

void UserProfile::LoadBankProfileData() 
{
	HelperFuncs::LogLine("Loading user profile...");

	BankFile file(_filePath);
	if (!file.FileExists()) { HelperFuncs::LogLine("No data exists"); return; }

	std::string fileType = file.ReadStringFromFile(); //to skip the file title
	uint64_t fileVersion = file.ReadUint64_tFromFile(); //to skip the file version

	if (fileType != BankFileSystem::FileTypeToString(FileType::userProfileData))
	{
		HelperFuncs::Log("File Mismatch: ");
		HelperFuncs::LogLine(fileType + " is not accepted for " + BankFileSystem::FileTypeToString(FileType::userProfileData));
		return;
	}

	switch (fileVersion)
	{
	default:
		HelperFuncs::Log("No file version found");
		break;

	case 0:
		LoadBankProfileData_Version_0(file);
		break;
	}

	HelperFuncs::LogLine("Loading complete");
}

void UserProfile::LoadBankProfileData_Version_0(BankFile & file) 
{
	_firstName = file.ReadStringFromFile();
	_middleName = file.ReadStringFromFile();
	_lastName = file.ReadStringFromFile();

	uint64_t fileNamesSize = file.ReadUint64_tFromFile();

	_accountsDataFileNames.resize(fileNamesSize);

	for (uint64_t i = 0; i < fileNamesSize; ++i)
	{
		_accountsDataFileNames[i] = file.ReadStringFromFile();
	}

	//add the accounts with their pre-loaded data(setting thier user profile data and giving them the path to get thier data from).
	for (std::string const& filePath : _accountsDataFileNames)
	{
		_bankAccounts.emplace_back(new BankAccount(this, filePath));
	}
}











//void UserProfile::SaveBankProfile() const
//{
//	HelperFuncs::LogLine("Saving user profile...");
//
//	uint64_t fileNamesSize = _accountsDataFileNames.size();
//	uint64_t firstNameLenght = _firstName.length();
//	uint64_t middleNameLenght = _middleName.length();
//	uint64_t lastNameLenght = _lastName.length();
//
//	std::ofstream out(_filePath, std::ios::binary);
//
//	out.write(reinterpret_cast<char*>(&fileNamesSize), sizeof(fileNamesSize));
//
//	for (std::string const& str : _accountsDataFileNames)
//	{
//		uint64_t strLenghts = str.length();
//
//		out.write(reinterpret_cast<char*>(&strLenghts), sizeof(strLenghts));
//		//sizeof isn't used in this next line cause str.c_str() is a char const*
//		out.write(str.c_str(), strLenghts);
//	}
//
//	out.write(reinterpret_cast<char*>(&firstNameLenght), sizeof(firstNameLenght));
//
//	out.write(_firstName.c_str(), firstNameLenght);
//
//	out.write(reinterpret_cast<char*>(&middleNameLenght), sizeof(middleNameLenght));
//
//	out.write(_middleName.c_str(), middleNameLenght);
//
//	out.write(reinterpret_cast<char*>(&lastNameLenght), sizeof(lastNameLenght));
//
//	out.write(_lastName.c_str(), lastNameLenght);
//
//	out.close();
//
//	HelperFuncs::LogLine("Saving complete");
//}
//
//void UserProfile::LoadBankProfile()
//{
//	HelperFuncs::LogLine("Loading user profile...");
//
//	uint64_t fileNamesSize;
//	uint64_t firstNameLenght;
//	uint64_t middleNameLenght;
//	uint64_t lastNameLenght;
//
//	std::ifstream in(_filePath, std::ios::binary);
//	//Checks if the file/data exists
//	if (!in) { HelperFuncs::LogLine("No data exists"); return; }
//
//	/*std::streampos currentReadPos = in.tellg(); //get starting cursor pos
//	in.seekg(0, std::ios::end); //go to the end of the file
//	std::streampos endReadPos = in.tellg(); //get end cursor pos
//	in.seekg(currentReadPos);*/
//
//	in.read(reinterpret_cast<char*>(&fileNamesSize), sizeof(fileNamesSize));
//
//	_accountsDataFileNames.resize(fileNamesSize);
//
//	for (uint64_t i = 0; i < fileNamesSize; ++i)
//	{
//		uint64_t strLenghts;
//
//		in.read(reinterpret_cast<char*>(&strLenghts), sizeof(strLenghts));
//		//pre-sizing the string to it's lenght and making all it's values null ('\0'). i could have used (' ') or ('k') as well
//		std::string str(strLenghts, '\0');
//		//i read to &str[0] and not &str cause it can't take dynamic data(i.e &str(string)) but it auto matically fills the rest of the string just by using the first
//		in.read(&str[0], strLenghts);
//
//		_accountsDataFileNames[i] = str;
//	}
//
//	//firstname load
//	in.read(reinterpret_cast<char*>(&firstNameLenght), sizeof(firstNameLenght));
//
//	std::string firstNameTemp(firstNameLenght, '\0');
//
//	in.read(&firstNameTemp[0], firstNameLenght);
//
//	_firstName = firstNameTemp;
//
//	//middlename load
//	in.read(reinterpret_cast<char*>(&middleNameLenght), sizeof(middleNameLenght));
//
//	std::string middleNameTemp(middleNameLenght, '\0');
//
//	in.read(&middleNameTemp[0], middleNameLenght);
//
//	_middleName = middleNameTemp;
//
//	//lastname load
//	in.read(reinterpret_cast<char*>(&lastNameLenght), sizeof(lastNameLenght));
//
//	std::string lastNameTemp(lastNameLenght, '\0');
//
//	in.read(&lastNameTemp[0], lastNameLenght);
//
//	_lastName = lastNameTemp;
//
//	in.close();
//
//	//add the accounts with their pre-loaded data(setting thier user profile data and giving them the path to get thier data from).
//	for (std::string const& filePath : _accountsDataFileNames)
//	{
//		_bankAccounts.push_back(new BankAccount(this, filePath));
//	}
//
//	HelperFuncs::LogLine("Loading complete");
//}