#pragma once
#include<iterator>
#include<utility>
#include<yuki/functional.hpp>

namespace yuki{
template<typename T,typename U>
constexpr auto& min(const T& lhs,const U& rhs) {return lhs<rhs ? lhs : rhs;}
template<typename T,typename U,typename C>
constexpr auto& min(const T& lhs,const U& rhs,C&& c) {return std::forward<C>(c)(lhs,rhs) ? lhs : rhs;}
template<typename T,typename U>
constexpr auto& max(const T& lhs,const U& rhs) {return lhs<rhs ? rhs : lhs;}
template<typename T,typename U,typename C>
constexpr auto& max(const T& lhs,const U& rhs,C&& c) {return std::forward<C>(c)(lhs,rhs) ? rhs : lhs;}


/// Weirdly, `std::count(_if)` returns `difference_type` rather than `size_type`. So I come up with this.
template<typename It,typename T>
constexpr size_t count(It first,const It last,const T& value){
    size_t n = 0;
    for(;first!=last;++first)
        if(*first==value)
            ++n;
    return n;
}
template<typename It,typename UnaryPredicate>
constexpr size_t count_if(It first,const It last,UnaryPredicate&& p){
    size_t n = 0;
    for(;first!=last;++first)
        if(std::forward<UnaryPredicate>(p)(*first))
          ++n;
    return n;
}


template<typename It,typename K,typename KV,typename C>
constexpr It first_equiv_greater(It first,const It end,const K& k,KV&& kv,C&& comp){
    size_t count = end-first;
    if(count==0 || std::forward<C>(comp)(std::forward<KV>(kv)(*(end-1)),k)) // back()<k
        return end;

    size_t step;
    It it{};
    while(count>0){
        step = count/2;
        it = first + step;
        if(!std::forward<C>(comp)(std::forward<KV>(kv)(*it),k)){ // *it >= k
            count = step;
        }else{
            first = ++it;
            count -= step+1;
        }
    }
    return first;
}

template<typename It,typename K>
constexpr It first_equiv_greater(It first,const It end,const K& k){
    return first_equiv_greater(first,end,k,yuki::Iden{},yuki::Less<void>{});
}

template<typename It,typename K,typename KV,typename C>
constexpr It first_greater(It first,const It end,const K& k,KV&& kv,C&& comp){
    size_t count = end-first;
    if(count==0 || !std::forward<C>(comp)(k,std::forward<KV>(kv)(*(end-1)))) // back()<=k
        return end;

    size_t step;
    It it{};
    while(count>0){
        step = count/2;
        it = first + step;
        if(std::forward<C>(comp)(k,std::forward<KV>(kv)(*it))){ // *it > k
            count = step;
        }else{
            first = ++it;
            count -= step+1;
        }
    }
    return first;
}

template<typename It,typename K>
constexpr It first_greater(It first,const It end,const K& k){
    return first_greater(first,end,k,yuki::Iden{},yuki::Less<void>{});
}

template<typename It,typename K,typename KV,typename C>
constexpr It last_less(It first,const It end,const K& k,KV&& kv,C&& comp){
    size_t count = end-first;
    if(count==0 || !std::forward<C>(comp)(std::forward<KV>(kv)(*first),k)) // front()>=k
        return end;

    size_t step = count/2;
    It it = first+step;
    while(step>0){
        if(std::forward<C>(comp)(std::forward<KV>(kv)(*it),k)){ // *it < k
            first = it;
            count -= step;
        }else{
            count = step;
        }
        step = count/2;
        it = first + step;
    }
    return first;
}

template<typename It,typename K>
constexpr It last_less(It first,const It end,const K& k){
    return last_less(first,end,k,yuki::Iden{},yuki::Less<void>{});
}

template<typename It,typename K,typename KV,typename C>
constexpr It last_less_equiv(It first,const It end,const K& k,KV&& kv,C&& comp){
    size_t count = end-first;
    if(count==0 || std::forward<C>(comp)(k,std::forward<KV>(kv)(*first))) // front()>k
        return end;

    size_t step = count/2;
    It it = first+step;
    while(step>0){
        if(!std::forward<C>(comp)(k,std::forward<KV>(kv)(*it))){ // *it <= k
            first = it;
            count -= step;
        }else{
            count = step;
        }
        step = count/2;
        it = first + step;
    }
    return first;
}

template<typename It,typename K>
constexpr It last_less_equiv(It first,const It end,const K& k){
    return last_less_equiv(first,end,k,yuki::Iden{},yuki::Less<void>{});
}

template<typename It,typename K,typename KV,typename C>
constexpr It bfind(It first,const It end,const K& k,KV&& kv,C&& comp){
    const It feg = first_equiv_greater(first,end,k,std::forward<KV>(kv),std::forward<C>(comp));
    return (feg!=end && !std::forward<C>(comp)(k,std::forward<KV>(kv)(*feg))) ? feg : end;
}

template<typename It,typename K>
constexpr It bfind(It first,const It end,const K& k){
    const It feg = first_equiv_greater(first,end,k);
    return (feg!=end && !(k<*feg)) ? feg : end;
}

template<typename It,typename K,typename KV,typename C>
constexpr bool bcontains(It first,const It end,const K& k,KV&& kv,C&& comp){
    const It feg = first_equiv_greater(first,end,k,std::forward<KV>(kv),std::forward<C>(comp));
    return (feg!=end && !std::forward<C>(comp)(k,std::forward<KV>(kv)(*feg))) ? true : false;
}

template<typename It,typename K>
constexpr bool bcontains(It first,const It end,const K& k){
   const It feg = first_equiv_greater(first,end,k);
    return (feg!=end && !(k<*feg)) ? true : false;
}

template<typename It,typename K,typename KV,typename C>
constexpr size_t bcount(It first,const It end,const K& k,KV&& kv,C&& comp){
    return first_greater(first,end,k,std::forward<KV>(kv),std::forward<C>(comp)) - first_equiv_greater(first,end,k,std::forward<KV>(kv),std::forward<C>(comp));
}

template<typename It,typename K>
constexpr size_t bcount(It first,const It end,const K& k){
    return first_greater(first,end,k) - first_equiv_greater(first,end,k);
}




template<typename It,typename F>
constexpr size_t remove_trailing(const It begin,It end,F&& f){
    while(end!=begin)
        if(!std::forward<F>(f)(*--end))
            return (++end)-begin;
    return end-begin;
}
} // namespace yuki
