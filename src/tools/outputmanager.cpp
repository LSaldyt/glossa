#include "outputmanager.hpp"

namespace tools
{

OutputManager::OutputManager(int verbosity) :
    level(verbosity)
{
}

void OutputManager::log(std::string message, int message_level)
{
    if (message_level <= level)
    {
        print(message);
    }
}

}
