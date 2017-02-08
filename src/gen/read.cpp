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
            //print(line);
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
                    if (symbol_groups[index_a].size() <= index_b)
                    {
                        print(line);
                    }
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

/**
 * Create a branch from lines in a constructor file
 * @param content Lines of constructor file
 * @param symbol_storage_generator Function for creating symbol storage
 * @param ec_creator function defining how to build an element constructor
 * @return Branch<string> which will follow user-defined logic to build a syntax element
 */
Branch<string> generateBranch(vector<string> content, 
                         SymbolStorageGenerator symbol_storage_generator,
                         ElementConstructorCreator<string> ec_creator)
{
    vector<ElementConstructor<string>> line_constructors;
    vector<Branch<string>>             nested_branches;

    auto if_body_start   = content.begin();
    auto else_body_start = content.begin(); // For collecting and processing contents of an if statement body
    auto if_body_end     = content.begin();
    auto else_body_end   = content.begin();

    bool has_else = false;

    const auto addNestedBranch = [&](auto start, auto end, auto conditionline, bool inverse)
    {
        vector<string> body(start, end);
        auto nested_branch                = generateBranch(body, symbol_storage_generator, ec_creator);
        auto original_condition_terms     = lex::seperate(*conditionline, {make_tuple(" ", false)});
        if (inverse)
        {
            nested_branch.condition_evaluator = inverseBranch(generateConditionEvaluator(slice(original_condition_terms, 1)));
        }
        else
        {
            nested_branch.condition_evaluator = generateConditionEvaluator(slice(original_condition_terms, 1));
        }
        nested_branches.push_back(nested_branch);
    };


    int nest_count = 0;

    auto it = content.begin();
    while (it != content.end())
    {
        auto terms = lex::seperate(*it, {make_tuple(" ", false)});
        if (not terms.empty())
        {
            auto keyword = terms[0];
            if (keyword == "branch")
            {
                if (nest_count == 0)
                {
                    if_body_start = it;
                    nested_branches.push_back(Branch<string>(defaultBranch, line_constructors, {}));
                    line_constructors.clear();
                }
                nest_count++;
            }
            else if (keyword == "elsebranch")
            {
                if (nest_count == 1)
                {
                    if_body_end     = it;
                    else_body_start = it;
                    has_else = true;
                }
            }
            else if (keyword == "end")
            {
                if (nest_count == 1)
                {
                    if (not has_else)
                    {
                        if_body_end = it;
                    }
                    addNestedBranch(if_body_start + 1, if_body_end, if_body_start, false);

                    if (has_else)
                    {
                        else_body_end = it;
                        addNestedBranch(else_body_start + 1, else_body_end, if_body_start, true);
                    }
                }
                nest_count--;
            }   
            else if (nest_count == 0)
            {
                line_constructors.push_back(ec_creator(*it));
            }
        }
        it++;
    }
    nested_branches.push_back(Branch<string>(defaultBranch, line_constructors, {}));
    line_constructors.clear();
    assert(nest_count == 0);

    return Branch<string>(defaultBranch, line_constructors, nested_branches);
}

vector<tuple<string, Constructor<string>>> generateConstructor(vector<string> content, 
        vector<tuple<string, FileConstructor>> file_constructors,
        ElementConstructorCreator<string> ec_creator)
{
    // Seperate constructor into header and source constructors
    assert(contains(content, "defines"s));
    auto defines_i = std::find(content.begin(), content.end(), "defines");
    auto declarations = vector<string>(content.begin(), defines_i);

    auto symbol_storage_generator = generateSymbolStorageGenerator(declarations);
    vector<string> definitions;

    string type = "definitions";
    string tag;
    bool first = true;
    auto last_it = content.begin();
    auto it = content.begin();

    // Iterate over file types for each constructor file
    vector<tuple<string, Constructor<string>>> constructors;
    for (auto file_constructor : file_constructors)
    {
        tag = get<0>(file_constructor);
        //print(tag);
        assert(contains(content, tag));
        it = std::find(content.begin(), content.end(), tag);

        if (first)
        {
            definitions  = vector<string>(defines_i + 1, it);
            last_it = it;
            first = false;
        }
        else
        {
            auto body = vector<string>(last_it + 1, it);
            //print("Created body");
            auto constructor = Constructor<string>(symbol_storage_generator, 
                                                   generateBranch(body, symbol_storage_generator, 
                                                       ec_creator),
                                                   definitions);
            constructors.push_back(make_tuple(type, constructor));
            last_it = it;
        }
        type = tag;
        //print("Done");
    }
    auto body = vector<string>(last_it + 1, content.end());
    auto constructor = Constructor<string>(symbol_storage_generator, 
                                           generateBranch(body, 
                                                          symbol_storage_generator, 
                                                          ec_creator), 
                                           definitions);
    constructors.push_back(make_tuple(type, constructor));
    return constructors;
}

}
