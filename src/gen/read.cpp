#include "read.hpp"
#include "../syntax/symbols/export.hpp"

namespace gen
{

/**
 * Builds a symbol storage from file description
 * Allows user to refer to symbols by name instead of array index
 * @param content Definition lines of constructor file
 * @return SymbolStorageGenerator function for building a symbol storage (tuple of dictionaries) from 2d array of symbols
 */
SymbolStorageGenerator generateSymbolStorageGenerator(vector<string> content)
{
    return [content](vector<vector<shared_ptr<Symbol>>>& symbol_groups){
        //print("Building symbol storage");
        SymbolStorage storage;
        for (auto line : content)
        {
            auto terms = lex::seperate(line, {make_tuple(" ", false)});
            assert(terms.size() == 3 or 
                   terms.size() == 4);
            auto identifier = terms[0];
            if (terms.size() == 3)
            {
                auto keyword = terms[0];
                if (keyword == "append")
                {
                    assert(contains(get<1>(storage), terms[2]));
                    assert(contains(get<0>(storage), terms[1]));
                    auto& symbols = get<1>(storage)[terms[2]];
                    auto& symbol  = get<0>(storage)[terms[1]];
                    symbols.push_back(symbol);
                }
                else if (keyword == "concat")
                {
                    assert(contains(get<1>(storage), terms[1]));
                    assert(contains(get<1>(storage), terms[2]));
                    auto& a_symbols = get<1>(storage)[terms[1]];
                    auto& b_symbols = get<1>(storage)[terms[2]];
                    concat(a_symbols, b_symbols);
                }
                else
                {
                    int index = std::stoi(terms[2]);
                    get<1>(storage)[identifier] = symbol_groups[index]; 
                }
            }
            else if (terms.size() == 4)
            {
                if (terms[2] == "names")
                {
                    vector<shared_ptr<Symbol>> names;
                    int index = std::stoi(terms[3]);
                    for (auto symbol : symbol_groups[index])
                    {
                        names.push_back(make_shared<Identifier>(Identifier(symbol->name())));
                    }
                    get<1>(storage)[identifier] = names;
                }
                else
                {
                    int index_a = std::stoi(terms[2]);
                    int index_b = std::stoi(terms[3]);
                    assert(symbol_groups.size() > index_a);
                    assert(symbol_groups[index_a].size() > index_b);
                    get<0>(storage)[identifier] = symbol_groups[index_a][index_b]; 
                }
            }
        }
        //print("Symbol storage creation finished");
        return storage;
    };
}

/**
 * Builds a function for evaluation conditions
 * @param terms whitespace seperated terms of a condition line
 * @return Predicate function used in branch creation
 */
ConditionEvaluator generateConditionEvaluator(vector<string> terms)
{
    assert(not terms.empty());
    auto keyword = terms[0];
    if (keyword == "defined")
    {
        assert(terms.size() == 2);
        auto identifier = terms[1];
        return [identifier](unordered_set<string>& names, SymbolStorage& symbol_storage, const vector<string>&)
        {
            string to_define = get<0>(symbol_storage)[identifier]->name();
            return contains(names, to_define); 
        };
    }
    else if (keyword == "equalTo")
    {
        assert(terms.size() == 3);
        return [terms](unordered_set<string>& names, SymbolStorage& symbol_storage, const vector<string>&)
        {
            assert(contains(get<0>(symbol_storage), terms[1]));
            auto name = get<0>(symbol_storage)[terms[1]]->name();
            return name == terms[2]; 
        };
    }
    else if (keyword == "empty")
    {
        assert(terms.size() == 2);
        return [terms](unordered_set<string>& names, SymbolStorage& symbol_storage, const vector<string>&)
        {
            assert(contains(get<1>(symbol_storage), terms[1]));
            return get<1>(symbol_storage)[terms[1]].empty();
        };
    }
    else if (keyword == "nonempty")
    {
        assert(terms.size() == 2);
        return [terms](unordered_set<string>& names, SymbolStorage& symbol_storage, const vector<string>&)
        {
            assert(contains(get<1>(symbol_storage), terms[1]));
            return not get<1>(symbol_storage)[terms[1]].empty();
        };
    }
    else if (keyword == "both")
    {
        assert(contains(terms, "and"s));
        auto split = std::find(terms.begin(), terms.end(), "and");
        vector<string> first(terms.begin() + 1, split);
        vector<string> second(split + 1, terms.end());
        auto a = generateConditionEvaluator(first);
        auto b = generateConditionEvaluator(second);
        return [a, b](unordered_set<string>& names, SymbolStorage& symbol_storage, const vector<string>& generated)
        {
            return a(names, symbol_storage, generated) and b(names, symbol_storage, generated);
        };
    }
    else
    {
        throw named_exception("Constructor keyword " + keyword + " is not defined");
    }
}


}
