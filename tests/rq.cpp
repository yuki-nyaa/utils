#include<yuki/RingQueue.hpp>
#include<cstdio>
struct Int{
    int value;
    static size_t d;
    constexpr Int(int a) noexcept : value(a) {}
    ~Int() noexcept {++d;}
};
size_t Int::d = 0;
int main(){
    yuki::RingQueue<int> rq0;
    rq0.emplace_back(4);
    rq0.emplace_back_overwrite(8);
    rq0.emplace_front(2);
    rq0.emplace_front_overwrite(4);
    rq0.emplace_back(16);
    rq0.emplace_front(8);
    rq0.emplace_back(32);
    rq0.emplace_front(16);
    rq0.emplace_back(64);
    printf("%zu %zu\n",rq0.size(),rq0.capacity());
    for(size_t i = 0;i<30;++i)
        printf("%d ",rq0[i]);

    printf("\n");

    yuki::RingQueue<int> rq00 = rq0;
    printf("%zu %zu\n",rq00.size(),rq00.capacity());
    for(size_t i = 0;i<30;++i)
        printf("%d ",rq00[i]);

    printf("\n");

    rq00.emplace_back(0);
    printf("%zu %zu\n",rq00.size(),rq00.capacity());
    for(size_t i = 0;i<30;++i)
        printf("%d ",rq00[i]);

    printf("\n");

    rq00.clear();
    rq00=rq0;
    printf("%zu %zu\n",rq00.size(),rq00.capacity());
    for(size_t i = 0;i<30;++i)
        printf("%d ",rq00[i]);

    printf("\n");

    rq00=std::move(rq0);
    printf("%zu %zu\n",rq00.size(),rq00.capacity());
    for(size_t i = 0;i<30;++i)
        printf("%d ",rq00[i]);

    printf("\n");

    yuki::RingQueue<Int> rq;
    rq.emplace_back(4);
    rq.emplace_back_overwrite(8);
    rq.emplace_front(2);
    rq.emplace_front_overwrite(4);
    rq.emplace_back(16);
    rq.emplace_front(8);
    rq.emplace_back(32);
    rq.emplace_front(16);
    printf("%zu\n",rq.capacity());
    // 16 8 4 2 4 8 16 32
    for(size_t i = 0;i<30;++i)
        printf("%d ",rq[i].value);

    printf("\n%zu ",Int::d); // 4 // Note: If no perfect forwarding is used, this number would be 8, due to the destruction of temporaries.

    rq.emplace_back_overwrite(32);
    printf("%zu\n",Int::d); // 5 // Note: If no perfect forwarding is used, this number would be 10, due to the destruction of temporaries.
    for(size_t i = 0;i<30;++i)
        printf("%d ",rq[i].value);

    //rq.emplace_front(64);
    //printf("\n%zu\n",Int::d); // 19=10+8+1

    printf("\n");
    rq.emplace_front_overwrite(64);
    for(size_t i = 0;i<30;++i)
        printf("%d ",rq[i].value);

    printf("\n");
    rq.pop_front(3);
    for(size_t i = 0;i<rq.size();++i)
        printf("%d ",rq[i].value);

    printf("\n");
    rq.pop_back(3);
    for(size_t i = 0;i<rq.size();++i)
        printf("%d ",rq[i].value);

    printf("\n\n");

    yuki::RingQueue<int> rq2(yuki::memset_tag,100,1); // Should be 16843009 with 32bit int.
    printf("%zu %zu\n",rq2.size(),rq2.capacity());
    for(size_t i = 0;i<rq2.size();++i)
        printf("%d ",rq2[i]);
}