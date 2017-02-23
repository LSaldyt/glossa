# Symbol Storage feature notes

When language features are parsed, a SymbolMatrix hold all of their elements, adressable by index.
Since this is largely a human interface, a dictionary, i.e. a SymbolStorage, will make the elements adressable by name.

This entails changes to grammar files, and removal of symbol storage creation from constructor and transformer files
Only one symbol storage will be created as soon as the language element is parsed

Grammar files dedicate the last line of the file to creating a matrix by line indices.
Instead, grammar files might name each line, i.e

// function grammar file
keyword def
identifier <- identifier wildcard
punctuator (
args <- sep , identifier wildcard
punctuator )
punctuator :
body <- many link statement
keyword end # From annotation step
// end function grammar file

Where a parsed function would return a dictionary of the form:
``` python
{
    'identifier' : 'f',
    'args' : ['x', 'y'],
    'body' : ['return x * y']
}
```

Alternatively, if grammar files are going to be changed to mimic CPython's grammar files, the new structure could look more like:

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

This also means rewriting the AST-transform feature....
