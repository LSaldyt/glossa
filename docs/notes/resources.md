[Diagram](https://www.google.com/url?sa=i&rct=j&q=&esrc=s&source=images&cd=&cad=rja&uact=8&ved=0ahUKEwjLz82vjt7OAhWJ6iYKHXBnAZ8QjRwIBw&url=http%3A%2F%2Fjava.wikia.com%2Fwiki%2FFile%3ACompiler.gif&psig=AFQjCNGyEaJuXzczW5IfdC-0hcy2lTRahw&ust=1472267601830781)

[Diagram2](http://courses.cs.vt.edu/cs1104/Compilers/compiler.gif)

Essentially, my model is this:

Source Code -> `IO` -> Terms -> `Lex` -> Tokens -> `Parse` -> AST -> `Gen` -> Target Code

The Match module exists to do string matching without regexes (readability at the cost of ease of use?)
The Types module defines Token types used in post-lexing and pre-parsing phases
The Syntax module defines both lower-level and higher-level symbolic types. Lower level symbols (ie Int() or Operator()) are generated during lexing (creation of *Symbolic* tokens). Higher level types (Statements, ie Function() or Assignment()) are generated during parsing, and also define their own code representations. This will change.

Experimental:
The Grammar module exists to simplify parsing (remove boilerplate code so that changing parsers is much simpler)

Future:
A true Gen module is still needed, since all code Gen is done by the Higher-Level symbols (Statements) themselves
