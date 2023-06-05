#include<yuki/uchar.hpp>
#include<cstdio>
int main(){
    using yuki::U8Char;
    constexpr U8Char c1 = U'\u1234';
    constexpr U8Char c2 = U'ク';
    constexpr U8Char c3 = U'ー';
    static_assert(static_cast<char32_t>(c1)==U'\u1234');
    static_assert(static_cast<char32_t>(c2)==U'ク');
    static_assert(static_cast<char32_t>(c3)==U'ー');
    constexpr U8Char eof = yuki::EOF_32;
    static_assert(static_cast<char32_t>(eof)==yuki::EOF_32);
    static_assert(yuki::to_u32<yuki::encoding::utf16be>(0xD8,0x01,0xDC,0x37)==0x10437);
    static_assert(yuki::to_u32<yuki::encoding::utf16be>(0xD8,0x52,0xDF,0x62)==0x24B62);
    static_assert(yuki::to_u32<yuki::encoding::utf16be>(0,0,0x20,0xAC)==0x20AC);

    using namespace yuki::literals;
    static_assert('\x90'_u8==U8Char(0x90));
    static_assert(0x90_uc==(unsigned char)0x90);
    static_assert('\x90'_uc==0x90);

    char arr1[100] = {};
    c1.write_to(arr1);
    printf("%x %x %x %x\n",arr1[0],arr1[1],arr1[2],arr1[3]);
    //size_t less = 0;
    //size_t equal = 0;
    //size_t greater = 0;
    //constexpr unsigned arr_total = 10000;
    //constexpr unsigned test_total = 10000000;
    //std::random_device rd;
    //std::mt19937 gen(rd());
    //std::uniform_int_distribution<unsigned> arr_distrib(0,0x10FFFF);
    //std::uniform_int_distribution<unsigned> index_distrib(0,arr_total-1);
    //yuki::UChar2 arr[arr_total];
    //for(auto i=arr_total;i!=0;--i)
    //    arr[i-1] = arr_distrib(gen);
    //unsigned ia;
    //unsigned ib;
    //for(auto i = test_total;i!=0;--i){
    //    ia = index_distrib(gen);
    //    ib = index_distrib(gen);
    //    std::strong_ordering result = arr[ia]<=>arr[ib];
    //    if(result==std::strong_ordering::less)
    //        ++less;
    //    else if(result==std::strong_ordering::equal)
    //        ++equal;
    //    else if(result==std::strong_ordering::greater)
    //        ++greater;
    //}
    //printf("%zu %zu %zu",less,equal,greater);
}