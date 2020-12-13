#define YUKI_UTILS_DEV

#include<iostream>
#include<string>
#include"multiset.hpp"

struct Foo{
    int a;
    std::basic_string<int> b;
    friend bool operator==(const Foo&,const Foo&) noexcept = default;
};
std::ostream& operator<<(std::ostream& o,const Foo& foo){
    o<<'('<<foo.a<<",(";
    for(const int i : foo.b)
        o<<i<<',';
    o<<"))";
    return o;
}

template<>
struct yuki::rough_less<Foo>{
    constexpr bool operator()(const Foo& lhs,const Foo& rhs) const noexcept {
        if(lhs.a<rhs.a)
            return true;
        else if(rhs.a<lhs.a)
            return false;
        else if(lhs.b.size()<rhs.b.size())
            return true;
        else if(rhs.b.size()<lhs.b.size())
            return false;
        return false;
    }
};

int main(){
    yuki::multiset<Foo,yuki::rough_less<Foo>> ms;
    ms.insert({0,{1,2,3,4}});
    ms.insert({0,{2,3,4,5}});
    ms.insert({0,{1,3,5,7}});
    ms.insert({0,{1,3,5,6}});
    // 4 (0,(1,2,3,4,)) (0,(1,3,5,6,))
    yuki::print_space(std::cout,ms.count_equiv({0,{1,2,3,4}}),*(ms.first_equiv({0,{1,2,3,4}})),*(ms.last_equiv({0,{1,2,3,4}})));
    // 1 (0,(1,2,3,4,)) (0,(1,2,3,4,))
    yuki::print_space(std::cout,ms.count_equal({0,{1,2,3,4}}),*(ms.first_equal({0,{1,2,3,4}})),*(ms.last_equal({0,{1,2,3,4}})));
    // 1 1
    yuki::print_space(std::cout,ms.first_equal({0,{1,2,3,5}})==ms.end(),ms.last_equal({0,{1,2,3,5}})==ms.end());

    // (0,(1,3,5,6,))
    yuki::print_space(std::cout,*(ms.insert_equiv({0,{5,6,7,8}})));
    // (0,(5,6,7,8,))
    yuki::print_space(std::cout,*(ms.insert_equal({0,{5,6,7,8}})));

    // 1
    yuki::print_space(std::cout,ms.erase_equal({0,{5,6,7,8}}));
    // 4
    yuki::print_space(std::cout,ms.erase_equiv({0,{7,8,9,10}}));
}