#pragma once
#include<cassert>
#include<yuki/tmp.hpp>

#include<memory>

// Adapted from https://github.com/groundswellaudio/swl-variant

namespace yuki{

namespace variant_impl_{
    /// dummy
    struct dm {static constexpr size_t elements = 1;};

    template<size_t I,typename A,typename B>
    struct uoe_alternative_impl {typedef A type;};
    template<typename A,typename B>
    struct uoe_alternative_impl<1,A,B> {typedef B type;};

    /// union of elements
    template<typename A,typename B>
    union uoe{
        A a;
        B b;

        static constexpr size_t elements = 2 ;

        template<size_t I>
        using alternative_t = typename uoe_alternative_impl<I,A,B>::type;

        constexpr uoe() noexcept = default;

        //constexpr uoe(const uoe&) noexcept requires (std::is_trivially_copy_constructible_v<A> && std::is_trivially_copy_constructible_v<B>) = default;
        constexpr uoe(const uoe&) noexcept {}

        //constexpr uoe(uoe&&) noexcept requires (std::is_trivially_move_constructible_v<A> && std::is_trivially_move_constructible_v<B>) = default;
        constexpr uoe(uoe&&) noexcept = default;

        constexpr ~uoe() noexcept requires (std::is_trivially_destructible_v<A> && std::is_trivially_destructible_v<B>) = default;
        constexpr ~uoe() noexcept {}

        //constexpr uoe& operator=(const uoe&) noexcept requires (std::is_trivially_copy_assignable_v<A> && std::is_trivially_copy_assignable_v<B>) = default;
        constexpr uoe& operator=(const uoe&) noexcept = default;

        //constexpr uoe& operator=(uoe&&) noexcept requires (std::is_trivially_move_assignable_v<A> && std::is_trivially_move_assignable_v<B>) = default;
        constexpr uoe& operator=(uoe&&) noexcept = default;

        template<typename... Args>
        explicit constexpr uoe(std::in_place_index_t<0>,Args&&... args) noexcept : a(std::forward<Args>(args)...) {}
        template<typename... Args>
        explicit constexpr uoe(std::in_place_index_t<1>,Args&&... args) noexcept : b(std::forward<Args>(args)...) {}

        template<size_t I>
        constexpr auto& get() & {
            if constexpr(I==0) return a;
            else return b;
        }

        template<size_t I>
        constexpr const auto& get() const& {
            if constexpr(I==0) return a;
            else return b;
        }

        template<size_t I>
        constexpr auto&& get() && {
            if constexpr(I==0) return a;
            else return b;
        }

        template<size_t I>
        constexpr const auto&& get() const&& {
            if constexpr(I==0) return a;
            else return b;
        }

        //constexpr void destroy(size_t i) noexcept {
        //    if(i==0) a.~A();
        //    else b.~B();
        //}

        //template<typename V>
        //constexpr void construct_from_variant(const size_t i,V&& v){
        //    if(i==0)
        //        std::construct_at<A>(std::addressof(a),std::forward<V>(v).a);
        //    else
        //        std::construct_at<B>(std::addressof(b),std::forward<V>(v).b);
        //}
    };

    template<size_t I,typename U0,typename U1,size_t elements0>
    struct uou_alternative_impl {typedef typename U1::template alternative_t<I-(elements0/2)-1> type;};
    template<size_t I,typename U0,typename U1,size_t elements0> requires (I<=elements0/2)
    struct uou_alternative_impl<I,U0,U1,elements0> {typedef typename U0::template alternative_t<I> type;};

    /// union of unions
    template<typename U0,typename U1>
    union uou{
        U0 u0;
        U1 u1;

        static constexpr size_t elements = U0::elements + U1::elements;
        static constexpr size_t elements0 = elements-1;

        template<size_t I>
        using alternative_t = typename uou_alternative_impl<I,U0,U1,elements0>::type;

        constexpr uou() noexcept = default;

        //constexpr uou(const uou&) noexcept requires (std::is_trivially_copy_constructible_v<U0> && std::is_trivially_copy_constructible_v<U1>) = default;
        constexpr uou(const uou&) noexcept = default;

        //constexpr uou(uou&&) noexcept requires (std::is_trivially_move_constructible_v<U0> && std::is_trivially_move_constructible_v<U1>) = default;
        constexpr uou(uou&&) noexcept = default;

        constexpr ~uou() noexcept requires (std::is_trivially_destructible_v<U0> && std::is_trivially_destructible_v<U1>) = default;
        constexpr ~uou() noexcept {}

        //constexpr uou& operator=(const uou&) noexcept requires (std::is_trivially_copy_assignable_v<U0> && std::is_trivially_copy_assignable_v<U1>) = default;
        constexpr uou& operator=(const uou&) noexcept = default;

        //constexpr uou& operator=(uou&&) noexcept requires (std::is_trivially_move_assignable_v<U0> && std::is_trivially_move_assignable_v<U1>) = default;
        constexpr uou& operator=(uou&&) noexcept = default;

        template<size_t I,typename... Args> requires (I<=elements0/2)
        explicit constexpr uou(std::in_place_index_t<I>,Args&&... args) noexcept : u0(std::in_place_index<I>,std::forward<Args>(args)...) {}
        template<size_t I,typename... Args>
        explicit constexpr uou(std::in_place_index_t<I>,Args&&... args) noexcept : u1(std::in_place_index<I-(elements0/2)-1>,std::forward<Args>(args)...) {}

        template<size_t I>
        constexpr auto& get() & {
            if constexpr(I<=elements0/2) return u0.template get<I>();
            else return u1.template get<I-(elements0/2)-1>();
        }

        template<size_t I>
        constexpr const auto& get() const& {
            if constexpr(I<=elements0/2) return u0.template get<I>();
            else return u1.template get<I-(elements0/2)-1>();
        }

        template<size_t I>
        constexpr auto&& get() && {
            if constexpr(I<=elements0/2) return u0.template get<I>();
            else return u1.template get<I-(elements0/2)-1>();
        }

        template<size_t I>
        constexpr const auto&& get() const&& {
            if constexpr(I<=elements0/2) return u0.template get<I>();
            else return u1.template get<I-(elements0/2)-1>();
        }

        //constexpr void destroy(size_t i){
        //    if(i<=elements0/2)
        //        u0.destroy(i);
        //    else{
        //        if constexpr (!std::is_same_v<U1,dm>)
        //            u1.destroy(i-(elements0/2)-1);
        //    }
        //}

        //template<typename V>
        //constexpr void construct_from_variant(const size_t i,V&& v){
        //    if(i<=elements0/2)
        //        u0.construct_from_variant(i,std::forward<V>(v).u0);
        //    else{
        //        if constexpr (!std::is_same_v<U1,dm>)
        //            u1.construct_from_variant(i-(elements0/2)-1,std::forward<V>(v).u1);
        //    }
        //}
    };

    // This should really be an alias template, if alias template can be specialized.
    template<bool is_first_pass,size_t remaining,typename... Ts>
    struct tree_union_helper;

    template<size_t remaining,typename A,typename B,typename... Ts> requires (remaining>=2)
    struct tree_union_helper<true,remaining,A,B,Ts...>{
        using type = typename tree_union_helper<true,remaining-2,Ts...,uoe<A,B>>::type;
    };
    template<typename A,typename... Ts>
    struct tree_union_helper<true,1,A,Ts...>{
        using type = typename tree_union_helper<false,sizeof...(Ts)+1,Ts...,uoe<A,dm>>::type;
    };
    template<typename... Ts>
    struct tree_union_helper<true,0,Ts...>{
        using type = typename tree_union_helper<false,sizeof...(Ts),Ts...>::type;
    };

    template<size_t remaining,typename A,typename B,typename... Ts> requires (remaining>=2)
    struct tree_union_helper<false,remaining,A,B,Ts...>{
        using type = typename tree_union_helper<false,remaining-2,Ts...,uou<A,B>>::type;
    };
    template<typename A,typename... Ts>
    struct tree_union_helper<false,1,A,Ts...>{
        using type = typename tree_union_helper<false,sizeof...(Ts)+1,Ts...,uou<A,dm>>::type;
    };
    template<typename... Ts>
    struct tree_union_helper<false,0,Ts...>{
        using type = typename tree_union_helper<false,sizeof...(Ts),Ts...>::type;
    };

    template<typename T>
    struct tree_union_helper<false,1,T>{
        using type = T;
    };

    template<typename... Ts>
    using tree_union = typename tree_union_helper<true,sizeof...(Ts),Ts...>::type;

    /*
    As an example:
        A B C D E -> C D E <A,B> -> E <A,B> <C,D> -> <A,B> <C,D> <E,dm> ->
        <E,dm> <<A,B>,<C,D>> -> <<A,B>,<C,D>> <<E,dm>,dm> ->
        <<<A,B>,<C,D>>,<<E,dm>,dm>>
    */


    #define YUKI_VARIANT_4(n,X) X(n) X(n+1) X(n+2) X(n+3)
    #define YUKI_VARIANT_8(n,X) YUKI_VARIANT_4(n,X) YUKI_VARIANT_4(n+4,X)
    #define YUKI_VARIANT_16(n,X) YUKI_VARIANT_4(n,X) YUKI_VARIANT_4(n+4,X) YUKI_VARIANT_4(n+8,X) YUKI_VARIANT_4(n+12,X)
    #define YUKI_VARIANT_32(n,X) YUKI_VARIANT_8(n,X) YUKI_VARIANT_8(n+8,X) YUKI_VARIANT_8(n+16,X) YUKI_VARIANT_8(n+24,X)
    #define YUKI_VARIANT_64(n,X) YUKI_VARIANT_16(n,X) YUKI_VARIANT_16(n+16,X) YUKI_VARIANT_16(n+32,X) YUKI_VARIANT_16(n+48,X)
    #define YUKI_VARIANT_128(n,X) YUKI_VARIANT_32(n,X) YUKI_VARIANT_32(n+32,X) YUKI_VARIANT_32(n+64,X) YUKI_VARIANT_32(n+96,X)
    #define YUKI_VARIANT_256(n,X) YUKI_VARIANT_64(n,X) YUKI_VARIANT_64(n+64,X) YUKI_VARIANT_64(n+128,X) YUKI_VARIANT_64(n+192,X)

    #define YUKI_VARIANT_SINGLE_VISIT_CASE(n) \
        case (n) : \
    		if constexpr(n < V_nocvref::size) \
			    return std::forward<F>(f)(std::forward<V>(v).template get<n>(),std::forward<Vs>(vs).template get<n>()...); \

    #define YUKI_VARIANT_SINGLE_VISIT_N(n) \
        template<typename R,typename F,typename V,typename... Vs> \
        constexpr R single_visit_##n(F&& f,V&& v,Vs&&... vs){ \
            typedef std::remove_cvref_t<V> V_nocvref; \
            switch(v.index()){ \
                YUKI_VARIANT_##n(0,YUKI_VARIANT_SINGLE_VISIT_CASE) \
            } \
            __builtin_unreachable();\
        }

    YUKI_VARIANT_SINGLE_VISIT_N(4)
    YUKI_VARIANT_SINGLE_VISIT_N(8)
    YUKI_VARIANT_SINGLE_VISIT_N(16)
    YUKI_VARIANT_SINGLE_VISIT_N(32)
    YUKI_VARIANT_SINGLE_VISIT_N(64)
    YUKI_VARIANT_SINGLE_VISIT_N(128)
    YUKI_VARIANT_SINGLE_VISIT_N(256)

    #undef YUKI_VARIANT_SINGLE_VISIT_N
    #undef YUKI_VARIANT_SINGLE_VISIT_CASE
    #undef YUKI_VARIANT_256
    #undef YUKI_VARIANT_128
    #undef YUKI_VARIANT_64
    #undef YUKI_VARIANT_32
    #undef YUKI_VARIANT_16
    #undef YUKI_VARIANT_8
    #undef YUKI_VARIANT_4

    template<size_t i,typename R,typename F,typename V,typename... Vs>
    constexpr R visit_at(F&& f,V&& v,Vs&&... vs) {return std::forward<F>(f)(std::forward<V>(v).template get<i>(),std::forward<Vs>(vs).template get<i>()...);}

    template<typename Seq,typename R,typename F,typename V,typename... Vs>
    struct single_visit_by_table;

    template<typename R,typename F,typename V,typename... Vs,size_t... is>
    struct single_visit_by_table<std::index_sequence<is...>,R,F,V,Vs...>{
        static constexpr R (*table[sizeof...(is)])(F&&,V&&,Vs&&...) = {&visit_at<is,R,F,V,Vs...>...};
        static constexpr R visit(F&& f,V&& v,Vs&&... vs) {return table[v.index()](std::forward<F>(f),std::forward<V>(v),std::forward<Vs>(vs)...);}
    };


    template<typename T>
    struct holds_alternative_impl;

    template<size_t... is>
    struct holds_alternative_impl<std::index_sequence<is...>> {
        static constexpr bool f(const size_t i) {return (... || (i==is));}
    };


    template<size_t I,typename T>
    struct overload_ret{
        static constexpr size_t value = I;
        typedef T type;
    };

    template<size_t I,typename T,bool>
    struct overload_unit {static overload_ret<I,T> f(T);};
    template<size_t I,typename T>
    struct overload_unit<I,T,false> {static void f();};

    template<typename Seq,typename S,typename... Ts>
    struct select_overload;

    template<size_t... is,typename S,typename... Ts>
    struct select_overload<std::index_sequence<is...>,S,Ts...> : overload_unit<is,Ts,yuki::is_braced_init_list_convertible_v<Ts,S>>... {using overload_unit<is,Ts,yuki::is_braced_init_list_convertible_v<Ts,S>>::f...;};
} // namespace variant_impl_

template<typename R,typename F,typename V>
constexpr R visit(F&& f,V&& v){
    typedef std::remove_cvref_t<V> V_nocvref;

    #define YUKI_VARIANT_VISIT_BRANCH(n) \
        else if constexpr(V_nocvref::size<=n) \
            return variant_impl_::single_visit_##n<R>(std::forward<F>(f),std::forward<V>(v));

    if constexpr(V_nocvref::size==1)
        return std::forward<F>(f)(std::forward<V>(v).template get<0>());
    YUKI_VARIANT_VISIT_BRANCH(4)
    YUKI_VARIANT_VISIT_BRANCH(8)
    YUKI_VARIANT_VISIT_BRANCH(16)
    YUKI_VARIANT_VISIT_BRANCH(32)
    YUKI_VARIANT_VISIT_BRANCH(64)
    YUKI_VARIANT_VISIT_BRANCH(128)
    YUKI_VARIANT_VISIT_BRANCH(256)
    else
        return variant_impl_::single_visit_by_table<std::make_index_sequence<V_nocvref::size>,R,F,V>::visit(std::forward<F>(f),std::forward<V>(v));

    #undef YUKI_VARIANT_VISIT_BRANCH
}

template<typename R,typename F,typename V0,typename... Vs>
constexpr R visit(F&& f,V0&& v0,Vs&&... vs){
    return visit<R>(
        [&f,&v0]<typename... Args>(Args&&... vsp) -> R {
            return visit<R>(
                [&f,&vsp...]<typename T>(T&& t)
                    {return std::forward<F>(f)(std::forward<T>(t),std::forward<Args>(vsp)...);},
                std::forward<V0>(v0)
            );
        },
        std::forward<Vs>(vs)...
    );
}

template<int=0,typename F,typename... Vs>
constexpr auto visit(F&& f,Vs&&... vs) -> decltype(std::forward<F>(f)(std::forward<Vs>(vs).template get<0>()...)) {
    typedef decltype(std::forward<F>(f)(std::forward<Vs>(vs).template get<0>()...)) R;
    return visit<R,F,Vs...>(std::forward<F>(f),std::forward<Vs>(vs)...);
}

template<int=0,typename F>
constexpr void visit(F&&) {}

template<typename R,typename F,typename V,typename... Vs>
constexpr R visit_homogenous(F&& f,V&& v,Vs&&... vs){
    {
    const auto i = v.index();
    assert((... && (vs.index()==i)));
    }

    typedef std::remove_cvref_t<V> V_nocvref;

    #define YUKI_VARIANT_VISITH_BRANCH(n) \
        else if constexpr(V_nocvref::size<=n) \
            return variant_impl_::single_visit_##n<R>(std::forward<F>(f),std::forward<V>(v),std::forward<Vs>(vs)...);

    if constexpr(V_nocvref::size==1)
        return std::forward<F>(f)(std::forward<V>(v).template get<0>(),std::forward<Vs>(vs).template get<0>()...);
    YUKI_VARIANT_VISITH_BRANCH(4)
    YUKI_VARIANT_VISITH_BRANCH(8)
    YUKI_VARIANT_VISITH_BRANCH(16)
    YUKI_VARIANT_VISITH_BRANCH(32)
    YUKI_VARIANT_VISITH_BRANCH(64)
    YUKI_VARIANT_VISITH_BRANCH(128)
    YUKI_VARIANT_VISITH_BRANCH(256)
    else
        return variant_impl_::single_visit_by_table<std::make_index_sequence<V_nocvref::size>,R,F,V,Vs...>::visit(std::forward<F>(f),std::forward<V>(v),std::forward<Vs>(vs)...);

    #undef YUKI_VARIANT_VISITH_BRANCH
}

template<int=0,typename F,typename... Vs>
constexpr auto visit_homogenous(F&& f,Vs&&... vs) -> decltype(std::forward<F>(f)(std::forward<Vs>(vs).template get<0>()...)) {
    typedef decltype(std::forward<F>(f)(std::forward<Vs>(vs).template get<0>()...)) R;
    return visit_homogenous<R,F,Vs...>(std::forward<F>(f),std::forward<Vs>(vs)...);
}

template<int=0,typename F>
constexpr void visit_homogenous(F&&) {}


inline constexpr struct destroy_visitor_t{
    template<typename T>
    static constexpr void operator()(T&& t) {
        typedef std::remove_cvref_t<T> T_nocvref;
        t.~T_nocvref();
    }
} destroy_visitor;

inline constexpr struct copy_visitor_t{
    template<typename Vt,typename Vs>
    static constexpr void operator()(Vt& vt,Vs&& vs) noexcept(std::is_constructible_v<Vt,Vs&&>) {
        if constexpr(std::is_constructible_v<Vt,Vs&&>)
            std::construct_at(std::addressof(vt),std::forward<Vs>(vs));
        else
            throw std::runtime_error("yuki::Variant: Attempt to copy/move a non-copy(move)able alternative!");
    }
} copy_visitor;

inline constexpr struct assign_visitor_t{
    template<typename Vt,typename Vs>
    static constexpr void operator()(Vt&& vt,Vs&& vs) noexcept(std::is_assignable_v<Vt&&,Vs&&>) {
        if constexpr(std::is_assignable_v<Vt&&,Vs&&>)
            std::forward<Vt>(vt)=std::forward<Vs>(vs);
        else
            throw std::runtime_error("yuki::Variant: Attempt to copy/move assign a non-copy(move)-assignable alternative!");
    }
} assign_visitor;

inline constexpr struct swap_visitor_t{
    template<typename Vt,typename Vs>
    static constexpr void operator()(Vt&& vt,Vs&& vs) noexcept(std::is_swappable_with_v<Vt&&,Vs&&>) {
        if constexpr(std::is_swappable_with_v<Vt&&,Vs&&>){
            using std::swap;
            swap(std::forward<Vt>(vt),std::forward<Vs>(vs));
        }else
            throw std::runtime_error("yuki::Variant: Attempt to swap a non-swappable alternative!");
    }
} swap_visitor;

template<typename T>
struct get_visitor_t{
    template<typename V>
    static constexpr T* operator()(V& v){
        if constexpr(std::is_constructible_v<T*,V*>)
            return std::addressof(v);
        else
            return nullptr;
    }
};

template<typename T>
inline constexpr get_visitor_t<T> get_visitor;

template<typename T>
struct is_assignable_from_visitor_t{
    template<typename V>
    static constexpr bool operator()(V&&) {return std::is_assignable_v<V&&,T>;}
};

template<typename T>
inline constexpr is_assignable_from_visitor_t<T> is_assignable_from_visitor;


template<typename... Ts>
struct Variant{
  public:
    static constexpr size_t size = sizeof...(Ts);
    typedef yuki::uint_auto_t<sizeof...(Ts)> index_t;
    template<size_t I>
    using alternative_t = typename variant_impl_::tree_union<Ts...>::template alternative_t<I>;
  private:
    union{
        variant_impl_::tree_union<Ts...> tu_;
        variant_impl_::dm dm_;
    };
    index_t active_ = 0;
  public:
    constexpr index_t index() const {return active_;}

    template<typename T>
    constexpr bool holds_alternative() const {return variant_impl_::holds_alternative_impl<yuki::find_type_t<T,Ts...>>::f(active_);}

    constexpr Variant() noexcept requires ((... && std::is_trivially_default_constructible_v<Ts>) || !std::is_trivially_default_constructible_v<yuki::type_switch_t<0,Ts...>>) = default;
    constexpr Variant() noexcept :
        tu_(std::in_place_index<0>),
        active_(0)
    {}

    constexpr Variant(const Variant&) noexcept requires (... && std::is_trivially_copy_constructible_v<Ts>) = default;
    constexpr Variant(const Variant& other) noexcept :
        dm_(),
        active_(other.active_)
    {visit_homogenous<void>(copy_visitor,*this,other);}

    constexpr Variant(Variant&&) noexcept requires (... && std::is_trivially_move_constructible_v<Ts>) = default;
    constexpr Variant(Variant&& other) noexcept :
        dm_(),
        active_(other.active_)
    {visit_homogenous<void>(copy_visitor,*this,std::move(other));}

    constexpr ~Variant() noexcept requires (... && std::is_trivially_destructible_v<Ts>) = default;
    constexpr ~Variant() noexcept {visit<void>(destroy_visitor,*this);}

    constexpr Variant& operator=(const Variant&) noexcept requires (... && std::is_trivially_copy_assignable_v<Ts>) = default;
    constexpr Variant& operator=(const Variant& other) noexcept {
        if(this!=&other){
            if(active_==other.active_)
                visit_homogenous<void>(assign_visitor,*this,other);
            else{
                visit<void>(destroy_visitor,*this);
                active_=other.active_;
                visit_homogenous<void>(copy_visitor,*this,other);
            }
        }
        return *this;
    }

    constexpr Variant& operator=(Variant&&) noexcept requires (... && std::is_trivially_move_assignable_v<Ts>) = default;
    constexpr Variant& operator=(Variant&& other) noexcept {
        if(this!=&other){
            if(active_==other.active_)
                visit_homogenous<void>(assign_visitor,*this,std::move(other));
            else{
                visit<void>(destroy_visitor,*this);
                active_=other.active_;
                visit_homogenous<void>(copy_visitor,*this,std::move(other));
            }
        }
        return *this;
    }

    friend constexpr void swap(Variant& lhs,Variant& rhs) noexcept {
        if(lhs.active_==rhs.active_)
            visit_homogenous<void>(swap_visitor,lhs,rhs);
        else{
            Variant temp = std::move(lhs);
            visit<void>(destroy_visitor,lhs);
            lhs.active_=rhs.active_;
            visit_homogenous<void>(copy_visitor,lhs,std::move(rhs));
            visit<void>(destroy_visitor,rhs);
            rhs.active_=temp.active_;
            visit_homogenous<void>(copy_visitor,rhs,std::move(temp));
        }
    }

    template<size_t I,typename... Args>
    explicit constexpr Variant(std::in_place_index_t<I>,Args&&... args) noexcept :
        tu_(std::in_place_index<I>,std::forward<Args>(args)...),
        active_(I)
    {}

    template<typename T,typename... Args,size_t I=yuki::type_to_num_v<T,Ts...>>
    explicit constexpr Variant(std::in_place_type_t<T>,Args&&... args) noexcept :
        tu_(std::in_place_index<I>,std::forward<Args>(args)...),
        active_(I)
    {}

    template<typename T,typename=std::enable_if_t<!std::is_same_v<std::remove_cvref_t<T>,Variant>>>
    constexpr Variant(T&& t) noexcept :
        Variant(std::in_place_index<decltype(variant_impl_::select_overload<std::make_index_sequence<size>,T&&,Ts...>::f({std::forward<T>(t)}))::value>,std::forward<T>(t))
    {}

    template<size_t I>
    constexpr decltype(auto) get() & {assert(I==active_);return tu_.template get<I>();}
    template<size_t I>
    constexpr decltype(auto) get() const& {assert(I==active_);return tu_.template get<I>();}
    template<size_t I>
    constexpr decltype(auto) get() && {assert(I==active_);return tu_.template get<I>();}
    template<size_t I>
    constexpr decltype(auto) get() const&& {assert(I==active_);return tu_.template get<I>();}

    template<typename T,size_t I=yuki::type_to_num_v<T,Ts...>>
    constexpr T& get() & {
        static_assert(I!=yuki::type_to_num_nomatch);
        return get<I>();
    }
    template<typename T,size_t I=yuki::type_to_num_v<T,Ts...>>
    constexpr const T& get() const& {
        static_assert(I!=yuki::type_to_num_nomatch);
        return get<I>();
    }
    template<typename T,size_t I=yuki::type_to_num_v<T,Ts...>>
    constexpr T&& get() && {
        static_assert(I!=yuki::type_to_num_nomatch);
        return get<I>();
    }
    template<typename T,size_t I=yuki::type_to_num_v<T,Ts...>>
    constexpr const T&& get() const&& {
        static_assert(I!=yuki::type_to_num_nomatch);
        return get<I>();
    }

    template<size_t I,typename... Args>
    constexpr alternative_t<I>& emplace(Args&&... args){
        static_assert(std::is_constructible_v<alternative_t<I>,Args&&...>);
        visit<void>(destroy_visitor,*this);
        active_=I;
        visit<void>(
            [&args...]<typename U>(U& u){
            if constexpr(std::is_constructible_v<U,Args&&...>)
                std::construct_at(std::addressof(u),std::forward<Args>(args)...);
            },
            *this
        );
        return get<I>();
    }

    template<typename T,typename... Args,size_t I=yuki::type_to_num_v<T,Ts...>>
    constexpr T& emplace(Args&&... args){
        static_assert(I!=yuki::type_to_num_nomatch);
        return emplace<I>(std::forward<Args>(args)...);
    }

    template<typename T,typename=std::enable_if_t<!std::is_same_v<std::remove_cvref_t<T>,Variant>>,size_t I=decltype(variant_impl_::select_overload<std::make_index_sequence<size>,T&&,Ts...>::f(std::declval<T>()))::value>
    constexpr alternative_t<I>& operator=(T&& t) noexcept {return emplace<I>(std::forward<T>(t));}

    template<size_t I,typename T>
    constexpr alternative_t<I>& emplace_or_assign(T&& t){
        static_assert(std::is_constructible_v<alternative_t<I>,T&&>);
        if(std::is_assignable_v<alternative_t<I>&,T&&> && I==active_){
            visit<void>([&t]<typename U>(U&& u){if constexpr(std::is_assignable_v<U&&,T&&>) std::forward<U>(u)=std::forward<T>(t);},*this);
            return get<I>();
        }else
            return emplace<I>(std::forward<T>(t));
    }

    template<typename T,typename U,size_t I=yuki::type_to_num_v<T,Ts...>>
    constexpr T& emplace_or_assign(U&& u){
        static_assert(I!=yuki::type_to_num_nomatch);
        if(std::is_assignable_v<T&,U&&> && holds_alternative<T>()){
            visit<void>([&u]<typename V>(V&& v){if constexpr(std::is_assignable_v<V&&,U&&>) std::forward<V>(v)=std::forward<U>(u);},*this);
            return *visit<T*>(get_visitor<T>,*this);
        }else
            return emplace<I>(std::forward<U>(u));
    }

    template<void* =nullptr,typename T,typename U=decltype(variant_impl_::select_overload<std::make_index_sequence<size>,T&&,Ts...>::f(std::declval<T>()))>
    constexpr void emplace_or_assign(T&& t){
        if(visit<bool>(is_assignable_from_visitor<T&&>,*this))
            visit<void>([&t]<typename V>(V&& v){if constexpr(std::is_assignable_v<V&&,T&&>) std::forward<V>(v)=std::forward<T>(t);},*this);
        else
            emplace<U::value>(std::forward<T>(t));
    }
};

template<>
struct Variant<> {
    static constexpr size_t size = 0;
    static constexpr size_t index() {return 0;}
};

struct Variant_Dummy {};

} // namespace yuki

/*
Implementation notes:

Performing a construction-visit after a destruction-visit actually is a UB, since the member function `.get<J>` is called on a union whose lifetime has already been ended. However, it looks like a non-UB approach would involve either a `reinterpret_cast` or a `new` expression, any of which is explicitly forbidden to appear in a `constexpr` function. So in short, either "UB and constexpr" or "no UB but no constexpr".
(This is not entirely correct, though, since a UB also cannot be `constexpr`. But since GCC 12.1 does not complain and produces desired results, I've decided to choose the UB approach.)
(Clang 14.0 seems to be unable to perform the correct overload resolution among constrained functions so I am unable to test the code with it.)
*/
