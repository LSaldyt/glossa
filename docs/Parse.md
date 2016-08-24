# Parsing

Parsing converts a vector of symbolic tokens into an Abstract Syntax Tree, which is represented by a vector of pointers to Statements, which are higher-level symbolic types.

Some examples of statement types are:
  - Assignment
  - Function definition
  - Function invocation

These are composed of Lower-level symbols and other statements, for example, a function definition:

```
Function:
  Name      (Identifier)
  Arguments (Identifiers)
  Body      (Statements)
  Return    (Expression)
```

Easily forms a tree, since other Functions can be defined in its body

Other symbolic types, like `Expression`, form trees of lower level Symbols:

```
Expression:
  Value
  Many((Operator, Value))
```

Parsing works by matching many symbols in a row. For example, in terms of parser combinators, expression building would look like:

```
inOrder({Value, many(inOrder({Operator, Value}))})
```

Which is almost exactly the definition above


