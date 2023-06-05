#pragma once
#include<string_view>
#include<unordered_set>
#include<yuki/functional.hpp>
#include<yuki/Hash.hpp>

namespace yuki{
template<typename K,typename A=std::allocator<K>>
using unordered_set_str = std::unordered_set<K,yuki::Transparent<yuki::Hash<std::string_view>>,yuki::Transparent<yuki::Equal_To<std::string_view>>,A>;
}
