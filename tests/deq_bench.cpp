#include<yuki/RingQueue.hpp>
#include<deque>
#include<random>

int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> arr_distrib(0,0x10FFFF);
    std::uniform_int_distribution<unsigned> two_distrib(0,1);

    //typedef std::deque<size_t> Deque;
    typedef yuki::RingQueue<size_t> Deque;

    Deque deq;

    //deq.reserve(134217728);

    for(size_t i=100000000;i!=0;--i){
        unsigned n = arr_distrib(gen);
        if(n%32==0){
            if(!deq.empty()){
                switch(two_distrib(gen)){
                    case 0 : deq.pop_back(); break;
                    default : deq.pop_front(); break;
                }
            }
        }else{
            switch(two_distrib(gen)){
                case 0 : deq.emplace_back(n); break;
                default : deq.emplace_front(n); break;
            }
        }
    }
    //printf("%zu",deq.size());
    //printf("%zu %zu",deq.size(),deq.capacity());

    // Without reserve std::deque is faster.
    // With reserve my deque is faster.
}