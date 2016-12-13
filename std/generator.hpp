#include <cstddef>
#include <iostream>
#include <limits>
#include <functional>
#include <assert.h>

#include "io.hpp"

template <typename value_type>
class Sequence 
    : public std::iterator<std::input_iterator_tag, value_type>
{
public:
    using transformation = std::function<value_type(value_type)>;
    Sequence(value_type initial, 
             value_type set_last, 
             transformation set_t) 
        : done  (false),
          value (initial),
          last  (set_last),
          t     (set_t)
    {
    }

    explicit          operator bool() const { return !done;  }
    value_type const& operator*()     const { return value;  }
    value_type const* operator->()    const { return &value; }

    Sequence& operator++() 
    {
        assert(!done);
        
        if (value != last)
        {
            value = t(value);
            return *this;
        }
        done = true;
        return *this;
    }

    Sequence operator++(int) 
    {
        Sequence const tmp(*this);
        ++*this;
        return tmp;
    }

    Sequence begin()
    {
        return Sequence(value, last, t);
    }

    Sequence end()
    {
        return Sequence(last, last, t);
    }

    bool operator!=(Sequence other)
    {
        return value != other.value;
    }

    explicit operator vector<value_type>() const
    {
        return vector<value_type>(begin(), end());
    }

private:
    bool       done;
    value_type value;
    value_type last;

    std::function<value_type(value_type)> t;
};

template <typename value_type>
Sequence<value_type> begin(Sequence<value_type>& s)
{
    return s.begin();
}

template <typename value_type>
Sequence<value_type> end(Sequence<value_type>& s)
{
    return s.end();
}


/*
template <typename value_type>
class ForSequence 
    : public std::iterator<std::input_iterator_tag, value_type>
{
public:
    using transformation = std::function<value_type(value_type)>;
    ForSequence(value_type initial, 
                transformation set_t) 
            : value (initial),
              t     (set_t)
    {
    }

    explicit          operator bool() const { return !done;  }
    value_type const& operator*()     const { return value;  }
    value_type const* operator->()    const { return &value; }

    ForSequence& operator++() 
    {
        value = t(value);
        return *this;
    }

    ForSequence operator++(int) 
    {
        Sequence const tmp(*this);
        ++*this;
        return tmp;
    }
private:
    value_type value;
    std::function<value_type(value_type)> t;
};




#include <iostream>
#include <algorithm>

namespace foo{
int i=0;

struct A
{
A()
{
std::generate(&v[0], &v[10], [&i](){  return ++i;} );
}

int v[10];
};

int *begin( A &v )
{
return &v.v[0];
}
int *last( A &v )
{
return &v.v[10];
}
} // namespace foo
*/
