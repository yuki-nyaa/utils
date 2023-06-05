#pragma once
#include<yuki/tmp.hpp>

namespace yuki{
// A flat tuple. Also an aggregate, after C++17.
template<size_t,typename T>
struct Tuple_Unit_ {T member;};

template<typename T,typename... Ts>
struct Tuple_Impl_;

template<size_t... ints,typename... Ts>
struct Tuple_Impl_<std::index_sequence<ints...>,Ts...> : Tuple_Unit_<ints,Ts>... {
    static constexpr size_t size = sizeof...(ints);
    template<size_t i>
    using member_type=yuki::type_switch_t<i,Ts...>;
    template<size_t i>
    using unit_type=yuki::type_switch_t<i,Tuple_Unit_<ints,Ts>...>;

    template<size_t i>
    constexpr auto& get() & noexcept {
        static_assert(i<sizeof...(ints),"The requested index is greater than the max.");
        return unit_type<i>::member;
    }
    template<size_t i>
    constexpr const auto& get() const& noexcept {
        static_assert(i<sizeof...(ints),"The requested index is greater than the max.");
        return unit_type<i>::member;
    }
    template<size_t i>
    constexpr member_type<i>&& get() && noexcept {
        static_assert(i<sizeof...(ints),"The requested index is greater than the max.");
        return static_cast<member_type<i>&&>(unit_type<i>::member);
    }
    template<size_t i>
    constexpr const member_type<i>&& get() const&& noexcept {
        static_assert(i<sizeof...(ints),"The requested index is greater than the max.");
        return static_cast<const member_type<i>&&>(unit_type<i>::member);
    }

    template<typename U>
    constexpr U& get() & noexcept {
        static_assert(yuki::is_any_of_v<U,Ts...>,"The requested type is not among the member types!");
        static_assert(yuki::is_unique_v<U,Ts...>,"The requested type is not unique!");
        return Tuple_Unit_<yuki::type_to_num_v<U,Ts...>,U>::member;
    }
    template<typename U>
    constexpr const U& get() const& noexcept {
        static_assert(yuki::is_any_of_v<U,Ts...>,"The requested type is not among the member types!");
        static_assert(yuki::is_unique_v<U,Ts...>,"The requested type is not unique!");
        return Tuple_Unit_<yuki::type_to_num_v<U,Ts...>,U>::member;
    }
    template<typename U>
    constexpr U&& get() && noexcept {
        static_assert(yuki::is_any_of_v<U,Ts...>,"The requested type is not among the member types!");
        static_assert(yuki::is_unique_v<U,Ts...>,"The requested type is not unique!");
        return static_cast<U&&>(Tuple_Unit_<yuki::type_to_num_v<U,Ts...>,U>::member);
    }
    template<typename U>
    constexpr const U&& get() const&& noexcept {
        static_assert(yuki::is_any_of_v<U,Ts...>,"The requested type is not among the member types!");
        static_assert(yuki::is_unique_v<U,Ts...>,"The requested type is not unique!");
        return static_cast<const U&&>(Tuple_Unit_<yuki::type_to_num_v<U,Ts...>,U>::member);
    }
};

template<typename... Ts>
struct Tuple : Tuple_Impl_<std::make_index_sequence<sizeof...(Ts)>,Ts...> {};

static_assert(std::is_aggregate_v<Tuple<unsigned,int,double,const char*,const int&>>,"You need C++17 or later to make `yuki::Tuple` an aggregate!");
} // namespace yuki

namespace std{
template<typename ... Ts>
struct tuple_size<yuki::Tuple<Ts...>> : integral_constant<size_t,sizeof...(Ts)> {};
template<size_t I,typename... Ts>
struct tuple_element<I,yuki::Tuple<Ts...>> {typedef yuki::type_switch_t<I,Ts...> type;};
} // namespace std
