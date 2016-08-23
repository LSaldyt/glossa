# Lexing

Lexing converts plain text sentences into a vector of tokens.

It has three steps:
  - Seperation of a sentence into terms (strings) based on seperators like whitespace and operators
  - Tokenization of terms (aka identification) - convert `'42'` to `Token('42', 'int', 'literal')`
  - Symbolization of tokens into `SymbolicTokens` (from Token('42', 'int', 'literal') to SymbolicToken(Integer(42), 'int', 'literal')

Seperation is fairly simple - iterate over a string, and seperate it when a set of seperating characters (a space, a plus sign, etc..) is encountered

Tokenization relys on good seperation. It will iterate over a set of terms, applying `LanguageLexer`s to them, converting them to `Token`s if a lexer succeeds. A lexer is a struct with three members:
  - matching function (term -> result)
  - subtype           (string)
  - type              (string)
If the matching function succeeds, a Token of the form (term, subtype, type) is returned

Once tokens are created, they are converted to their symbolic forms through a dictionary that maps either types or subtypes to symbolic constructors:

```
const auto symbolic_dictionary = std::unordered_map<std::string, std::function<std::shared_ptr<Symbol>(Terms)>>({
        'int' : [](std::string i){Integer(std::stoi(i))} // Convert string to int (We already know that it is convertable, since it matched our definition of int)
        [...]
    })
```


