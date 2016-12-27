# Progtran

Progran is a simple transpiler, based on the principles of formal grammars. The program uses a database of grammar files to convert between several programming languages (Currently Haskell, Python, and C++). 

### Demonstrations

To run demonstrations:
- Download Progtran
- Navigate to Progtran's home directory
- Ensure g++/gcc, Python and cmake are installed
- Install Haskell/ghc if you plan to run the Haskell related demos
``` python
# Python -> C++ quicksort demo (default)
./demo.py
# View oher available demos:
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
