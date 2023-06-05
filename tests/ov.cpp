#include<cstdio>
#include<random>
#include<set>
#include<yuki/Set.hpp>
#include<yuki/Set_OV.hpp>

struct Int{
    unsigned i;
    static size_t d;
    Int() noexcept = default;
    Int(unsigned p) noexcept : i(p) {}
    ~Int() noexcept {++d;}
    friend bool operator<(Int lhs,Int rhs) {return lhs.i<rhs.i;}
};

size_t Int::d = 0;

struct Int2{
    unsigned i[32];
    static size_t d2;
    Int2() noexcept = default;
    Int2(unsigned p) noexcept : i{p} {}
    ~Int2() noexcept {++d2;}
    friend bool operator<(const Int2& lhs,const Int2& rhs) {return lhs.i[0]<rhs.i[0];}
};

size_t Int2::d2 = 0;

struct size_t2{
    size_t a;
    char32_t b=100;
    char32_t c=100;
    friend constexpr bool operator<(size_t2 lhs,size_t2 rhs) {return lhs.a<rhs.a;}
};

int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> distrib(0,0x10FFFF);

    //typedef Int T;
    //typedef Int2 T;
    //typedef unsigned T;
    typedef size_t2 T;


    typedef yuki::Ordered_Vector<T> Set;
    //typedef yuki::MultiSet<T> Set;
    //typedef std::multiset<T> Set;

    Set ov1;
    for(size_t i=100000;i!=0;--i){
        for(size_t j=300;j!=0;--j)
            //ov1.emplace(distrib(gen));
            ov1.insert({distrib(gen)});
        ov1.clear();
    }

    // The performance depends on container size, i.e. `j` in the inner loop.
    // On my machine the crossover point is around 700 for `unsigned`. Around 225 for `Int`. Around 50 for `Int2`.
}