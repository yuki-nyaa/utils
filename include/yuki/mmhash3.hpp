#pragma once
#include<cassert>
#include<cstdint>
#include<type_traits>

namespace yuki{
/*
Fixed the following problems of the original `MurmurHash3_x86_32`:
1. Performing `getblock32` on a reinterpret-casted `const uint32_t*` violates the strict aliasing rule.
2. `uint32_t` is in fact OPTIONAL by C/C++ Standard. My version works with any integer type wider than 32 bits and produces the same results.

Improvements:
1. `constexpr`
*/
template<typename UI32=uint_least32_t>
constexpr UI32 mmhash3(const char* data,const std::type_identity_t<UI32> len,std::type_identity_t<UI32> seed=0xbc9f1d34){
    static_assert(sizeof(UI32)>=4);
    assert(len<=0xFFFFFFFFU);

    constexpr auto rotl32 = [](const UI32 u,const UI32 r) -> UI32
    {
        if constexpr(sizeof(UI32)==4)
            return (u<<r)|(u>>(32-r));
        else
            return ((u<<r)|(u>>(32-r)))&0xFFFFFFFFU;
    };

    constexpr auto getblock32 = [](const char* const p) -> UI32
    {
        constexpr auto to_32 = [](const char c)->UI32
        {
            return static_cast<UI32>(static_cast<unsigned char>(c));
        };
        return to_32(p[0]) | (to_32(p[1])<<8) | (to_32(p[2])<<16) | (to_32(p[3])<<24);
    };

    constexpr auto to_32 = [](UI32& u){
        if constexpr(sizeof(UI32)!=4)
            u%=0x100000000;
    };

    to_32(seed);

    constexpr UI32 c1 = 0xcc9e2d51;
    constexpr UI32 c2 = 0x1b873593;

    // body
    for(UI32 i=len/4;i;--i,data+=4){
        UI32 k1 = getblock32(data);

        k1 *= c1;
        to_32(k1);
        k1 = rotl32(k1,15);
        k1 *= c2;
        to_32(k1);

        seed ^= k1;
        seed = rotl32(seed,13);
        seed = seed*5+0xe6546b64;
        to_32(seed);
    }

    // tail
    UI32 k1 = 0;
    switch(len&3U){
        case 3: k1 ^= static_cast<unsigned char>(data[2])<<16;
                [[fallthrough]];
        case 2: k1 ^= static_cast<unsigned char>(data[1])<<8;
                [[fallthrough]];
        case 1: k1 ^= static_cast<unsigned char>(data[0]);
                k1 *= c1;
                to_32(k1);
                k1 = rotl32(k1,15);
                k1 *= c2;
                to_32(k1);
                seed ^= k1;
    };

    // finalization
    seed ^= len;
    seed ^= seed>>16;
    seed *= 0x85ebca6b;
    to_32(seed);
    seed ^= seed>>13;
    seed *= 0xc2b2ae35;
    to_32(seed);
    seed ^= seed>>16;
    return seed;
} // mmhash3
} // namespace yuki
