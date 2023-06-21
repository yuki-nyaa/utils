#pragma once
#include<stdexcept>
#include<yuki/Allocator.hpp>

namespace yuki{

template<typename K,typename M>
struct KM_Pair{
    static_assert(std::is_const_v<K>);
    K key;
    M mapped;
};

struct Get_Key{
    template<typename K,typename M>
    static constexpr const K& operator()(const KM_Pair<K,M>& p) {return p.key;}
    typedef void is_transparent;
};

template<
    template<typename /*K*/,typename /*V*/,typename /*KV*/,typename /*C*/,typename /*A*/,typename...> class B,
    typename K,
    typename M,
    typename C,
    typename A,
    typename... Others>
struct Basic_Map : protected B<K,KM_Pair<const K,M>,Get_Key,C,A,Others...>{
    typedef B<K,KM_Pair<const K,M>,Get_Key,C,A,Others...> tree_type;

    using typename tree_type::key_type;
    typedef M mapped_type;
    using typename tree_type::value_type;
    using typename tree_type::key_compare;
    using typename tree_type::allocator_type;
    using typename tree_type::pointer;
    using typename tree_type::const_pointer;
    using typename tree_type::size_type;
    using typename tree_type::difference_type;

    constexpr Basic_Map() noexcept = default;

    template<typename... Args,typename=std::enable_if_t<std::is_constructible_v<tree_type,yuki::unique_tag_t,Args&&...>>>
    constexpr Basic_Map(Args&&... args) noexcept :
        tree_type(yuki::unique_tag,std::forward<Args>(args)...)
    {}

    template<typename... Args> requires std::is_constructible_v<tree_type,Args&&...>
    constexpr Basic_Map(yuki::from_ordered_tag_t,Args&&... args) noexcept : tree_type(std::forward<Args>(args)...) {}

    template<typename... Args> requires (!std::is_constructible_v<tree_type,Args&&...> && std::is_constructible_v<tree_type,yuki::from_ordered_tag_t,Args&&...>)
    constexpr Basic_Map(yuki::from_ordered_tag_t,Args&&... args) noexcept : tree_type(yuki::from_ordered_tag,std::forward<Args>(args)...) {}

    Basic_Map& assign(yuki::from_ordered_tag_t,const tree_type& tree) noexcept {tree_type::operator=(tree); return *this;}
    Basic_Map& assign(yuki::from_ordered_tag_t,tree_type&& tree) noexcept {tree_type::operator=(std::move(tree)); return *this;}

    friend constexpr void swap(Basic_Map& lhs,Basic_Map& rhs) noexcept {swap(static_cast<tree_type&>(lhs),static_cast<tree_type&>(rhs));}

    constexpr tree_type& tree() {return *this;}
    constexpr const tree_type& tree() const {return *this;}

    using tree_type::clear;
    using tree_type::empty;
    using tree_type::size;

    typedef typename tree_type::non_const_iterator iterator;
    using typename tree_type::const_iterator;

    iterator begin() {using yuki::const_kast; return const_kast(tree_type::begin());}
    const_iterator begin() const {return tree_type::begin();}
    constexpr iterator end() {using yuki::const_kast; return const_kast(tree_type::end());}
    constexpr const_iterator end() const {return tree_type::end();}

    iterator min() {using yuki::const_kast; return const_kast(tree_type::min());}
    const_iterator min() const {return tree_type::min();}
    iterator max() {using yuki::const_kast; return const_kast(tree_type::max());}
    const_iterator max() const {return tree_type::max();}

    using tree_type::first_greater_tp;
    using tree_type::first_greater;

    template<typename K2>
    iterator first_greater_tp(const K2& k) {using yuki::const_kast; return const_kast(first_greater_tp(k));}
    iterator first_greater(const K& k) {using yuki::const_kast; return const_kast(first_greater_tp(k));;}
    template<typename K2>
    iterator first_greater(const K2& k) requires requires{typename C::is_transparent;}
        {using yuki::const_kast; return const_kast(first_greater_tp(k));}

    using tree_type::first_equiv_greater_tp;
    using tree_type::first_equiv_greater;

    template<typename K2>
    iterator first_equiv_greater_tp(const K2& k) {using yuki::const_kast; return const_kast(first_equiv_greater_tp(k));}
    iterator first_equiv_greater(const K& k) {using yuki::const_kast; return const_kast(first_equiv_greater_tp(k));;}
    template<typename K2>
    iterator first_equiv_greater(const K2& k) requires requires{typename C::is_transparent;}
        {using yuki::const_kast; return const_kast(first_equiv_greater_tp(k));}

    template<typename K2>
    iterator find_tp(const K2& k) {using yuki::const_kast; return const_kast(tree_type::find_any_tp(k));}
    iterator find(const K& k) {using yuki::const_kast; return const_kast(tree_type::find_any_tp(k));;}
    template<typename K2>
    iterator find(const K2& k) requires requires{typename C::is_transparent;}
        {using yuki::const_kast; return const_kast(tree_type::find_any_tp(k));}

    template<typename K2>
    const_iterator find_tp(const K2& k) const {return tree_type::find_any_tp(k);}
    const_iterator find(const K& k) const {return tree_type::find_any_tp(k);}
    template<typename K2>
    const_iterator find(const K2& k) const requires requires{typename C::is_transparent;}
        {return tree_type::find_any_tp(k);}

    using tree_type::contains_tp;
    using tree_type::contains;

    template<typename K2>
    M& at_tp(const K2& k) noexcept(false) {
        if(const iterator i=find_tp(k); i!=end())
            return i->mapped;
        else
            throw std::out_of_range("yuki::Basic_Map::at(_tp): Out of range!");
    }
    M& at(const K& k) noexcept(false) {return at_tp(k);}
    template<typename K2>
    M& at(const K2& k) noexcept(false) requires requires{typename C::is_transparent;} {return at_tp(k);}

    template<typename K2>
    const M& at_tp(const K2& k) const noexcept(false) {
        if(const const_iterator i=find_tp(k); i!=end())
            return i->mapped;
        else
            throw std::out_of_range("yuki::Basic_Map::at(_tp) const: Out of range!");
    }
    const M& at(const K& k) const noexcept(false) {return at_tp(k);}
    template<typename K2>
    const M& at(const K2& k) const noexcept(false) requires requires{typename C::is_transparent;} {return at_tp(k);}

    /// @note Unlike `std::map::operator[]`, this implementation does not insert new value when an equivalent key is not found, to match vector's behaviour, which is more natural IMO. `insert_or_assign` should be used instead when an insertion is desirable.
    M& operator[](const K& k) {return find(k)->mapped;}
    template<typename K2>
    M& operator[](const K2& k) requires requires{typename C::is_transparent;}
        {return find(k)->mapped;}
    const M& operator[](const K& k) const {return find(k)->mapped;}
    template<typename K2>
    const M& operator[](const K2& k) const requires requires{typename C::is_transparent;}
        {return find(k)->mapped;}

    template<typename K2,typename... Args>
    yuki::IB_Pair<iterator> emplace_tp(K2&& k,Args&&... args){
        const yuki::IB_Pair<const_iterator> ibp = tree_type::emplace_unique_at_tp(k,std::forward<K2>(k),std::forward<Args>(args)...);
        using yuki::const_kast;
        return {const_kast(ibp.iterator),ibp.has_inserted};
    }

    template<typename... Args>
    yuki::IB_Pair<iterator> emplace(const K& k,Args&&... args) {return emplace_tp(k,std::forward<Args>(args)...);}

    template<typename... Args>
    yuki::IB_Pair<iterator> emplace(K&& k,Args&&... args) {return emplace_tp(std::move(k),std::forward<Args>(args)...);}

    template<typename K2,typename... Args>
    yuki::IB_Pair<iterator> emplace(K2&& k,Args&&... args) requires requires{typename C::is_transparent;}
        {return emplace_tp(std::forward<K2>(k),std::forward<Args>(args)...);}

    template<typename K2,typename Mp=M>
    yuki::IB_Pair<iterator> insert_or_assign_tp(K2&& k,Mp&& m){
        const yuki::IB_Pair<iterator> ibp = emplace_tp(std::forward<K2>(k),std::forward<Mp>(m));
        if(!ibp.has_inserted)
            ibp.iterator->mapped = std::forward<Mp>(m);
        return ibp;
    }

    template<typename Mp=M>
    yuki::IB_Pair<iterator> insert_or_assign(const K& k,Mp&& m) {return insert_or_assign_tp(k,std::forward<Mp>(m));}

    template<typename Mp=M>
    yuki::IB_Pair<iterator> insert_or_assign(K&& k,Mp&& m) {return insert_or_assign_tp(std::move(k),std::forward<Mp>(m));}

    template<typename K2,typename Mp=M>
    yuki::IB_Pair<iterator> insert_or_assign(K2&& k,Mp&& m) requires requires{typename C::is_transparent;}
        {return insert_or_assign_tp(std::forward<K2>(k),std::forward<Mp>(m));}

    using tree_type::erase;

    template<typename K2>
    size_type erase_tp(const K2& k) const {return tree_type::erase_unique_tp(k);}
    size_type erase(const K& k) const {return tree_type::erase_unique_tp(k);}
    template<typename K2>
    size_type erase(const K2& k) const requires requires{typename C::is_transparent;}
        {return tree_type::erase_unique_tp(k);}

    template<typename K2,typename M2,typename C2,typename A2,typename... Others2,typename... Mg>
    void merge(const Basic_Map<B,K2,M2,C2,A2,Others2...>& other,Mg&&... mg) {tree_type::merge_unique(other,std::forward<Mg>(mg)...);}
    template<typename K2,typename M2,typename C2,typename A2,typename... Others2,typename... Mg>
    void merge(Basic_Map<B,K2,M2,C2,A2,Others2...>&& other,Mg&&... mg) {tree_type::merge_unique(std::move(other),std::forward<Mg>(mg)...);}
}; // Basic_Map<B,K,M,C,A,Others...>

template<
    template<typename /*K*/,typename /*V*/,typename /*KV*/,typename /*C*/,typename /*A*/,typename...> class B,
    typename K,
    typename M,
    typename C,
    typename A,
    typename... Others>
using Basic_MultiMap = B<K,KM_Pair<const K,M>,Get_Key,C,A,Others...>;
} // namespace yuki
