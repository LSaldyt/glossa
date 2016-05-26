# Lexer

### The `Lexer` class takes a `Language` definition, and then lexes a string, returning `Tokens`, which are essentially tagged strings, ex: `"2000" : "number"`

##### `Token` is officially implemented as `std::tuple<Term, Term>`, where `Term` is a synonym for `std::string`

### The `Language` constructor:

       Language(const ParseFunction& set_number_parser     = digits,
                       const ParseFunction& set_identifier_parser = alphas,
                       const Terms& set_keywords  = Terms(),
                       const Terms& set_operators = Terms())
                       
##### Based on this information, the lexer can tag `"number"`, `"identifier"`, `"keyword"`, and `"operator"`
##### A tokenParser can be constructed to parse a list of types (`Tokens`), ex: `tokenParser(inOrder(justFrom({"number", "operator", "number"})))`

### It would be a good idea to write a function that simplifies the above construction process, such as :

      const auto buildTokenParser = [](std::vector<std::string> strings){return tokenParser(inOrder(justFrom(strings)));};
      
