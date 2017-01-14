#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include <exception>
#include <iostream>
#include <algorithm>

#include <sstream>
#include <fstream>

#include <assert.h>
#include <sys/time.h> // To keep constant frametime
#include <unistd.h>

using namespace std::string_literals;

/**
 * Collection of standard-library like functions used throughout the compiler
 */
namespace tools
{

using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::shared_ptr;
using std::make_shared;
using std::string;
using std::tuple;
using std::make_tuple;
using std::get;

using std::function;
using std::exception;

template <typename T> using shared_vec = vector<shared_ptr<T>>;

/// Allows quick failure with traceback and annotation
class named_exception : public exception
{
public:
    named_exception(std::string set_name);
    virtual const char* what() const throw();
private:
    std::string name;
};

/// Abstract IO
std::vector<std::string> readFile(string filename);
/// Abstract IO
void writeFile(vector<string> content, string filename);

/// Abstract IO
template <typename T>
void print(T t) {
    std::cout << t << "\n";
}

/// Abstract IO
template <typename T, typename... Args>
void print(T first, Args... args) {
    print(first);
    print(args...);
    std::cout << "\n";
}

/** 
 * Resizes a vector using iterator based constructors
 */
template <typename T>
vector<T> slice(vector<T> original, int begin_offset=0, int end_offset=0)
{
    auto size = original.size();
    assert(not (begin_offset > size));
    assert(not (abs(end_offset) > size));
    return vector<T>(original.begin() + begin_offset, original.end() + end_offset);
}

/// Common string manipulations made easy
string sliceString(string original, int begin_offset=0, int end_offset=0);

/**
 * Concatenates two vectors of the same type
 */
template <typename T>
void concat(vector<T>& a, const vector<T>& b)
{
    a.insert(a.end(), b.begin(), b.end());
}

/**
 * Shorthand for sorting any vector with a custom comparison function 
 */
template <typename T>
void sortBy(vector<T>& data, auto predicate)
{
    std::sort(data.begin(), data.end(), predicate);
}

/**
 * Shorthand for checking contents of common data structures
 */
template <typename T>
bool contains(const unordered_map<string, T>& data_structure, const string& key)
{
    return data_structure.find(key) != data_structure.end();
}

/**
 * Shorthand for checking contents of common data structures
 */
template <typename T>
bool contains(const vector<T>& vec, const T& val)
{
    return std::find(vec.begin(), vec.end(), val) != vec.end();
}

/**
 * Shorthand for checking contents of common data structures
 */
template <typename T>
bool contains(const unordered_set<T>& data_structure, const T& key)
{
    return data_structure.find(key) != data_structure.end();
}

/**
 * Common string manipulation, useful in code generation
 */
void replaceAll( string &s, const string &search, const string &replace );

/**
 * Common string manipulation, useful in code generation
 */
string repeatString(const string&s, int n);
unsigned long long getTime();
}
