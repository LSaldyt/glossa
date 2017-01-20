#include "outputmanager.hpp"

namespace tools
{

OutputManager::OutputManager(int verbosity) :
    level(verbosity)
{
}

void OutputManager::log(std::string message, int message_level)
{
    assert(message_level >= 1);
    if (message_level <= level)
    {
        print(repeatString("    ", message_level - 1) + message);
    }
}

}
