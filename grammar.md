# Grammar feature notes

### Grammar file further improvments

CPython function definition, for reference
```
funcdef: 'def' NAME parameters ['->' test] ':' suite
```

It would be nice to borrow some other CPython grammar features, most notable being simplified anyOf syntax:
`(a | b | c)` = `anyOf link a link b link c`
many syntax:
`stmt*` = `many link statement`
etc...

Current funcdef grammar file 
``` python
funcdef: `keyword def` `@identifier identifier wildcard` `@args link parameters` `punctuator :` `@body many link statement` 
```

Slightly improved:
``` python
funcdef: 'def' `@identifier identifier **` `@args parameters` ':' `@body *statement`

Grouped as:

['funcdef:', '\'def\'', '`@identifier identifier wildcard`'... ]
```

Seperate first by backticks, then by whitespace:

Text inside of backticks will be further whitespace seperated (it indicates an element that will be saved as part of the MultiSymbolTable)

Single words in single quotes will be parsed as subtypes

** will replace wildcard

* prefixing a type will replace many

If not surrounded in single quotes, and only a single term, will be parsed as a link statement
