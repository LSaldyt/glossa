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
    
    string sliceString(string original, int begin_offset, int end_offset)
    {
        auto size = original.size();
        assert(not (begin_offset > size));
        assert(not (abs(end_offset) > size));
        return string(original.begin() + begin_offset, original.end() + end_offset);
    }

    void replaceAll( string &s, const string &search, const string &replace )
    {
        for (size_t pos = 0; ;pos += replace.length())
        {
            pos = s.find( search, pos );
            if( pos == string::npos ) break;
            s.erase( pos, search.length() );
            s.insert( pos, replace );
        }
    }

    string repeatString(const string&s, int n)
    {
        string repr = "";
        for (int i = 0; i < n; i++)
        {
            repr += s;
        }
        return repr;
    }
}
