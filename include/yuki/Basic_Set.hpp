#pragma once
#include<yuki/Pair.hpp>
#include<yuki/functional.hpp>
#include<yuki/Allocator.hpp>

namespace yuki{
template<
    template<typename /*K*/,typename /*V*/,typename /*KV*/,typename /*C*/,typename /*A*/,typename...> class B,
    typename K,
    typename V,
    typename KV,
    typename C,
    typename A,
    typename... Others>
struct Basic_GSet : protected B<K,V,KV,C,A,Others...>{
    typedef B<K,V,KV,C,A,Others...> Tree_Base;

    using typename Tree_Base::key_type;
    using typename Tree_Base::value_type;
    using typename Tree_Base::key_compare;
    using typename Tree_Base::allocator_type;
    using typename Tree_Base::pointer;
    using typename Tree_Base::const_pointer;
    using typename Tree_Base::size_type;
    using typename Tree_Base::difference_type;

    constexpr Basic_GSet() noexcept = default;

    template<typename... Args,typename=std::enable_if_t<std::is_constructible_v<Tree_Base,yuki::unique_tag_t,Args&&...>>>
    constexpr Basic_GSet(Args&&... args) noexcept :
        Tree_Base(yuki::unique_tag,std::forward<Args>(args)...)
    {}

    Basic_GSet(yuki::from_ordered_tag_t,const Tree_Base& tree) noexcept : Tree_Base(tree) {}
    constexpr Basic_GSet(yuki::from_ordered_tag_t,Tree_Base&& tree) noexcept : Tree_Base(std::move(tree)) {}

    Basic_GSet& assign(yuki::from_ordered_tag_t,const Tree_Base& tree) noexcept {Tree_Base::operator=(tree); return *this;}
    Basic_GSet& assign(yuki::from_ordered_tag_t,Tree_Base&& tree) noexcept {Tree_Base::operator=(std::move(tree)); return *this;}

    friend constexpr void swap(Basic_GSet& lhs,Basic_GSet& rhs) noexcept {swap(static_cast<Tree_Base&>(lhs),static_cast<Tree_Base&>(rhs));}

    constexpr Tree_Base& tree_base() {return *this;}
    constexpr const Tree_Base& tree_base() const {return *this;}

    using Tree_Base::clear;
    using Tree_Base::empty;
    using Tree_Base::size;

    using typename Tree_Base::non_const_iterator;
    using typename Tree_Base::const_iterator;

    using Tree_Base::begin;
    using Tree_Base::end;

    using Tree_Base::min;
    using Tree_Base::max;

    using Tree_Base::first_greater_tp;
    using Tree_Base::first_greater;
    using Tree_Base::first_equiv_greater_tp;
    using Tree_Base::first_equiv_greater;

    template<typename K2>
    const_iterator find_tp(const K2& k) const {return Tree_Base::find_any_tp(k);}
    const_iterator find(const K& k) const {return Tree_Base::find_any_tp(k);}
    template<typename K2>
    const_iterator find(const K2& k) const requires requires{typename C::is_transparent;}
        {return Tree_Base::find_any_tp(k);}

    using Tree_Base::contains_tp;
    using Tree_Base::contains;

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename K2,typename... Args>
    yuki::IB_Pair<const_iterator> emplace_at_tp(const K2& k,Args&&... args){
        return Tree_Base::emplace_unique_at_tp(k,std::forward<Args>(args)...);
    }

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename... Args>
    yuki::IB_Pair<const_iterator> emplace_at(const K& k,Args&&... args)
        {return Tree_Base::emplace_unique_at_tp(k,std::forward<Args>(args)...);}

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename K2,typename... Args>
    yuki::IB_Pair<const_iterator> emplace_at(const K2& k,Args&&... args) requires requires{typename C::is_transparent;}
        {return Tree_Base::emplace_unique_at_tp(k,std::forward<Args>(args)...);}

    yuki::IB_Pair<const_iterator> insert(const V& v) {return Tree_Base::insert_unique(v);}
    yuki::IB_Pair<const_iterator> insert(V&& v) {return Tree_Base::insert_unique(std::move(v));}

    using Tree_Base::erase;

    template<typename K2>
    size_type erase_tp(const K2& k) const {return Tree_Base::erase_unique_tp(k);}
    size_type erase(const K& k) const {return Tree_Base::erase_unique_tp(k);}
    template<typename K2>
    size_type erase(const K2& k) const requires requires{typename C::is_transparent;}
        {return Tree_Base::erase_unique_tp(k);}

    template<typename K2,typename KV2,typename C2,typename A2,typename... Others2,typename... M>
    void merge(const Basic_GSet<B,K2,V,KV2,C2,A2,Others2...>& other,M&&... m){
        Tree_Base::merge_unique(other,std::forward<M>(m)...);
    }
    template<typename K2,typename KV2,typename C2,typename A2,typename... Others2,typename... M>
    void merge(Basic_GSet<B,K2,V,KV2,C2,A2,Others2...>&& other,M&&... m){
        Tree_Base::merge_unique(std::move(other),std::forward<M>(m)...);
    }
}; // struct Basic_GSet


template<
    template<typename /*K*/,typename /*V*/,typename /*KV*/,typename /*C*/,typename /*A*/,typename...> class B,
    typename K,
    typename C,
    typename A,
    typename... Others>
using Basic_Set = Basic_GSet<B,K,K,Iden,C,A,Others...>;


template<
    template<typename /*K*/,typename /*V*/,typename /*KV*/,typename /*C*/,typename /*A*/,typename...> class B,
    typename K,
    typename C,
    typename A,
    typename... Others>
using Basic_MultiSet = B<K,K,Iden,C,A,Others...>;
} // namespace yuki
