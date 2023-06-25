#include<random>
#include<yuki/RB_Tree.hpp>

size_t sum;

int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> distrib(0,100);

    for(size_t i=1000;i!=0;--i){
        sum=0;
        yuki::RB_Tree<unsigned,unsigned,yuki::Iden> rbt;
        for(size_t j=100000;j!=0;--j)
            rbt.emplace(distrib(gen));
        //for(const unsigned u : rbt) sum+=u;
        //rbt.recursive_traverse([](const unsigned u)static{sum+=u;}); // 10% faster than simple iteration, on my machine.
        //rbt.recursive_popfront([](const unsigned u)static{sum+=u;}); // 4% faster than `recursive_traverse`, on my machine.
    }
}
