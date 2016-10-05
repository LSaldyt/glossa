#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <memory>

#include <exception>
#include <iostream>
#include <algorithm>

#include <sstream>
#include <fstream>

namespace tools
{

using std::vector;
using std::unordered_map;
using std::shared_ptr;
using std::make_shared;
using std::string;
using std::tuple;
using std::make_tuple;
using std::get;

using std::function;
using std::exception;

template <typename T> using shared_vec = vector<shared_ptr<T>>;

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
vector<T> slice(vector<T> original, int begin_offset, int end_offset)
{
    auto size = original.size();
    if (begin_offset > size or abs(end_offset) > size)
    {
        print("Could not slice vector of size ", size);
        throw exception();
    }
    return vector<T>(original.begin() + begin_offset, original.end() + end_offset);
}

template <typename T>
void concat(vector<T>& a, const vector<T>& b)
{
    a.insert(a.end(), b.begin(), b.end());
}

template <typename T>
void sortBy(vector<T> data, auto predicate)
{
    sort(data.begin(), data.end(), predicate);
}

}
