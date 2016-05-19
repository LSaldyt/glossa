#include "Compiler.hpp"
#include "readFile.hpp"

int main()
{
    std::cout << "Starting" << std::endl;
    auto file = readFile("test_file.txt");
    for (auto line : file)
    {
        std::cout << line << std::endl;
    }
}
