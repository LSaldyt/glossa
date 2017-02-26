# Symbol Storage feature notes

### Decoupling and file structure

Language directory structure:

``` txt
languages/
    a/
        grammar (single file)
        lex     (directory)
```


### Grammar file further improvments

// CPython function definition, for reference
funcdef: 'def' NAME parameters ['->' test] ':' suite
// End CPython function definition

// function grammar file
``` python
funcdef: `keyword def` `$name$ identifier wildcard` `$args$ link parameters` `punctuator :` `$body$ many link statement` 
```
// end function grammar file

Instead of using raw inOrder, a new function could be defined:
parse tokens -> result consumed remaining symboldict

In the process, it would be nice to borrow some other CPython grammar features, most notable being simplified anyOf syntax:
`(a | b | c)` = `anyOf link a link b link c`
many syntax:
`stmt*` = `many link statement`
etc...
 
### AST Transformation feature

Temporarily disabled
