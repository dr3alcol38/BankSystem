#pragma once
#include <string>
#include <vector>

class RequestAndResponseManager
{
	private:
		std::string _requestFolder = "C:/Users/Cole/AppData/LocalLow/BankSystem/Requests/";

		std::vector<std::string> requesFilesList;

	public:
		RequestAndResponseManager();
		~RequestAndResponseManager();

		void CheckForRequestFiles();
};