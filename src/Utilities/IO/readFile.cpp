#include "readFile.hpp"


std::vector<std::string> readFile(std::string filename)
{
    std::vector<std::string> toReturn;

    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line))
    {
        toReturn.push_back(line);
    }

    return toReturn;
}
