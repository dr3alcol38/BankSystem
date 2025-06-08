#pragma once
#include <iostream>

namespace HelperFuncs
{
    void Log(std::string const& log);
    void LogLine(std::string const& log);

    template <typename T>
    void InputCheck(T& t)
    {
        while (true)
        {
            std::cin >> t;

            if (std::cin.fail())
            {
                std::cin.clear();

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid value" << '\n';
            }
            else
            {
                break;
            }
        }
    }
}