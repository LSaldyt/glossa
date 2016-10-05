#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

std::vector<std::string> readFile(std::string filename);
void writeFile(std::vector<std::string> content, std::string filename);

namespace tools
{
}

