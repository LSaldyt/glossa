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

    std::vector<ParseFunction> functions = { digits, parseOp, digits };

    Parser<ParseFunction> parser(functions, mathematical);

    SymbolGenerators symbols = {[](void) { return Symbol(); }, [](void) { return Symbol(); }, [](void) { return Symbol(); }};

    Lexer ExpressionLexer(symbols, parser);

    auto file = readFile("test_file.txt");
    for (auto line : file)
    {
        auto lex_result = ExpressionLexer.lex(line);
        if(lex_result.result)
        {
            for (auto token : lex_result.lexed)
            {
                std::cout << std::get<0>(token) << std::endl;
            }
        }
        std::cout << std::endl;
    }
}
