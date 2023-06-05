#pragma once
#include<yuki/Ordered_Vector.hpp>
#include<yuki/Basic_Map.hpp>
#include<yuki/functional.hpp>

namespace yuki{
template<typename K,typename M,typename C=yuki::Less<K>,typename A=yuki::Allocator<yuki::KM_Pair<const K,M>>,typename EC=Default_EC<2,1,4>>
using Map_OV = Basic_Map<Basic_Ordered_Vector,K,M,C,A,EC>;

template<typename K,typename M,typename C=yuki::Less<K>,typename A=yuki::Allocator<yuki::KM_Pair<const K,M>>,typename EC=Default_EC<2,1,4>>
using MultiMap_OV = Basic_MultiMap<Basic_Ordered_Vector,K,M,C,A,EC>;
}

