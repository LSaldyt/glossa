#include "read.hpp"
#include "../syntax/symbols/export.hpp"

namespace gen
{

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
        return [identifier](unordered_set<string>& names, MultiSymbolTable& ms_table)
        {
            assert(contains(ms_table, identifier));
            auto ms_group = ms_table[identifier];
            assert(not ms_group.empty());
            string to_define = ms_group[0]->name();
            return contains(names, to_define); 
        };
    }
    else if (keyword == "equalTo")
    {
        assert(terms.size() == 3);
        return [terms](unordered_set<string>& names, MultiSymbolTable& ms_table)
        {
            auto id = terms[1];
            assert(contains(ms_table, terms[1]));
            auto ms_group = ms_table[id];
            assert(not ms_group.empty());
            auto name = ms_group[0]->name();
            return name == terms[2]; 
        };
    }
    else if (keyword == "empty")
    {
        assert(terms.size() == 2);
        return [terms](unordered_set<string>& names, MultiSymbolTable& ms_table)
        {
            auto id = terms[1];
            assert(contains(ms_table, id));
            return ms_table[id].empty();
        };
    }
    else if (keyword == "nonempty")
    {
        assert(terms.size() == 2);
        return [terms](unordered_set<string>& names, MultiSymbolTable& ms_table)
        {
            auto id = terms[1];
            print(id);
            assert(contains(ms_table, id));
            return not ms_table[id].empty();
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
        return [a, b](unordered_set<string>& names, MultiSymbolTable& ms_table)
        {
            return a(names, ms_table) and b(names, ms_table);
        };
    }
    else
    {
        throw named_exception("Constructor keyword " + keyword + " is not defined");
    }
}



}
