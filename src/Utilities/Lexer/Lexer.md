# Lexer

### The job of the `Lexer` is to join a set of terms that were parsed with a set of `SymbolGenerators`:

##### Essentially, each `SymbolGenerator` is a Constructor that takes a string and returns a pointer to a symbolic type. However, the generator itself will not check that the string is actually convertible to the symbolic type, instead it will simply throw an error if it is not. It is the `Parser`'s job to ensure that `Lexer` recieves the correct types.

      using SymbolGenerator  = std::function<Symbol(void)>;
      using SymbolGenerators = std::vector<SymbolGenerator>;

##### An example of `Symbol` would be the `Operator` class:

      struct Operator : public Symbol
      {
          Op op;
      };
      
##### Where `Symbol` is an empty struct

##### A `SymbolGenerator` for the `Operator` class should take a string and return the corresponding operator.
##### Actually, it should return a pointer to the corresponding operator, allowing for polymorphism of symbols. This currently isn't a feature, but it will be added very soon...

### Lexer will return a LexResult:

    struct LexResult
    {
        bool   result;
        Syntax::Tokens lexed;
    };
    
##### Where `Tokens` are defined as:

    using Token  = std::tuple<Parse::Term, SymbolGenerator>;
    using Tokens = std::vector<Token>;

