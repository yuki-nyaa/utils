#pragma once
#include<yuki/Ordered_Vector.hpp>
#include<yuki/Basic_Set.hpp>
#include<yuki/functional.hpp>

namespace yuki{
template<typename K,typename C=yuki::Less<K>,typename A=yuki::Allocator<K>,typename EC=Default_EC<2,1,4>>
using MultiSet_OV = Basic_MultiSet<Basic_Ordered_Vector,K,C,A,EC>;

template<typename K,typename V,typename KV,typename C=yuki::Less<K>,typename A=yuki::Allocator<V>,typename EC=Default_EC<2,1,4>>
using GSet_OV = Basic_GSet<Basic_Ordered_Vector,K,V,KV,C,A,EC>;

template<typename K,typename C=yuki::Less<K>,typename A=yuki::Allocator<K>,typename EC=Default_EC<2,1,4>>
using Set_OV = Basic_Set<Basic_Ordered_Vector,K,C,A,EC>;
}

