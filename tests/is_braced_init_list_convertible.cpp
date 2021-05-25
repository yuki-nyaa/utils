#include"../utils.hpp"
#include<initializer_list>

struct Foo1{
    Foo1()=default;
};

struct Foo2{
    explicit Foo2()=default;
};

struct Foo3{
    explicit Foo3(int,double,Foo1);
};

struct Foo4{
    Foo4(std::initializer_list<int>);
};

struct Foo5{ // Note: this is an aggregate.
    int a;
    const char* b;
};

struct Foo6{ // Note: this is an aggregate.
    Foo2 foo2;
};

struct Foo7{
    Foo7()=default;
    Foo7(const Foo7&)=default;
    Foo7(double);
};

struct Foo8{
    Foo8(long);
    explicit Foo8(long long);
};

int main(){
    using namespace yuki;
    static_assert(is_braced_init_list_convertible_v<Foo1>);
    static_assert(!is_braced_init_list_convertible_v<Foo2>);
    static_assert(!is_braced_init_list_convertible_v<Foo3,int,double,Foo1>);
    static_assert(is_braced_init_list_convertible_v<Foo4,int,int,int>);
    static_assert(is_braced_init_list_convertible_v<Foo4,short,bool,int>);
    static_assert(!is_braced_init_list_convertible_v<Foo4,int,long long>); // Narrowing conversion not allowed.
    static_assert(is_braced_init_list_convertible_v<Foo5,int,const char*>);
    static_assert(!is_braced_init_list_convertible_v<Foo6>); // The default-ctor of `Foo6' is non-explicit, but aggregate initializtion does not call the default-ctor at all.
    static_assert(is_braced_init_list_convertible_v<Foo7[100],Foo7,Foo7,double,Foo7>);

    // Note: `is_braced_init_list_convertible<To,From>` is different from `std::is_convertible<From,To>`.
    static_assert(is_braced_init_list_convertible_v<Foo5,int>);
    static_assert(!std::is_convertible_v<int,Foo5>);
    // Even if you don't consider the obvious counterexamples of narrowing conversions and aggregates, they are still different.
    static_assert(!is_braced_init_list_convertible_v<Foo8,int>);
    static_assert(std::is_convertible_v<int,Foo8>);
    // This is because with the copy-list-initialization syntax, all ctors, including the explicit ones, participate in overload resolution, while in plain copy-initialization, explicit ctors are not considered at all. The copy-list-initialization only prohibits an explicit ctor being the best match.
}