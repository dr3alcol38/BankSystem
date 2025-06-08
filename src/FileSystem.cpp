#include "FileSystem.h"

#include <fstream>

#include "HelperFuncs.h"

std::string BankFileSystem::fileTypeStings[4] = {"userProfile", "bankAccountData", 	"requestFile", "responseFile"};

BankFileSystem::BankFileSystem()
{

}

BankFileSystem::~BankFileSystem()
{

}

std::string BankFileSystem::FileTypeToString(FileType const& fileType) 
{
	return fileTypeStings[(int)fileType];
}

bool BankFileSystem::FileExists(std::string const& filePath)
{
	std::fstream file(filePath, std::ios::in | std::ios::out | std::ios::binary);

	if (file.is_open())
	{
		file.close();
		return true;
	}

	return false;
}

std::fstream BankFileSystem::CreateNewFile(std::string const& filePath)
{
	std::fstream file(filePath, std::ios::out | std::ios::binary);

	if (!file.is_open())
	{
		std::ofstream createNew(filePath, std::ios::binary);

		if (createNew.fail()) 
		{
			HelperFuncs::LogLine("Failed to create file at " + filePath);
			return std::fstream();
		}

		HelperFuncs::LogLine("New file created: " + filePath);
		file.open(filePath, std::ios::out | std::ios::binary);
	}
	
	//HelperFuncs::LogLine("File at : " + filePath + " already exists");

	file.close();

	return file;
}

void BankFileSystem::CreateBankFile(std::string const& filePath, FileType const& fileType)
{
	return CreateBankFile(filePath, fileType, 0);
}

void BankFileSystem::CreateBankFile(std::string const& filePath, FileType const& fileType, uint64_t const& fileVersion)
{
	std::fstream file = CreateNewFile(filePath);

	if (file.fail())
	{
		HelperFuncs::LogLine("Failed to create " + fileTypeStings[(int)fileType] + " file");
	}

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::binary); }

	//files header (what kind of file is this)
	std::string fileHeader = fileTypeStings[(int)fileType];
	uint64_t fileHeaderLenght = fileHeader.length();

	//files version
	uint64_t fileVersionNumber = fileVersion;

	//write file data
	file.write(reinterpret_cast<char*>(&fileHeaderLenght), sizeof(fileHeaderLenght));
	file.write(fileHeader.c_str(), fileHeaderLenght);

	file.write(reinterpret_cast<char*>(&fileVersionNumber), sizeof(fileVersionNumber));

	file.close();
}

void BankFileSystem::ClearFileData(std::string const& filePath) 
{
	if (!FileExists(filePath)) { return; }

	std::fstream file(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

	if (file.fail())
	{
		
	}

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::trunc | std::ios::binary); }

	file.close();
}

uint64_t BankFileSystem::GetFileSize(std::string const& filePath) 
{
	if (!FileExists(filePath)) { return 0; }

	//std::ios::ate takes the cursor to the end of the file and it needs readig acces hence the std::ios::in
	std::ifstream file(filePath, std::ios::in | std::ios::ate | std::ios::binary);
	if (!file.is_open()) { file.open(filePath, std::ios::ate | std::ios::binary); }

	std::streamsize fileSize = file.tellg();

	file.close();

	return static_cast<uint64_t>(fileSize);
}

void BankFileSystem::AddDataToFile(std::string const& filePath, char& data) 
{
	std::fstream file(filePath, std::ios::app | std::ios::binary);

	if (file.fail())
	{
		HelperFuncs::LogLine("File doesn't exist");
		return;
	}

	//HelperFuncs::LogLine("File exists");

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::binary); }

	int8_t dataInInt8_t = static_cast<int8_t>(data);
	
	//write file data
	file.write(reinterpret_cast<char*>(&dataInInt8_t), sizeof(dataInInt8_t));

	file.close();
}

void BankFileSystem::AddDataToFile(std::string const& filePath, std::string & data)
{
	if (data.length() < 1) { HelperFuncs::LogLine("Data to small"); return; }

	std::fstream file(filePath, std::ios::app | std::ios::binary);

	if (file.fail())
	{
		HelperFuncs::LogLine("File doesn't exist");
		return;
	}

	//HelperFuncs::LogLine("File exists");

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::binary); }

	//get the lenght of the string
	uint64_t stringLenght = data.length();

	//write file data
	file.write(reinterpret_cast<char*>(&stringLenght), sizeof(stringLenght));
	file.write(data.c_str(), stringLenght);

	file.close();
}

void BankFileSystem::AddDataToFile(std::string const& filePath, bool& data)
{
	std::fstream file(filePath, std::ios::app | std::ios::binary);

	if (file.fail())
	{
		HelperFuncs::LogLine("File doesn't exist");
		return;
	}

	//HelperFuncs::LogLine("File exists");

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::binary); }

	uint8_t dataInChar = 1;

	if (data == false) { dataInChar = 0; }

	//write file data
	file.write(reinterpret_cast<char*>(&dataInChar), sizeof(dataInChar));

	file.close();
}

void BankFileSystem::AddDataToFile(std::string const& filePath, uint64_t & data)
{
	std::fstream file(filePath, std::ios::app | std::ios::binary);

	if (file.fail())
	{
		HelperFuncs::LogLine("File doesn't exist");
		return;
	}
	
	//HelperFuncs::LogLine("File exists");

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::binary); }

	//write file data
	file.write(reinterpret_cast<char*>(&data), sizeof(data));

	file.close();
}

void BankFileSystem::AddDataToFile(std::string const& filePath, double & data)
{
	std::fstream file(filePath, std::ios::app | std::ios::binary);

	if (file.fail())
	{
		HelperFuncs::LogLine("File doesn't exist");
		return;
	}

	//HelperFuncs::LogLine("File exists");

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::binary); }

	//write file data
	file.write(reinterpret_cast<char*>(&data), sizeof(data));

	file.close();
}

char BankFileSystem::ReadCharFromFile(std::string const& filePath, uint64_t const& readOffset) 
{
	std::fstream file(filePath, std::ios::in | std::ios::binary);

	if (file.fail())
	{
		HelperFuncs::LogLine("File doesn't exist");
		return 0;
	}

	//get file size
	uint64_t fileSize = GetFileSize(filePath);
	//HelperFuncs::LogLine("File exists");

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::binary); }

	//move cursor to file offset
	file.seekg(readOffset);

	char placeholder;
	int8_t dataFromInt8_t;

	//check if the file still has data
	if (fileSize < readOffset) { HelperFuncs::LogLine("Read offset beyond file limits"); return 0; }
	else
	{
		//check if theres enought bytes to be read
		if ((fileSize - readOffset) < sizeof(int8_t)) { HelperFuncs::LogLine("Not enough bytes to read from for char value"); return 0; }
	}

	//write file data
	file.read(reinterpret_cast<char*>(&dataFromInt8_t), sizeof(dataFromInt8_t));

	file.close();

	placeholder = static_cast<char>(dataFromInt8_t);

	return placeholder;
}

std::string	BankFileSystem::ReadStringFromFile(std::string const& filePath, uint64_t const& readOffset)
{
	std::fstream file(filePath, std::ios::in | std::ios::binary);

	if (file.fail())
	{
		HelperFuncs::LogLine("File doesn't exist");
		return "\0";
	}

	//get file size
	uint64_t fileSize = GetFileSize(filePath);
	//HelperFuncs::LogLine("File exists");

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::binary); }

	//move cursor to file offset
	file.seekg(readOffset);

	uint64_t stringLenght;

	//check if the file still has data
	if (fileSize < readOffset) { HelperFuncs::LogLine("Read offset beyond file limits"); return "\0"; }
	else
	{
		//check if theres enought bytes to be read
		if ((fileSize - readOffset) < sizeof(uint64_t)) { HelperFuncs::LogLine("Not enough bytes to read from for string lenght"); return "\0"; }
	}

	//write file data
	file.read(reinterpret_cast<char*>(&stringLenght), sizeof(stringLenght));

	std::string placeholder(stringLenght, '\0');

	//check if the file still has data  //adding the size of uint64_t cause the readoffset is in the (old) locationt and thinks we're still reading the string lenght
	if (fileSize < (readOffset + sizeof(uint64_t))) { HelperFuncs::LogLine("Read offset beyond file limits"); return "\0"; }
	else
	{
		//check if theres enought bytes to be read   //adding the size of uint64_t cause the readoffset is in the (old) locationt and thinks we're still reading the string lenght
		if ((fileSize - (readOffset + sizeof(uint64_t))) < stringLenght) { HelperFuncs::LogLine("Not enough bytes to read from for string value"); return "\0"; }
	}

	file.read(&placeholder[0], stringLenght);

	file.close();

	return placeholder;
}

bool BankFileSystem::ReadBoolFromFile(std::string const& filePath, uint64_t const& readOffset) 
{
	std::fstream file(filePath, std::ios::in | std::ios::binary);

	if (file.fail())
	{
		HelperFuncs::LogLine("File doesn't exist");
		return 0;
	}

	//get file size
	uint64_t fileSize = GetFileSize(filePath);
	//HelperFuncs::LogLine("File exists");

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::binary); }

	//move cursor to file offset
	file.seekg(readOffset);

	bool placeholder = false;
	uint8_t dataFromUint8_t;

	//check if the file still has data
	if (fileSize < readOffset) { HelperFuncs::LogLine("Read offset beyond file limits"); return 0; }
	else
	{
		//check if theres enought bytes to be read
		if ((fileSize - readOffset) < sizeof(uint8_t)) { HelperFuncs::LogLine("Not enough bytes to read from for bool value"); return 0; }
	}

	//write file data
	file.read(reinterpret_cast<char*>(&dataFromUint8_t), sizeof(dataFromUint8_t));

	file.close();

	if (dataFromUint8_t != 0) { placeholder = true; }

	return placeholder;
}

uint64_t BankFileSystem::ReadUint64_tFromFile(std::string const& filePath, uint64_t const& readOffset)
{
	std::fstream file(filePath, std::ios::in | std::ios::binary);

	if (file.fail())
	{
		HelperFuncs::LogLine("File doesn't exist");
		return 0;
	}

	//get file size
	uint64_t fileSize = GetFileSize(filePath);
	//HelperFuncs::LogLine("File exists");

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::binary); }

	//move cursor to file offset
	file.seekg(readOffset);

	uint64_t placeholder;

	//check if the file still has data
	if (fileSize < readOffset) { HelperFuncs::LogLine("Read offset beyond file limits"); return 0; }
	else
	{
		//check if theres enought bytes to be read
		if ((fileSize - readOffset) < sizeof(uint64_t)) { HelperFuncs::LogLine("Not enough bytes to read from for uint64_t value"); return 0; }
	}

	//write file data
	file.read(reinterpret_cast<char*>(&placeholder), sizeof(placeholder));

	file.close();

	return placeholder;
}

double BankFileSystem::ReadDoubleFromFile(std::string const& filePath, uint64_t const& readOffset)
{
	std::fstream file(filePath, std::ios::in | std::ios::binary);

	if (file.fail())
	{
		HelperFuncs::LogLine("File doesn't exist");
		return 0;
	}

	//get file size
	uint64_t fileSize = GetFileSize(filePath);
	//HelperFuncs::LogLine("File exists");

	if (!file.is_open()) { file.open(filePath, std::ios::out | std::ios::binary); }

	//move cursor to file offset
	file.seekg(readOffset);

	double placeholder;

	//check if the file still has data
	if (fileSize < readOffset) { HelperFuncs::LogLine("Read offset beyond file limits"); return 0; }
	else
	{
		//check if theres enought bytes to be read
		if ((fileSize - readOffset) < sizeof(uint64_t)) { HelperFuncs::LogLine("Not enough bytes to read from for double value"); return 0; }
	}

	//write file data
	file.read(reinterpret_cast<char*>(&placeholder), sizeof(placeholder));

	file.close();

	return placeholder;
}

#pragma region //leave this alone 
namespace {
	extern "C" __declspec(dllimport) unsigned long __stdcall GetModuleFileNameA(void* hModule, char* lpFilename, unsigned long nSize);
}

std::string BankFileSystem::GetExeFolder()
{
	//i do not understand this code 
	//don't touch what works but you don't undrstand
	//it finds the path to the exe file
	//this only work when the exe file has been built
	//it returns the directory the exe is in (i.e c:/downloads/games/)

#ifndef MAX_PATH
#define MAX_PATH 260
#endif // !MAX_PATH

#ifdef _DEBUG

	return "C:/Users/Cole/AppData/LocalLow/BankSystem/";

#else

	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);

	std::string fullPath(path);
	uint64_t pos = fullPath.find_last_of("\\ /");
	return fullPath.substr(0, pos) + '/';

#endif // DEBUG
}
#pragma endregion