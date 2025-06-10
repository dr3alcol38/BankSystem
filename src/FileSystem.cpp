#include "FileSystem.h"

#include <fstream>

#include "HelperFuncs.h"

std::string FileSystem::fileTypeStrings[4] = {"userProfile", "bankAccountData", 	"requestFile", "responseFile"};

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem()
{

}

std::string FileSystem::FileTypeToString(FileType const& fileType) 
{
	return fileTypeStrings[(int)fileType];
}

bool FileSystem::FileExists(std::string const& filePath)
{
	std::ifstream check(filePath, std::ios::binary);
	bool exists = check.good();

	if (!exists) {
		HelperFuncs::LogLine("File doesn't exist. Path: " + filePath);
	}

	return exists;
}

void FileSystem::CreateNewFile(std::string const& filePath)
{
	std::fstream file(filePath, std::ios::out | std::ios::binary);

	if (!file.is_open())
	{
		//HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		std::ofstream createNew(filePath, std::ios::binary);

		if (createNew.fail()) 
		{
			HelperFuncs::LogLine("Failed to create file at " + filePath);
		}

		HelperFuncs::LogLine("New file created: " + filePath);
		file.open(filePath, std::ios::out | std::ios::binary);
	}
	
	//HelperFuncs::LogLine("File at : " + filePath + " already exists");

	file.close();
}

void FileSystem::ClearFileData(std::string const& filePath)
{
	if (!FileExists(filePath)) { return; }

	std::fstream file(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return;
	}

	file.close();
}

uint64_t FileSystem::GetFileSize(std::string const& filePath)
{
	if (!FileExists(filePath)) { return 0; }

	std::fstream file(filePath, std::ios::in | std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return 0;
	}

	std::streampos pos = file.tellg();

	if (pos == -1)
	{
		HelperFuncs::LogLine("Failed to get file size. Path: " + filePath);
		return 0;
	}

	return static_cast<uint64_t>(pos);
}

void FileSystem::AddDataToFile(std::fstream& file, std::string const& filePath, char& data, bool manuallyCloseFstream)
{
	if (!file.is_open())
	{
		file.clear(); // reset any failbits
		file.open(filePath, std::ios::app | std::ios::binary);
	}

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return;
	}

	//HelperFuncs::LogLine("File exists");

	int8_t dataInInt8_t = static_cast<int8_t>(data);
	
	//write file data
	file.write(reinterpret_cast<char*>(&dataInInt8_t), sizeof(dataInInt8_t));

	if (manuallyCloseFstream == true) 
	{
		file.close();
	}
}

void FileSystem::AddDataToFile(std::fstream& file, std::string const& filePath, std::string & data, bool manuallyCloseFstream)
{
	if (data.length() < 1) { HelperFuncs::LogLine("Data to small"); return; }

	if (!file.is_open())
	{
		file.clear(); // reset any failbits
		file.open(filePath, std::ios::app | std::ios::binary);
	}

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return;
	}

	//HelperFuncs::LogLine("File exists");

	//get the lenght of the string
	uint64_t stringLenght = data.length();

	//write file data
	file.write(reinterpret_cast<char*>(&stringLenght), sizeof(stringLenght));
	file.write(data.c_str(), stringLenght);

	if (manuallyCloseFstream == true)
	{
		file.close();
	}
}

void FileSystem::AddDataToFile(std::fstream& file, std::string const& filePath, bool& data, bool manuallyCloseFstream)
{
	if (!file.is_open())
	{
		file.clear(); // reset any failbits
		file.open(filePath, std::ios::app | std::ios::binary);
	}

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return;
	}

	//HelperFuncs::LogLine("File exists");

	uint8_t dataInChar = data == false ? 0 : 1;

	//write file data
	file.write(reinterpret_cast<char*>(&dataInChar), sizeof(dataInChar));

	if (manuallyCloseFstream == true)
	{
		file.close();
	}
}

void FileSystem::AddDataToFile(std::fstream& file, std::string const& filePath, uint64_t & data, bool manuallyCloseFstream)
{
	if (!file.is_open())
	{
		file.clear(); // reset any failbits
		file.open(filePath, std::ios::app | std::ios::binary);
	}

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return;
	}
	
	//HelperFuncs::LogLine("File exists");

	//write file data
	file.write(reinterpret_cast<char*>(&data), sizeof(data));

	if (manuallyCloseFstream == true)
	{
		file.close();
	}
}

void FileSystem::AddDataToFile(std::fstream& file, std::string const& filePath, double & data, bool manuallyCloseFstream)
{
	if (!file.is_open())
	{
		file.clear(); // reset any failbits
		file.open(filePath, std::ios::app | std::ios::binary);
	}

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return;
	}

	//HelperFuncs::LogLine("File exists");

	//write file data
	file.write(reinterpret_cast<char*>(&data), sizeof(data));

	if (manuallyCloseFstream == true)
	{
		file.close();
	}
}

char FileSystem::ReadCharFromFile(std::fstream& file, std::string const& filePath, uint64_t const& readOffset, bool manuallyCloseFstream)
{
	//get file size
	uint64_t fileSize = GetFileSize(filePath);
	//HelperFuncs::LogLine("File exists");

	if (!file.is_open())
	{
		file.clear(); // reset any failbits
		file.open(filePath, std::ios::in | std::ios::binary);
	}

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return 0;
	}

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

	if (manuallyCloseFstream == true)
	{
		file.close();
	}

	placeholder = static_cast<char>(dataFromInt8_t);

	return placeholder;
}

std::string	FileSystem::ReadStringFromFile(std::fstream& file, std::string const& filePath, uint64_t const& readOffset, bool manuallyCloseFstream)
{
	//get file size
	uint64_t fileSize = GetFileSize(filePath);
	//HelperFuncs::LogLine("File exists");

	if (!file.is_open())
	{
		file.clear(); // reset any failbits
		file.open(filePath, std::ios::in | std::ios::binary);
	}

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return "";
	}

	//move cursor to file offset
	file.seekg(readOffset);

	uint64_t stringLenght;

	//check if the file still has data
	if (fileSize < readOffset) { HelperFuncs::LogLine("Read offset beyond file limits"); return ""; }
	else
	{
		//check if theres enought bytes to be read
		if ((fileSize - readOffset) < sizeof(uint64_t)) { HelperFuncs::LogLine("Not enough bytes to read from for string lenght"); return ""; }
	}

	//write file data
	file.read(reinterpret_cast<char*>(&stringLenght), sizeof(stringLenght));

	std::string placeholder(stringLenght, '\0');

	//check if the file still has data  //adding the size of uint64_t cause the readoffset is in the (old) locationt and thinks we're still reading the string lenght
	if (fileSize < (readOffset + sizeof(uint64_t))) { HelperFuncs::LogLine("Read offset beyond file limits"); return "\0"; }
	else
	{
		//check if theres enought bytes to be read   //adding the size of uint64_t cause the readoffset is in the (old) locationt and thinks we're still reading the string lenght
		if ((fileSize - (readOffset + sizeof(uint64_t))) < stringLenght) { HelperFuncs::LogLine("Not enough bytes to read from for string value"); return ""; }
	}

	file.read(&placeholder[0], stringLenght);

	if (manuallyCloseFstream == true)
	{
		file.close();
	}

	return placeholder;
}

bool FileSystem::ReadBoolFromFile(std::fstream& file, std::string const& filePath, uint64_t const& readOffset, bool manuallyCloseFstream)
{
	//get file size
	uint64_t fileSize = GetFileSize(filePath);
	//HelperFuncs::LogLine("File exists");

	if (!file.is_open())
	{
		file.clear(); // reset any failbits
		file.open(filePath, std::ios::in | std::ios::binary);
	}

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return 0;
	}


	//move cursor to file offset
	file.seekg(readOffset);

	bool placeholder;
	uint8_t dataFromUint8_t;

	//check if the file still has data
	if (fileSize < readOffset) { HelperFuncs::LogLine("Read offset beyond file limits"); return false; }
	else
	{
		//check if theres enought bytes to be read
		if ((fileSize - readOffset) < sizeof(uint8_t)) { HelperFuncs::LogLine("Not enough bytes to read from for bool value"); return 0; }
	}

	//write file data
	file.read(reinterpret_cast<char*>(&dataFromUint8_t), sizeof(dataFromUint8_t));

	if (manuallyCloseFstream == true)
	{
		file.close();
	}

	placeholder = dataFromUint8_t == 0 ? false : true;

	return placeholder;
}

uint64_t FileSystem::ReadUint64_tFromFile(std::fstream& file, std::string const& filePath, uint64_t const& readOffset, bool manuallyCloseFstream)
{
	//get file size
	uint64_t fileSize = GetFileSize(filePath);
	//HelperFuncs::LogLine("File exists");

	if (!file.is_open())
	{
		file.clear(); // reset any failbits
		file.open(filePath, std::ios::in | std::ios::binary);
	}

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return 0;
	}

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

	if (manuallyCloseFstream == true)
	{
		file.close();
	}

	return placeholder;
}

double FileSystem::ReadDoubleFromFile(std::fstream& file, std::string const& filePath, uint64_t const& readOffset, bool manuallyCloseFstream)
{
	//get file size
	uint64_t fileSize = GetFileSize(filePath);
	//HelperFuncs::LogLine("File exists");

	if (!file.is_open())
	{
		file.clear(); // reset any failbits
		file.open(filePath, std::ios::in | std::ios::binary);
	}

	if (!file.is_open())
	{
		HelperFuncs::LogLine("File failed to open. Path: " + filePath);
		return 0;
	}

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

	if (manuallyCloseFstream == true)
	{
		file.close();
	}

	return placeholder;
}

#pragma region //leave this alone 
namespace {
	extern "C" __declspec(dllimport) unsigned long __stdcall GetModuleFileNameA(void* hModule, char* lpFilename, unsigned long nSize);
}

std::string FileSystem::GetExeFolder()
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