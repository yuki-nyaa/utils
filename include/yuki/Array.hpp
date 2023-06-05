// Experimental
#pragma once
#include<cstddef>
#include<cassert>
#include<utility>
#include<concepts>
#include<yuki/tmp.hpp>

namespace yuki{
template<typename T,size_t s>
struct Array{
    T data[s];
    static constexpr size_t size = s;

    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    friend constexpr void swap(Array& lhs,Array& rhs) noexcept {
        using std::swap;
        for(size_t i=0;i<s;++i)
            swap(lhs.data[i],rhs.data[i]);
    }

    constexpr T* begin() {return data;}
    constexpr const T* begin() const {return data;}
    constexpr T* end() {return data+s;}
    constexpr const T* end() const {return data+s;}

    friend constexpr bool operator==(const Array&,const Array&) noexcept = default;
    friend constexpr std::strong_ordering operator<=>(const Array&,const Array&) noexcept = default;

    constexpr T& operator[](const size_t i) {assert(i<s);return data[i];}
    constexpr const T& operator[](const size_t i) const {assert(i<s);return data[i];}
};


template<std::unsigned_integral T,T n>
constexpr yuki::Array<T,n> make_integer_array(){
    yuki::Array<T,n> ret;
    for(T i=0;i<n;++i)
        ret.data[i]=i;
    return ret;
}

template<std::unsigned_integral T,T n>
inline constexpr yuki::Array<T,n> integer_array = make_integer_array<T,n>();

template<size_t n>
constexpr yuki::Array<yuki::uint_auto_t<n>,n> make_index_array() {return make_integer_array<yuki::uint_auto_t<n>,n>();}

template<size_t n>
inline constexpr yuki::Array<yuki::uint_auto_t<n>,n> index_array = make_integer_array<yuki::uint_auto_t<n>,n>();
} // namespace yuki
