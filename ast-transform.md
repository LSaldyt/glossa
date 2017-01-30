# AST transformations

In the compilation of high-level languages, language specific features in the AST should be converted to more common language features

For example, Haskell's pattern matching gets converted to a case statement
Python's with statement might become a try-finally

In general, higher-level language features are sugar for lower-lever ones

### Technicalities of transformations

Given a parsed type that is an element of the AST:

MultiSymbol (annotation):
    2D array of elements, addressable by index

Convert it to another type (or series of types), and reinsert it into the AST

For example, a print statement in python2:

MultiSymbol (print):
    Value ("Hello World")

If porting to python3, this gets converted to a functioncall:

MultiSymbol (functioncall):
    Identifier (print)
    Value      ("Hello World")

Which is then compilable by python3's constructor set.
This means that the python3 constructor set _does not need to_ include a print-specific constructor file

Ideally, transformation-description files will support the same branching that constructor files do

For example, a print statement without a newline character:

MutliSymbol (print):
    Value ("Hello World")
    Value (,)

```
val     = 0 0
newline = 1
branch nonempty newline
// Return:
// MultiSymbol(functioncall):
//     Identifier  (print)
//     Value       ("Hello World")
//     MultiSymbol (defaultarg)
//         Identifier (end)
//         Value      ('')
// 
elsebranch
// MultiSymbol (functioncall):
//     Identifier (print)
//     Value      ("Hello World")
end
```
