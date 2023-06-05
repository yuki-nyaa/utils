// Experimental
#pragma once
#include<yuki/Pair.hpp>
#include<yuki/Vector.hpp>
#include<iterator>

namespace yuki{

template<typename Vec>
struct Vec_Zeroth{
    static auto& operator()(Vec& v) {return v[0];}
    static const auto& operator()(const Vec& v) {return v[0];}
};

template<
    template<typename /*K*/,typename /*V*/,typename /*KV*/,typename /*C*/,typename /*A*/,typename...> class B,
    typename K,
    typename C,
    typename Vec,
    typename A,
    typename... Others>
struct Basic_VMultiSet : protected B<K,Vec,Vec_Zeroth<Vec>,C,A,Others...>{
    typedef B<K,Vec,Vec_Zeroth<Vec>,C,A,Others...> Tree_Base;

    using typename Tree_Base::key_type;
    using typename Tree_Base::value_type;
    using typename Tree_Base::key_compare;
    using typename Tree_Base::allocator_type;
    using typename Tree_Base::pointer;
    using typename Tree_Base::const_pointer;
    using typename Tree_Base::size_type;
    using typename Tree_Base::difference_type;

    typedef Vec bucket_type;

    using Tree_Base::Tree_Base;
    friend constexpr void swap(Basic_VMultiSet& lhs,Basic_VMultiSet& rhs) noexcept {swap(static_cast<Tree_Base&>(lhs),static_cast<Tree_Base&>(rhs));}

    constexpr Tree_Base& tree_base() {return *this;}
    constexpr const Tree_Base& tree_base() const {return *this;}

    using Tree_Base::clear;
    using Tree_Base::empty;
    constexpr size_type bucket_count() const {return Tree_Base::size();}
    constexpr size_type size() const {return s_;}

    typedef typename Tree_Base::non_const_iterator non_const_bucket_iterator;
    typedef typename Tree_Base::const_iterator const_bucket_iterator;
    typedef typename Vec::iterator non_const_local_iterator;
    typedef typename Vec::const_iterator const_local_iterator;

    const_bucket_iterator bucket_begin() const {return Tree_Base::begin();}
    constexpr const_bucket_iterator bucket_end() const {return Tree_Base::end();}
  private:
    template<typename VMSp,typename IB>
    struct iterator_tp_{
        VMSp vms;
        IB bucket;
        typename bucket_type::size_type local;

        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::remove_reference_t<decltype((*bucket)[local])> value_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type* pointer;
        typedef value_type& reference;

        constexpr iterator_tp_() noexcept = default;
        constexpr iterator_tp_(VMSp v,IB b,typename bucket_type::size_type l = 0) noexcept : vms(v),bucket(b),local(l) {}

        explicit constexpr operator bool() const noexcept {return static_cast<bool>(vms);}

        constexpr value_type& operator*() const {return (*bucket)[local];}
        constexpr decltype(bucket->begin()) operator->() const {return bucket->begin()+local;}
        iterator_tp_& operator++(){
            ++local;
            if(local==bucket->size()){
                ++bucket;
                local=0;
            }
            return *this;
        }
        iterator_tp_& operator--(){
            if(bucket==vms->bucket_end() || local==0){
                --bucket;
                local=bucket->size();
            }
            --local;
            return *this;
        }
        iterator_tp_ operator++(int) {iterator_tp_ tmp = *this; operator++(); return tmp;}
        iterator_tp_ operator--(int) {iterator_tp_ tmp = *this; operator--(); return tmp;}

        friend constexpr bool operator==(const iterator_tp_ lhs,const iterator_tp_ rhs) noexcept {
            return lhs.vms==rhs.vms
                && lhs.bucket==rhs.bucket
                && (lhs.bucket==lhs.vms->bucket_end() || lhs.local==rhs.local);
        }

        constexpr operator iterator_tp_<add_lconst_t<VMSp>,add_lconst_t<IB>>() const noexcept {return {vms,bucket,local};}

        friend constexpr iterator_tp_<remove_lconst_t<VMSp>,remove_lconst_t<IB>> const_kast(const iterator_tp_<VMSp,IB> it) {using yuki::const_kast; return {const_kast(it.vms),const_kast(it.bucket),it.local};}
    };
  public:
    typedef iterator_tp_<Basic_VMultiSet*,non_const_bucket_iterator> non_const_iterator;
    typedef iterator_tp_<const Basic_VMultiSet*,const_bucket_iterator> const_iterator;

    const_iterator begin() const {return {this,Tree_Base::begin()};}
    const_iterator end() const {return {this,Tree_Base::end()};}

    const_iterator min() const { return {this,Tree_Base::min()};}
    const_iterator max() const {
        const const_bucket_iterator m = Tree_Base::max();
        const typename bucket_type::size_type l = m!=Tree_Base::end() ? m->size()-1 : 0;
        return {this,m,l};
    }

    const_iterator first_greater(const K& k) const {return {this,Tree_Base::first_greater(k)};}
    const_iterator first_equiv_greater(const K& k) const {return {this,Tree_Base::first_equiv_greater(k)};}
    const_iterator find(const K& k) const {return {this,Tree_Base::find_any(k)};}
    using Tree_Base::contains;

    size_type count(const K& k) const {
        const const_bucket_iterator f = Tree_Base::find_any(k);
        return f!=Tree_Base::end() ? f->size() : 0;
    }

    const_iterator insert(const K& k){
        const yuki::IB_Pair<const_bucket_iterator> ibp = Tree_Base::template emplace_unique_at(k,yuki::from_variadic_tag,k);
        typename bucket_type::size_type l=0;
        using yuki::const_kast;
        value_type& v = *const_kast(ibp.iterator);
        if(!ibp.has_inserted){
            v.emplace_back(k);
            l = v.size()-1;
        }
        ++s_;
        return {this,ibp.iterator,l};
    }

    const_iterator insert(K&& k){
        const yuki::IB_Pair<const_bucket_iterator> ibp = Tree_Base::template emplace_unique_at(k,yuki::from_variadic_tag,std::move(k));
        typename bucket_type::size_type l=0;
        using yuki::const_kast;
        value_type& v = *const_kast(ibp.iterator);
        if(!ibp.has_inserted){
            v.emplace_back(std::move(k));
            l = v.size()-1;
        }
        ++s_;
        return {this,ibp.iterator,l};
    }

    void erase(const_iterator i){
        using yuki::const_kast;
        const non_const_bucket_iterator ncb = const_kast(i.bucket);
        ncb->erase(i.bucket->begin()+i.local);
        if(ncb->empty())
            Tree_Base::erase(ncb);
        --s_;
    }

    size_type erase(const K& k){
        const const_bucket_iterator i = Tree_Base::find_any(k);
        if(i!=Tree_Base::end()){
            size_type bs = i->size();
            Tree_Base::erase(i);
            s_ -= bs;
            return bs;
        }
        return 0;
    }

  private:
    size_type s_ = 0;
};
} // namespace yuki
