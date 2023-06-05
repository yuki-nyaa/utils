#include<cstdio>
#include<random>
#include<yuki/Vector.hpp>
#include<vector>
int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> arr_distrib(0,0x10FFFF);

    ////typedef std::vector<unsigned> vector;
    //typedef yuki::Vector<unsigned,yuki::Allocator<unsigned>,yuki::Default_EC<17,8>> vector;

    //vector vec;

    ////vec.reserve(134217728);

    //for(size_t i=100000000;i!=0;--i){
    //    unsigned n = arr_distrib(gen);
    //    if(n%32==0){
    //        if(!vec.empty())
    //            vec.pop_back();
    //    }else{
    //        vec.emplace_back(n);
    //    }
    //}
    //printf("%zu %zu",vec.size(),vec.capacity());

    //// Without reservation std::vector is faster.
    //// With reservation my vector is faster.
    //// That said, since the expansion factor of my vector is adjustable, a factor of 3 would make my vector faster. With a total number of 100000000 this factor even happens to reduce memory usage.
    //// Even a curious factor of 17/8 would make my vector faster.
    //// Given all these, it rather puzzles me why std::vector is faster without reservation.

    typedef std::vector<yuki::Vector<unsigned>> vector;
    //typedef yuki::Vector<yuki::Vector<unsigned>> vector;

    const yuki::Vector<unsigned> vec0{1,2,3,4,5,6,7,8,9};

    vector vec;

    vec.reserve(16777216);

    for(size_t i=10000000;i!=0;--i){
        unsigned n = arr_distrib(gen);
        if(n%16==0){
            if(!vec.empty())
                vec.pop_back();
        }else{
            vec.emplace_back(vec0);
        }
    }
    printf("%zu %zu",vec.size(),vec.capacity());
}