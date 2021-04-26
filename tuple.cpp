// Flat-tuple illustration.
#include<yuki/utils.hpp>
template<size_t index_p,typename T>
struct tuple_unit_{
    static constexpr size_t index=index_p;
    typedef T type;
    T member;
};
template<typename T,typename... Ts>
struct tuple_impl_;
template<size_t... ints,typename... Ts>
struct tuple_impl_<std::index_sequence<ints...>,Ts...> : tuple_unit_<ints,Ts>... {
    template<size_t i>
    using type=yuki::type_switch_t<i,Ts...>;
    template<size_t i>
    using unit=yuki::type_switch_t<i,tuple_unit_<ints,Ts>...>;
};
template<typename... Ts>
struct tuple : tuple_impl_<std::make_index_sequence<sizeof...(Ts)>,Ts...> {};
int main(){
    tuple<int,int,int,int> tp{};
    tp.decltype(tp)::unit<2>::member=100;
    fmt::print("{}",tp.decltype(tp)::unit<2>::member);
}