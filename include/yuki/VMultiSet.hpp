// Experimental
#pragma once
#include<yuki/Basic_VMultiSet.hpp>
#include<yuki/RB_Tree.hpp>
#include<yuki/functional.hpp>

namespace yuki{
template<
    typename K,
    typename C = yuki::Less<K>,
    typename Vec = yuki::Vector<K>,
    typename A = yuki::Allocator<RB_Tree_Node<Vec>>>
using VMultiSet = Basic_VMultiSet<RB_Tree,K,C,Vec,A>;
}
