#include "constructor.hpp"

namespace gen
{

/**
 * Construction methods for filetypes
 */
struct FileConstructor
{
    string extension;
    vector<string> default_content;

    FileConstructor(string set_extension, vector<string> set_default_content);
    tuple<string, vector<string>> operator()(vector<string> constructed, string filename);
};

}
