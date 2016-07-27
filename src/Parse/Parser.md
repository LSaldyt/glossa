# Parser

### This module defines a set of parsers in the `Parse` namespace:

    const auto just;     // takes a string and parses that string exactly
    const auto wildcard; // parses any term
    const auto subsetOf; // ensures a term is a subset of a string
    const auto anyOf;    // takes a list of parsers and parses any of them
    const auto many;     // takes a single parser and parses it until it fails.
    const auto digits;
    const auto alphas;
    const auto puncts;
    const auto uppers;
    const auto lowers;
    const auto parseOp;
    const auto tokenParser; // Converts a parseFunction into a tokenParser
    const auto inOrder;     // Takes a list of parsers, and parses them in order

##### All of these are of the type `ParseFunction`, which returns a `Result` from a list of terms:

    using ParseFunction   = std::function<Result(Terms)>;
    using ParseFunctions  = std::vector<ParseFunction>;
    
    struct Result : public Consumed
    {
        Terms remaining;
    };
    
##### Where `Consumed` is of type:
    
    struct Consumed
    {
        bool   result;
        Terms parsed;
    };
    
##### There is also a `TokenResult`, which is the same as `Result`, except `parsed` and `remaining` hold `Tokens`, not `Terms`
    
##### The `Parse` module is the basis of the `Lexer` module, as a `Lexer` will take a list of symbols and a parser, parsing a sentence and pairing the appropriate terms with the appropriate symbols. However, the `tokenParser` function converts a string parser to one that parses a list of tokens, which would be returned from a `Lexer`

### `ParseFunctions` can be created by the user either from scratch or from a set of templates:

    using Consumer        = std::function<Consumed(Terms)>;
    using Comparator      = std::function<bool(Term)>;

    ParseFunction parseTemplate(Consumer consumer);
    ParseFunction singleTemplate(Comparator comparator);
    ParseFunction multiTemplate(Consumer consumer);
    
##### These are relatively straightforward: the `parseTemplate` and `multiTemplate` both take `Consumers` and use those to generate a result. However, `multiTemplate` repeats the same consumer over and over until it fails, and always returns true. `singleTemplate` simply takes a `Comparator`, which returns a `bool` given a `string`, and uses that to create a single token `ParseFunction`.

#### Custom `ParseFunction`

##### A custom `ParseFunction` can easily be defined, as long as it follows the template of `std::function<Result(Terms)>`, meaning that it returns `Result` object given a list of strings (`vector` of `std::string`s, aliased as `Terms`). For example:

        //A parse function that never consumes anything and always fails:
        const auto customParseFunction = [](Terms terms){return Result(false, Terms(), terms);};
        
        //A custom wildcard function:
        const auto customWildcard = [](Terms terms)
        {
            if (terms.size() > 0)
            {
                //Return the first term as parsed and the remaining terms as remaining
                return Result(true, Terms(terms.begin(), terms.begin() + 1), 
                                    Terms(terms.begin() + 1, terms.end());
            }
            else
            {
                return Result(false, Terms(), terms);
            }
        };
