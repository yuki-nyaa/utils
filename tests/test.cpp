#define YUKI_UTILS_DEV
#include"set.hpp"
using namespace yuki;
int main(){
    multiset<int> ms;
}
//struct Foo{
//    int a;
//    std::basic_string<int> b;
//    friend bool operator==(const Foo&,const Foo&) = default;
//};
//template<>
//struct yuki::rough_less<Foo>{
//    bool operator()(const Foo& lhs,const Foo& rhs)const{
//        if(lhs.a<rhs.a)
//            return true;
//        else if(rhs.a<lhs.a)
//            return false;
//        else if(lhs.b.size()<rhs.b.size())
//            return true;
//        else if(rhs.b.size()<lhs.b.size())
//            return false;
//        return false;
//    }
//};
//int main(){
//    yuki::multiset<Foo,yuki::rough_less<Foo>,std::equal_to<Foo>> ms;
//    Foo a = {10,{1,2,3,4}};
//    Foo b = {10,{2,3,4,5}};
//    ms.emplace(a);
//    ms.emplace(b);
//    yuki::print_space(std::cout,ms.count_equiv(a),ms.count_equal(a),yuki::equiv<int>{}(1,2));
//}