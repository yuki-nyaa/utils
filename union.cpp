// Adapted from https://github.com/groundswellaudio/swl-variant
#include<type_traits>
#include<cstdlib>
#include<cstdio>

struct union_dummy {};

template<typename A,typename B>
union union_of_elements{
    A a;
    B b;
};
template<typename U1,typename U2>
union union_of_unions{
    U1 u1;
    U2 u2;
};

// This should really be an alias template, if alias template can be specialized.
template<bool is_first_pass,size_t remaining,typename... Ts>
struct tree_union_helper;

template<size_t remaining,typename A,typename B,typename... Ts> requires (remaining>=2) // The requires clause can be removed, at the cost of the number of specializations.
struct tree_union_helper<true,remaining,A,B,Ts...>{
    using type = typename tree_union_helper<true,remaining-2,Ts...,union_of_elements<A,B>>::type;
};
template<typename A,typename... Ts>
struct tree_union_helper<true,1,A,Ts...>{
    using type = typename tree_union_helper<false,sizeof...(Ts)+1,Ts...,union_of_elements<A,union_dummy>>::type;
};
template<typename... Ts>
struct tree_union_helper<true,0,Ts...>{
    using type = typename tree_union_helper<false,sizeof...(Ts),Ts...>::type;
};

template<size_t remaining,typename A,typename B,typename... Ts> requires (remaining>=2)
struct tree_union_helper<false,remaining,A,B,Ts...>{
    using type = typename tree_union_helper<false,remaining-2,Ts...,union_of_unions<A,B>>::type;
};
template<typename A,typename... Ts>
struct tree_union_helper<false,1,A,Ts...>{
    using type = typename tree_union_helper<false,sizeof...(Ts)+1,Ts...,union_of_unions<A,union_dummy>>::type;
};
template<typename... Ts>
struct tree_union_helper<false,0,Ts...>{
    using type = typename tree_union_helper<false,sizeof...(Ts),Ts...>::type;
};

template<typename T>
struct tree_union_helper<false,1,T>{
    using type = T;
};

template<typename... Ts>
using tree_union = typename tree_union_helper<true,sizeof...(Ts),Ts...>::type;

int main(){
    printf("%u %u %u %u",sizeof(tree_union<int>),sizeof(tree_union<int,long long>),sizeof(tree_union<int,long long,const char*>),sizeof(tree_union<int,long long,const char*,bool>));
}