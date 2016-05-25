#include "Compiler.hpp"
#include "Utilities/IO/readFile.hpp"
#include "Utilities/Parser/Parser.hpp"
#include "Utilities/Syntax/Symbols.hpp"
#include "Utilities/Lexer/Lexer.hpp"
#include <iostream>
#include <typeinfo>

int main()
{
    using namespace Parse;
    using namespace Syntax;

    // std::vector<ParseFunction> functions = { digits, parseOp, digits };
    //
    // Parser<ParseFunction> parser(functions, mathematical);
    //
    // SymbolGenerator symbolgen = [] (std::string) { return std::make_shared<Symbol>(Symbol()); };
    //
    // SymbolGenerators symbols = {symbolgen, symbolgen, symbolgen};
    //
    // Lexer ExpressionLexer(symbols, parser);

    auto file = readFile("test_file.txt");
    for (auto line : file)
    {
        for(auto t : seperate(line, mathematical))
        {
            std::cout << "-" <<  t << "-" <<  std::endl;
        }
    }
}
