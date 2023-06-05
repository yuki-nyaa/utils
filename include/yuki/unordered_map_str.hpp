#pragma once
#include<string_view>
#include<unordered_map>
#include<yuki/functional.hpp>
#include<yuki/Hash.hpp>

namespace yuki{
template<typename K,typename M,typename A=std::allocator<std::pair<const K,M>>>
using unordered_map_str = std::unordered_map<K,M,yuki::Transparent<yuki::Hash<std::string_view>>,yuki::Transparent<yuki::Equal_To<std::string_view>>,A>;
}
