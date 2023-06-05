#pragma once
#include<string_view>
#include<yuki/CHashTable.hpp>
#include<yuki/functional.hpp>
#include<yuki/Hash.hpp>

namespace yuki{
template<typename V,typename KV,typename VN,size_t slots>
using CHashTable_Str = CHashTable<std::string_view,V,KV,yuki::Transparent<yuki::Hash<std::string_view>>,yuki::Transparent<yuki::Equal_To<std::string_view>>,VN,slots>;
}
