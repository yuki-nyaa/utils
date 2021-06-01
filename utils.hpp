#pragma once
#include<string>
#include<type_traits>
#include<memory>
#include<limits>
#include<functional>
#include<ctime>
#include<fmt/core.h>
#include<fmt/chrono.h>

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

    template<typename T,typename U,typename... Vs>
    struct is_same : std::conditional_t<yuki::is_same<T,Vs...>::value && yuki::is_same<T,U>::value, std::true_type, std::false_type> {};

    template<typename T,typename U>
    struct is_same<T,U> : std::is_same<T,U> {};

    template<typename T,typename U,typename... Vs>
    inline constexpr bool is_same_v = yuki::is_same<T,U,Vs...>::value;

    template<typename T,typename U,typename... Vs>
    struct is_pairwise_different : std::conditional_t<is_pairwise_different<T,Vs...>::value && is_pairwise_different<U,Vs...>::value && !std::is_same_v<T,U>, std::true_type, std::false_type> {};

    template<typename T,typename U>
    struct is_pairwise_different<T,U> : std::conditional_t<!std::is_same_v<T,U>,std::true_type,std::false_type> {};

    template<typename T,typename U,typename... Vs>
    inline constexpr bool is_pairwise_different_v = is_pairwise_different<T,U,Vs...>::value;

    template<typename T,typename... Us>
    struct is_any_of : std::disjunction<std::is_same<T,Us>...> {};

    template<typename T,typename... Us>
    inline constexpr bool is_any_of_v = is_any_of<T,Us...>::value;

    template<typename T,typename... Us>
    struct is_unique : std::negation<is_any_of<T,Us...>> {};

    template<typename T,typename... Us>
    inline constexpr bool is_unique_v = !is_any_of<T,Us...>::value;

    template<typename T>
    struct static_assert_dummy : always_std_false_type<T> {};

    template<typename T>
    inline constexpr bool static_assert_dummy_v = always_false_v<T>;


    struct type_switch_nomatch {};

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


    // Similar to `type_switch`.
    template<typename T>
    inline constexpr T i_th_nomatch=std::numeric_limits<T>::max();

    template<size_t head,size_t index,typename T,T... ints>
    struct i_th_helper_;
    template<size_t head,size_t index,typename T,T int0,T... ints>
    struct i_th_helper_<head,index,T,int0,ints...> : std::conditional_t<index==head,std::integral_constant<T,int0>,i_th_helper_<head+1,index,T,ints...>> {};
    template<size_t head,size_t index,typename T>
    struct i_th_helper_<head,index,T> {static constexpr T value=i_th_nomatch<T>;};

    template<size_t index,typename T,T... ints>
    struct i_th : i_th_helper_<0,index,T,ints...> {};
    template<size_t index,typename T,T... ints>
    inline constexpr T i_th_v = i_th<index,T,ints...>::value;

    // Specialization for `std::integer_sequence`.
    template<size_t index,typename T,T... ints>
    struct i_th<index,std::integer_sequence<T,ints...>> : i_th<index,T,ints...> {};
    template<size_t index,typename T,T... ints>
    inline constexpr T i_th_v<index,std::integer_sequence<T,ints...>> = i_th<index,T,ints...>::value;



    inline constexpr size_t type_to_num_nomatch = -1;

    template<size_t counter,typename T,typename... Us>
    struct type_to_num_helper_ : std::integral_constant<size_t,type_to_num_nomatch> {};
    template<size_t counter,typename T,typename U0, typename... Un>
    struct type_to_num_helper_<counter,T,U0,Un...> : std::conditional_t<std::is_same_v<T,U0>,std::integral_constant<size_t,counter>,type_to_num_helper_<counter+1,T,Un...>> {};

    //* Gets the index of T (first occurrence) among the type-list Us. Same with C++ array, the index starts at 0. Returns `type_to_num_nomatch` when there is no match.
    template<typename T,typename... Us>
    struct type_to_num : type_to_num_helper_<0,T,Us...> {};
    template<typename T,typename... Us>
    inline constexpr size_t type_to_num_v = type_to_num<T,Us...>::value;



    // Automatically chooses the smallest type to hold a given value.
    // If two unsigned integral types happen to be of the same size, for example, on my machine `(unsigned) int` and `(unsigned) long` have the same size, than the "smaller" type (`(unsigned) int` in my example) is chosen.
    // Note that the built-in `auto` and `decltype` deduction facilities only deduce types bigger than `(unsigned) int`. This is probably because `(unsigned) int` has better performance. So if you aren't going to store a large number of data then simply use the built-in facilities.
    // Also note that this is different from `(u)int_least8_t` or `boost::(u)int_t<N>::least` which take the number of bits as argument.
    enum struct uint_enum {UCHAR,USHORT,UINT,ULONG,ULLONG};
    constexpr uint_enum uint_auto_f(unsigned long long a) noexcept {
        unsigned s_char = (a>std::numeric_limits<unsigned char>::max()) ? 1 : 0;
        unsigned s_short = (a>std::numeric_limits<unsigned short>::max()) ? 1 : 0;
        unsigned s_int = (a>std::numeric_limits<unsigned int>::max()) ? 1 : 0;
        unsigned s_long = (a>std::numeric_limits<unsigned long>::max()) ? 1 : 0;
        switch(s_char+s_short+s_int+s_long){
            case 0 : return uint_enum::UCHAR;
            case 1 : return uint_enum::USHORT;
            case 2 : return uint_enum::UINT;
            case 3 : return uint_enum::ULONG;
            case 4 : return uint_enum::ULLONG;
        }
        return uint_enum::ULLONG; // Fallback.
    }

    template<unsigned long long a,uint_enum flag = uint_auto_f(a)>
    struct uint_auto;
    template<unsigned long long a>
    struct uint_auto<a,uint_enum::UCHAR> {typedef unsigned char type;};
    template<unsigned long long a>
    struct uint_auto<a,uint_enum::USHORT> {typedef unsigned short type;};
    template<unsigned long long a>
    struct uint_auto<a,uint_enum::UINT> {typedef unsigned int type;};
    template<unsigned long long a>
    struct uint_auto<a,uint_enum::ULONG> {typedef unsigned long type;};
    template<unsigned long long a>
    struct uint_auto<a,uint_enum::ULLONG> {typedef unsigned long long type;};
    template<unsigned long long a>
    using uint_auto_t = typename uint_auto<a>::type;

    enum struct int_enum {CHAR,SHORT,INT,LONG,LLONG};
    constexpr int_enum int_auto_f(long long a) noexcept {
        unsigned s_char = (a>std::numeric_limits<signed char>::max() || a<std::numeric_limits<signed char>::lowest()) ? 1 : 0;
        unsigned s_short = (a>std::numeric_limits<short>::max() || a<std::numeric_limits<short>::lowest()) ? 1 : 0;
        unsigned s_int = (a>std::numeric_limits<int>::max() || a<std::numeric_limits<int>::lowest()) ? 1 : 0;
        unsigned s_long = (a>std::numeric_limits<long>::max() || a<std::numeric_limits<long>::lowest()) ? 1 : 0;
        switch(s_char+s_short+s_int+s_long){
            case 0 : return int_enum::CHAR;
            case 1 : return int_enum::SHORT;
            case 2 : return int_enum::INT;
            case 3 : return int_enum::LONG;
            case 4 : return int_enum::LLONG;
        }
        return int_enum::LLONG; // Fallback.
    }

    template<long long a,int_enum flag = int_auto_f(a)>
    struct int_auto;
    template<long long a>
    struct int_auto<a,int_enum::CHAR> {typedef signed char type;};
    template<long long a>
    struct int_auto<a,int_enum::SHORT> {typedef short type;};
    template<long long a>
    struct int_auto<a,int_enum::INT> {typedef int type;};
    template<long long a>
    struct int_auto<a,int_enum::LONG> {typedef long type;};
    template<long long a>
    struct int_auto<a,int_enum::LLONG> {typedef long long type;};

    template<long long a>
    using int_auto_t = typename int_auto<a>::type;


    struct is_braced_init_list_convertible_helper{
        template <typename T>
        static int t_dummy(const T&); // Note: plain `T` is wrong because it will decay.

        template <typename T,typename... Args>
        static std::true_type test(int,decltype(t_dummy<T>({std::declval<Args>()...}))=0);
        template <typename T,typename... Args>
        static std::false_type test(long);
    };
    template<typename T,typename... Args>
    struct is_braced_init_list_convertible : decltype(is_braced_init_list_convertible_helper::template test<T,Args...>(0)) {};
    template<typename T,typename... Args>
    inline constexpr bool is_braced_init_list_convertible_v = is_braced_init_list_convertible<T,Args...>::value;


    template<typename E> requires std::is_enum_v<E>
    constexpr std::underlying_type_t<E> to_underlying(E e) noexcept {return static_cast<std::underlying_type_t<E>>(e);}
}

namespace yuki{
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
}

namespace yuki{
    template<typename... Ts>
    constexpr void ignore(const Ts&... args) {(...,void(args));}

    // You may find this rather pointless. The story is that sometimes I want to inhibit IMPLICIT derived-to-base conversion while permitting EXPLICIT cast. So I decide to inherit privately (or protectedly) and befriend (some instances of) this function to achieve the effect. Since `static_cast` is a KEYWORD and not a normal identifier such as `std::static_cast`, the spelling is changed.
    template<typename T,typename S>
    constexpr T statik_kast(S&& s) {return static_cast<T>(std::forward<S>(s));}

    template<typename T,typename U >
    std::unique_ptr<T> static_pointer_cast(std::unique_ptr<U>&& r){
        return std::unique_ptr<T>(static_cast<typename std::unique_ptr<T>::pointer>(r.release()));
    }
}

namespace yuki{
    template<typename T,typename CharT>
    struct simple_formatter{ // Used only with empty format string.
        constexpr auto parse(fmt::basic_format_parse_context<CharT>& ctx) -> typename fmt::basic_format_parse_context<CharT>::iterator {return ctx.begin();}
    };

    template<typename T>
    void print_line(const T& message,FILE* fp=stdout) {fmt::print(fp,"{}\n",message);}

    template<char sep = ' ',typename... Ts>
    void print_space(FILE* fp,const Ts&... messages) {(...,(fmt::print(fp,"{}{}",messages,sep))); fprintf(fp,"\n");}

    template<typename... Args>
    void try_print(FILE* fp,std::string_view fmt,const Args&... args){
        if(fp)
            fmt::print(fp,fmt,args...);
    }

    inline int try_fprintf(FILE* fp,const char* format){
        if(fp)
            return fprintf(fp,format);
        else
            return 0;
    }

    // The standard does not say what would happen with `fclose(NULL/nullptr)`. It even does not say it's undefined.
    inline int try_fclose(FILE* fp){
        if(fp)
            return fclose(fp);
        else
            return 0;
    }

    inline void print_loc(FILE* fp,std::string_view filename,size_t line,size_t col){
        fmt::print(fp,"{}:{}:{}: ",filename,line,col);
    }

    template<typename... Args>
    void print_error(FILE* fp,std::string_view fmt,const Args&... args){
        fprintf(fp,"ERROR: ");
        fmt::print(fp,fmt,args...);
        fflush(fp);
    }
    template<typename... Args>
    void print_warning(FILE* fp,std::string_view fmt,const Args&... args){
        fprintf(fp,"WARNING: ");
        fmt::print(fp,fmt,args...);
        fflush(fp);
    }
    template<typename... Args>
    void print_note(FILE* fp,std::string_view fmt,const Args&... args){
        fprintf(fp,"--NOTE: ");
        fmt::print(fp,fmt,args...);
        fflush(fp);
    }


    template<typename CharT,typename Traits,typename Allocator=std::allocator<CharT>>
    std::pair<std::basic_string<CharT,Traits,Allocator>,std::basic_string<CharT,Traits,Allocator>> split_first(std::basic_string_view<CharT,Traits> source,const CharT sign,const Allocator& alloc=Allocator{}){
        typename std::basic_string_view<CharT,Traits>::size_type pos=source.find(sign);
        if(pos==std::basic_string_view<CharT,Traits>::npos)
            throw std::runtime_error("Split-sign not found!");
        return {std::basic_string<CharT,Traits,Allocator>(source.substr(0,pos),alloc),std::basic_string<CharT,Traits,Allocator>(source.substr(pos+1,source.size()),alloc)};
    }
    template<typename CharT,typename Traits=std::char_traits<CharT>,typename Allocator=std::allocator<CharT>,typename View> requires std::is_convertible_v<const View&, std::basic_string_view<CharT, Traits>>
    std::pair<std::basic_string<CharT,Traits,Allocator>,std::basic_string<CharT,Traits,Allocator>> split_first(const View& source,const CharT sign,const Allocator& alloc=Allocator{}){
        std::basic_string_view<CharT,Traits> sv=source;
        return split_first<CharT,Traits,Allocator>(sv,sign,alloc);
    }

    template<typename CharT,typename Traits,typename Allocator=std::allocator<CharT>>
    std::pair<std::basic_string<CharT,Traits,Allocator>,std::basic_string<CharT,Traits,Allocator>> split_last(std::basic_string_view<CharT,Traits> source,const CharT sign,const Allocator& alloc=Allocator{}){
        typename std::basic_string_view<CharT,Traits>::size_type pos=source.rfind(sign);
        if(pos==std::basic_string_view<CharT,Traits>::npos)
            throw std::runtime_error("Split-sign not found!");
        return {std::basic_string<CharT,Traits,Allocator>(source.substr(0,pos),alloc),std::basic_string<CharT,Traits,Allocator>(source.substr(pos+1,source.size()),alloc)};
    }
    template<typename CharT,typename Traits=std::char_traits<CharT>,typename Allocator=std::allocator<CharT>,typename View> requires std::is_convertible_v<const View&, std::basic_string_view<CharT, Traits>>
    std::pair<std::basic_string<CharT,Traits,Allocator>,std::basic_string<CharT,Traits,Allocator>> split_last(const View& source,const CharT sign,const Allocator& alloc=Allocator{}){
        std::basic_string_view<CharT,Traits> sv=source;
        return split_last<CharT,Traits,Allocator>(sv,sign,alloc);
    }

    template<typename CharT,typename Traits>
    std::pair<std::basic_string_view<CharT,Traits>,std::basic_string_view<CharT,Traits>> vsplit_first(std::basic_string_view<CharT,Traits> source,const CharT sign){
        typename std::basic_string_view<CharT,Traits>::size_type pos=source.find(sign);
        if(pos==std::basic_string_view<CharT,Traits>::npos)
            throw std::runtime_error("Split-sign not found!");
        return {source.substr(0,pos),source.substr(pos+1,source.size())};
    }
    template<typename CharT,typename Traits=std::char_traits<CharT>,typename View> requires std::is_convertible_v<const View&, std::basic_string_view<CharT, Traits>>
    std::pair<std::basic_string_view<CharT,Traits>,std::basic_string_view<CharT,Traits>> vsplit_first(const View& source,const CharT sign){
        std::basic_string_view<CharT,Traits> sv=source;
        return vsplit_first<CharT,Traits>(sv,sign);
    }

    template<typename CharT,typename Traits>
    std::pair<std::basic_string_view<CharT,Traits>,std::basic_string_view<CharT,Traits>> vsplit_last(std::basic_string_view<CharT,Traits> source,const CharT sign){
        typename std::basic_string_view<CharT,Traits>::size_type pos=source.rfind(sign);
        if(pos==std::basic_string_view<CharT,Traits>::npos)
            throw std::runtime_error("Split-sign not found!");
        return {source.substr(0,pos),source.substr(pos+1,source.size())};
    }
    template<typename CharT,typename Traits=std::char_traits<CharT>,typename View> requires std::is_convertible_v<const View&, std::basic_string_view<CharT, Traits>>
    std::pair<std::basic_string_view<CharT,Traits>,std::basic_string_view<CharT,Traits>> vsplit_last(const View& source,const CharT sign){
        std::basic_string_view<CharT,Traits> sv=source;
        return vsplit_last<CharT,Traits>(sv,sign);
    }

    inline std::pair<std::string,std::string> split_filename(std::string_view filename){
        std::string_view::size_type pos=filename.rfind('.');
        if(pos==std::string_view::npos)
            return {{filename.data(),filename.size()},{}};
        return {std::string(filename.substr(0,pos)),std::string(filename.substr(pos+1,filename.size()))};
    }

    inline std::pair<std::string_view,std::string_view> vsplit_filename(std::string_view filename){
        std::string_view::size_type pos=filename.rfind('.');
        if(pos==std::string_view::npos)
            return {filename,{}};
        return {filename.substr(0,pos),filename.substr(pos+1,filename.size())};
    }


    inline std::time_t internal_time_;

    template<typename... Ts>
    void dbgout_base(FILE* fp, const char* program_name, std::string_view fmt, const Ts&... messages){
        std::time(&internal_time_);
        fmt::print(fp,"YDBG - {} - {:%Y-%m-%d %H:%M:%S} - ",program_name,fmt::localtime(internal_time_));
        fmt::print(fp,fmt,messages...);
        fflush(fp);
    }
}

namespace yuki{
    //* Weirdly, `std::count(_if)` returns `difference_type` rather than `size_type`. So I come up with this.
    template<typename InputIt,typename T>
    constexpr auto count(InputIt first, InputIt last, const T& value) -> typename std::iterator_traits<InputIt>::size_type {
        typename std::iterator_traits<InputIt>::difference_type n = 0;
        for (; first != last; ++first){
            if (*first == value)
                ++n;
        }
        return n;
    }
    template<typename InputIt,typename UnaryPredicate>
    constexpr auto count_if(InputIt first, InputIt last, UnaryPredicate p) -> typename std::iterator_traits<InputIt>::difference_type {
        typename std::iterator_traits<InputIt>::difference_type n = 0;
        for (; first != last; ++first){
            if (p(*first))
              ++n;
        }
        return n;
    }

    // To be specialized for rough compare.
    template<typename T>
    struct rough_less{
        constexpr bool operator()(const T& lhs,const T& rhs) const {return lhs<rhs;}
    };
    template<typename T>
    struct rough_greater{
        constexpr bool operator()(const T& lhs,const T& rhs) const {return lhs>rhs;}
    };

    template<typename T,typename Compare = std::less<T>>
    struct equiv{
        private:
            Compare comp;
        public:
            constexpr equiv() noexcept = default;
            explicit constexpr equiv(const Compare& comp_other) noexcept(std::is_nothrow_copy_constructible_v<Compare>) : comp(comp_other) {}
            constexpr Compare get_compare() const noexcept {return comp;}
            constexpr void set_compare(const Compare& comp_other) noexcept(std::is_nothrow_copy_assignable_v<Compare>) {comp=comp_other;}
            constexpr bool operator()(const T& lhs,const T& rhs) const {return !comp(lhs,rhs) && !comp(rhs,lhs);}
    };
}