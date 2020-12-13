#include<utility>
#include<iostream>
struct Foo{
    Foo() = default;
    Foo(const Foo&) {std::cout<<"copy"<<std::endl;}
    Foo& operator=(const Foo&) = default;
    Foo(Foo&&) {std::cout<<"move"<<std::endl;}
    Foo& operator=(Foo&&) = default;
};
template<bool flag,typename T,typename U>
auto conditional_return(T&& first,U&& second) -> std::conditional_t<flag,T,U> {
    if constexpr(flag)
        return first;
    else
        return second;
}
int main(){
    conditional_return<true,Foo,int>({},2);
}