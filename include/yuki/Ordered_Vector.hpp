#pragma once
#include<yuki/Vector.hpp>
#include<yuki/algorithm.hpp>
#include<yuki/Pair.hpp>
#include<compare>

namespace yuki{

template<typename It>
struct Move_Iterator{
    It base;

    constexpr decltype(auto) operator*() const {return std::move(*base);}
    constexpr It operator->() const {return base;}
    constexpr Move_Iterator& operator++() {++base; return *this;}
    constexpr Move_Iterator operator++(int) {Move_Iterator orig=*this; ++base; return orig;}

    friend constexpr bool operator==(Move_Iterator,Move_Iterator) noexcept = default;
    friend constexpr std::strong_ordering operator<=>(Move_Iterator,Move_Iterator) noexcept = default;
};

template<typename K,typename V,typename KV,typename C=yuki::Less<K>,typename A=yuki::Allocator<V>,typename EC=yuki::Default_EC<2,1,4>>
struct Basic_Ordered_Vector : protected Vector<V,A,EC>, protected KV, protected C{
    typedef Vector<V,A,EC> vector_type;
    typedef K key_type;
    using typename vector_type::value_type;
    typedef KV key_of_value_type;
    typedef C key_compare;
    using typename vector_type::allocator_type;
    using typename vector_type::ec_type;

    using typename vector_type::pointer;
    using typename vector_type::const_pointer;
    using typename vector_type::size_type;
    using typename vector_type::difference_type;

    constexpr Basic_Ordered_Vector() noexcept = default;

    Basic_Ordered_Vector(const Basic_Ordered_Vector& other) noexcept:
        vector_type(static_cast<const vector_type&>(other)),
        KV(yuki::select_on_container_copy_construction<KV>(other)),
        C(yuki::select_on_container_copy_construction<C>(other))
    {}

    constexpr Basic_Ordered_Vector(Basic_Ordered_Vector&&) noexcept = default;

    Basic_Ordered_Vector& operator=(const Basic_Ordered_Vector& other) noexcept {
        vector_type::operator=(other);
        KV::operator=(other);
        C::operator=(other);
        return *this;
    }

    Basic_Ordered_Vector& operator=(Basic_Ordered_Vector&& other) noexcept {
        vector_type::operator=(static_cast<vector_type&&>(other));
        KV::operator=(static_cast<KV&&>(other));
        C::operator=(static_cast<C&&>(other));
        return *this;
    }

    friend constexpr void swap(Basic_Ordered_Vector& lhs,Basic_Ordered_Vector& rhs) noexcept {
        using std::swap;
        swap(static_cast<vector_type&>(lhs),static_cast<vector_type&>(rhs));
        swap(static_cast<KV&>(lhs),static_cast<KV&>(rhs));
        swap(static_cast<C&>(lhs),static_cast<C&>(rhs));
    }

    template<typename Cp = C&&,typename Ap = A&&,typename KVp = KV&&,typename ECp = EC&&,typename = std::enable_if_t<std::is_constructible_v<C,Cp&&>>>
    explicit constexpr Basic_Ordered_Vector(Cp&& cp, Ap&& ap = A{}, KVp&& kvp = KV{}, ECp&& ecp = EC{}) noexcept :
        vector_type(std::forward<Ap>(ap),std::forward<ECp>(ecp)),
        KV(std::forward<KVp>(kvp)),
        C(std::forward<Cp>(cp))
    {}

    template<typename Cp = C&&,typename Ap = A&&,typename KVp = KV&&,typename ECp = EC&&>
    Basic_Ordered_Vector(yuki::reserve_tag_t,const size_type count,Cp&& cp = C{}, Ap&& ap = A{}, KVp&& kvp = KV{}, ECp&& ecp= EC{}) noexcept :
        vector_type(yuki::reserve_tag,count,std::forward<Ap>(ap),std::forward<ECp>(ecp)),
        KV(std::forward<KVp>(kvp)),
        C(std::forward<Cp>(cp))
    {}

    template<typename Cp = C&&,typename Ap = A&&,typename KVp = KV&&,typename ECp = EC&&>
    Basic_Ordered_Vector(const size_type count,const V& t,Cp&& cp = C{}, Ap&& ap = A{}, KVp&& kvp = KV{}, ECp&& ecp= EC{}) noexcept :
        vector_type(count,t,std::forward<Ap>(ap),std::forward<ECp>(ecp)),
        KV(std::forward<KVp>(kvp)),
        C(std::forward<Cp>(cp))
    {}

    template<typename Cp = C&&,typename Ap = A&&,typename KVp = KV&&,typename ECp = EC&&,typename=std::enable_if_t<std::is_constructible_v<C,Cp&&>>>
    explicit Basic_Ordered_Vector(const size_type count,Cp&& cp = C{}, Ap&& ap = A{}, KVp&& kvp = KV{}, ECp&& ecp= EC{}) noexcept :
        vector_type(count,std::forward<Ap>(ap),std::forward<ECp>(ecp)),
        KV(std::forward<KVp>(kvp)),
        C(std::forward<Cp>(cp))
    {}

    template<typename Cp = C&&,typename Ap = A&&,typename KVp = KV&&,typename ECp = EC&&>
    Basic_Ordered_Vector(yuki::memset_tag_t,const size_type count,unsigned char c=0,Cp&& cp = C{}, Ap&& ap = A{}, KVp&& kvp = KV{}, ECp&& ecp= EC{}) noexcept :
        vector_type(yuki::memset_tag,count,c,std::forward<Ap>(ap),std::forward<ECp>(ecp)),
        KV(std::forward<KVp>(kvp)),
        C(std::forward<Cp>(cp))
    {}

    template<typename It,typename Cp = C&&,typename Ap = A&&,typename KVp = KV&&,typename ECp = EC&&>
    Basic_Ordered_Vector(yuki::from_ordered_tag_t,const It p,const size_type count,Cp&& cp = C{}, Ap&& ap = A{}, KVp&& kvp = KV{}, ECp&& ecp= EC{}) noexcept :
        vector_type(p,count,std::forward<Ap>(ap),std::forward<ECp>(ecp)),
        KV(std::forward<KVp>(kvp)),
        C(std::forward<Cp>(cp))
    {}

    template<typename Cp = C&&,typename Ap = A&&,typename KVp = KV&&,typename ECp = EC&&>
    Basic_Ordered_Vector(yuki::from_ordered_tag_t,std::initializer_list<V> il,Cp&& cp = C{}, Ap&& ap = A{}, KVp&& kvp = KV{}, ECp&& ecp= EC{}) noexcept :
        vector_type(il.begin(),il.size(),std::forward<Ap>(ap),std::forward<ECp>(ecp)),
        KV(std::forward<KVp>(kvp)),
        C(std::forward<Cp>(cp))
    {}

    template<typename... Args,typename Cp = C&&,typename Ap = A&&,typename KVp = KV&&,typename ECp = EC&&,typename = std::enable_if_t<std::is_constructible_v<C,Cp&&>>>
    Basic_Ordered_Vector(yuki::from_ordered_tag_t,yuki::from_variadic_tag_t,Cp&& cp,Ap&& ap,KVp&& kvp,ECp&& ecp,Args&&... args) noexcept :
        vector_type(yuki::from_variadic_tag,std::forward<Ap>(ap),std::forward<ECp>(ecp),std::forward<Args>(args)...),
        KV(std::forward<KVp>(kvp)),
        C(std::forward<Cp>(cp))
    {}

    template<typename... Args>
    Basic_Ordered_Vector(yuki::from_ordered_tag_t,yuki::from_variadic_tag_t,Args&&... args) noexcept :
        vector_type(yuki::from_variadic_tag,A(),EC(),std::forward<Args>(args)...),
        KV(),
        C()
    {}

    void assign(size_type count,const V& t){
        vector_type::clear();
        vector_type::reserve(count);
        for(;count;--count)
            vector_type::emplace_back(t);
    }

    template<typename It>
    void assign(yuki::from_ordered_tag_t,It p,size_type count){
        vector_type::clear();
        vector_type::reserve(count);
        for(;count;++p,--count)
            vector_type::emplace_back(*p);
    }

    void assign(yuki::from_ordered_tag_t,std::initializer_list<V> il){
        assign(yuki::from_ordered_tag,il.begin(),il.size());
    }

    template<typename... Args>
    void assign(yuki::from_ordered_tag_t,yuki::from_variadic_tag_t,Args&&... args){
        vector_type::clear();
        vector_type::reserve(sizeof...(Args));
        (...,vector_type::emplace_back(*args));
    }

    using vector_type::clear;
    using vector_type::deallocate;
    using vector_type::reset;
    using vector_type::empty;
    using vector_type::size;
    using vector_type::capacity;

    constexpr vector_type& vector() {return *this;}
    constexpr const vector_type& vector() const {return *this;}

    constexpr const V& front() const {return vector_type::front();}
    constexpr const V& back() const {return vector_type::back();}
    constexpr const V& operator[](const size_type i) const {return vector_type::operator[](i);}

    typedef typename vector_type::iterator non_const_iterator;
    using typename vector_type::const_iterator;
    constexpr const_iterator begin() const {return vector_type::begin();}
    constexpr const_iterator end() const {return vector_type::end();}

    using vector_type::reserve;
    using vector_type::pop_back;
    using vector_type::pop_back_v;
    using vector_type::recapacity;
    using vector_type::shrink_to_fit;

    constexpr const_iterator min() const {return vector_type::begin();}
    constexpr const_iterator max() const {return empty() ? end() : end()-1;}

    template<typename K2>
    const_iterator first_equiv_greater_tp(const K2& k) const {
        return yuki::first_equiv_greater(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    const_iterator first_equiv_greater(const K& k) const {
        return yuki::first_equiv_greater(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    template<typename K2>
    const_iterator first_equiv_greater(const K2& k) const requires requires{typename C::is_transparent;} {
        return yuki::first_equiv_greater(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }

    template<typename K2>
    const_iterator first_greater_tp(const K2& k) const {
        return yuki::first_greater(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    const_iterator first_greater(const K& k) const {
        return yuki::first_greater(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    template<typename K2>
    const_iterator first_greater(const K2& k) const requires requires{typename C::is_transparent;} {
        return yuki::first_greater(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }

    template<typename K2>
    const_iterator last_less_tp(const K2& k) const {
        return yuki::last_less(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    const_iterator last_less(const K& k) const {
        return yuki::last_less(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    template<typename K2>
    const_iterator last_less(const K2& k) const requires requires{typename C::is_transparent;} {
        return yuki::last_less(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }

    template<typename K2>
    const_iterator last_less__equivtp(const K2& k) const {
        return yuki::last_less_equiv(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    const_iterator last_less_equiv(const K& k) const {
        return yuki::last_less_equiv(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    template<typename K2>
    const_iterator last_less_equiv(const K2& k) const requires requires{typename C::is_transparent;} {
        return yuki::last_less_equiv(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }

    template<typename K2>
    const_iterator find_any_tp(const K2& k) const {
        return yuki::bfind(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    const_iterator find_any(const K& k) const {
        return yuki::bfind(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    template<typename K2>
    const_iterator find_any(const K2& k) const requires requires{typename C::is_transparent;} {
        return yuki::bfind(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }

    template<typename K2>
    bool contains_tp(const K2& k) const {
        return yuki::bcontains(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    bool contains(const K& k) const {
        return yuki::bcontains(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }
    template<typename K2>
    bool contains(const K2& k) const requires requires{typename C::is_transparent;} {
        return yuki::bcontains(begin(),end(),k,static_cast<const KV&>(*this),static_cast<const C&>(*this));
    }

    template<typename K2>
    size_type count_tp(const K2& k) const {
        return first_greater_tp(k) - first_equiv_greater_tp(k);
    }
    size_type count(const K& k) const {return count_tp(k);}
    template<typename K2>
    size_type count(const K2& k) const requires requires{typename C::is_transparent;}
        {return count_tp(k);}

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename K2,typename... Args>
    const_iterator emplace_at_tp(const K2& k,Args&&... args){
        return vector_type::emplace(first_greater_tp(k),std::forward<Args>(args)...);
    }
    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename... Args>
    const_iterator emplace_at(const K& k,Args&&... args){
        return vector_type::emplace(first_greater_tp(k),std::forward<Args>(args)...);
    }
    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename K2,typename... Args>
    const_iterator emplace_at(const K2& k,Args&&... args) requires requires{typename C::is_transparent;} {
        return vector_type::emplace(first_greater_tp(k),std::forward<Args>(args)...);
    }

    const_iterator insert(const V& v) {return emplace_at_tp(KV::opetator()(v),v);}
    const_iterator insert(V&& v) {return emplace_at_tp(KV::opetator()(v),std::move(v));}

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename K2,typename... Args>
    yuki::IB_Pair<const_iterator> emplace_unique_at_tp(const K2& k,Args&&... args){
        const_iterator fg = first_greater_tp(k);
        if(fg==begin() || C::operator()(KV::operator()(*(fg-1)),k))
            return {vector_type::emplace(fg,std::forward<Args>(args)...),true};
        else
            return {--fg,false};
    }

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename... Args>
    yuki::IB_Pair<const_iterator> emplace_unique_at(const K& k,Args&&... args)
        {return emplace_unique_at_tp(k,std::forward<Args>(args)...);}

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename K2,typename... Args>
    yuki::IB_Pair<const_iterator> emplace_unique_at(const K2& k,Args&&... args) requires requires{typename C::is_transparent;}
        {return emplace_unique_at_tp(k,std::forward<Args>(args)...);}

    yuki::IB_Pair<const_iterator> insert_unique(const V& v) {return emplace_unique_at_tp(KV::operator()(v),v);}

    yuki::IB_Pair<const_iterator> insert_unique(V&& v) {return emplace_unique_at_tp(KV::operator()(v),std::move(v));}

    const_iterator erase(const const_iterator cpos,const size_type count_erase=1) {return vector_type::erase(cpos,count_erase);}

    template<typename K2>
    size_type erase_tp(const K2& k){
        const const_iterator feg = first_equiv_greater_tp(k);
        const const_iterator fg = first_greater_tp(k);
        erase(feg,fg-feg);
        return fg-feg;
    }
    size_type erase(const K& k) {return erase_tp(k);}
    template<typename K2>
    size_type erase(const K2& k) requires requires{typename C::is_transparent;}
        {return erase_tp(k);}

    template<typename K2>
    size_type erase_unique_tp(const K2& k){
        if(const const_iterator i=find_any_tp(k); i!=end()){
            erase(i);
            return 1;
        }
        return 0;
    }
    size_type erase_unique(const K& k) {return erase_unique_tp(k);}
    template<typename K2>
    size_type erase_unique(const K2& k) requires requires{typename C::is_transparent;}
        {return erase_unique_tp(k);}

  private:
    struct Default_Merger{
        template<typename D,typename S>
        static constexpr void operator()(D&,const S&) {}
    };
  public:
    // Note: All "merge_out_of_place" functions assumes that the values in `other` and `*this`, when mapped to `K` via `swap_zone.KV::operator()`, satisfy `swap_zone.C::operator()`.
    template<typename K2,typename KV2,typename C2,typename A2,typename EC2,typename M=Default_Merger>
    void merge_out_of_place(const Basic_Ordered_Vector<K2,V,KV2,C2,A2,EC2>& other,Basic_Ordered_Vector& swap_zone,M&& m=Default_Merger{})
        {merge_out_of_place_impl<false,false>(other.begin(),other.end(),swap_zone,std::forward<M>(m));}
    template<typename K2,typename KV2,typename C2,typename EC2,typename M=Default_Merger>
    void merge_out_of_place(Basic_Ordered_Vector<K2,V,KV2,C2,A,EC2>&& other,Basic_Ordered_Vector& swap_zone,M&& m=Default_Merger{})
        {merge_out_of_place_impl<false,true>(Move_Iterator{other.vector_type::begin()},Move_Iterator{other.vector_type::end()},swap_zone,std::forward<M>(m)); other.deallocate();}
    template<typename K2,typename KV2,typename C2,typename A2,typename EC2,typename M=Default_Merger>
    void merge_out_of_place(const Basic_Ordered_Vector<K2,V,KV2,C2,A2,EC2>& other,M&& m=Default_Merger{})
        {Basic_Ordered_Vector swap_zone(yuki::reserve_tag,size()+other.size()); merge_out_of_place_impl<false,false>(other.begin(),other.end(),swap_zone,std::forward<M>(m));}
    template<typename K2,typename KV2,typename C2,typename EC2,typename M=Default_Merger>
    void merge_out_of_place(Basic_Ordered_Vector<K2,V,KV2,C2,A,EC2>&& other,M&& m=Default_Merger{})
        {Basic_Ordered_Vector swap_zone(yuki::reserve_tag,size()+other.size()); merge_out_of_place_impl<false,true>(Move_Iterator{other.vector_type::begin()},Move_Iterator{other.vector_type::end()},swap_zone,std::forward<M>(m)); other.deallocate();}

    template<typename K2,typename KV2,typename C2,typename A2,typename EC2,typename M=Default_Merger>
    void merge_unique_out_of_place(const Basic_Ordered_Vector<K2,V,KV2,C2,A2,EC2>& other,Basic_Ordered_Vector& swap_zone,M&& m=Default_Merger{})
        {merge_out_of_place_impl<true,false>(other.begin(),other.end(),swap_zone,std::forward<M>(m));}
    template<typename K2,typename KV2,typename C2,typename EC2,typename M=Default_Merger>
    void merge_unique_out_of_place(Basic_Ordered_Vector<K2,V,KV2,C2,A,EC2>&& other,Basic_Ordered_Vector& swap_zone,M&& m=Default_Merger{})
        {merge_out_of_place_impl<true,true>(Move_Iterator{other.vector_type::begin()},Move_Iterator{other.vector_type::end()},swap_zone,std::forward<M>(m)); other.deallocate();}
    template<typename K2,typename KV2,typename C2,typename A2,typename EC2,typename M=Default_Merger>
    void merge_unique_out_of_place(const Basic_Ordered_Vector<K2,V,KV2,C2,A2,EC2>& other,M&& m=Default_Merger{})
        {Basic_Ordered_Vector swap_zone(yuki::reserve_tag,size()+other.size()); merge_out_of_place_impl<true,false>(other.begin(),other.end(),swap_zone,std::forward<M>(m));}
    template<typename K2,typename KV2,typename C2,typename EC2,typename M=Default_Merger>
    void merge_unique_out_of_place(Basic_Ordered_Vector<K2,V,KV2,C2,A,EC2>&& other,M&& m=Default_Merger{})
        {Basic_Ordered_Vector swap_zone(yuki::reserve_tag,size()+other.size()); merge_out_of_place_impl<true,true>(Move_Iterator{other.vector_type::begin()},Move_Iterator{other.vector_type::end()},swap_zone,std::forward<M>(m)); other.deallocate();}

    template<typename K2,typename KV2,typename C2,typename A2,typename EC2,typename M=Default_Merger>
    void merge(const Basic_Ordered_Vector<K2,V,KV2,C2,A2,EC2>& other,M&& m=Default_Merger{}) {merge_out_of_place(other,std::forward<M>(m));}
    template<typename K2,typename KV2,typename C2,typename EC2,typename M=Default_Merger>
    void merge(Basic_Ordered_Vector<K2,V,KV2,C2,A,EC2>&& other,M&& m=Default_Merger{}) {merge_out_of_place(std::move(other),std::forward<M>(m));}

    template<typename K2,typename KV2,typename C2,typename A2,typename EC2,typename M=Default_Merger>
    void merge_unique(const Basic_Ordered_Vector<K2,V,KV2,C2,A2,EC2>& other,M&& m=Default_Merger{}) {merge_unique_out_of_place(other,std::forward<M>(m));}
    template<typename K2,typename KV2,typename C2,typename EC2,typename M=Default_Merger>
    void merge_unique(Basic_Ordered_Vector<K2,V,KV2,C2,A,EC2>&& other,M&& m=Default_Merger{}) {merge_unique_out_of_place(std::move(other),std::forward<M>(m));}

  private:
    template<bool unique,bool destroy,typename It,typename M>
    void merge_out_of_place_impl(It r,const It re,Basic_Ordered_Vector& swap_zone,M&& m){
        non_const_iterator l = vector_type::begin();
        const non_const_iterator le = vector_type::end();

        auto append_back = [&swap_zone,&m](const auto i){
            if constexpr(unique){
                if(swap_zone.empty() || swap_zone.C::operator()(swap_zone.KV::operator()(swap_zone.back()),swap_zone.KV::operator()(*i)))
                    swap_zone.vector_type::emplace_back(*i);
                else
                    std::forward<M>(m)(swap_zone.back(),*i);
            }else
                swap_zone.vector_type::emplace_back(*i);
            if constexpr(destroy)
                i->~V();
        };

        while(1){
            if(l==le){
                for(;r!=re;++r)
                    append_back(r);
                break;
            }else{
                if(r==re){
                    for(;l!=le;++l)
                        append_back(l);
                    break;
                }else{
                    if(swap_zone.C::operator()(swap_zone.KV::operator()(*r),swap_zone.KV::operator()(*l)))
                        append_back(r++);
                    else
                        append_back(l++);
                }
            }
        }

        vector_type::force_clear();
        swap(*this,swap_zone);
    }

    friend Basic_Ordered_Vector operator+(const Basic_Ordered_Vector& lhs,const Basic_Ordered_Vector& rhs){
        Basic_Ordered_Vector ov(yuki::reserve_tag,lhs.size()+rhs.size());

        const_iterator l = lhs.begin();
        const_iterator r = rhs.begin();
        const const_iterator le = lhs.end();
        const const_iterator re = rhs.end();

        while(1){
            if(l==le){
                for(;r!=re;++r)
                    ov.vector_type::emplace_back(*r);
                break;
            }else{
                if(r==re){
                    for(;l!=le;++l)
                        ov.vector_type::emplace_back(*l);
                    break;
                }else{
                    if(ov.C::operator()(ov.KV::operator()(*r),ov.KV::operator()(*l)))
                        ov.vector_type::emplace_back(*r++);
                    else
                        ov.vector_type::emplace_back(*l++);
                }
            }
        }

        return ov;
    }
}; // struct Basic_Ordered_Vector<K,V,KV,C,A,EC>


//template<typename It>
//auto merge(It it,size_t count) -> std::remove_cvref_t<decltype(*it)> {
//    typedef std::remove_cvref_t<decltype(*it)> OV;
//    typedef typename OV::const_iterator const_iterator;

//    OV ov(yuki::reserve_tag,lhs.size()+rhs.size());

//    const_iterator l = lhs.begin();
//    const_iterator r = rhs.begin();
//    const const_iterator le = lhs.end();
//    const const_iterator re = rhs.end();

//    const_iterator i;

//    while(1){
//        if(l==le){
//            if(r==re)
//                return ov;
//            else
//               i = r++;
//        }else{
//            if(r==re)
//                i = l++;
//            else{
//                if(ov.compare(*r,*l))
//                    i = r++;
//                else
//                    i = l++;
//            }
//        }
//        ov.vector().emplace_back(*i);
//    }
//}

template<typename T,typename C = yuki::Less<T>,typename A=yuki::Allocator<T>,typename EC=yuki::Default_EC<2,1,4>>
using Ordered_Vector = Basic_Ordered_Vector<T,T,Iden,C,A,EC>;
} // namespace yuki
