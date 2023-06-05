#include<yuki/Allocator.hpp>
#include<yuki/Pair.hpp>
#include<cstdio>
#include<string>
unsigned destroyed = 0;
struct ND {unsigned i; constexpr ND(unsigned n) : i(n){} ~ND() {++destroyed;}};

template<typename T>
struct Fancy_Pointer{
  private:
    T* p;
    size_t off;
  public:
    Fancy_Pointer(T* p2,size_t off2) : p(p2),off(off2) {}
    Fancy_Pointer(std::nullptr_t) : p(nullptr),off(0) {}
    T& operator*() const {return p[off];}
    friend T* iterator_unwrap(Fancy_Pointer p) noexcept {return p.p+p.off;}
    friend bool operator==(const Fancy_Pointer& lhs,const Fancy_Pointer& rhs) {return (lhs.p+lhs.off) == (rhs.p+rhs.off);}
};

struct V{
    virtual int foo() {return 1;}
};

struct Empty {};

struct V2 : virtual Empty {};

int main(){
    printf("TEST A1\n");

    yuki::Allocator<int> a;
    auto p = a.allocate(100);
    for(int i=0;i<100;++i)
        yuki::construct(p+i,i);
    for(int i=0;i<100;++i)
        printf("%d ",p[i]);
    for(int i=0;i<100;++i)
        yuki::destroy(p+i);
    a.deallocate(p,100);

    printf("\n");
    printf("TEST LA1\n");

    yuki::List_Allocator<int> la1;
    auto p2 = la1.allocate(100);
    for(int i=0;i<100;++i)
        yuki::construct(p2+i,i);
    for(int i=0;i<100;++i)
        printf("%d ",p2[i]);
    yuki::destroy(p2,100);
    la1.deallocate(p2,100);
    la1.clear();

    printf("\n");
    printf("TEST LA2\n");

    yuki::List_Allocator<ND> la2;
    auto p3 = la2.allocate(100);
    for(int i=0;i<100;++i)
        yuki::construct(p3+i,i);
    for(int i=0;i<100;++i)
        printf("%u ",p3[i].i);
    yuki::destroy(p3,100);
    printf("\n%u ",destroyed);
    la2.deallocate(p3,100);
    printf("%u ",destroyed);
    la2.clear();
    printf("%u ",destroyed);

    printf("\n");
    printf("TEST FP1\n");

    {
    using yuki::iterator_unwrap;
    assert(iterator_unwrap(nullptr)==nullptr);
    }

    int i1 = 100;

    {
    int* ip1 = &i1;
    using yuki::iterator_unwrap;
    assert(iterator_unwrap(ip1)==&i1);
    }

    Fancy_Pointer<int> fp1(&i1,0);
    {
    using yuki::iterator_unwrap;
    assert(*iterator_unwrap(fp1)==100);
    printf("%d ",*iterator_unwrap(fp1));
    }
    yuki::construct(fp1,200);
    {
    using yuki::iterator_unwrap;
    assert(*iterator_unwrap(fp1)==200);
    printf("%d ",*iterator_unwrap(fp1));
    }

    YUKI_CONSTRUCT(fp1,{201});
    {
    using yuki::iterator_unwrap;
    assert(*iterator_unwrap(fp1)==201);
    printf("%d ",*iterator_unwrap(fp1));
    }

    printf("\n");
    printf("TEST AAD1\n");

    {
    yuki::Array_Allocator_Dynamic<std::string> aad;
    printf("%zu\n",sizeof(aad));
    std::string* p1 = aad.allocate();
    ::new(p1) std::string("asfjdasopvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv"); // Not sure if this is UB or not.
    std::string* p2 = aad.allocate();
    std::string* p3 = aad.allocate();
    aad.deallocate(p2);
    assert(aad.free_count()==30);
    printf("%s\n",p1->c_str());
    p1->std::string::~string(); // Not sure if this is UB or not.
    aad.deallocate(p1);
    assert(aad.free_count()==31);
    printf("%s\n",p1->c_str());
    }

    printf("\n");
    printf("TEST AAD2\n");

    {
    yuki::Array_Allocator_Dynamic<V> aad;
    V* p1 = aad.allocate();
    // printf("%d ",p1->foo()); // Segfault
    ::new(p1) V();
    printf("%d ",p1->foo());
    p1->~V();
    // printf("%d ",p1->foo()); // UB. No segfault here though.
    aad.deallocate(p1);
    }

    printf("\n");
    printf("TEST AAD3\n");

    {
    yuki::Array_Allocator_Dynamic<V2> aad;
    V2* p1 = aad.allocate();
    ::new(p1) V2();
    p1->~V2();
    aad.deallocate(p1);
    }

    printf("\n");
    printf("TEST AAD4\n");

    {
    yuki::Array_Allocator_Dynamic<int> aad;
    int* p1 = aad.allocate();
    ::new(p1) int(8);
    printf("%d",*p1);
    aad.deallocate(p1);
    }

    printf("\n");
    printf("TEST AAD5\n");

    {
    yuki::Array_Allocator_Dynamic<V,0> aad;
    V* p1 = aad.allocate();
    // printf("%d ",p1->foo()); // Segfault
    ::new(p1) V();
    printf("%d ",p1->foo());
    p1->~V();
    // printf("%d ",p1->foo()); // UB. No segfault here though.
    aad.deallocate(p1);
    }

    printf("\n");
    printf("TEST YC\n");

    yuki::Pair<int,const char*> yp1;
    YUKI_CONSTRUCT(&yp1,{8,"foo"});
    printf("%d - %s\n",yp1.zeroth,yp1.first);
}