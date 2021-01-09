#include"utils.hpp"
using namespace yuki;
int main(){
    // 1,1,1,1
    // Note: The particular number depends on the platform.
    // Note: On my platform `int` has the same size as `long`, so there is no test for `long`.
    yuki::print_space(std::cout,std::is_same_v<uint_auto_t<123>,unsigned char>,std::is_same_v<uint_auto_t<1000>,unsigned short>,std::is_same_v<uint_auto_t<65537>,unsigned int>,std::is_same_v<uint_auto_t<4294967296>,unsigned long long>);

    // 1,1,1,1
    yuki::print_space(std::cout,std::is_same_v<int_auto_t<-123>,signed char>,std::is_same_v<int_auto_t<-1000>,short>,std::is_same_v<int_auto_t<-32769>,int>,std::is_same_v<int_auto_t<-2147483649>,long long>);
}