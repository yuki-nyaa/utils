#pragma once
#include<yuki/RB_Tree.hpp>
#include<yuki/Basic_Set.hpp>
#include<yuki/functional.hpp>

namespace yuki{
template<typename K,typename C=yuki::Less<K>,typename A=yuki::Allocator<RB_Tree_Node<K>>>
using MultiSet = Basic_MultiSet<RB_Tree,K,C,A>;

template<typename K,typename V,typename KV,typename C=yuki::Less<K>,typename A=yuki::Allocator<RB_Tree_Node<V>>>
using GSet = Basic_GSet<RB_Tree,K,V,KV,C,A>;

template<typename K,typename C=yuki::Less<K>,typename A=yuki::Allocator<RB_Tree_Node<K>>>
using Set = Basic_Set<RB_Tree,K,C,A>;
}
