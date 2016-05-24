# Parser

### This module defines a set of parsers in the `Parse` namespace:

    const auto just; 
    const auto wildcard; 
    const auto subsetOf; 
    const auto anyOf;
    const auto many;
    const auto digits;
    const auto alphas;
    const auto puncts;
    const auto uppers;
    const auto lowers;
    const auto parseOp;

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
    
### Any ParseFunction can be used with the `Parser` class, which parses a list of parsers in order, returning their collective results:

    template <typename T>
    class Parser
    {
    public:

        std::vector<T> parsers;
        Seperators seperators;
        
        Parser(const std::vector<T>& set_parsers,
               const Seperators& set_seperators)
        {
            parsers    = set_parsers;
            seperators = set_seperators;
        }
        ~Parser(){}

        Result parse(const Terms& original_terms)
        {
            Terms parsed;
            Terms terms = original_terms;

            for (auto parser : parsers)
            {
                auto parse_result = parser(terms);
                if(parse_result.result)
                {
                    parsed.insert( parsed.end(), parse_result.parsed.begin(), parse_result.parsed.end() );
                    terms  = parse_result.remaining;
                }
                else
                {
                    return Result (false, Terms(), original_terms);
                }
            }
            return Result(true, parsed, terms);
        }
    };
    
##### Because of the template, `Parser` is actually capable of taking a list of `Parser`s as well. The `()` operator is overloaded, so a fully constructed `Parser` can be called as if it were a `ParseFunction`. `Parser` also is capable of tokenizing a sentence, given to the parser as a string. This is where the Seperator list comes in.
  
      using Seperator       = std::tuple<char, bool>;
      using Seperators      = std::vector<Seperator>;
      
##### The `Parse` module is the basis of the `Lexer` module, as a `Lexer` will take a list of symbols and a parser, parsing a sentence and pairing the appropriate terms with the appropriate symbols.
