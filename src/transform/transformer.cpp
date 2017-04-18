#include "../syntax/symbols/export.hpp"
#include "transformer.hpp"

namespace transform 
{
ElementConstructorCreator<vector<string>> ec_creator = [](string s)
{
    ElementConstructor<vector<string>> ec;
    ec = [s](unordered_set<string>& names,
              MultiSymbolTable& ms_table,
              string filename,
              vector<string>& definitions,
              int nesting,
              OutputManager logger)
    {
        vector<string> terms = lex::seperate(s, {make_tuple(" ", false)});
        return terms;
    };
    return ec;
};

Transformer::Transformer(vector<string> transformer_files, string directory)
{
    for (auto file : transformer_files)
    {
        auto content = readFile(directory + file);
        auto constructor = generateTransformConstructor<vector<string>>(content,
                ec_creator
                );
        transformation_map[file] = constructor;
    }
}

Transformer::Transformer()
{
}

void Transformer::operator()(IdentifiedGroups& identified_groups)
{
    for (auto& id_group : identified_groups)
    {
        auto& tag      = get<0>(id_group);
        auto& ms_table = get<1>(id_group);
        _transform(tag, ms_table);
    }
}

void Transformer::_keyword_transform(vector<string>& terms, 
                                     string& otag, 
                                     MultiSymbolTable& oms_table,
                                     RegisterMap& register_map)
{
    assert(not terms.empty());
    auto keyword = terms[0];

    if (keyword == "createreg")
    {
        assert(terms.size() == 2);
        register_map[terms[1]] = make_tuple("", MultiSymbolTable());
        return;
    }

    MultiSymbolTable& ms_table = oms_table;
    string&           tag      = otag;
    if (keyword == "reg")
    {
        assert(terms.size() > 2);
        auto reg_name = terms[1];
        err_if(not contains(register_map, reg_name), "Register " + reg_name + " not found");
        auto& reg_tuple = register_map[reg_name];
        terms = slice(terms, 2);
        tag      = get<0>(reg_tuple);
        ms_table = get<1>(reg_tuple);
    }

    keyword = terms[0];
    if (contains(keyword, "transfer"))
    {
        assert(terms.size() == 4);

        auto reg_name = terms[1];
        err_if(not contains(register_map, reg_name), "Register " + reg_name + " not found");
        auto a = terms[2];
        auto b = terms[3];
        err_if(not contains(oms_table, a), a + " not in original table");
        auto& reg_ms_table = get<1>(register_map[reg_name]);

        if (contains(keyword, "append"))
        {
            assert(contains(reg_ms_table, b));
            concat(reg_ms_table[b], oms_table[a]);
        }
        else
        {
            reg_ms_table[b] = oms_table[a];
        }
    }
    else if (contains(keyword, "add") or contains(keyword, "append"))
    {
        assert(terms.size() == 4);
        auto creator = syntax::generatorMap.at(terms[2]);
        auto symbol  = creator({terms[3]});
        if (contains(keyword, "add") or not contains(ms_table, terms[1]))
        {
            assert(not contains(ms_table, terms[1])); // If "add" branch
            ms_table[terms[1]] = vector<shared_ptr<Symbol>>({symbol});
        }
        else
        {
            ms_table[terms[1]].push_back(symbol);
        }
    }
    else if (contains(keyword, "copy") or contains(keyword, "move"))
    {
        assert(terms.size() == 3);
        auto a = terms[1];
        auto b = terms[2];
        assert(contains(ms_table, a));
        if (contains(keyword, "append"))
        {
            assert(contains(ms_table, b));
            concat(ms_table[b], ms_table[a]);
        }
        else
        {
            ms_table[b] = ms_table[a];
        }
        if (contains(keyword, "move"))
        {
            ms_table.erase(a);
        }
    }
    else if (contains(keyword, "retag"))
    {
        assert(terms.size() == 2);
        tag = terms[1];
    }
    else if (contains(keyword, "pushback"))
    {
        assert(terms.size() == 2);
        auto reg_name = terms[1];
        auto key      = terms[2];
        err_if(not contains(register_map, reg_name), "Register " + reg_name + " not found");
        auto& reg_tuple    = register_map[reg_name];
        auto& reg_tag      = get<0>(reg_tuple);
        auto& reg_ms_table = get<1>(reg_tuple);
        auto symbol = make_shared<MultiSymbol>(MultiSymbol(reg_tag, reg_ms_table));
        if (contains(keyword, "override") or not contains(oms_table, key))
        {
            oms_table[key] = vector<shared_ptr<Symbol>>({symbol});
        }
        else
        {
            oms_table[key].push_back(symbol);
        }
        // Reset
        reg_tag = "";
        reg_ms_table = MultiSymbolTable();
    }
    else if (contains(keyword, "delete"))
    {
        ms_table.erase(terms[1]);
    }
    else
    {
        throw named_exception("Invalid keyword transform: " + keyword);
    }
}

void Transformer::_transform(string& tag, MultiSymbolTable& ms_table)
{
    for (auto kv : transformation_map)
    {
        if (kv.first == tag)
        {
            RegisterMap reg_map;
            print("Transforming " + tag);
            unordered_set<string> names;
            auto keyword_transforms = kv.second(names, 
                                                ms_table, 
                                                "none"); 
            for (auto terms : keyword_transforms)
            {
                _keyword_transform(terms, tag, ms_table, reg_map);
            }
        }
    }
    for (auto& kv : ms_table)
    {
        for (auto& symbol : kv.second)
        {
            auto id_group  = symbol->to_id_group(); 
            auto& tag      = get<0>(id_group);
            auto& ms_table = get<1>(id_group);
            if (tag != "undefined")
            {
                _transform(tag, ms_table);
                symbol->modify_id_group(tag, ms_table);
            }
        }
    }
}

}
