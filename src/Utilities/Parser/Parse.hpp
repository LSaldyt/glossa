#pragma once
#include <iostream>
#include <istream>
#include <ostream>
#include <iterator>
#include <sstream>
#include <algorithm>

std::string strip_punctuation(const std::string& sentence)
{
    std::string result;
    std::remove_copy_if(sentence.begin(), sentence.end(),
                            std::back_inserter(result), //Store output
                            std::ptr_fun<int, int>(&std::ispunct)
                           );
    return result;
}

std::vector<std::string> tokenize(const std::string& sentence)
{
    std::string str = strip_punctuation(sentence);

    // construct a stream from the string
    std::stringstream strstr(str);

    // use stream iterators to copy the stream to the vector as whitespace separated strings
    std::istream_iterator<std::string> it(strstr);
    std::istream_iterator<std::string> end;
    std::vector<std::string> results(it, end);

    return results;
};
