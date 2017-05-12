/// Copyright 2017 Lucas Saldyt
#include "fileconstructor.hpp"

namespace gen
{


FileConstructor::FileConstructor(string set_extension, vector<string> set_default_content) : extension(set_extension), default_content(set_default_content)
{

}

/**
 * Create a file from a file template
 */
tuple<string, vector<string>> FileConstructor::operator()(vector<string> constructed, string filename)
{
    vector<string> file_content(default_content);
    concat(file_content, constructed);
    string filepath = filename + extension;
    return make_tuple(filepath, file_content);
}

}
