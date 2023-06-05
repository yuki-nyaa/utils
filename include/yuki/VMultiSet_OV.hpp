// Experimental
#pragma once
#include<yuki/Ordered_Vector.hpp>
#include<yuki/Basic_VMultiSet.hpp>
#include<yuki/functional.hpp>

namespace yuki{
template<
    typename K,
    typename C = yuki::Less<K>,
    typename Vec = yuki::Vector<K>,
    typename A = yuki::Allocator<Vec>,
    typename EC = Default_EC<2,1,4>>
using VMultiSet_OV = Basic_VMultiSet<Basic_Ordered_Vector,K,C,Vec,A,EC>;
}
