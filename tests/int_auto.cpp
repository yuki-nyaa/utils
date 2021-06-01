#include"../utils.hpp"
int main(){
    using namespace yuki;
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
}