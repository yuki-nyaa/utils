#pragma once
#include<type_traits>
#include<cstddef>
#include<utility>
#include<cstring>

namespace yuki{
template<typename It,typename size_type_p=size_t>
struct Span{
  private:
    It begin_;
    It end_;
  public:
    typedef std::remove_reference_t<decltype(*begin_)> value_type;
    typedef size_type_p size_type;

    constexpr Span() noexcept :
        begin_(nullptr),
        end_(nullptr)
    {}

    constexpr Span(const Span& other) noexcept = default;

    constexpr Span(const It b,const It e) noexcept :
        begin_(b),
        end_(e)
    {}

    constexpr Span(const It it,const size_type s) noexcept :
        begin_(it),
        end_(it+s)
    {}

    template<typename R,typename=
        std::enable_if_t<!std::is_same_v<std::remove_cvref_t<R>,Span> && !std::is_convertible_v<R&&,It>>
    >
    constexpr Span(R&& r) noexcept :
        begin_(std::forward<R>(r).begin()),
        end_(std::forward<R>(r).end())
    {}

    Span(std::nullptr_t) = delete;

    template<typename T,size_t N>
    constexpr Span(T(&arr)[N]) noexcept :
        begin_(arr),
        end_(arr+N)
    {}

    constexpr size_type size() const {return end_-begin_;}
    constexpr bool empty() const {return begin_==end_;}

    typedef It iterator;

    constexpr iterator begin() const {return begin_;}
    constexpr iterator end() const {return end_;}

    constexpr decltype(auto) operator[](const size_type i) const {assert(i<size()); return begin_[i];}
    constexpr decltype(auto) front() const {assert(!empty()); return *begin_;}
    constexpr decltype(auto) back() const {assert(!empty()); It it=end_;return *--it;}

    constexpr void remove_prefix() {assert(!empty()); ++begin_;}
    constexpr void remove_suffix() {assert(!empty()); --end_;}
    constexpr void remove_prefix(const size_type n) {assert(n<=size()); begin_+=n;}
    constexpr void remove_suffix(const size_type n) {assert(n<=size()); end_-=n;}

    constexpr Span subspan(const size_type off,const size_type n) const {assert(off+n<=size()); return {begin_+off,begin_+off+n};}
    constexpr Span first_n(const size_type n) const {assert(n<=size()); return {begin_,begin_+n};}
    constexpr Span last_n(const size_type n) const {assert(n<=size()); return {end_-n,end_};}
    constexpr Span no_first_n(const size_type n) const {assert(n<=size()); return {begin_+n,end_};}
    constexpr Span no_last_n(const size_type n) const {assert(n<=size()); return {begin_,end_-n};}
    constexpr Span no_first_last_n(const size_type first,const size_type last) const {assert(first+last<=size()); return {begin_+first,end_-last};}

    friend constexpr bool operator==(const Span& lhs,const Span& rhs) noexcept {
        if(lhs.size()!=rhs.size())
            return false;
        if constexpr(std::is_pointer_v<It> && std::has_unique_object_representations_v<value_type>){
            if(!std::is_constant_evaluated())
                return memcmp(lhs.begin_,rhs.begin_,lhs.size()*sizeof(value_type))==0;
        }
        iterator lb = lhs.begin_;
        iterator rb = rhs.begin_;
        const iterator le = lhs.end_;
        for(;lb!=le;++lb,++rb)
            if(*lb!=*rb)
                return false;
        return true;
    }
};

template<typename It,typename size_type>
Span(It,size_type) -> Span<It,size_type>;
template<typename It>
Span(It,It) -> Span<It>;
template<typename R>
Span(R&& r) -> Span<decltype(std::forward<R>(r).begin()),typename std::remove_cvref_t<R>::size_type>;
template<typename T,size_t N>
Span(T(&arr)[N]) -> Span<T*,size_t>;

static_assert(std::is_trivially_copyable_v<Span<const char*>>);
static_assert(std::is_standard_layout_v<Span<const char*>>);

typedef Span<const char*,size_t> String_View;
typedef Span<const char8_t*,size_t> U8String_View;
typedef Span<const char16_t*,size_t> U16String_View;
typedef Span<const char32_t*,size_t> U32String_View;

namespace literals{
constexpr String_View operator""_sp(const char* const str,const size_t len) {return {str,len};}
constexpr U8String_View operator""_sp(const char8_t* const str,const size_t len) {return {str,len};}
constexpr U16String_View operator""_sp(const char16_t* const str,const size_t len) {return {str,len};}
constexpr U32String_View operator""_sp(const char32_t* const str,const size_t len) {return {str,len};}
}
}
