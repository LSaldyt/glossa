#include "IO.hpp"


std::vector<std::string> readFile(std::string filename)
{
    // Read an entire file, line by line into a vector
    std::vector<std::string> content;

    std::ifstream infile(filename);
    std::string line;

    while (std::getline(infile, line))
    {
        content.push_back(line);
    }

    return content;
}

void writeFile(std::vector<std::string> content, std::string filename)
{
    // Write a vector of lines into a file
    std::ofstream file;
    file.open(filename);
    for (auto line : content)
    {
        file << (line + "\n");
    }
    file.close();
}
namespace tools
{
}
