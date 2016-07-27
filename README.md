# Compiler

A simple compiler:

[Parser](https://github.com/BananaSky/Compiler/blob/master/src/Parser/Parser.md)  
[Lexer](https://github.com/BananaSky/Compiler/blob/master/src/Lexer/Lexer.md)  
[IO](https://github.com/BananaSky/Compiler/blob/master/src/IO/IO.md)  

(Documentation for code gen and other features will be added as they become more robust)

Currently:

The existing compiler can convert something like:

f(x, y):
    g(z):
        return z * z
    return g

Into:
    
const auto f = [=](auto x, auto y){
    const auto g = [=](auto z){
            return x * z + y;
        };
     return g;
     };

But is very brittle and only supports function definitions, simple expressions, assignment, and function calls.

