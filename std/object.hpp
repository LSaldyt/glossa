#pragma once
#include <memory>
#include <iostream>
#include <string>
#include <unordered_set>
#include <functional>

// http://stackoverflow.com/questions/18856824/ad-hoc-polymorphism-and-heterogeneous-containers-with-value-semantics

/*
std::string str(const std::string& s)
{
    return s;
}
*/

template <typename T>
std::string str(const T& t);

class Object;

class ObjectInterface 
{
public:
    virtual ~ObjectInterface() {}
    virtual std::string __str__() = 0;
    virtual std::unique_ptr<ObjectInterface> clone() const = 0;
    
    virtual bool __lt__(const std::unique_ptr<ObjectInterface>& other) const = 0;
    virtual bool __eq__(const std::unique_ptr<ObjectInterface>& other) const = 0; 
    virtual std::unique_ptr<ObjectInterface> __add__(const std::unique_ptr<ObjectInterface>& other) const = 0;
    virtual std::unique_ptr<ObjectInterface> __sub__(const std::unique_ptr<ObjectInterface>& other) const = 0;
    virtual std::unique_ptr<ObjectInterface> __div__(const std::unique_ptr<ObjectInterface>& other) const = 0;
    virtual std::unique_ptr<ObjectInterface> __mul__(const std::unique_ptr<ObjectInterface>& other) const = 0;

    virtual std::size_t __hash__() const = 0;
};

template <typename T> class ObjectImpl: public ObjectInterface 
{
public:
    template <typename ...Ts> ObjectImpl( Ts&&...ts ) 
        : t( std::forward<Ts>(ts)... ) {}

    virtual std::string __str__() override { return str(t); }

    virtual std::unique_ptr<ObjectInterface> clone() const override
    {
        return std::make_unique<ObjectImpl<T>>( t ); // This is C++14
    }

    virtual bool __lt__(const std::unique_ptr<ObjectInterface>& other) const override
    {
        auto other_impl = dynamic_cast<ObjectImpl<T>*>(other.get());
        return t < other_impl->t;
    }

    virtual bool __eq__(const std::unique_ptr<ObjectInterface>& other) const override
    {
        auto other_impl = dynamic_cast<ObjectImpl<T>*>(other.get());
        return t == other_impl->t;
    }

    virtual std::unique_ptr<ObjectInterface> __add__(const std::unique_ptr<ObjectInterface>& other) const override
    {
        auto other_impl = dynamic_cast<ObjectImpl<T>*>(other.get());
        return std::make_unique<ObjectImpl<T>>(t + other_impl->t); 
    }

    virtual std::unique_ptr<ObjectInterface> __sub__(const std::unique_ptr<ObjectInterface>& other) const override
    {
        auto other_impl = dynamic_cast<ObjectImpl<T>*>(other.get());
        return std::make_unique<ObjectImpl<T>>(t - other_impl->t); 
    }

    virtual std::unique_ptr<ObjectInterface> __div__(const std::unique_ptr<ObjectInterface>& other) const override
    {
        auto other_impl = dynamic_cast<ObjectImpl<T>*>(other.get());
        return std::make_unique<ObjectImpl<T>>(t / other_impl->t); 
    }

    virtual std::unique_ptr<ObjectInterface> __mul__(const std::unique_ptr<ObjectInterface>& other) const override
    {
        auto other_impl = dynamic_cast<ObjectImpl<T>*>(other.get());
        return std::make_unique<ObjectImpl<T>>(t * other_impl->t); 
    }

    virtual std::size_t __hash__() const override
    {
        return std::hash<T>()(t);
    }

    T t;
};


class Object
{
public:
    template <typename T> Object( T t )
        : p( std::make_unique<ObjectImpl<T>>( std::move(t) ) ) {}
    Object( const Object& other ) 
        : p( other.p->clone() ) {}
    Object( Object && other ) noexcept 
        : p( std::move(other.p) ) {}
    void swap( Object & other ) noexcept 
        { p.swap(other.p); }
    Object & operator=( Object other ) 
        { swap(other); }
    virtual std::string __str__() 
        { return p->__str__(); }
    bool __lt__ (const Object& other) const
        { return p->__lt__(other.p); }
    bool __eq__ (const Object& other) const
        { return p->__eq__(other.p); }

    Object __add__ (const Object& other) const
    {
        Object o = *this;
        o.p = p->__add__(other.p); 
        return o;
    }

    Object __sub__ (const Object& other) const
    {
        Object o = *this;
        o.p = p->__sub__(other.p); 
        return o;
    }

    Object __div__ (const Object& other) const
    {
        Object o = *this;
        o.p = p->__div__(other.p); 
        return o;
    }

    Object __mul__ (const Object& other) const
    {
        Object o = *this;
        o.p = p->__mul__(other.p); 
        return o;
    }

    Object& operator+= (const Object& other) 
    {
        p = p->__add__(other.p); 
        return *this;
    }
    Object& operator-= (const Object& other) 
    {
        p = p->__sub__(other.p); 
        return *this;
    }
    Object& operator/= (const Object& other) 
    {
        p = p->__div__(other.p); 
        return *this;
    }
    Object& operator*= (const Object& other)
    {
        p = p->__mul__(other.p); 
        return *this;
    }

    std::size_t __hash__() const
    {
        return p->__hash__();
    }

    template <typename T>
    explicit operator T() const 
    { 
        try
        {
            auto impl = dynamic_cast<ObjectImpl<T>*>(p.get());
            return impl->t;
        }
        catch(const std::exception& e)
        {
            std::cout << "Could not convert object" << std::endl;
        }
    }

private:
    std::unique_ptr<ObjectInterface> p;
};

namespace std
{
    string to_string(Object o);
}

template <typename T>
std::string str(const T& t)
{
    return std::to_string(t);
}

std::ostream& operator<<(std::ostream& os, Object& obj);

inline bool operator< (const Object& lhs, const Object& rhs){ return lhs.__lt__(rhs); }
inline bool operator> (const Object& lhs, const Object& rhs){ return rhs < lhs; }
inline bool operator<=(const Object& lhs, const Object& rhs){ return !(lhs > rhs); }
inline bool operator>=(const Object& lhs, const Object& rhs){ return !(lhs < rhs); }

inline bool operator==(const Object& lhs, const Object& rhs){ return lhs.__eq__(rhs); }
inline bool operator!=(const Object& lhs, const Object& rhs){ return !(lhs == rhs); }

Object operator+(Object lhs, const Object& rhs);
Object operator-(Object lhs, const Object& rhs);
Object operator/(Object lhs, const Object& rhs);
Object operator*(Object lhs, const Object& rhs);

namespace std 
{
template <>
struct hash<Object>
{
    std::size_t operator()(Object const& o) const
    {
        return o.__hash__();
    }
};

}
