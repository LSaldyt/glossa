## Things to address in code generation

Biggest problem: lots of hard-coded string literals

Second biggest: little code re-use, lots of duplicated code, especially for seperate header/source generation functions

End goal: customizable generation, similar to grammar files for parsing

Consider the example of generating an assignment statement in c++:

given the _identifier_, and a `shared_ptr<Symbol>` for the _value_:

if _identifier_ in namespace:
return _identifier_ = _value_
else:
return auto _identifier_ = _value_

Even in this simple example, the code generation step has pieces of logic thrown in

An example of what a construction file for assignment statements might look like:

```
header # empty
source
branch _identifier_ defined
_identifier_ = _value_
branch else
auto _identifier_ = _value_
```

where branch/other keywords are interpreted as logic instructions, and _identifier_ and _value_ are replaced with their contents (i.e. 'x' and 42)

For a more complicated language construct, such as a function:

```
header # defined entirely in header, because of templates
branch _identifier_ is main

int main ( int argc , char ** argv)
{
indented generate _body_
}

branch else

templateline arglist 
auto _identifier_ ( arglist ) 
{
indented generate _body_
}

source
# empty
```

How would parsing/generating things from these files work?
Firstly, there would be a defined file structure:

```
header
# branches, header representation
source
# branches, header representation
```

### Pseudocode:

```
for line in file:
    if line is "header":
        # begin header
    if line is "source":
        # begin source
    tokens = lex/sep(line)
    if tokens[0] is "branch"
        # save branch condition, begin new branch
    else
        # continue to add to current branch/either header or source
```

For more complex language constructs, eg class:

```
template < typename t_type, typename t_inheritance >
class __dog__ : public t_inheritance
{
public:
t_type type;
__dog__(t_type set_type, t_inheritance set_inheritance) : type(set_type), t_inheritance(set_inheritance) {}

};
template < typename t_set_name, typename t_set_type>
auto dog(t_set_name set_name, t_set_type set_type)
{
    return __dog__<decltype(set_type), decltype(animal(set_name, "bark"s))>(set_type, animal(set_name, "bark"s));
};
```

```
header
branch _inheritance_ is not none or notempty _constructor_args_
template < 
branch _inheritance_ is not none
typename T_inheritance
endbranch
>
endbranch
class _name_ 
branch _inheritance_ is not none
: public T_inheritance
endbranch
{
public:
genmembers _members_
_mangledname_ (arglist _members_) : initializers _members_ {}
};

templateline _members_
auto _name_ ( arglist _members_ )
{
return _mangledname_< decllines _members_ > ( _constructor_args_ );
}
source
# No class source (templates)
```
