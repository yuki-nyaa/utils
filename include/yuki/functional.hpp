#pragma once
#include<compare>

namespace yuki{

template<typename F>
struct Transparent : private F{
    using F::F;
    using F::operator();
    typedef void is_transparent;
};


inline constexpr std::compare_three_way compare3;


template<typename T=void>
struct Less{
    static constexpr bool operator()(const T& lhs,const T& rhs) {return lhs<rhs;}
};

template<>
struct Less<void>{
    template<typename T,typename U>
    static constexpr decltype(auto) operator()(const T& lhs,const U& rhs) {return lhs<rhs;}
    template<typename T,typename U>
    static constexpr bool operator()(T* const lhs,U* const rhs) {return compare3(lhs,rhs)<0;}
    typedef void is_transparent;
};

template<typename T>
struct Less<T*>{
    static constexpr bool operator()(T* const lhs,T* const rhs) {return compare3(lhs,rhs)<0;}
};


template<typename T=void>
struct Greater{
    static constexpr bool operator()(const T& lhs,const T& rhs) {return lhs>rhs;}
};

template<>
struct Greater<void>{
    template<typename T,typename U>
    static constexpr decltype(auto) operator()(const T& lhs,const U& rhs) {return lhs>rhs;}
    template<typename T,typename U>
    static constexpr bool operator()(T* const lhs,U* const rhs) {return compare3(lhs,rhs)>0;}
    typedef void is_transparent;
};

template<typename T>
struct Greater<T*>{
    static constexpr bool operator()(T* const lhs,T* const rhs) {return compare3(lhs,rhs)>0;}
};


template<typename T=void>
struct Equal_To{
    static constexpr bool operator()(const T& lhs,const T& rhs) {return lhs==rhs;}
};

template<>
struct Equal_To<void>{
    template<typename T,typename U>
    static constexpr decltype(auto) operator()(const T& lhs,const U& rhs) {return lhs==rhs;}
    typedef void is_transparent;
};


struct Iden{
    template<typename V>
    static constexpr V&& operator()(V&& v) {return static_cast<V&&>(v);}
    typedef void is_transparent;
};


struct False{
    template<typename... Args>
    static constexpr bool operator()(const Args&...) {return false;}
    typedef void is_transparent;
};


template<size_t num=2,size_t denom=1,size_t lb=1,size_t ub=(size_t)-1>
struct Default_EC{
    static_assert(denom!=0);
    static_assert(num>=denom);
    static_assert(lb<=ub);

    static constexpr size_t numerator = num;
    static constexpr size_t denominator = denom;
    static constexpr size_t lower_bound = lb;
    static constexpr size_t upper_bound = ub;
    static constexpr size_t operator()(const size_t size,const size_t cap,const size_t added){
        size_t added_pending = (num-denom)*cap/denom;
        if(added_pending < lb)
            added_pending = lb;
        if(cap+added_pending < size+added)
            return size+added;
        return added_pending>ub ? cap+ub : cap+added_pending;
    }
};
} // namespace yuki
