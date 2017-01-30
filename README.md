# Progtran

Progran is a simple transpiler, based on the principles of formal grammars. The program uses a database of grammar files to convert between several programming languages (Currently Haskell, Python, and C++). 

<object data="description.pdf" type="application/pdf" width="700px" height="700px">
    <embed src="description">
        This browser does not support PDFs. Please download the PDF to view it: <a href="description.pdf">Download PDF</a>.</p>
    </embed>
</object>

### Demonstrations

To run demonstrations:
- Download Progtran
- Navigate to Progtran's home directory
- Ensure g++/gcc, Python and cmake are installed
- Install Haskell/ghc if you plan to run the Haskell related demos
``` python
# Python -> C++ quicksort demo (default)
./demo.py
# View other available demos:
./demo.py --show
p_quicksort      python3 cpp
h_quicksort      haskell cpp
python           python3 cpp
haskell          haskell cpp
listcomp         python3 cpp
pattern_matching haskell cpp 
do               haskell cpp
# Choose one of the above demos:
./demo.py h_quicksort
# Output from Haskell -> C++ quicksort demo
#...
```

Input code for each demo is in `examples/demoname` where `demoname` is the name of the demonstration that was run (e.g. `Python`)
Generated code can be found in `examples/output/demoname_output/` where `demoname` is the name of the demonstration that was run (e.g. `Python`)

### Python -> Cpp example

Here are the sources from the main demo (python to cpp conversion)

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

def main():
    l = sort([3, 2, 12, 9, 4, 68, 17, 1, 2, 3, 4, 5, 6, 12, 9  , 8, 7, 6,5, 4, 743])

if __name__ == "__main__":
    main()
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
            ;
            if (x == pivot)
            {
                equal.push_back(x);
            }
            ;
            if (x > pivot)
            {
                greater.push_back(x);
            }
            ;
        };
        return sort(less) + equal + sort(greater);
    };
}
auto __py_main__ ()
{
    auto l = sort(std::vector<Object>({3,2,12,9,4,68,17,1,2,3,4,5,6,12,9,8,7,6,5,4,743}));
}

// main.cpp
#include "main.hpp"

int main(int argc, char ** argv)
{
    __py_main__();
}
```

### Documentation

To generate documentation, run `doxygen Doxyfile` in the home directory of Progtran

Then, navigate to `docs/doxygen`:

Use `firefox index.html` to open documentation in your browser

If doxygen is not installed, it can be by the command `sudo apt-get install doxygen`
