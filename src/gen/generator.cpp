#include "generator.hpp"

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
    assert(contains(content, "header"s));
    assert(contains(content, "source"s));
    auto header_i = std::find(content.begin(), content.end(), "header");
    auto source_i = std::find(content.begin(), content.end(), "source");

    auto declarations = vector<string>(content.begin(), header_i);
    auto header       = vector<string>(header_i + 1, source_i);
    auto source       = vector<string>(source_i + 1, content.end());

    print("DECLARATIONS");
    auto symbol_storage_generator = generateSymbolStorageGenerator(declarations);

    print("HEADER");
    auto header_constructor = Constructor(symbol_storage_generator, generateBranch(header, symbol_storage_generator));

    print("SOURCE");
    auto source_constructor = Constructor(symbol_storage_generator, generateBranch(source, symbol_storage_generator));

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
                    addNestedBranch(if_body_start, if_body_end, if_body_start, false);
                }

                if (has_else)
                {
                    else_body_end = it;
                    addNestedBranch(else_body_start, else_body_end, if_body_start, true);
                }
            }
            else
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
    return [](SymbolStorage& storage){
        return "//empty :)";
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
        return [identifier](SymbolStorage& symbol_storage)
        {
            return contains(get<0>(symbol_storage), identifier) or
                   contains(get<1>(symbol_storage), identifier); 
        };
    }
    else
    {
        throw named_exception("Constructor keyword " + keyword + " is not defined");
    }
}

tuple<vector<string>, vector<string>> Generator::operator()(vector<vector<shared_ptr<Symbol>>>& symbol_groups, string filename)
{
    assert(contains(construction_map, filename));
    auto constructors = construction_map[filename]; 
    auto header = get<0>(constructors);
    auto source = get<1>(constructors);

    return make_tuple(header(symbol_groups), source(symbol_groups));
}

}
