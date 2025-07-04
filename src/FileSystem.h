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

class FileSystem
{
private:

public:
	static std::string fileTypeStrings[4];

	FileSystem();
	~FileSystem();

	static std::string FileTypeToString(FileType const& fileType);

	static bool FileExists(std::string const& filePath);

	static void CreateNewFile(std::string const& filePath);

	static void DeleteFile(std::string const& filePath);

	static void ClearFileData(std::string const& filePath);

	static uint64_t GetFileSize(std::string const& filePath);

	//writes
	static void			AddDataToFile		(std::fstream& file, std::string const& filePath, char			 data, bool manuallyCloseFstream = true);
	static void			AddDataToFile		(std::fstream& file, std::string const& filePath, std::string&   data, bool manuallyCloseFstream = true);
	static void			AddDataToFile		(std::fstream& file, std::string const& filePath, bool			 data, bool manuallyCloseFstream = true);
	static void			AddDataToFile		(std::fstream& file, std::string const& filePath, uint64_t		 data, bool manuallyCloseFstream = true);
	static void			AddDataToFile		(std::fstream& file, std::string const& filePath, double		 data, bool manuallyCloseFstream = true);

	//reads
	static char			ReadCharFromFile	(std::fstream& file, std::string const& filePath, uint64_t readOffset, bool manuallyCloseFstream = true);
	static std::string	ReadStringFromFile	(std::fstream& file, std::string const& filePath, uint64_t readOffset, bool manuallyCloseFstream = true);
	static bool			ReadBoolFromFile	(std::fstream& file, std::string const& filePath, uint64_t readOffset, bool manuallyCloseFstream = true);
	static uint64_t		ReadUint64_tFromFile(std::fstream& file, std::string const& filePath, uint64_t readOffset, bool manuallyCloseFstream = true);
	static double		ReadDoubleFromFile	(std::fstream& file, std::string const& filePath, uint64_t readOffset, bool manuallyCloseFstream = true);

	static std::string GetExeFolder();
};

#include <vector>
#include <functional>

struct BankFile
{
private:
	std::fstream _file;
	std::string _filePath;
	bool _hasFile = false;
	uint64_t _readOffset = 0;
	std::vector<std::function<void()>> _functionBuffer;

	#pragma region //Write to file functions
	void WriteDataToFile(char& data)
	{
		if (!FileExists()) { return; }

		FileSystem::AddDataToFile(_file, GetFilePath(), data, false);
	}

	void WriteDataToFile(std::string& data)
	{
		if (!FileExists()) { return; }

		FileSystem::AddDataToFile(_file, GetFilePath(), data, false);
	}

	void WriteDataToFile(bool& data)
	{
		if (!FileExists()) { return; }

		FileSystem::AddDataToFile(_file, GetFilePath(), data, false);
	}

	void WriteDataToFile(uint64_t& data)
	{
		if (!FileExists()) { return; }

		FileSystem::AddDataToFile(_file, GetFilePath(), data, false);
	}

	void WriteDataToFile(double& data)
	{
		if (!FileExists()) { return; }

		FileSystem::AddDataToFile(_file, GetFilePath(), data, false);
	}
	#pragma endregion

	#pragma region //Function buffer
	void AddToFunctionBuffer(char& data)
	{
		// = allows the lambda to keep it's version of data
		_functionBuffer.emplace_back([=]() { char copy = data; WriteDataToFile(copy); });
	}

	void AddToFunctionBuffer(std::string& data)
	{
		// = allows the lambda to keep it's version of data
		_functionBuffer.emplace_back([=]() { std::string copy = data; WriteDataToFile(copy); });
	}

	void AddToFunctionBuffer(bool& data)
	{
		// = allows the lambda to keep it's version of data
		_functionBuffer.emplace_back([=]() { bool copy = data; WriteDataToFile(copy); });
	}

	void AddToFunctionBuffer(uint64_t& data)
	{
		// = allows the lambda to keep it's version of data
		_functionBuffer.emplace_back([=]() { uint64_t copy = data; WriteDataToFile(copy); });
	}

	void AddToFunctionBuffer(double& data)
	{
		// = allows the lambda to keep it's version of data
		_functionBuffer.emplace_back([=]() { double copy = data; WriteDataToFile(copy); });
	}

	void ClearFunctionBuffer()
	{
		if (_functionBuffer.size() > 0)
		{
			for (std::function<void()>& func : _functionBuffer)
			{
				func = []() {};
			}
		}

		_functionBuffer.clear();
	}

	void FlushFunctionBuffer()
	{
		if (_functionBuffer.size() > 0) 
		{
			for (std::function<void()> & func : _functionBuffer)
			{
				func();
			}
		}
	}
	#pragma endregion

public:
	std::string _dataTypesList;

	BankFile(std::string const& filePath) : _filePath(filePath)
	{
		if (FileSystem::FileExists(_filePath)) { _hasFile = true; }
	}

	//Copy constructor
	BankFile(BankFile const&) = delete;

	//Move constructor
	BankFile(BankFile &&) = delete;

	~BankFile()
	{
		if (_file.is_open()) { CloseFile(); }
	}

	bool FileExists() 
	{
		return FileSystem::FileExists(GetFilePath());
	}

	std::string const& GetFilePath() const
	{
		return _filePath;
	}

	void CreateFile()
	{
		if (FileExists()) { return; }

		FileSystem::CreateNewFile(GetFilePath());

		if (FileSystem::FileExists(_filePath)) { _hasFile = true; }
	}

	void ClearFileData()
	{
		if (!FileExists()) { return; }

		FileSystem::ClearFileData(GetFilePath());
		
		//FlushFunctionBuffer();
		ClearFunctionBuffer();

		CloseFile();
	}

	uint64_t GetFileSize()
	{
		return FileSystem::GetFileSize(GetFilePath());
	}
	
	void SetReadOffset(uint64_t const& offset)
	{
		_readOffset = offset;
	}

	void ResetReadOffset() 
	{
		_readOffset = 0;
	}

	void SaveFile()
	{
		FlushFunctionBuffer();

		ClearFunctionBuffer();

		CloseFile();
	}

	void CloseFile()
	{
		if (!FileExists()) { return; }

		_file.close();
		_file.clear();

		ClearFunctionBuffer();
	}

	#pragma region //Add data to file functions
	void AddDataToFile(char& data)
	{
		AddToFunctionBuffer(data);
	}

	void AddDataToFile(std::string& data)
	{
		AddToFunctionBuffer(data);
	}

	void AddDataToFile(bool& data)
	{
		AddToFunctionBuffer(data);
	}

	void AddDataToFile(uint64_t& data)
	{
		AddToFunctionBuffer(data);
	}

	void AddDataToFile(double& data)
	{
		AddToFunctionBuffer(data);
	}
	#pragma endregion

	#pragma region //Read from file functions
	char ReadCharFromFile()
	{
		if (!FileExists()) { return 0; }

		uint64_t placeHolder = _readOffset;
		//using int8_t cause the char is casted to an int8_t when saved
		_readOffset += sizeof(int8_t);

		return FileSystem::ReadCharFromFile(_file, GetFilePath(), placeHolder, false);
	}

	std::string ReadStringFromFile()
	{
		if (!FileExists()) { return ""; }

		std::string string = FileSystem::ReadStringFromFile(_file, GetFilePath(), _readOffset, false);

		_readOffset += string.length();
		_readOffset += sizeof(uint64_t);

		return string;
	}

	bool ReadBoolFromFile()
	{
		if (!FileExists()) { return 0; }

		uint64_t placeHolder = _readOffset;
		//using int8_t cause the bool is casted to an uint8_t when saved
		_readOffset += sizeof(uint8_t);

		return FileSystem::ReadBoolFromFile(_file, GetFilePath(), placeHolder, false);
	}
	
	uint64_t ReadUint64_tFromFile()
	{
		if (!FileExists()) { return 0; }

		uint64_t placeHolder = _readOffset;

		_readOffset += sizeof(uint64_t);

		return FileSystem::ReadUint64_tFromFile(_file, GetFilePath(), placeHolder, false);
	}
	
	double ReadDoubleFromFile()
	{
		if (!FileExists()) { return 0; }

		uint64_t placeHolder = _readOffset;

		_readOffset += sizeof(double);

		return FileSystem::ReadDoubleFromFile(_file, GetFilePath(), placeHolder, false);
	}
	#pragma endregion
};