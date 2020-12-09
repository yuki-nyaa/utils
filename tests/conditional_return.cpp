#include"utils.hpp"

template<bool flag>
auto foo() -> std::conditional_t<flag,std::pair<int,double>,std::string> {
    YUKI_CONDITIONAL_RETURN(flag,YUKI_PROTECT({1,1}),"foo");
}

int main(){
    yuki::print_space(std::cout,foo<true>().first,foo<false>());
}