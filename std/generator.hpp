#include <cstddef>
#include <iostream>
#include <limits>
#include <assert.h>

class PairSequence : public std::iterator<std::input_iterator_tag, std::pair<unsigned, unsigned>>
{
    // C++03
    typedef void (PairSequence::*BoolLike)();
    void non_comparable();
public:
    // C++11 (explicit aliases) 
    using iterator_category = std::input_iterator_tag;
    using value_type = std::pair<unsigned, unsigned>;
    using reference = value_type const&;
    using pointer = value_type const*;
    using difference_type = ptrdiff_t;

    PairSequence(): done(false) {}

    // C++11
    explicit operator bool() const { return !done; }

    // C++03
    // Safe Bool idiom
    operator BoolLike() const 
    {
        return done ? 0 : &PairSequence::non_comparable;
    }

    reference operator*()  const { return ij; }
    pointer   operator->() const { return &ij; }

    PairSequence& operator++() 
    {
        static unsigned const Max = std::numeric_limits<unsigned>::max();

        assert(!done);

        if (ij.second != Max) { ++ij.second; return *this; }
        if (ij.first != Max) { ij.second = 0; ++ij.first; return *this; }

        done = true;
        return *this;
    }

    PairSequence operator++(int) 
    {
        PairSequence const tmp(*this);
        ++*this;
        return tmp;
    }

private:
    bool done;
    value_type ij;
};
