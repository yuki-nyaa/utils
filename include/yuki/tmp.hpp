#pragma once
#include<type_traits>
#include<utility>
#include<cstddef>
#include<limits>

// Used to protect `<>` or `{}` list in macro arguments, e.g. `SOME_MACRO(YUKI_PROTECT({1,1,1}))`. This is because the preprocessor only matches `()`, so `SOME_MACRO({1,1,1})` is parsed to have 3 arguments, namely `{1`, `1` and `1}`.
#define YUKI_PROTECT(...) __VA_ARGS__

// Well, although I hate macros, sometimes there seems to be no better way. Copy/move elision in a return statement only applies to prvalue, not xvalue, so fiddling with `std::forward` doesn't help. Also NRVO does not apply to function parameters. Also I have to say that `std::forward` and forwarding references are really a headache for me.
#define YUKI_CONDITIONAL_RETURN(flag,first,second) \
    do{ \
        if constexpr(flag) \
            return first; \
        else \
            return second; \
    }while(0)


namespace yuki{ // Type traits and other compile-time facilities.

/// Like `std::declval` but the return type is strictly the same as given.
template<typename T>
T declval() noexcept {static_assert(false);}

template<typename T,typename...>
using always_type = T;
template<typename...>
using always_std_true_type = std::true_type;
template<typename...>
using always_std_false_type = std::false_type;

template<typename...>
inline constexpr bool always_true_v = true;
template<typename...>
inline constexpr bool always_false_v = false;


template<typename... Ts>
struct is_same : std::false_type {};

template<typename T,typename... Vs>
struct is_same<T,T,Vs...> : is_same<T,Vs...> {};

template<typename T>
struct is_same<T> : std::true_type {};

template<>
struct is_same<> : std::true_type {};

template<typename... Ts>
inline constexpr bool is_same_v = yuki::is_same<Ts...>::value;


template<typename... Ts>
struct is_pairwise_different;

template<typename T,typename U,typename... Vs>
struct is_pairwise_different<T,U,Vs...> {static constexpr bool value = is_pairwise_different<T,Vs...>::value && is_pairwise_different<U,Vs...>::value;};

template<typename T,typename... Vs>
struct is_pairwise_different<T,T,Vs...> : std::false_type {};

template<typename T>
struct is_pairwise_different<T> : std::true_type {};

template<>
struct is_pairwise_different<> : std::true_type {};

template<typename... Ts>
inline constexpr bool is_pairwise_different_v = is_pairwise_different<Ts...>::value;


template<typename T,typename... Us>
struct is_any_of;

template<typename T,typename U,typename... Vs>
struct is_any_of<T,U,Vs...> : is_any_of<T,Vs...> {};

template<typename T,typename... Vs>
struct is_any_of<T,T,Vs...> : std::true_type {};

template<typename T>
struct is_any_of<T> : std::false_type {};

template<typename T,typename... Us>
inline constexpr bool is_any_of_v = is_any_of<T,Us...>::value;


template<bool has_encountered,typename T,typename... Us>
struct is_unique_impl_;

template<typename T,typename U,typename... Vs>
struct is_unique_impl_<false,T,U,Vs...> : is_unique_impl_<false,T,Vs...> {};

template<typename T,typename... Vs>
struct is_unique_impl_<false,T,T,Vs...> : is_unique_impl_<true,T,Vs...> {};

template<typename T>
struct is_unique_impl_<false,T> : std::true_type {};

template<typename T,typename U,typename... Vs>
struct is_unique_impl_<true,T,U,Vs...> : is_unique_impl_<true,T,Vs...> {};

template<typename T,typename... Vs>
struct is_unique_impl_<true,T,T,Vs...> : std::false_type {};

template<typename T>
struct is_unique_impl_<true,T> : std::true_type {};

template<typename T,typename... Us>
struct is_unique : is_unique_impl_<false,T,Us...> {};

template<typename T,typename... Us>
inline constexpr bool is_unique_v = is_unique<T,Us...>::value;


template<typename,typename...>
struct is_0th : std::false_type {};

template<typename T,typename... Others>
struct is_0th<T,T,Others...> : std::true_type {};

template<typename T,typename... Pack>
inline constexpr bool is_0th_v = is_0th<T,Pack...>::value;


template<typename T>
inline constexpr bool static_assert_dummy = false;


struct type_switch_nomatch {};

template<size_t i,typename... Ts>
struct type_switch {typedef type_switch_nomatch type;};

template<size_t i,typename T,typename... Us>
struct type_switch<i,T,Us...> : type_switch<i-1,Us...> {};

template<typename T,typename... Us>
struct type_switch<0,T,Us...> {typedef T type;};

template<size_t i,typename... Ts>
using type_switch_t = typename type_switch<i,Ts...>::type;


// Similar to `type_switch`.
template<typename T>
inline constexpr T i_th_nomatch=std::numeric_limits<T>::max();

template<size_t i,typename T,T... ts>
struct i_th {static constexpr T value = i_th_nomatch<T>;};

template<size_t i,typename T,T t,T... us>
struct i_th<i,T,t,us...> : i_th<i-1,T,us...> {};

template<typename T,T t,T... us>
struct i_th<0,T,t,us...> {static constexpr T value = t;};

template<size_t index,typename T,T... ints>
inline constexpr T i_th_v = i_th<index,T,ints...>::value;


template<size_t index,typename T>
struct int_seq_i_th;
template<size_t index,typename T,T... ints>
struct int_seq_i_th<index,std::integer_sequence<T,ints...>> : i_th<index,T,ints...> {};

template<size_t index,typename T>
inline constexpr auto int_seq_i_th_v = int_seq_i_th<index,T>::value;


inline constexpr size_t type_to_num_nomatch = -1;

template<size_t counter,typename T,typename... Us>
struct type_to_num_helper_  {static constexpr size_t value = type_to_num_nomatch;};
template<size_t counter,typename T,typename... Vs>
struct type_to_num_helper_<counter,T,T,Vs...> {static constexpr size_t value=counter;};
template<size_t counter,typename T,typename U, typename... Vs>
struct type_to_num_helper_<counter,T,U,Vs...> : type_to_num_helper_<counter+1,T,Vs...> {};

/// Gets the index of `T` (first occurrence) among the type-list `Us`. Same with C++ array, the index starts at 0. Returns `type_to_num_nomatch` when there is no match.
template<typename T,typename... Us>
struct type_to_num : type_to_num_helper_<0,T,Us...> {};
template<typename T,typename... Us>
inline constexpr size_t type_to_num_v = type_to_num<T,Us...>::value;

template<typename Seq,size_t counter,typename T,typename... Us>
struct find_type_helper_  {typedef Seq type;};
template<size_t counter,typename T,typename... Vs,size_t... is>
struct find_type_helper_<std::index_sequence<is...>,counter,T,T,Vs...> {using type = typename find_type_helper_<std::index_sequence<is...,counter>,counter+1,T,Vs...>::type;};
template<size_t counter,typename T,typename U,typename... Vs,size_t... is>
struct find_type_helper_<std::index_sequence<is...>,counter,T,U,Vs...> {using type = typename find_type_helper_<std::index_sequence<is...>,counter+1,T,Vs...>::type;};

/// Finds all occurrences of `T` among the type-list `Us`, and records the indices in the returned `std::index_sequence`. Same with C++ array, the index starts at 0. Returns empty sequence when there is no match.
template<typename T,typename... Us>
struct find_type {using type = typename find_type_helper_<std::index_sequence<>,0,T,Us...>::type;};
template<typename T,typename... Us>
using find_type_t = typename find_type_helper_<std::index_sequence<>,0,T,Us...>::type;




//template<size_t B,size_t N>
//struct make_index_sequence_impl_;

//template<typename S0,typename S1>
//struct make_index_sequence_impl_2_;
//template<size_t... Is,size_t... Js>
//struct make_index_sequence_impl_2_<std::index_sequence<Is...>,std::index_sequence<Js...>> {};


//template<size_t B>
//struct make_index_sequence_impl_<B,0> {typedef std::index_sequence<> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,1> {typedef std::index_sequence<B> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,2> {typedef std::index_sequence<B,B+1> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,3> {typedef std::index_sequence<B,B+1,B+2> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,4> {typedef std::index_sequence<B,B+1,B+2,B+3> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,5> {typedef std::index_sequence<B,B+1,B+2,B+3,B+4> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,6> {typedef std::index_sequence<B,B+1,B+2,B+3,B+4,B+5> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,7> {typedef std::index_sequence<B,B+1,B+2,B+3,B+4,B+5,B+6> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,8> {typedef std::index_sequence<B,B+1,B+2,B+3,B+4,B+5,B+6,B+7> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,9> {typedef std::index_sequence<B,B+1,B+2,B+3,B+4,B+5,B+6,B+7,B+8> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,10> {typedef std::index_sequence<B,B+1,B+2,B+3,B+4,B+5,B+6,B+7,B+8,B+9> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,11> {typedef std::index_sequence<B,B+1,B+2,B+3,B+4,B+5,B+6,B+7,B+8,B+9,B+10> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,12> {typedef std::index_sequence<B,B+1,B+2,B+3,B+4,B+5,B+6,B+7,B+8,B+9,B+10,B+11> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,13> {typedef std::index_sequence<B,B+1,B+2,B+3,B+4,B+5,B+6,B+7,B+8,B+9,B+10,B+11,B+12> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,14> {typedef std::index_sequence<B,B+1,B+2,B+3,B+4,B+5,B+6,B+7,B+8,B+9,B+10,B+11,B+12,B+13> type;};
//template<size_t B>
//struct make_index_sequence_impl_<B,15> {typedef std::index_sequence<B,B+1,B+2,B+3,B+4,B+5,B+6,B+7,B+8,B+9,B+10,B+11,B+12,B+13,B+14> type;};

//template<size_t N>
//using make_index_sequence = typename make_index_sequence_impl_<N>::type;




/// Automatically chooses the smallest type to hold a given value.
/// If two unsigned integral types happen to be of the same size, for example, on my machine `(unsigned) int` and `(unsigned) long` have the same size, than the "smaller" type (`(unsigned) int` in my example) is chosen.
/// @note The built-in `auto` and `decltype` deduction facilities only deduce types bigger than `(unsigned) int`. This is probably because `(unsigned) int` has better performance. So if you aren't going to store a large number of data then simply use the built-in facilities.
/// @note This is different from `(u)int_least8_t` or `boost::(u)int_t<N>::least` which take the number of bits as argument.
enum struct uint_enum {UCHAR,USHORT,UINT,ULONG,ULLONG};

template<typename Start = unsigned char>
constexpr uint_enum uint_auto_f(const unsigned long long a) noexcept {
    static_assert(std::is_integral_v<Start> && std::is_unsigned_v<Start>);
    const size_t which = type_to_num_v<Start,unsigned char,unsigned short,unsigned,unsigned long,unsigned long long>;
    const unsigned s_char = (a>std::numeric_limits<unsigned char>::max() || which>0 ) ? 1 : 0;
    const unsigned s_short = (a>std::numeric_limits<unsigned short>::max() || which>1 ) ? 1 : 0;
    const unsigned s_int = (a>std::numeric_limits<unsigned int>::max() || which>2 ) ? 1 : 0;
    const unsigned s_long = (a>std::numeric_limits<unsigned long>::max() || which>3 ) ? 1 : 0;
    switch(s_char+s_short+s_int+s_long){
        case 0 : return uint_enum::UCHAR;
        case 1 : return uint_enum::USHORT;
        case 2 : return uint_enum::UINT;
        case 3 : return uint_enum::ULONG;
        case 4 : return uint_enum::ULLONG;
    }
    return uint_enum::ULLONG; // Fallback.
}

template<unsigned long long a,typename S = unsigned char,uint_enum flag = uint_auto_f<S>(a)>
struct uint_auto;
template<unsigned long long a,typename S>
struct uint_auto<a,S,uint_enum::UCHAR> {typedef unsigned char type;};
template<unsigned long long a,typename S>
struct uint_auto<a,S,uint_enum::USHORT> {typedef unsigned short type;};
template<unsigned long long a,typename S>
struct uint_auto<a,S,uint_enum::UINT> {typedef unsigned int type;};
template<unsigned long long a,typename S>
struct uint_auto<a,S,uint_enum::ULONG> {typedef unsigned long type;};
template<unsigned long long a,typename S>
struct uint_auto<a,S,uint_enum::ULLONG> {typedef unsigned long long type;};

template<unsigned long long a,typename S = unsigned char>
using uint_auto_t = typename uint_auto<a,S>::type;

enum struct int_enum {CHAR,SHORT,INT,LONG,LLONG};

template<typename Start = signed char>
constexpr int_enum int_auto_f(const long long a) noexcept {
    static_assert(std::is_integral_v<Start> && std::is_signed_v<Start>);
    const size_t which = type_to_num_v<Start,signed char,short,int,long,long long>;
    const unsigned s_char = ( (a>std::numeric_limits<signed char>::max() || a<std::numeric_limits<signed char>::lowest()) || which>0 ) ? 1 : 0;
    const unsigned s_short = ( (a>std::numeric_limits<short>::max() || a<std::numeric_limits<short>::lowest()) || which>1 ) ? 1 : 0;
    const unsigned s_int = ( (a>std::numeric_limits<int>::max() || a<std::numeric_limits<int>::lowest()) || which>2 ) ? 1 : 0;
    const unsigned s_long = ( (a>std::numeric_limits<long>::max() || a<std::numeric_limits<long>::lowest()) || which>3 ) ? 1 : 0;
    switch(s_char+s_short+s_int+s_long){
        case 0 : return int_enum::CHAR;
        case 1 : return int_enum::SHORT;
        case 2 : return int_enum::INT;
        case 3 : return int_enum::LONG;
        case 4 : return int_enum::LLONG;
    }
    return int_enum::LLONG; // Fallback.
}

template<long long a,typename S = signed char,int_enum flag = int_auto_f<S>(a)>
struct int_auto;
template<long long a,typename S>
struct int_auto<a,S,int_enum::CHAR> {typedef signed char type;};
template<long long a,typename S>
struct int_auto<a,S,int_enum::SHORT> {typedef short type;};
template<long long a,typename S>
struct int_auto<a,S,int_enum::INT> {typedef int type;};
template<long long a,typename S>
struct int_auto<a,S,int_enum::LONG> {typedef long type;};
template<long long a,typename S>
struct int_auto<a,S,int_enum::LLONG> {typedef long long type;};

template<long long a,typename S = signed char>
using int_auto_t = typename int_auto<a,S>::type;


struct is_braced_init_list_convertible_helper{
    template <typename T>
    static int t_dummy(const T&); ///< @note plain `T` is wrong because it will decay.

    template <typename T,typename... Args>
    static std::true_type test(int,decltype(t_dummy<T>({std::declval<Args>()...}))=0);
    template <typename T,typename... Args>
    static std::false_type test(long);
};
template<typename T,typename... Args>
struct is_braced_init_list_convertible : decltype(is_braced_init_list_convertible_helper::template test<T,Args...>(0)) {};
template<typename T,typename... Args>
inline constexpr bool is_braced_init_list_convertible_v = is_braced_init_list_convertible<T,Args...>::value;

//template<typename P,typename T>
//struct is_pointer_to : std::conditional_t<std::is_pointer_v<P> && std::is_same_v<std::remove_cv_t<std::remove_pointer_t<P>>,T>,std::true_type,std::false_type> {};
//template<typename P,typename T>
//inline constexpr bool is_pointer_to_v = is_pointer_to<P,T>::value;

//template<typename P>
//struct is_pointer_to_const : std::conditional_t<std::is_pointer_v<P> && std::is_const_v<std::remove_pointer_t<P>>,std::true_type,std::false_type> {};
//template<typename P>
//inline constexpr bool is_pointer_to_const_v = is_pointer_to_const<P>::value;

//template<typename P,typename T>
//struct is_pointer_to_const_to : std::conditional_t<is_pointer_to_const_v<P> && is_pointer_to_v<P,T>,std::true_type,std::false_type> {};
//template<typename P,typename T>
//inline constexpr bool is_pointer_to_const_to_v = is_pointer_to_const_to<P,T>::value;

//template<typename P>
//struct is_pointer_to_non_const : std::conditional_t<std::is_pointer_v<P> && !std::is_const_v<std::remove_pointer_t<P>>,std::true_type,std::false_type> {};
//template<typename P>
//inline constexpr bool is_pointer_to_non_const_v = is_pointer_to_non_const<P>::value;

//template<typename P,typename T>
//struct is_pointer_to_non_const_to : std::conditional_t<is_pointer_to_non_const_v<P> && is_pointer_to_v<P,T>,std::true_type,std::false_type> {};
//template<typename P,typename T>
//inline constexpr bool is_pointer_to_non_const_to_v = is_pointer_to_non_const_to<P,T>::value;
} // namespace yuki

namespace yuki{
template<typename T>
constexpr T& as_non_const(const T& t) {return const_cast<T&>(t);}
template<typename T>
constexpr T&& as_non_const(const T&& t) {return const_cast<T&&>(t);}

template<typename T>
constexpr T* const_kast(const T* const p) noexcept {return const_cast<T*>(p);}

template<typename T>
struct add_lconst;
template<typename T>
struct add_lconst<T*> {typedef const T* type;};
template<typename T>
struct add_lconst<T&> {typedef const T& type;};
template<typename T>
using add_lconst_t = typename add_lconst<T>::type;

namespace remove_lconst_impl_{
    using yuki::const_kast;
    template<typename T>
    auto foo(T t) {return const_kast(t);}
}
template<typename T>
struct remove_lconst {typedef decltype(remove_lconst_impl_::foo(T{})) type;};
template<typename T>
struct remove_lconst<T*> {typedef std::remove_const_t<T>* type;};
template<typename T>
struct remove_lconst<T&> {typedef std::remove_const_t<T>& type;};
template<typename T>
using remove_lconst_t = typename remove_lconst<T>::type;
}

namespace yuki{
template<typename... Ts>
constexpr void ignore(const Ts&...) {}

// You may find this rather pointless. The story is that sometimes I want to inhibit IMPLICIT derived-to-base conversion while permitting EXPLICIT cast. So I decide to inherit privately (or protectedly) and befriend (some instances of) this function to achieve the effect. Since `static_cast` is a KEYWORD and not a normal identifier such as `std::static_cast`, the spelling is changed.
template<typename T,typename S>
constexpr T statik_kast(S&& s) {return static_cast<T>(std::forward<S>(s));}
} // namespace yuki

namespace yuki{
#if __cplusplus>=202002L
template<typename T,typename... Args>
concept Has_Operator_Delete = requires {T::operator delete(std::declval<Args>()...);};
template<typename T,typename... Args>
concept Has_Operator_Delete_A = requires {T::operator delete[](std::declval<Args>()...);};

// TODO supports over-aligned types.
template<typename T_out,typename T_in,typename... PArgs>
inline void static_delete(T_in*& ptr,PArgs&&... pargs){
    static_cast<T_out*>(ptr)->T_out::~T_out(); // From [class.virtual] : Explicit qualiﬁcation with the scope operator (7.5.4.3) suppresses the virtual call mechanism.
    if constexpr(Has_Operator_Delete<T_out,void*,decltype(std::forward<PArgs>(pargs))...>)
        T_out::operator delete(ptr,std::forward<PArgs>(pargs)...);
    else
        ::operator delete(ptr,std::forward<PArgs>(pargs)...);
    ptr=nullptr;
}
template<typename T_out,typename T_in,typename... PArgs>
inline void static_delete_a(T_in*& ptr,PArgs&&... pargs){
    static_cast<T_out*>(ptr)->T_out::~T_out();
    if constexpr(Has_Operator_Delete_A<T_out,void*,decltype(std::forward<PArgs>(pargs))...>)
        T_out::operator delete[](ptr,std::forward<PArgs>(pargs)...);
    else
        ::operator delete[](ptr,std::forward<PArgs>(pargs)...);
    ptr=nullptr;
}
#endif
} // namespace yuki
