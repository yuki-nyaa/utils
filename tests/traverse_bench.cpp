#include<random>
#include<set>
#include<yuki/RB_Tree.hpp>

size_t sum;

int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> distrib(0,100);

    yuki::RB_Tree<unsigned,unsigned,yuki::Iden> rbt;
    //std::multiset<unsigned> rbt;
    for(size_t i=100000;i!=0;--i)
        rbt.emplace(distrib(gen));

    for(size_t i=10000;i!=0;--i){
        sum=0;
        for(const unsigned u : rbt) sum+=u; // No notable difference between `yuki::` and `std::`, on my machine.
        //rbt.recursive_traverse([](const unsigned u)static{sum+=u;}); // 3.79x faster than simple iteration, on my machine.
    }
}
