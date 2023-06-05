#pragma once
#include<cstddef>
#include<cassert>
#include<initializer_list>
#include<stdexcept>
#include<array>
#include<yuki/tmp.hpp>
#include<yuki/Pair.hpp>
#include<yuki/Allocator.hpp>

namespace yuki{
template<typename K,typename V,typename KV,typename H,typename KE,typename VN,size_t slots>
struct CHashTable : private KV,private H,private KE,private VN{
    static_assert(slots>0 && (slots&(slots-1))==0); // Power of 2

    typedef K key_type;
    typedef V value_type;
    typedef KV key_of_value_type;
    typedef H hasher;
    typedef KE key_equal;
    typedef VN value_nil;

    static constexpr size_t max_size = slots;
  private:
    std::array<V,slots> storage;
    struct Range{
        yuki::uint_auto_t<slots> begin;
        yuki::uint_auto_t<slots> end;
    };
    std::array<Range,slots> range;
    size_t size_;
  public:
    constexpr CHashTable() noexcept :
        storage{},
        range{},
        size_(0)
    {}

    constexpr CHashTable(const CHashTable& other) noexcept :
        KV(yuki::select_on_container_copy_construction<KV>(other)),
        H(yuki::select_on_container_copy_construction<H>(other)),
        KE(yuki::select_on_container_copy_construction<KE>(other)),
        VN(yuki::select_on_container_copy_construction<VN>(other)),
        storage(other.storage),
        range(other.range),
        size_(other.size_)
    {}

    // No move ctor/assignment provided, as it could possibly break the moved-from `CHashTable`.

    // Well, "emplace" is in fact not a very appropriate name for the following functions, since an assignment is to be done instead of a construction.

    /// @pre `KE::operator(KV::operator(V(v)),k)`
    template<typename V2>
    constexpr V* emplace_at(const K& k,V2&& v){
        const size_t h = H::operator()(k);
        for(size_t i=range[h%slots].end;i<slots;++i){
            const size_t dh = (h+i*(h%2==0?h+1:h))%slots;
            if(VN::operator()(storage[dh])){
                storage[dh] = std::forward<V2>(v);
                if(range[h%slots].begin==0 && range[h%slots].end==0)
                    range[h%slots].begin=i;
                range[h%slots].end = i+1;
                ++size_;
                return &(storage[dh]); // Well, this should really be `std::addressof`, but because the header "<memory>" is kind of huge, and also because people don't usually overload `operator&`, I've decided to go for a plain `&`.
            }
        }
        assert(false); // The table is full. Please increase `slots`.
        std::unreachable();
    }

    template<typename V2>
    constexpr V* emplace(V2&& v) {return emplace_at(KV::operator()(v),std::forward<V2>(v));}

    /// @pre `KE::operator(KV::operator(V(v)),k)`
    template<typename K2,typename V2>
    constexpr yuki::IB_Pair<V*> emplace_unique_at_tp(const K2& k,V2&& v){
        const size_t h = H::operator()(k);
        const size_t re = range[h%slots].end;

        size_t i=range[h%slots].begin;

        for(;i<re;++i)
            if(KE::operator()(k,KV::operator()(storage[(h+i*(h%2==0?h+1:h))%slots])))
                return {&(storage[(h+i*(h%2==0?h+1:h))%slots]),false};

        for(;i<slots;++i){
            const size_t dh = (h+i*(h%2==0?h+1:h))%slots;
            if(VN::operator()(storage[dh])){
                storage[dh] = std::forward<V2>(v);
                if(range[h%slots].begin==0 && range[h%slots].end==0)
                    range[h%slots].begin=i;
                range[h%slots].end = i+1;
                ++size_;
                return {&(storage[dh]),true};
            }
        }
        assert(false); // The table is full. Please increase `slots`.
        std::unreachable();
    }

    template<typename V2>
    constexpr yuki::IB_Pair<V*> emplace_unique_at(const K& k,V2&& v) {return emplace_unique_at_tp(k,std::forward<V2>(v));}

    template<typename K2,typename V2>
    constexpr yuki::IB_Pair<V*> emplace_unique_at(const K2& k,V2&& v) requires requires{typename KE::is_transparent;}
        {return emplace_unique_at(k,std::forward<V2>(v));}

    template<typename V2>
    constexpr yuki::IB_Pair<V*> emplace_unique_tp(V2&& v) {return emplace_unique_at_tp(KV::operator()(v),std::forward<V2>(v));}

    constexpr yuki::IB_Pair<V*> emplace_unique(const V& v) {return emplace_unique_at_tp(KV::operator()(v),v);}

    constexpr yuki::IB_Pair<V*> emplace_unique(V&& v) {return emplace_unique_at_tp(KV::operator()(v),std::move(v));}

    template<typename V2>
    constexpr yuki::IB_Pair<V*> emplace_unique(V2&& v) requires requires{typename KE::is_transparent;}
        {return emplace_unique_at_tp(KV::operator()(v),std::forward<V2>(v));}

    template<typename It>
    constexpr CHashTable(It it,size_t s) noexcept :
        storage{},
        range{},
        size_(0)
    {
        for(;s!=0;++it,--s)
            emplace(*it);
    }

    template<typename V2,size_t N>
    constexpr CHashTable(const V2(&arr)[N]) noexcept :
        CHashTable(arr,N)
    {}

    constexpr CHashTable(std::initializer_list<V> il) noexcept :
        CHashTable(il.begin(),il.size())
    {}

    template<typename It>
    constexpr CHashTable(yuki::unique_tag_t,It it,size_t s) noexcept :
        storage{},
        range{},
        size_(0)
    {
        for(;s!=0;++it,--s)
            emplace_unique(*it);
    }

    template<typename V2,size_t N>
    constexpr CHashTable(yuki::unique_tag_t,const V2(&arr)[N]) noexcept :
        CHashTable(yuki::unique_tag,arr,N)
    {}

    constexpr CHashTable(yuki::unique_tag_t,std::initializer_list<V> il) noexcept :
        CHashTable(yuki::unique_tag,il.begin(),il.size())
    {}

    constexpr size_t size() const {return size_;}

    template<typename K2>
    constexpr const V* find_tp(const K2& k) const {
        const size_t h = H::operator()(k);
        const yuki::uint_auto_t<slots> end = range[h%slots].end;
        for(yuki::uint_auto_t<slots> begin=range[h%slots].begin;begin!=end;++begin)
            if(KE::operator()(k,KV::operator()(storage[(h+begin*(h%2==0?h+1:h))%slots])))
                return &(storage[(h+begin*(h%2==0?h+1:h))%slots]);
        return nullptr;
    }
    constexpr const V* find(const K& k) const {return find_tp(k);}
    template<typename K2>
    constexpr const V* find(const K2& k) const requires requires{typename KE::is_transparent;} {return find_tp(k);}

    constexpr const V& operator[](const K& k) const {return *find(k);}

    template<typename K2>
    const V& at_tp(const K2& k) const noexcept(false) {
        if(const V* const p=find_tp(k); p)
            return *p;
        else
            throw std::out_of_range("yuki::CHashTable::at(_tp) const: Out of range!");
    }
    const V& at(const K& k) const noexcept(false) {return at_tp(k);}
    template<typename K2>
    constexpr const V& at(const K2& k) const noexcept(false) requires requires{typename KE::is_transparent;} {return at_tp(k);}
};
} // namespace yuki
