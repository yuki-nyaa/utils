#pragma once
#include<stdexcept>
#include<yuki/Allocator.hpp>

namespace yuki{

template<typename K,typename M>
struct KM_Pair{
    static_assert(std::is_const_v<K>);
    K key;
    M mapped;

    struct Key{
        static constexpr K& operator()(KM_Pair<K,M>& p) {return p.key;}
        static constexpr const K& operator()(const KM_Pair<K,M>& p) {return p.key;}
    };
};

template<
    template<typename /*K*/,typename /*V*/,typename /*KV*/,typename /*C*/,typename /*A*/,typename...> class B,
    typename K,
    typename M,
    typename C,
    typename A,
    typename... Others>
struct Basic_Map : protected B<K,KM_Pair<const K,M>,typename KM_Pair<const K,M>::Key,C,A,Others...>{
    typedef B<K,KM_Pair<const K,M>,typename KM_Pair<const K,M>::Key,C,A,Others...> Tree_Base;

    using typename Tree_Base::key_type;
    typedef M mapped_type;
    using typename Tree_Base::value_type;
    using typename Tree_Base::key_compare;
    using typename Tree_Base::allocator_type;
    using typename Tree_Base::pointer;
    using typename Tree_Base::const_pointer;
    using typename Tree_Base::size_type;
    using typename Tree_Base::difference_type;

    constexpr Basic_Map() noexcept = default;

    template<typename... Args,typename=std::enable_if_t<std::is_constructible_v<Tree_Base,yuki::unique_tag_t,Args&&...>>>
    constexpr Basic_Map(Args&&... args) noexcept :
        Tree_Base(yuki::unique_tag,std::forward<Args>(args)...)
    {}

    Basic_Map(yuki::from_ordered_tag_t,const Tree_Base& tree) noexcept : Tree_Base(tree) {}
    constexpr Basic_Map(yuki::from_ordered_tag_t,Tree_Base&& tree) noexcept : Tree_Base(std::move(tree)) {}

    Basic_Map& assign(yuki::from_ordered_tag_t,const Tree_Base& tree) noexcept {Tree_Base::operator=(tree); return *this;}
    Basic_Map& assign(yuki::from_ordered_tag_t,Tree_Base&& tree) noexcept {Tree_Base::operator=(std::move(tree)); return *this;}

    friend constexpr void swap(Basic_Map& lhs,Basic_Map& rhs) noexcept {swap(static_cast<Tree_Base&>(lhs),static_cast<Tree_Base&>(rhs));}

    constexpr Tree_Base& tree_base() {return *this;}
    constexpr const Tree_Base& tree_base() const {return *this;}

    using Tree_Base::clear;
    using Tree_Base::empty;
    using Tree_Base::size;

    typedef typename Tree_Base::non_const_iterator iterator;
    using typename Tree_Base::const_iterator;

    iterator begin() {using yuki::const_kast; return const_kast(Tree_Base::begin());}
    const_iterator begin() const {return Tree_Base::begin();}
    constexpr iterator end() {using yuki::const_kast; return const_kast(Tree_Base::end());}
    constexpr const_iterator end() const {return Tree_Base::end();}

    iterator min() {using yuki::const_kast; return const_kast(Tree_Base::min());}
    const_iterator min() const {return Tree_Base::min();}
    iterator max() {using yuki::const_kast; return const_kast(Tree_Base::max());}
    const_iterator max() const {return Tree_Base::max();}

    using Tree_Base::first_greater_tp;
    using Tree_Base::first_greater;

    template<typename K2>
    iterator first_greater_tp(const K2& k) {using yuki::const_kast; return const_kast(first_greater_tp(k));}
    iterator first_greater(const K& k) {using yuki::const_kast; return const_kast(first_greater_tp(k));;}
    template<typename K2>
    iterator first_greater(const K2& k) requires requires{typename C::is_transparent;}
        {using yuki::const_kast; return const_kast(first_greater_tp(k));}

    using Tree_Base::first_equiv_greater_tp;
    using Tree_Base::first_equiv_greater;

    template<typename K2>
    iterator first_equiv_greater_tp(const K2& k) {using yuki::const_kast; return const_kast(first_equiv_greater_tp(k));}
    iterator first_equiv_greater(const K& k) {using yuki::const_kast; return const_kast(first_equiv_greater_tp(k));;}
    template<typename K2>
    iterator first_equiv_greater(const K2& k) requires requires{typename C::is_transparent;}
        {using yuki::const_kast; return const_kast(first_equiv_greater_tp(k));}

    template<typename K2>
    iterator find_tp(const K2& k) {using yuki::const_kast; return const_kast(Tree_Base::find_any_tp(k));}
    iterator find(const K& k) {using yuki::const_kast; return const_kast(Tree_Base::find_any_tp(k));;}
    template<typename K2>
    iterator find(const K2& k) requires requires{typename C::is_transparent;}
        {using yuki::const_kast; return const_kast(Tree_Base::find_any_tp(k));}

    template<typename K2>
    const_iterator find_tp(const K2& k) const {return Tree_Base::find_any_tp(k);}
    const_iterator find(const K& k) const {return Tree_Base::find_any_tp(k);}
    template<typename K2>
    const_iterator find(const K2& k) const requires requires{typename C::is_transparent;}
        {return Tree_Base::find_any_tp(k);}

    using Tree_Base::contains_tp;
    using Tree_Base::contains;

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
        const yuki::IB_Pair<const_iterator> ibp = Tree_Base::emplace_unique_at_tp(k,std::forward<K2>(k),std::forward<Args>(args)...);
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

    using Tree_Base::erase;

    template<typename K2>
    size_type erase_tp(const K2& k) const {return Tree_Base::erase_unique_tp(k);}
    size_type erase(const K& k) const {return Tree_Base::erase_unique_tp(k);}
    template<typename K2>
    size_type erase(const K2& k) const requires requires{typename C::is_transparent;}
        {return Tree_Base::erase_unique_tp(k);}

    template<typename C2,typename A2,typename... Others2>
    void merge(const Basic_Map<B,K,M,C2,A2,Others2...>& other) {Tree_Base::merge_unique(other);}
    template<typename C2,typename A2,typename... Others2,typename... Mg>
    void merge(const Basic_Map<B,K,M,C2,A2,Others2...>& other,Mg&&... mg) {Tree_Base::merge_unique(other,std::forward<Mg>(mg)...);}
    template<typename C2,typename A2,typename... Others2>
    void merge(Basic_Map<B,K,M,C2,A2,Others2...>&& other) {Tree_Base::merge_unique(std::move(other));}
    template<typename C2,typename A2,typename... Others2,typename... Mg>
    void merge(Basic_Map<B,K,M,C2,A2,Others2...>&& other,Mg&&... mg) {Tree_Base::merge_unique(std::move(other),std::forward<Mg>(mg)...);}
}; // Basic_Map<B,K,M,C,A,Others...>

template<
    template<typename /*K*/,typename /*V*/,typename /*KV*/,typename /*C*/,typename /*A*/,typename...> class B,
    typename K,
    typename M,
    typename C,
    typename A,
    typename... Others>
using Basic_MultiMap = B<K,KM_Pair<const K,M>,typename KM_Pair<const K,M>::Key,C,A,Others...>;
} // namespace yuki
