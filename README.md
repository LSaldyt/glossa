# Glossa 

Glossa is a simple multi-language source-to-source compiler. The program uses a database of description files to convert between several programming languages (Currently Python2, Python3, FORTRAN, and C++). 

Possible input languages are Python2, Python3, and FORTRAN.

Possible output languages are Python3 and C++.

Glossa also has many proof of concept languages:

Auta: DSL for automation, built on top of Python3

structured\_english: Structured language that compiles to Java. Used to show students the need for precision in language.

laypython: Proof of concept that a more verbose, english like version of Python3 could be built using Glossa.


Glossa has a haskell demo, but the Haskell language is no longer supported with the current version of the software. However, it easily could be, if the correct description files were written.

For more information about how Glossa works, view my [paper](paper.pdf) and [presentation](https://docs.google.com/presentation/d/1w7LSmaNJLec89L3Sbt80ifWkb7AFh7tvpgxqdahggYI/edit?usp=sharing).

### Demonstrations

To run demonstrations:
- g++, Python2/3, and cmake are required for main demos
- gfortran required for FORTRAN demos
- ghc required for Haskell demos

``` python
# Python -> C++ quicksort demo (default)
./demo.py
# View other available demos:
./demo.py --show
# Available demos by name
python3             : python3 -> cpp
class               : python3 -> cpp
porting             : python2 -> python3
auta                : auta -> python3
laypython           : laypython -> python3
haskell             : haskell -> cpp
natural_java        : structured_english -> java
fortran             : fortran -> cpp
text_prediction     : python2 -> python3
benchmark           : python3 -> cpp
integrate           : python3 -> cpp
pyfortran           : fortran -> python3
python2             : python2 -> cpp
```

Input code for each demo is in `examples/demoname` where `demoname`, and generated code can be found in `examples/output/demoname_output/` where `demoname` is the name of the demonstration that was run (e.g. `python3`)

### Python -> Cpp example

``` python
def sort(array):
    less    = []
    equal   = []
    greater = []

    if len(array) <= 1:
        return array
    else:
        pivot = array[0]
        for x in array:
            if x < pivot:
                less.append(x)
            if x == pivot:
                equal.append(x)
            if x > pivot:
                greater.append(x)
        return sort(less) + equal + sort(greater)
```

``` cpp
// main.hpp
#include "../std/std.hpp"
template <typename T_array>
auto sort (T_array array)
{
    auto less = std::vector<Object>({});
    auto equal = std::vector<Object>({});
    auto greater = std::vector<Object>({});
    if (len(array) <= 1)
    {
        return array;
    }
    else
    {
        auto pivot = array[0];
        for (auto x : array)
        {
            if (x < pivot)
            {
                less.push_back(x);
            }
            if (x == pivot)
            {
                equal.push_back(x);
            }
            if (x > pivot)
            {
                greater.push_back(x);
            }
        }
        return sort(less) + equal + sort(greater);
    }
}
```

### Documentation

To generate documentation, run `doxygen Doxyfile` in the home directory of Progtran

Then, navigate to `docs/doxygen`:

Open `index.html` in your browser

Alternatively, used the `./view_docs` script, which does this automatically.

If doxygen is not installed, use `sudo apt-get install doxygen`
