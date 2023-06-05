#include<string>
#include<yuki/tmp.hpp>
#include<yuki/Pair.hpp>
#include<yuki/print.hpp>

template<bool flag>
auto foo() -> std::conditional_t<flag,yuki::Pair<int,double>,std::string> {
    YUKI_CONDITIONAL_RETURN(flag,YUKI_PROTECT({1,1}),"foo");
}

template<typename T>
constexpr int sad(){
    if constexpr(std::is_same_v<T,int>)
        return 100;
    else
        static_assert(yuki::static_assert_dummy<T>);
}

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
    // 1 foo
    yuki::print_space(stdout,foo<true>().first,foo<false>());

    printf("\n");


    static_assert(yuki::is_same_v<>);
    static_assert(yuki::is_same_v<int>);
    static_assert(yuki::is_same_v<int,int>);
    static_assert(yuki::is_same_v<int,int,int,int>);
    static_assert(!yuki::is_same_v<int,int,int,double>);

    static_assert(yuki::is_pairwise_different_v<>);
    static_assert(yuki::is_pairwise_different_v<int>);
    static_assert(yuki::is_pairwise_different_v<int,double>);
    static_assert(!yuki::is_pairwise_different_v<int,int>);
    static_assert(yuki::is_pairwise_different_v<char,signed char,unsigned char,short,int,long>);
    static_assert(!yuki::is_pairwise_different_v<char,signed char,unsigned char,short,char,long>);
    static_assert(!yuki::is_pairwise_different_v<char,signed char,unsigned char,short,int,char>);

    static_assert(yuki::is_any_of_v<int,double,double,int>);
    static_assert(!yuki::is_any_of_v<int,double,double,double>);
    static_assert(!yuki::is_any_of_v<int>);

    static_assert(yuki::is_unique_v<int>);
    static_assert(yuki::is_unique_v<int,double,double,int>);
    static_assert(!yuki::is_unique_v<double,int,double,double>);

    static_assert(yuki::i_th_v<4,size_t,5,2,4,7,8> ==8);
    static_assert(yuki::i_th_v<5,size_t,5,2,4,7,8> ==(size_t)-1);
    static_assert(yuki::int_seq_i_th_v<5,std::make_index_sequence<10>> ==5);


    static_assert(sad<int>()==100);
    //static_assert(sad<double>()!=100);


    using yuki::uint_auto_t;
    using yuki::int_auto_t;
    // Note: The particular number depends on the platform.
    // Note: On my platform `int` has the same size as `long`, so there is no test for `long`.
    static_assert(std::is_same_v<uint_auto_t<123>,unsigned char>);
    static_assert(std::is_same_v<uint_auto_t<1000>,unsigned short>);
    static_assert(std::is_same_v<uint_auto_t<65537>,unsigned int>);
    static_assert(std::is_same_v<uint_auto_t<4294967296>,unsigned long long>);

    static_assert(std::is_same_v<int_auto_t<-123>,signed char>);
    static_assert(std::is_same_v<int_auto_t<-1000>,short>);
    static_assert(std::is_same_v<int_auto_t<-32769>,int>);
    static_assert(std::is_same_v<int_auto_t<-2147483649>,long long>);

    static_assert(std::is_same_v<uint_auto_t<123,unsigned>,unsigned>);
    static_assert(std::is_same_v<uint_auto_t<1000,unsigned short>,unsigned short>);
    static_assert(std::is_same_v<uint_auto_t<65537,unsigned long long>,unsigned long long>);
    static_assert(std::is_same_v<uint_auto_t<4294967296,unsigned long>,unsigned long long>);

    static_assert(std::is_same_v<int_auto_t<-123,long>,long>);
    static_assert(std::is_same_v<int_auto_t<-1000>,short>);
    static_assert(std::is_same_v<int_auto_t<-32769>,int>);
    static_assert(std::is_same_v<int_auto_t<-2147483649>,long long>);


    using yuki::is_braced_init_list_convertible_v;
    static_assert(is_braced_init_list_convertible_v<Foo1>);
    static_assert(!is_braced_init_list_convertible_v<Foo2>);
    static_assert(!is_braced_init_list_convertible_v<Foo3,int,double,Foo1>);
    static_assert(is_braced_init_list_convertible_v<Foo4,int,int,int>);
    static_assert(is_braced_init_list_convertible_v<Foo4,short,bool,int>);
    static_assert(!is_braced_init_list_convertible_v<Foo4,int,long long>); // Narrowing conversion not allowed.
    static_assert(is_braced_init_list_convertible_v<Foo5,int,const char*>);
    static_assert(!is_braced_init_list_convertible_v<Foo6>); // The default ctor of `Foo6` is non-explicit, but aggregate initialization does not call the default ctor of `Foo6` at all! Instead it calls the default ctor of `Foo2` directly, which IS an explicit ctor. Therefore `Foo6` is not initializable from an empty braced-init-list.
    static_assert(is_braced_init_list_convertible_v<Foo7[100],Foo7,Foo7,double,Foo7>);

    // Note: `is_braced_init_list_convertible<To,From>` is different from `std::is_convertible<From,To>`.
    static_assert(is_braced_init_list_convertible_v<Foo5,int>);
    static_assert(!std::is_convertible_v<int,Foo5>);
    // Even if you don't consider the obvious counterexamples of narrowing conversions and aggregates, they are still different.
    static_assert(!is_braced_init_list_convertible_v<Foo8,int>);
    static_assert(std::is_convertible_v<int,Foo8>);
    //< This is because with the copy-list-initialization syntax, all ctors, including the explicit ones, participate in overload resolution, while in plain copy-initialization, explicit ctors are not considered at all. The copy-list-initialization only prohibits an explicit ctor being the best match.

    static_assert(std::is_same_v<yuki::find_type_t<int,int,char,char,char,int>,std::index_sequence<0,4>>);
    static_assert(std::is_same_v<yuki::find_type_t<int,char,char,char>,std::index_sequence<>>);
    static_assert(std::is_same_v<yuki::find_type_t<int>,std::index_sequence<>>);
}