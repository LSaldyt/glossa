#include "dictionary.hpp"

namespace syntax
{

Dictionary::Dictionary(vector<vector<shared_ptr<Symbol>>> symbol_groups)
{
    for (int i = 0; i < symbol_groups[0].size(); i+=2)
    {
        keys.push_back(symbol_groups[0][i]);
        values.push_back(symbol_groups[0][i+1]);
    }
}

string Dictionary::source(unordered_set<string>& names)
{
    string kv_pairs = "";
    for (int i =0; i < keys.size(); i++)
    { 
        kv_pairs += "{";
        kv_pairs += keys[i]->source(names);
        kv_pairs += ", ";
        kv_pairs += values[i]->source(names);
        kv_pairs += "}";
        if (i+1 != keys.size()) //If not on last iteration
        {
            kv_pairs += ",\n";
        }
    }
    return "std::unordered_map<decltype(" + keys[0]->source(names) + "), decltype(" + values[0]->source(names) + ")>{ \n" + kv_pairs + "\n }";
}

}
