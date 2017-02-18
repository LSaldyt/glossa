#pragma once
#include "constructor.hpp"
#include "fileconstructor.hpp"

namespace gen
{
    SymbolStorageGenerator generateSymbolStorageGenerator(vector<string> content);
    ConditionEvaluator     generateConditionEvaluator(vector<string> terms);

    /**
     * Create a branch from lines in a constructor file
     * @param content Lines of constructor file
     * @param symbol_storage_generator Function for creating symbol storage
     * @param ec_creator function defining how to build an element constructor
     * @return Branch<string> which will follow user-defined logic to build a syntax element
     */
    template <typename T>
    Branch<T> generateBranch(vector<string> content, 
                             SymbolStorageGenerator symbol_storage_generator,
                             ElementConstructorCreator<T> ec_creator)
    {
        vector<ElementConstructor<T>> line_constructors;
        vector<Branch<T>>             nested_branches;

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
                        nested_branches.push_back(Branch<T>(defaultBranch, line_constructors, {}));
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
        nested_branches.push_back(Branch<T>(defaultBranch, line_constructors, {}));
        line_constructors.clear();
        assert(nest_count == 0);

        return Branch<T>(defaultBranch, line_constructors, nested_branches);
    }


    template <typename T>
    vector<tuple<string, Constructor<T>>> generateConstructor(vector<string> content, 
            vector<tuple<string, FileConstructor>> file_constructors,
            ElementConstructorCreator<T> ec_creator)
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
        vector<tuple<string, Constructor<T>>> constructors;
        for (auto file_constructor : file_constructors)
        {
            tag = get<0>(file_constructor);
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
                auto constructor = Constructor<T>(symbol_storage_generator, 
                                                  generateBranch(body, 
                                                      symbol_storage_generator, 
                                                      ec_creator),
                                                      definitions);
                constructors.push_back(make_tuple(type, constructor));
                last_it = it;
            }
            type = tag;
        }
        auto body = vector<string>(last_it + 1, content.end());
        auto constructor = Constructor<T>(symbol_storage_generator, 
                                          generateBranch(body, 
                                              symbol_storage_generator, 
                                              ec_creator), 
                                          definitions);
        constructors.push_back(make_tuple(type, constructor));
        return constructors;
    }

}
