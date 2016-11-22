#include "generator.hpp"
#include "../syntax/symbols/symbol.hpp"

namespace gen 
{

Generator::Generator(vector<string> filenames, string directory)
{
    for (auto filename : filenames)
    {
        print("Reading constructor file: " + filename);
        construction_map[filename] = read(directory + filename);
    }
}

tuple<Constructor, Constructor> Generator::read(string filename)
{
    auto content = readFile(filename);
    
    // Seperate constructor into header and source constructors
    assert(contains(content, "defines"s));
    assert(contains(content, "header"s));
    assert(contains(content, "source"s));
    auto defines_i = std::find(content.begin(), content.end(), "defines");
    auto header_i  = std::find(content.begin(), content.end(), "header");
    auto source_i  = std::find(content.begin(), content.end(), "source");

    auto declarations = vector<string>(content.begin(), defines_i);
    auto definitions  = vector<string>(defines_i + 1, header_i);
    auto header       = vector<string>(header_i  + 1, source_i);
    auto source       = vector<string>(source_i  + 1, content.end());

    print("DECLARATIONS");
    auto symbol_storage_generator = generateSymbolStorageGenerator(declarations);

    print("HEADER");
    auto header_constructor = Constructor(symbol_storage_generator, generateBranch(header, symbol_storage_generator), definitions);

    print("SOURCE");
    auto source_constructor = Constructor(symbol_storage_generator, generateBranch(source, symbol_storage_generator), definitions);

    return make_tuple(header_constructor, source_constructor);
}

Branch Generator::generateBranch(vector<string> content, SymbolStorageGenerator symbol_storage_generator)
{
    vector<LineConstructor> line_constructors;
    vector<Branch>          nested_branches;
    
    auto if_body_start   = content.begin();
    auto else_body_start = content.begin(); // For collecting and processing contents of an if statement body
    auto if_body_end     = content.begin();
    auto else_body_end   = content.begin();

    bool in_conditional = false;
    bool has_else = false;

    const auto addNestedBranch = [&](auto start, auto end, auto conditionline, bool inverse)
    {
        vector<string> body(start, end);
        auto nested_branch                = generateBranch(body, symbol_storage_generator);
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

    auto it = content.begin();
    while (it != content.end())
    {
        auto terms = lex::seperate(*it, {make_tuple(" ", false)});
        if (not terms.empty())
        {
            auto keyword = terms[0];
            if (keyword == "if")
            {
                if_body_start = it;
                in_conditional = true;
            }
            else if (keyword == "else")
            {
                if_body_end     = it;
                else_body_start = it;
                has_else = true;
            }
            else if (keyword == "endif")
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
                in_conditional = false;
            }
            else if (not in_conditional)
            {
                line_constructors.push_back(generateLineConstructor(terms));
            }
        }
        it++;
    }

    return Branch(defaultBranch, line_constructors, nested_branches);
}

LineConstructor Generator::generateLineConstructor(vector<string> terms)
{
    return [terms, this](unordered_set<string>& names, SymbolStorage& storage, bool source){
        print("Constructing Line");
        string representation = "";
        if (not terms.empty())
        {
            auto keyword = terms[0];
            if (keyword == "sep")
            {
                assert(terms.size() == 3 or terms.size() == 4);
                assert(contains(get<1>(storage), terms[2]));
                auto symbols = get<1>(storage)[terms[2]];
                string formatter = "@";
                if (terms.size() == 4)
                {
                    formatter = terms[3];
                }
                representation += sepWith(*this, symbols, names, source, terms[1], formatter);
            }
            else
            {
                print("No keyword found");
                for (auto t : terms)
                {
                    print(t);
                    if (t[0] == '$')
                    {
                        auto symbol = get<0>(storage)[t];
                        if (source)
                        {
                            representation += symbol->source(*this, names) + " ";
                        }
                        else 
                        {
                            representation += symbol->header(*this, names) + " ";
                        }
                    }
                    else
                    {
                        representation += t + " ";
                    }
                }
            }
        }
        replaceAll(representation, "&", " ");
        replaceAll(representation, "^", "\n");
        return representation;
    };
}

SymbolStorageGenerator Generator::generateSymbolStorageGenerator(vector<string> content)
{
    return [content](vector<vector<shared_ptr<Symbol>>>& symbol_groups){
        SymbolStorage symbol_storage;
        for (auto line : content)
        {
            auto terms = lex::seperate(line, {make_tuple(" ", false)});
            assert(terms.size() == 3 or 
                   terms.size() == 4);
            auto identifier = terms[0];
            if (terms.size() == 3)
            {
                int index = std::stoi(terms[2]);
                get<1>(symbol_storage)[identifier] = symbol_groups[index]; 
            }
            else if (terms.size() == 4)
            {
                int index_a = std::stoi(terms[2]);
                int index_b = std::stoi(terms[3]);
                get<0>(symbol_storage)[identifier] = symbol_groups[index_a][index_b]; 
            }
        }
        return symbol_storage;
    };
}

ConditionEvaluator Generator::generateConditionEvaluator(vector<string> terms)
{
    assert(not terms.empty());
    auto keyword = terms[0];
    if (keyword == "defined")
    {
        assert(terms.size() == 2);
        auto identifier = terms[1];
        return [identifier](unordered_set<string>& names, SymbolStorage& symbol_storage)
        {
            string to_define = get<0>(symbol_storage)[identifier]->name();
            print("Checking if " + to_define + " is defined");
            return contains(names, to_define) or
                   contains(names, to_define); 
        };
    }
    else if (keyword == "equalTo")
    {
        assert(terms.size() == 3);
        return [terms](unordered_set<string>& names, SymbolStorage& symbol_storage)
        {
            assert(contains(get<0>(symbol_storage), terms[1]));
            auto name = get<0>(symbol_storage)[terms[1]]->name();
            return name == terms[2]; 
        };
    }
    else
    {
        throw named_exception("Constructor keyword " + keyword + " is not defined");
    }
}

tuple<vector<string>, vector<string>> Generator::operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>& symbol_groups, string symbol_type)
{
    print(symbol_type);
    assert(contains(construction_map, symbol_type));
    auto constructors = construction_map[symbol_type]; 
    auto header = get<0>(constructors);
    auto source = get<1>(constructors);

    return make_tuple(header(names, symbol_groups, false), source(names, symbol_groups, true));
}

}
