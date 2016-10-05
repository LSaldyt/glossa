#include "tools.hpp"

namespace tools
{
    vector<string> readFile(string filename)
    {
        // Read an entire file, line by line into a vector
        vector<string> content;

        std::ifstream infile(filename);
        string line;

        while (getline(infile, line))
        {
            content.push_back(line);
        }

        return content;
    }

    void writeFile(vector<string> content, string filename)
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

    named_exception::named_exception(std::string set_name) : name(set_name)
    {
    }
    const char* named_exception::what() const throw()
    {
        return name.c_str();
    }
    
}
