# Syntax

### The `Syntax` Module is for defining language constructs, such as `integers` or `strings`, but also for defining higher level concepts like `binary expressions`

##### Alongside definitions for `Syntax`, there will also be `SymbolGenerators`, which will take `Tokens` and return the corresponding language constructs.
##### Ex: ("2000", "number") -> (Integer(std::stoi("2000"))) -> Integer(2000)
