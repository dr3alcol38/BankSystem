#include "HelperFuncs.h"

namespace HelperFuncs
{
    void Log(std::string const& log)
    {
        std::cout << log;
    }

    void LogLine(std::string const& log)
    {
        std::cout << log << '\n';
    }
}