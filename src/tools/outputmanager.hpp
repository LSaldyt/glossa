/// Copyright 2017 Lucas Saldyt
#pragma once
#include "io.hpp"

namespace tools
{

class OutputManager
{
public:
    OutputManager(int verbosity=0);

    void log(std::string message, int message_level=1);

private:
    int level;
};

}
