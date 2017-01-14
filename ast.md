# Notes on AST improvements

Saving/Comparison of ASTs

How should an AST be saved?

```
MultiSymbol (main)
    MultiSymbol (statement)
        MultiSymbol (functioncall)
            Identifier (__py_main__)
```

Might become

```
main 
    functioncall
        __py_main__
    # other content of main body
```

which would be parsed into python as:

``` python
[
('main', [('functioncall', [__py_main__])])
]
```

Once in this form, ASTs can easily be compared
