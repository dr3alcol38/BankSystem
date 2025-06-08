#pragma once
#include <string>
#include <fstream>

enum class FileType
{
	userProfileData	= 0,
	bankAccountData = 1,
	requestFile		= 2,
	responseFile	= 3
};

class BankFileSystem
{
private:
	static std::fstream CreateNewFile(std::string const& filePath);

public:
	static std::string fileTypeStings[4];

	BankFileSystem();
	~BankFileSystem();

	static std::string FileTypeToString(FileType const& fileType);

	static bool FileExists(std::string const& filePath);

	static void CreateBankFile(std::string const& filePath, FileType const& fileType);
	static void CreateBankFile(std::string const& filePath, FileType const& fileType, uint64_t const& fileVersion);

	static void ClearFileData(std::string const& filePath);

	static uint64_t GetFileSize(std::string const& filePath);

	//writes
	static void AddDataToFile(std::string const& filePath, char& data);
	static void AddDataToFile(std::string const& filePath, std::string & data);
	static void AddDataToFile(std::string const& filePath, bool& data);
	static void AddDataToFile(std::string const& filePath, uint64_t & data);
	static void AddDataToFile(std::string const& filePath, double & data);

	//reads
	static char			ReadCharFromFile	(std::string const& filePath, uint64_t const& readOffset);
	static std::string	ReadStringFromFile	(std::string const& filePath, uint64_t const& readOffset);
	static bool			ReadBoolFromFile	(std::string const& filePath, uint64_t const& readOffset);
	static uint64_t		ReadUint64_tFromFile(std::string const& filePath, uint64_t const& readOffset);
	static double		ReadDoubleFromFile	(std::string const& filePath, uint64_t const& readOffset);

	static std::string GetExeFolder();
};

struct BankFile
{
private:
	bool _hasFile = false;

public:
	std::string _filePath;
	uint64_t _readOffset = 0;

	BankFile(std::string const& filePath) : _filePath(filePath)
	{
		if (BankFileSystem::FileExists(_filePath)) { _hasFile = true; }
	}

	//Copy constructor
	BankFile(BankFile const&) = delete;

	//Move constructor
	BankFile(BankFile const&&) = delete;

	~BankFile()
	{

	}

	bool FileExists() 
	{
		return BankFileSystem::FileExists(GetFilePath());
	}

	std::string const GetFilePath() const
	{
		return _filePath;
	}

	void CreateFile(FileType const& fileType)
	{
		if (FileExists()) { return; }

		BankFileSystem::CreateBankFile(GetFilePath(), fileType);

		if (BankFileSystem::FileExists(_filePath)) { _hasFile = true; }
	}

	void CreateFile(FileType const& fileType, uint64_t const& fileVersion)
	{
		if (FileExists()) { return; }

		BankFileSystem::CreateBankFile(GetFilePath(), fileType, fileVersion);

		if (BankFileSystem::FileExists(_filePath)) { _hasFile = true; }
	}

	void ClearFileData(FileType const& fileType)
	{
		if (!FileExists()) { return; }

		BankFileSystem::ClearFileData(GetFilePath());

		BankFileSystem::CreateBankFile(GetFilePath(), fileType);
	}

	uint64_t GetFileSize()
	{
		return BankFileSystem::GetFileSize(GetFilePath());
	}
	
	void SetReadOffset(uint64_t const& offset)
	{
		_readOffset = offset;
	}

	void ResetReadOffset() 
	{
		_readOffset = 0;
	}

	void AddDataToFile(char& data)
	{
		if (!FileExists()) { return; }

		BankFileSystem::AddDataToFile(GetFilePath(), data);
	}

	void AddDataToFile(std::string & data)
	{
		if (!FileExists()) { return; }

		BankFileSystem::AddDataToFile(GetFilePath(), data);
	}

	void AddDataToFile(bool& data)
	{
		if (!FileExists()) { return; }

		BankFileSystem::AddDataToFile(GetFilePath(), data);
	}

	void AddDataToFile(uint64_t & data)
	{
		if (!FileExists()) { return; }

		BankFileSystem::AddDataToFile(GetFilePath(), data);
	}

	void AddDataToFile(double & data)
	{
		if (!FileExists()) { return; }

		BankFileSystem::AddDataToFile(GetFilePath(), data);
	}

	char ReadCharFromFile()
	{
		if (!FileExists()) { return 0; }

		uint64_t placeHolder = _readOffset;

		_readOffset += sizeof(uint64_t);

		return BankFileSystem::ReadCharFromFile(GetFilePath(), placeHolder);
	}

	std::string ReadStringFromFile()
	{
		if (!FileExists()) { return "The file wasn't accessible"; }

		std::string string = BankFileSystem::ReadStringFromFile(GetFilePath(), _readOffset);

		_readOffset += string.length();
		_readOffset += sizeof(uint64_t);

		return string;
	}

	bool ReadBoolFromFile()
	{
		if (!FileExists()) { return 0; }

		uint64_t placeHolder = _readOffset;

		_readOffset += sizeof(uint64_t);

		return BankFileSystem::ReadBoolFromFile(GetFilePath(), placeHolder);
	}
	
	uint64_t ReadUint64_tFromFile()
	{
		if (!FileExists()) { return 0; }

		uint64_t placeHolder = _readOffset;

		_readOffset += sizeof(uint64_t);

		return BankFileSystem::ReadUint64_tFromFile(GetFilePath(), placeHolder);
	}
	
	double ReadDoubleFromFile()
	{
		if (!FileExists()) { return 0; }

		uint64_t placeHolder = _readOffset;

		_readOffset += sizeof(double);

		return BankFileSystem::ReadDoubleFromFile(GetFilePath(), placeHolder);
	}
};