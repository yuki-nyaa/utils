#pragma once
#include<iostream>
#include<string>
#include<type_traits>
#include<memory>

#if defined(DEBUG) && !defined(NDEBUG)
#define YUKI_DBGOUT(message) std::cout<<"\n"<<"YUKI_DBGOUT: "<<message<<std::endl
#endif

namespace yuki{
    // You may find this rather pointless. The story is that sometimes I want to inhibit IMPLICIT derived-to-base conversion while permitting EXPLICIT cast. So I decide to inherit privately (or protectedly) and befriend (some instances of) this function to achieve the effect. Since `static_cast` is a KEYWORD and not a normal identifier such as `std::static_cast`, the spelling is changed.
    template<typename T,typename S>
    constexpr T statik_kast(S&& s){ return static_cast<T>(std::forward<S>(s)); }
}

namespace yuki{ // Concepts
    template<typename T>
    concept Enum = std::is_enum_v<T>;

    template<typename T>
    concept Reference = std::is_reference_v<T>;
}
namespace yuki{ // Type traits
    template<typename T,typename U,typename... Vs>
    struct is_same : std::conditional_t<yuki::is_same<T,Vs...>::value && yuki::is_same<T,U>::value, std::true_type, std::false_type> {};

    template<typename T,typename U>
    struct is_same<T,U> : std::is_same<T,U> {};

    template<typename T,typename U,typename... Vs>
    inline constexpr bool is_same_v = yuki::is_same<T,U,Vs...>::value;


    template<typename T,typename U,typename... Vs>
    struct is_pairwise_different : std::conditional_t<is_pairwise_different<T,Vs...>::value && is_pairwise_different<U,Vs...>::value && is_pairwise_different<T,U>::value, std::true_type, std::false_type> {};

    template<typename T,typename U>
    struct is_pairwise_different<T,U> : std::conditional_t<!std::is_same<T,U>::value,std::true_type,std::false_type> {};

    template<typename T,typename U,typename... Vs>
    inline constexpr bool is_pairwise_different_v = is_pairwise_different<T,U,Vs...>::value;


    template<typename T,typename... Us>
    struct is_unique : std::negation<std::disjunction<std::is_same<T,Us>...>> {};

    template<typename T,typename... Us>
    inline constexpr bool is_unique_v = is_unique<T,Us...>::value;

    template<typename T>
    struct static_assert_dummy : std::negation<std::is_same<T,T>> {};

    template<typename T>
    inline constexpr bool static_assert_dummy_v = static_assert_dummy<T>::value;

    //* Adds low-level `const` to reference types. Simply adds `const` for non-reference types, including pointer types.
    template<typename T>
    struct add_lconst_ref {typedef T const type;};
    template<typename T>
    struct add_lconst_ref<T&> {typedef const T& type;};
    template<typename T>
    struct add_lconst_ref<T&&> {typedef const T&& type;};
    template<typename T>
    using add_lconst_ref_t = typename add_lconst_ref<T>::type;

    //* Adds low-level `const` to pointer types. Simply adds `const` for non-pointer types, including reference types.
    template<typename T>
    struct add_lconst_ptr {typedef T const type;};
    template<typename T>
    struct add_lconst_ptr<T*> {typedef const T* type;};
    template<typename T>
    using add_lconst_ptr_t = typename add_lconst_ptr<T>::type;

    //* Adds low-level `const` to reference AND pointer types. Simply adds `const` for other types.
    template<typename T>
    struct add_lconst {typedef T const type;};
    template<typename T>
    struct add_lconst<T&> {typedef const T& type;};
    template<typename T>
    struct add_lconst<T&&> {typedef const T&& type;};
    template<typename T>
    struct add_lconst<T*> {typedef const T* type;};
    template<typename T>
    using add_lconst_t =  typename add_lconst<T>::type;

    //* Removes low-level `const` from reference types. Simply removes `const` for non-reference types, including pointer types.
    template<typename T>
    struct remove_lconst_ref : std::remove_const<T> {};
    template<typename T>
    struct remove_lconst_ref<const T&> {typedef T& type;};
    template<typename T>
    struct remove_lconst_ref<const T&&> {typedef T&& type;};
    template<typename T>
    using remove_lconst_ref_t = typename remove_lconst_ref<T>::type;

    //* Removes low-level `const` from pointer types. Simply removes `const` for non-pointer types, including reference types.
    template<typename T>
    struct remove_lconst_ptr : std::remove_const<T> {};
    template<typename T>
    struct remove_lconst_ptr<const T*> {typedef T* type;};
    template<typename T>
    using remove_lconst_ptr_t = typename remove_lconst_ptr<T>::type;

    //* Removes low-level `const` from reference AND pointer types. Simply removes `const` for other types.
    template<typename T>
    struct remove_lconst : std::remove_const<T> {};
    template<typename T>
    struct remove_lconst<const T&> {typedef T& type;};
    template<typename T>
    struct remove_lconst<const T&&> {typedef T&& type;};
    template<typename T>
    struct remove_lconst<const T*> {typedef T* type;};
    template<typename T>
    using remove_lconst_t =  typename remove_lconst<T>::type;


    struct type_switch_nomatch{};

    template<size_t head,size_t index,typename... Ts>
    struct type_switch_helper_;
    template<size_t head,size_t index,typename T_head,typename... Tn>
    struct type_switch_helper_<head,index,T_head,Tn...> : std::conditional<index==head,T_head,typename type_switch_helper_<head+1,index,Tn...>::type> {};
    template<size_t head,size_t index>
    struct type_switch_helper_<head,index> {typedef type_switch_nomatch type;};

    template<size_t index,typename... Ts>
    struct type_switch : type_switch_helper_<0,index,Ts...> {}; // Same with C++ array, the index starts at 0.
    template<size_t index,typename... Ts>
    using type_switch_t = typename type_switch<index,Ts...>::type;


    inline constexpr size_t type_to_num_nomatch = (size_t)-1;
    template<size_t counter,typename T,typename... Us>
    struct type_to_num_helper_ : std::integral_constant<size_t,type_to_num_nomatch> {};
    template<size_t counter,typename T,typename U0, typename... Un>
    struct type_to_num_helper_<counter,T,U0,Un...> : std::conditional_t<std::is_same_v<T,U0>,std::integral_constant<size_t,counter>,type_to_num_helper_<counter+1,T,Un...>> {};

    //* Gets the index of T (first occurrence) among the type-list Us. Same with C++ array, the index starts at 0. Returns `type_to_num_nomatch` when there is no match.
    template<typename T,typename... Us>
    struct type_to_num : type_to_num_helper_<0,T,Us...> {};
    template<typename T,typename... Us>
    inline constexpr size_t type_to_num_v = type_to_num<T,Us...>::value;
}

// From James Adkison in https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11
template<yuki::Enum T>
constexpr std::ostream& operator<<(std::ostream& stream, const T& e){
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

namespace yuki{
    template<bool if_flush=true,typename T>
    void print_line(const T& message,std::ostream& o = std::cout){
        if constexpr(if_flush){
            o<<message<<std::endl;
        }else{
            o<<message<<"\n";
        }
    }

    inline void print_space(std::ostream& o){ o<<std::endl; }
    template<typename T,typename... Ts>
    void print_space(std::ostream& o, const T& message1, const Ts&... messages){
        o<<message1<<" ";
        print_space(o,messages...);
    }

    namespace err{
        inline constexpr const char* ERR="ERROR: ";
        inline constexpr const char* WARN="WARNING: ";
        inline constexpr const char* INDENT="    ";
        inline constexpr const char* NOTE="Note: ";
        inline constexpr const char* CONTEXT="Context: ";
        inline constexpr const char* INOTE="--Note: ";
        inline constexpr const char* ICONTEXT="--Context: ";
        template<bool if_flush=true,typename T>
        void error_line(const T& message){ print_line(message,std::cerr); }
    }

    [[nodiscard("Are you really sure that the split is possible?")]]
    inline bool split(std::string_view source, std::string& lhs, std::string& rhs, const char sign){
        std::string_view::size_type pos=source.find(sign);
        if(pos==std::string_view::npos)
            return false;
        lhs=std::string(source.substr(0,pos));
        rhs=std::string(source.substr(pos+1,source.size()));
        return true;
    }

    constexpr void ignore(){}
    template<typename T,typename... Ts>
    constexpr void ignore(const T& first,const Ts&... args){
        (void)first;
        ignore(args...);
    }

    // Note: although no explicit move semantics is used, `r` is in fact moved. So don't attempt to access `r` after the cast. After all, `std::unique_ptr` cannot be copied, so I decide to design it this way to save one `std::move`.
    template<typename T,typename U >
    std::unique_ptr<T> static_pointer_cast(std::unique_ptr<U>& r){
        return std::unique_ptr<T>(statik_kast<typename std::unique_ptr<T>::element_type*>(r.release()));
    }
}
