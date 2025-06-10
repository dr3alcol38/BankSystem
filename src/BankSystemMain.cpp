//#include <iostream>
#include <fstream>

#include "HelperFuncs.h"
#include "FileSystem.h"
#include "UserProfile.h"
#include "BankAccount.h"

void BankProgramme();

/*
    queue system to take file requests from the user app
    the user app doesn't need to have any real classes, just ways to send and recive info (my custom binary format) from the bank system app
    the communication files (between the bank and the user) will have versions and a fixed format per version
    possible format for the data files{
                                            file version(uint64_t)
                                            file type(string, so the lenght of the string will be in a uint64_t)(login request, create account request, transfer request e.t.c)
                                            info based on the file type
*/

int main()
{
    std::string path = FileSystem::GetExeFolder() + "test.bankData";

    {
        BankProgramme();
    }

    //BankFile file(path);
    //if (!file.FileExists()) 
    //{ 
    //    file.CreateFile(FileType::userProfileData);
    //}
    //else
    //{
    //    file.ClearFileData(FileType::userProfileData);
    //}

    //std::string k1 = "The first string";
    //std::string k2 = "The second string";

    //file.AddDataToFile(k1);
    //file.AddDataToFile(k2);
    //file.AddDataToFile(k1);
    //file.SaveFile();

    //HelperFuncs::LogLine(file.ReadStringFromFile());
    //HelperFuncs::LogLine(std::to_string(file.ReadUint64_tFromFile()));
    //HelperFuncs::LogLine(file.ReadStringFromFile());
    //HelperFuncs::LogLine(file.ReadStringFromFile());
    //HelperFuncs::LogLine(file.ReadStringFromFile());
    //file.CloseFile();

    return 0;
}

void BankProgramme()
{
    UserProfile _bankAccountManager1 = { "Oluwadamilare", "Oluwafimihan", "Cole", 0 };
    char _interact = ' ';
    float _figure = 0;
    int _accountID;

    while (true)
    {
        HelperFuncs::LogLine("(C)reate or (S)elect account");

        if (_bankAccountManager1.GetAccountsCount() > 0)
        {
            HelperFuncs::Log("Available accounts: ");
            if (_bankAccountManager1.GetAccountsCount() > 0) { HelperFuncs::LogLine(_bankAccountManager1.GetAllAccountsIDs()); }
        }

        HelperFuncs::InputCheck(_interact);
        _interact = std::toupper(_interact);

        if (_interact == 'C')
        {
            int _accountID;
            float _initialAccountBalance;

            while (true)
            {
                HelperFuncs::LogLine("Input account AccountID");

                HelperFuncs::InputCheck(_accountID);

                if (_bankAccountManager1.GetBankAccountByID(_accountID) == nullptr)
                {
                    break;
                }

                HelperFuncs::LogLine("AccountID exists");
            }

            HelperFuncs::LogLine("Input initial account balance");

            HelperFuncs::InputCheck(_initialAccountBalance);

            _bankAccountManager1.AddBankAccount(_accountID, _initialAccountBalance);
        }
        else if (_interact == 'S')
        {
            while (true)
            {
                HelperFuncs::LogLine("Input account AccountID");

                HelperFuncs::InputCheck(_accountID);

                if (_bankAccountManager1.GetBankAccountByID(_accountID) != nullptr)
                {
                    BankAccount* account = _bankAccountManager1.GetBankAccountByID(_accountID);

                    while (true)
                    {
                        HelperFuncs::LogLine("(W)ithdraw,(D)eposit,(B)alance,(T)ransactionHistory or (L)eave");

                        HelperFuncs::InputCheck(_interact);
                        _interact = std::toupper(_interact);

                        if (_interact == 'W')
                        {
                            HelperFuncs::LogLine("Enter amount to withdraw");

                            HelperFuncs::InputCheck(_figure);

                            account->Withdraw(_figure);
                        }

                        if (_interact == 'D')
                        {
                            HelperFuncs::LogLine("Enter amount to deposit");

                            HelperFuncs::InputCheck(_figure);

                            account->Deposit(_figure);
                        }

                        if (_interact == 'B')
                        {
                            HelperFuncs::LogLine("Account balance: " + std::to_string(account->GetBalance()));
                        }

                        if (_interact == 'T')
                        {
                            account->PrintTransactionLog();
                        }

                        if (_interact == 'L') { break; }
                    }
                }
                else
                {
                    HelperFuncs::LogLine("Invalid AccountID");
                }

                break;
            }
        }
        else
        {
            break;
        }
    }
}