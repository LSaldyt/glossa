#pragma once
#include <memory>
#include <string>


std::string str(const std::string& s)
{
    return s;
}

template <typename T>
std::string str(const T& t);

class ObjectInterface 
{
public:
    virtual ~ObjectInterface() {}
    virtual std::string __str__() = 0;
    virtual std::unique_ptr<ObjectInterface> clone() const = 0;
};

template <typename T> class ObjectImpl: public ObjectInterface 
{
public:
    template <typename ...Ts> ObjectImpl( Ts&&...ts ) 
        : t( std::forward<Ts>(ts)... ) {}
    virtual std::string __str__() override { return str(t); };
    virtual std::unique_ptr<ObjectInterface> clone() const override
    {
        return std::make_unique<ObjectImpl<T>>( t ); // This is C++14
    }

private:
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
    std::string __str__() 
        { return p->__str__(); }
private:
    std::unique_ptr<ObjectInterface> p;
};

std::string str(Object& o)
{
    return o.__str__();
}

template <typename T>
std::string str(const T& t)
{
    return std::to_string(t);
}
