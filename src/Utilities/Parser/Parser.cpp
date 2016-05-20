
#include "Parser.hpp"
#include <iostream>
#include <istream>
#include <ostream>
#include <iterator>
#include <sstream>
#include <algorithm>


Parser::Parser (){}
Parser::~Parser(){}

std::vector<std::string> Parser::tokenize(std::string sentence)
{
    std::string str = "The quick brown fox";

    // construct a stream from the string
    std::stringstream strstr(str);

    // use stream iterators to copy the stream to the vector as whitespace separated strings
    std::istream_iterator<std::string> it(strstr);
    std::istream_iterator<std::string> end;
    std::vector<std::string> results(it, end);

    // send the vector to stdout.
    //std::ostream_iterator<std::string> oit(std::cout);
    //std::copy(results.begin(), results.end(), oit);
    for (auto result : results)
    {
        std::cout << result << std::endl;
    }
};
//NamedResult parser(std::vector<std::string> tokens)
//{

//}
