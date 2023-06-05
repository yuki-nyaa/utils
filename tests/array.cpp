#include<yuki/Array.hpp>
#include<utility>

template<yuki::Array a>
struct Foo {typedef typename decltype(a)::value_type value_type; static constexpr size_t size = a.size;};

template<size_t n>
constexpr size_t number() {return n;}

//template<size_t... Is>
//constexpr size_t variadic(Is... is) {return (0+...+is);}

//template<typename T,size_t n>
//constexpr size_t func(const yuki::Array<T,n>& arr){
//    variadic();
//}

int main(){
    constexpr auto arr = yuki::make_index_array<65535>();
    static_assert(arr[10000]==10000);
    static_assert(number<arr.data[100]>()==100);

    Foo<{"abcde"}> foo2;
    static_assert(std::is_same_v<Foo<{"abcde"}>::value_type,char>);
    static_assert(foo2.size==6);
}