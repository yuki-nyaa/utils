#pragma once

namespace yuki{
template<typename It>
struct IB_Pair{
    It iterator;
    bool has_inserted;

    constexpr decltype(auto) operator*() const {return *iterator;}
    constexpr It operator->() const {return iterator;}
};

// `std::pair` is too complicated IMO. And sometimes inefficient, creating one more unnecessary copy when the compiler can't deduce type for braced-init-list.
template<typename T0,typename T1>
struct Pair{
    T0 zeroth;
    T1 first;
};

template<typename T0,typename T1,typename T2>
struct Triple {T0 zeroth;T1 first;T2 second;};
}
