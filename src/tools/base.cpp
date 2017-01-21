#include "base.hpp"

namespace tools
{

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

    unsigned long long getTime()
    {
        struct timeval now;
        gettimeofday (&now, NULL);
        return now.tv_usec + (unsigned long long)now.tv_sec * 1000000;
    }

    void exception_assert(bool b)
    {
        if (not b)
        {
            throw named_exception("Assertion failed");
        }
    }

}
