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
    typedef B<K,V,KV,C,A,Others...> tree_type;

    using typename tree_type::key_type;
    using typename tree_type::value_type;
    using typename tree_type::key_compare;
    using typename tree_type::allocator_type;
    using typename tree_type::pointer;
    using typename tree_type::const_pointer;
    using typename tree_type::size_type;
    using typename tree_type::difference_type;

    constexpr Basic_GSet() noexcept = default;

    template<typename... Args,typename=std::enable_if_t<std::is_constructible_v<tree_type,yuki::unique_tag_t,Args&&...>>>
    constexpr Basic_GSet(Args&&... args) noexcept : tree_type(yuki::unique_tag,std::forward<Args>(args)...) {}

    template<typename... Args> requires std::is_constructible_v<tree_type,Args&&...>
    constexpr Basic_GSet(yuki::from_ordered_tag_t,Args&&... args) noexcept : tree_type(std::forward<Args>(args)...) {}

    template<typename... Args> requires (!std::is_constructible_v<tree_type,Args&&...> && std::is_constructible_v<tree_type,yuki::from_ordered_tag_t,Args&&...>)
    constexpr Basic_GSet(yuki::from_ordered_tag_t,Args&&... args) noexcept : tree_type(yuki::from_ordered_tag,std::forward<Args>(args)...) {}

    Basic_GSet& assign(yuki::from_ordered_tag_t,const tree_type& tree) noexcept {tree_type::operator=(tree); return *this;}
    Basic_GSet& assign(yuki::from_ordered_tag_t,tree_type&& tree) noexcept {tree_type::operator=(std::move(tree)); return *this;}

    friend constexpr void swap(Basic_GSet& lhs,Basic_GSet& rhs) noexcept {swap(static_cast<tree_type&>(lhs),static_cast<tree_type&>(rhs));}

    constexpr tree_type& tree() {return *this;}
    constexpr const tree_type& tree() const {return *this;}

    using tree_type::clear;
    using tree_type::empty;
    using tree_type::size;

    using typename tree_type::non_const_iterator;
    using typename tree_type::const_iterator;

    using tree_type::begin;
    using tree_type::end;

    using tree_type::min;
    using tree_type::max;

    using tree_type::first_greater_tp;
    using tree_type::first_greater;
    using tree_type::first_equiv_greater_tp;
    using tree_type::first_equiv_greater;

    template<typename K2>
    const_iterator find_tp(const K2& k) const {return tree_type::find_any_tp(k);}
    const_iterator find(const K& k) const {return tree_type::find_any_tp(k);}
    template<typename K2>
    const_iterator find(const K2& k) const requires requires{typename C::is_transparent;}
        {return tree_type::find_any_tp(k);}

    using tree_type::contains_tp;
    using tree_type::contains;

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename K2,typename... Args>
    yuki::IB_Pair<const_iterator> emplace_at_tp(const K2& k,Args&&... args){
        return tree_type::emplace_unique_at_tp(k,std::forward<Args>(args)...);
    }

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename... Args>
    yuki::IB_Pair<const_iterator> emplace_at(const K& k,Args&&... args)
        {return tree_type::emplace_unique_at_tp(k,std::forward<Args>(args)...);}

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename K2,typename... Args>
    yuki::IB_Pair<const_iterator> emplace_at(const K2& k,Args&&... args) requires requires{typename C::is_transparent;}
        {return tree_type::emplace_unique_at_tp(k,std::forward<Args>(args)...);}

    yuki::IB_Pair<const_iterator> insert(const V& v) {return tree_type::insert_unique(v);}
    yuki::IB_Pair<const_iterator> insert(V&& v) {return tree_type::insert_unique(std::move(v));}

    using tree_type::erase;

    template<typename K2>
    size_type erase_tp(const K2& k) const {return tree_type::erase_unique_tp(k);}
    size_type erase(const K& k) const {return tree_type::erase_unique_tp(k);}
    template<typename K2>
    size_type erase(const K2& k) const requires requires{typename C::is_transparent;}
        {return tree_type::erase_unique_tp(k);}

    template<typename K2,typename V2,typename KV2,typename C2,typename A2,typename... Others2,typename... M>
        requires (std::is_same_v<V2,V> || requires{typename KV::is_transparent;} || requires{typename C::is_transparent;})
    void merge(const Basic_GSet<B,K2,V2,KV2,C2,A2,Others2...>& other,M&&... m){
        tree_type::merge_unique(other,std::forward<M>(m)...);
    }
    template<typename K2,typename V2,typename KV2,typename C2,typename A2,typename... Others2,typename... M>
        requires (std::is_same_v<V2,V> || requires{typename KV::is_transparent;} || requires{typename C::is_transparent;})
    void merge(Basic_GSet<B,K2,V2,KV2,C2,A2,Others2...>&& other,M&&... m){
        tree_type::merge_unique(std::move(other),std::forward<M>(m)...);
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
