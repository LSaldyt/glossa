#include "generator.hpp"
#include "../syntax/symbols/export.hpp"

namespace gen 
{

/**
 * Constructs a generator
 * @param filenames Construction files
 * @param directory Directory that construction files reside in
 */
Generator::Generator(vector<string> filenames, string directory)
{
    readStructureFile(directory + "file");
    for (auto filename : filenames)
    {
        print("Loading constructor " + filename);
        construction_map[filename] = readConstructor(directory + filename);
    }
    ec_creator = [this](string s){ return this->generateElementConstructor(s);}; 
    vector<string> default_body = {"branch contains val", "$val$", "end"};
    default_constructor = Constructor<string>(
                                generateBranch<string>(default_body, ec_creator),
                                {}
                                );
}

/**
 * Function for creating a file template
 * @param filename File containing the file template description
 */
void Generator::readStructureFile(string filename)
{
    auto content = readFile(filename);
    string last_type = "none";
    string last_extension = "none";
    vector<string> default_content;

    for (auto line : content)
    {
        auto terms = lex::seperate(line, {make_tuple(" ", false)});
        if (not terms.empty())
        {
            if (terms[0] == "file")
            {
                if (last_type != "none")
                {
                    file_constructors.push_back(make_tuple(last_type, FileConstructor(last_extension, default_content)));
                    default_content.clear();
                }
                assert(terms.size() == 3);
                last_type      = terms[1];
                last_extension = terms[2];
            }
            else
            {
                default_content.push_back(line);
            }
        }
    }
    if (last_type != "none")
    {
        file_constructors.push_back(make_tuple(last_type, FileConstructor(last_extension, default_content)));
        default_content.clear();
    }
}

/**
 * Builds a constructor for a particular syntax element
 * @param filename File containing the constructor description
 * @return vector of annotated file constructors (i.e. {("header", Constructor), ("source", Constructor)}
 */
vector<tuple<string, Constructor<string>>> Generator::readConstructor(string filename)
{
    auto content    = readFile(filename);
    ElementConstructorCreator<string> ec_creator = [this](string s){ return this->generateElementConstructor(s);}; 
    return generateConstructor<string>(content, file_constructors, ec_creator);
}


/**
 * High level function for generating source code for a particular langauge
 * @param names Namespace
 * @param symbol_groups 2D array of symbols
 * @param symbol_type High level symbol name to be created
 * @param filename Name of file to be created
 * @param nesting Indentation level
 * @return Vector of files 
 */
vector<tuple<string, string, vector<string>>> Generator::operator()(unordered_set<string>& names, 
                                                                    MultiSymbolTable&      ms_table, 
                                                                    string                 symbol_type, 
                                                                    string                 filename,
                                                                    int                    nesting,
                                                                    OutputManager          logger)
{
    logger.log("Running generator for " + symbol_type, 2);
    vector<tuple<string, string, vector<string>>> files;
    unordered_set<string> added_names;
    vector<tuple<string, Constructor<string>>> constructors;
    if (not contains(construction_map, symbol_type))
    {
        err_if(ms_table.size() != 1, "\"" + symbol_type + "\" is not in the construction map and cannot be built using a default constructor (id)");
        for (auto fc : file_constructors)
        {
            auto filetype = get<0>(fc);
            constructors.push_back(make_tuple(filetype, 
                        default_constructor
                            ));
        }
    }
    else
    {
        constructors = construction_map[symbol_type];
    }
    for (auto t : constructors)
    {
        auto type        = get<0>(t);
        auto constructor = get<1>(t);
        unordered_set<string> local_names(names);

        string extension;
        vector<string> default_content;
        for (auto fc : file_constructors)
        {
            if (get<0>(fc) == type)
            {
                if (filename != "none")
                {
                    for (auto line : get<1>(fc).default_content)
                    {
                        default_content.push_back(format(filename, line));
                    }
                }
                extension = get<1>(fc).extension;
                break;
            }
        } 
        auto constructed = constructor(local_names, ms_table, type, nesting, logger);
        concat(default_content, constructed);
        added_names.insert(local_names.begin(), local_names.end());
        files.push_back(make_tuple(type, filename + extension, default_content));
    }
    names.insert(added_names.begin(), added_names.end());
    return files;
}

/**
 * Builds a line constructor from a line in a constructor file
 * @param line Line in constructor file
 * @return ElementConstructor<string>, see TypeDef
 */
ElementConstructor<string> Generator::generateElementConstructor(string line)
{
    auto terms = lex::seperate(line, {make_tuple("`", true)}, {});
    return [terms, line, this](unordered_set<string>& names, 
                               MultiSymbolTable& ms_table, 
                               string filetype, 
                               vector<string>& definitions, 
                               int nesting, 
                               OutputManager logger)
    {
        string representation;
        if (terms.size() == 1)
        {
            auto to_format = lex::seperate(line, {make_tuple("$", true)}, {}); // Symbols to be replaced are surrounded in "$"
            bool formatting_symbol = false;
            for (auto t : to_format)
            {
                if (t == "$")
                {
                    formatting_symbol = !formatting_symbol;
                }
                else
                {
                    if (formatting_symbol)
                    {
                        representation += formatSymbol(t, names, ms_table, filetype, definitions);
                    }
                    else
                    {
                        representation += t; // Evenly numbered terms do not need to be formatted
                    }
                }
            }
        }
        else
        {
            bool special_formatting = false;
            for (auto t : terms)
            {
                if (t == "`")
                {
                    special_formatting = !special_formatting;
                }
                else if (special_formatting)
                {
                    auto slc = generateSpecialElementConstructor(t);
                    representation += slc(names, ms_table, filetype, definitions, nesting, logger);
                }
                else
                {
                    auto lc = generateElementConstructor(t);
                    representation += lc(names, ms_table, filetype, definitions, nesting, logger);
                }
            }
        }
        replaceAll(representation, "EMPTY",   "");
        replaceAll(representation, "SPACE",   " ");
        replaceAll(representation, "NEWLINE", "\n");
        replaceAll(representation, "INDENT",  repeatString("    ", nesting));
        return representation;
    };
}

/**
 * Builds a backtick seperated line constructor
 * @param line content inside of backticks
 * @return ElementConstructor<string>, see typedef
 */
ElementConstructor<string> Generator::generateSpecialElementConstructor(string line)
{
    auto terms = lex::seperate(line, {make_tuple(" ", false)});
    return [terms, line, this](unordered_set<string>& names, 
                               MultiSymbolTable& ms_table, 
                               string filetype, 
                               vector<string>& definitions, 
                               int nesting, 
                               OutputManager logger)
    {
        string representation = "";
        if (not terms.empty())
        {
            auto keyword = terms[0];
            if (keyword == "sep")
            {
                assert(terms.size() == 3 or terms.size() == 4 or terms.size() == 5);
                assert(contains(ms_table, terms[2]));
                auto symbols = ms_table[terms[2]];
                string formatter = "@";
                if (terms.size() > 3)
                {
                    formatter = terms[3];
                }
                representation += sepWith(*this, symbols, names, filetype, terms[1], formatter, nesting);
            }
            else if (keyword == "block") // e.g. block body @;
            {
                assert(terms.size() == 2 or terms.size() == 3);
                //print(terms[1]);
                assert(contains(ms_table, terms[1]));
                auto symbols = ms_table[terms[1]];
                string formatter = "@";
                if (terms.size() > 2)
                {
                    formatter = terms[2];
                }
                auto block = sepWith(*this, symbols, names, filetype, "\n", formatter, nesting + 1); // The only place where nesting increases
                auto block_terms = lex::seperate(block, {make_tuple("\n", true)}, {});
                representation += repeatString("    ", nesting);
                for (auto term : block_terms)
                {
                    if (term == "\n")
                    {
                        representation += term + repeatString("    ", nesting);
                    }
                    else
                    {
                        representation += term;
                    }
                }
            }
            else
            {
                throw named_exception("Unknown special line constructor: " + line);
            }
        }
        return representation;
    };
}

/**
 * Function for retrieving/building a symbol from storage
 * @param s The user-defined name for the symbol
 * @param names Namespace
 * @param storage Dual dictionaries containing symbol types
 * @param filetype The target filetype to generate for
 * @param definitions Defined names
 * @return Formatted string representing a stored symbol
 */
string Generator::formatSymbol (string s, unordered_set<string>& names, MultiSymbolTable& ms_table, string filetype, vector<string>& definitions)
{
    err_if(not contains(ms_table, s), s + " is not in the symbol table");
    auto ms_group = ms_table[s];         
    assert(ms_group.size() == 1);

    auto symbol         = ms_group[0];
    auto representation = symbol->representation(*this, names, filetype);
    auto new_name       = symbol->name();
    if (new_name != "none" and contains(definitions, s)) 
    {
        if (contains(names, new_name))
        {
            //print("Name " + new_name + " is already defined");
        }
        else
        {
            //print("Adding name: " + new_name);
            names.insert(new_name);
        }
    }
    return representation;
}

}
