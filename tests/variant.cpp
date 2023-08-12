#include<yuki/Variant.hpp>
#include<yuki/Pair.hpp>
#include<cstdio>
#include<string>
#include<optional>

using namespace yuki;
using namespace yuki::variant_impl_;

struct NT{
    int a;
    constexpr NT(int p=0) noexcept : a(p) {}
    constexpr NT(const NT& other) noexcept {a=other.a;}
    constexpr ~NT() noexcept {}
    constexpr NT& operator=(const NT&) noexcept {return *this;}
    constexpr NT& operator=(NT&&) noexcept {return *this;}
};

struct MyStr{
    const char* s;
    MyStr(const char* ss) noexcept : s(ss) {}
    ~MyStr() noexcept {fputs("MyStr::~MyStr()\n",stderr);}
};

struct ncp{
    constexpr ncp() noexcept = default;
    ncp(const ncp&)=delete;
};

// TEST
struct visitor{
    template<typename... Args>
    static constexpr auto operator()(int,Args...) {return 11;}
    template<typename... Args>
    static constexpr auto operator()(long long,Args...) {return 22;}
    template<typename... Args>
    static constexpr auto operator()(const char*,Args...) {return 44;}
};

//constexpr Variant<int,long long,const char*,bool> v0(std::in_place_index<2>,"bcd");
constexpr Variant<int,long long,const char*,bool,char*,bool> gv0(std::in_place_index<5>,false);
static_assert(visit(visitor{},gv0,gv0,gv0)==11);

static_assert(std::is_same_v<decltype(gv0)::alternative_t<0>,int>);
static_assert(std::is_same_v<decltype(gv0)::alternative_t<1>,long long>);
static_assert(std::is_same_v<decltype(gv0)::alternative_t<2>,const char*>);
static_assert(std::is_same_v<decltype(gv0)::alternative_t<3>,bool>);
static_assert(std::is_same_v<decltype(gv0)::alternative_t<4>,char*>);

static_assert(gv0.holds_alternative<bool>());

// TEST
constexpr Variant<int,long long,NT> gv1;
static_assert(gv1.get<0>()==0);
static_assert(!std::is_trivially_default_constructible_v<decltype(gv1)>);

// TEST
constexpr Variant<int,long long,NT> gv2(std::in_place_index<2>,250);
static_assert(gv2.get<2>().a==250);
constexpr Variant<int,long long,NT> gv22 = gv2;
// static_assert(v22.get<2>().a==250); // Looks like as of C++20 there is no way to make this test pass. libstdc++'s variant does not pass either. libc++'s variant is even worse.

// TEST
constexpr long long test_emplace(const char* p){
    Variant<int,long long,const char*,bool> v(std::in_place_index<2>,p);
    return v.emplace<1>(1000);
}
static_assert(test_emplace("foo2")==1000);

// TEST
struct MyStr2{
    const char* s;
};
constexpr Variant<int,long long,NT> gv3 = 11;
static_assert(gv3.index()==0);
constexpr Variant<const char*,bool> gv4 = "blah";
static_assert(gv4.index()==0);
constexpr Variant<MyStr2,bool> gv5 = "blah";
static_assert(gv5.index()==0);
constexpr double d = 3.14;
constexpr Variant<int,std::reference_wrapper<const double>> gv6 = d;
static_assert(gv6.index()==1);
constexpr Variant<char,std::optional<char16_t>> gv7 = u'\u2043';
static_assert(gv7.index()==1);

// TEST
constexpr bool test_swap(const char* s){
    Variant<MyStr2,bool> v0(std::in_place_index<0>,s);
    Variant<MyStr2,bool> v1(std::in_place_index<1>,true);
    swap(v0,v1);
    return v0.get<1>();
}
static_assert(test_swap("test_swap")==true);

// TEST
constexpr yuki::Pair<size_t,int> test_ea0(){
    Variant<int,int,int> v(std::in_place_index<2>,100);
    v.emplace_or_assign<int>(200);
    return {v.index(),v.get<2>()};
}
constexpr yuki::Pair<size_t,int> p0 = test_ea0();
static_assert(p0.zeroth==2 && p0.first==200);

constexpr size_t test_ea1(){
    Variant<float,int> v(std::in_place_index<0>,0);
    v.emplace_or_assign(0);
    return v.index();
}
static_assert(test_ea1()==0);

constexpr size_t test_ea2(){
    Variant<float,int> v(std::in_place_index<0>,0);
    v=0;
    return v.index();
}
static_assert(test_ea2()==1);


int main(){
    // clang++ -I../include -I/usr/local/include -L/usr/local/lib -std=c++20 -Wextra -O2 -S variant.cpp -o variant.asm

    // TEST
    printf("%d\n",gv22.get<2>().a);

    // TEST
    printf("%zu %zu %zu %zu\n",sizeof(tree_union<int>),sizeof(tree_union<int,long long>),sizeof(tree_union<int,long long,const char*>),sizeof(tree_union<int,long long,const char*,bool>));
    tree_union<int,long long,const char*,bool,bool> tu(std::in_place_index<2>,"abc");
    printf("%zu %s\n\n",tu.elements,tu.get<2>());

    static_assert(std::is_trivially_destructible_v<decltype(tu)>);

    // TEST
    Variant<int,long long,const char*,bool,MyStr> v0(std::in_place_index<4>,"bcd");
    printf("%s ",v0.get<4>().s);

    printf("%d ",visit<int>([](auto&&...){return 100L;},v0,v0,v0));

    v0.emplace<1>(1000);
    printf("%lld\n",v0.get<1>());
    v0=1;
    printf("%zu %d\n",v0.index(),v0.get<0>());

    // TEST
    Variant<int,long,ncp> v1(std::in_place_index<0>,2000);
    Variant<int,long,ncp> v11=v1; // Should NOT trigger an assertion failure.
    assert(v11.get<0>() ==2000);
    Variant<int,long,ncp> v2(std::in_place_index<2>);
    Variant<int,long,ncp> v22=v2; // Should trigger an assertion failure.
}
