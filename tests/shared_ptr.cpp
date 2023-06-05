#include<cstdio>
#include<yuki/shared_ptr.hpp>

int main(){
    using namespace yuki;
    int a = 1;
    shared_ptr<int> sp1 = &a;
    printf("%u ",sp1.use_count());
    {
    shared_ptr<int> sp2 = sp1;
    printf("%u ",sp1.use_count());
    }
    printf("%u ",sp1.use_count());
    shared_ptr<int> sp3 = sp1;
    {
    shared_ptr<int> sp2 = std::move(sp1);
    printf("%u ",sp1.use_count());
    printf("%u ",sp2.use_count());
    }
}