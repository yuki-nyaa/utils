#pragma once
#include<string_view>
#include<yuki/mmhash3.hpp>

namespace yuki{
template<typename T>
struct Hash{
    static constexpr size_t operator()(const T& t) {return static_cast<size_t>(t);}
};

template<>
struct Hash<void>{
    template<typename T>
    static constexpr size_t operator()(const T& t) {return static_cast<size_t>(t);}
    typedef void is_transparent;
};

template<>
struct Hash<std::string_view>{
    static constexpr size_t operator()(const std::string_view sv) {return yuki::mmhash3(sv.data(),sv.size());}
};
}
