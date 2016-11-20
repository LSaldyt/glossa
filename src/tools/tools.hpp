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

using namespace std::string_literals;

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

class named_exception : public exception
{
public:
    named_exception(std::string set_name);
    virtual const char* what() const throw();
private:
    std::string name;
};

std::vector<std::string> readFile(string filename);
void writeFile(vector<string> content, string filename);

template <typename T>
void print(T t) {
    std::cout << t << "\n";
}

template <typename T, typename... Args>
void print(T first, Args... args) {
    print(first);
    print(args...);
    std::cout << "\n";
}

template <typename T>
vector<T> slice(vector<T> original, int begin_offset=0, int end_offset=0)
{
    auto size = original.size();
    if (begin_offset > size or abs(end_offset) > size)
    {
        print("Could not slice vector of size ", size);
        throw exception();
    }
    return vector<T>(original.begin() + begin_offset, original.end() + end_offset);
}

string sliceString(string original, int begin_offset=0, int end_offset=0);

template <typename T>
void concat(vector<T>& a, const vector<T>& b)
{
    a.insert(a.end(), b.begin(), b.end());
}

template <typename T>
void sortBy(vector<T>& data, auto predicate)
{
    std::sort(data.begin(), data.end(), predicate);
}

template <typename T>
bool contains(const unordered_map<string, T>& data_structure, const string& key)
{
    return data_structure.find(key) != data_structure.end();
}

template <typename T>
bool contains(const vector<T>& vec, const T& val)
{
    return std::find(vec.begin(), vec.end(), val) != vec.end();
}

}
