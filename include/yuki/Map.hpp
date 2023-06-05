#pragma once
#include<yuki/RB_Tree.hpp>
#include<yuki/Basic_Map.hpp>
#include<yuki/functional.hpp>

namespace yuki{
template<typename K,typename M>
using Map_RB_Tree_Node = RB_Tree_Node<yuki::KM_Pair<const K,M>>;

template<typename K,typename M,typename C = yuki::Less<K>,typename A = yuki::Allocator<Map_RB_Tree_Node<K,M>>>
using Map = Basic_Map<RB_Tree,K,M,C,A>;

template<typename K,typename M,typename C = yuki::Less<K>,typename A = yuki::Allocator<Map_RB_Tree_Node<K,M>>>
using MultiMap = Basic_MultiMap<RB_Tree,K,M,C,A>;
}

