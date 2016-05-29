#include "IO.hpp"


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

void writeFile(std::vector<std::string> content, std::string filename)
{
    std::ofstream file;
    file.open (filename);
    for (auto line : content)
    {
        file << (line + "\n");
    }
    file.close();
}
