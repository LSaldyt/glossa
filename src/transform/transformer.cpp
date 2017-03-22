#include "../syntax/symbols/export.hpp"
#include "transformer.hpp"
/*

namespace transform 
{
ElementConstructorCreator<shared_ptr<Symbol>> ec_creator = [](string s)
{
    ElementConstructor<shared_ptr<Symbol>> ec;
    auto terms = lex::seperate(s, {make_tuple(" ", false)});
    assert(not terms.empty());
    if (terms.size() == 1)
    {
        auto keyword = terms[0];
        if (keyword == "newrow" or keyword == "endsymbol")
        {
            ec = [keyword](unordered_set<string>& names,
                      SymbolStorage& symbol_storage,
                      string filename,
                      vector<string>& definitions,
                      int nesting,
                      OutputManager logger)
            {
                return make_shared<SentinelSymbol>(SentinelSymbol("__" + keyword + "__"));
            };
        }
        else
        {
            ec = [keyword](unordered_set<string>& names,
                           SymbolStorage& symbol_storage,
                           string filename,
                           vector<string>& definitions,
                           int nesting,
                           OutputManager logger)
            {
                shared_ptr<Symbol> s;
                if (contains(get<0>(symbol_storage), keyword))
                {
                    s = get<0>(symbol_storage)[keyword];
                }
                else
                {
                    auto symbol_list = get<1>(symbol_storage)[keyword];
                    SymbolMatrix mx;
                    for (auto item : symbol_list)
                    {
                        mx.push_back(vector<shared_ptr<Symbol>>({item}));
                    }
                    s = make_shared<MultiSymbol>(MultiSymbol("__symbol_list__", mx));
                };
                return s;
            };
        }
    }
    else if (terms[0] == "name:")
    {
        auto name = terms[1];
        ec = [name](unordered_set<string>& names,
                  SymbolStorage& symbol_storage,
                  string filename,
                  vector<string>& definitions,
                  int nesting,
                  OutputManager logger)
        {
            return make_shared<SentinelSymbol>(SentinelSymbol("__name__", name));
        };
    }
    else if (terms[0] == "subsymbol:")
    {
        auto name = terms[1];
        ec = [name](unordered_set<string>& names,
                  SymbolStorage& symbol_storage,
                  string filename,
                  vector<string>& definitions,
                  int nesting,
                  OutputManager logger)
        {
            return make_shared<SentinelSymbol>(SentinelSymbol("__subsymbol__", name));
        };
    }
    else if (terms[0] == "literal:")
    {
        assert(terms.size() == 3);
        auto key = terms[1];
        auto val = terms[2];
        auto s = syntax::generatorMap.at(key)({val});
        ec = [s](unordered_set<string>& names,
                  SymbolStorage& symbol_storage,
                  string filename,
                  vector<string>& definitions,
                  int nesting,
                  OutputManager logger)
        {
            return s;
        };
    }
    else
    {
        print(s);
        throw std::exception();
    }
    return ec;
};

Transformer::Transformer(vector<string> transformer_files, string directory)
{
    for (auto file : transformer_files)
    {
        auto content = readFile(directory + file);
        auto constructor = generateTransformConstructor<shared_ptr<Symbol>>(content,
                ec_creator
                );
        transformation_map[file] = constructor;
    }
}

void Transformer::operator()(IdentifiedGroups& identified_groups)
{
    print("Modifying identifed groups");
    for (auto& id_group : identified_groups)
    {
        auto& tag = get<0>(id_group);
        auto& mx  = get<1>(id_group);
        transform(tag, mx);
    }
}

void Transformer::transform(string& tag, SymbolMatrix& symbol_matrix)
{
    for (auto kv : transformation_map)
    {
        if (kv.first == tag)
        {
            print("Transforming " + tag);
            unordered_set<string> names;
            SymbolMatrix new_mx;
            new_mx.push_back(vector<shared_ptr<Symbol>>());
            auto generated = kv.second(names, symbol_matrix, "transform:");
            for (auto s : generated)
            {
                auto s_tag = s->abstract();
                auto s_val = s->name();
                if (s_tag == "__name__")
                {
                    tag = s_val;
                }
                else if (s_tag == "__newrow__")
                {
                    new_mx.push_back(vector<shared_ptr<Symbol>>());
                }
                else if (s_tag[0] == '_')
                {
                    print(s_tag);
                    throw std::exception();
                }
                else
                {
                    auto id_group = s->to_id_group();
                    auto id_tag = get<0>(id_group);
                    auto id_mx  = get<1>(id_group);
                    if (id_tag == "__symbol_list__")
                    {
                        for (auto row : id_mx)
                        {
                            for (auto item : row)
                            {
                                new_mx.back().push_back(item);
                            }
                        }
                    }
                    else
                    {
                        new_mx.back().push_back(s);
                    }
                }
            }
            symbol_matrix = new_mx;
        }
    }
    for (auto& row : symbol_matrix)
    {
        for (auto& symbol : row)
        {
            auto id_group = symbol->to_id_group(); 
            auto& tag = get<0>(id_group);
            auto& mx  = get<1>(id_group);
            if (tag != "undefined")
            {
                transform(tag, mx);
                symbol->modify_id_group(tag, mx);
            }
        }
    }
}

}
*/
