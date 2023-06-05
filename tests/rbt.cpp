#include<yuki/Set.hpp>
#include<yuki/Map.hpp>
#include<yuki/vmultiSet.hpp>
#include<yuki/Set_OV.hpp>
#include<yuki/Map_OV.hpp>
#include<yuki/vmultiSet_OV.hpp>
#include<cstdio>
#include<random>
#include<set>

struct Big{
    static size_t c;
    unsigned long long arr[50];
    ~Big() noexcept {++c;}
    Big(unsigned u) noexcept : arr{u} {}
    friend bool operator<(const Big& lhs,const Big& rhs) {return lhs.arr[0]<rhs.arr[0];}
};

size_t Big::c = 0;

template<typename T>
struct Tree{
    typedef yuki::MultiSet<T> a;
    typedef std::multiset<T> b;
    typedef yuki::Set<T> c;
    typedef std::set<T> d;
    typedef yuki::VMultiSet<T> e;

    typedef yuki::MultiSet_OV<T> f;
    typedef yuki::Set_OV<T> g;
    typedef yuki::VMultiSet_OV<T> h;
};

int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> distrib1(0,10);
    std::uniform_int_distribution<unsigned> distrib2(0,4);

    //{
    //Tree<Big>::a t;
    ////t.tree_base().reserve(100);

    //yuki::Vector<decltype(t.begin())> its(yuki::reserve_tag,2000);

    ////Tree<Big>::a::non_const_iterator ncit = const_kast(t.begin());

    //for(size_t j=0;j!=5;++j){
    //    printf("ROUND %zu\n",j);
    //    for(size_t i=100;i!=0;--i){
    //        unsigned u = distrib1(gen);
    //        unsigned u2 = distrib2(gen);

    //        if(u2==0){
    //            its.emplace_back(t.emplace(u));
    //            printf("\"%u\" ",u);
    //        }else{
    //            t.emplace(u);
    //            printf("%u ",u);
    //        }
    //    }

    //    printf("\n\n");

    //    for(auto i = t.begin();i!=t.end();++i)
    //        printf("%u ",i->arr[0]);

    //    printf("\n");

    //    printf("%zu %zu\n",t.erase(6),t.erase(8));

    //    for(auto i = t.begin();i!=t.end();++i)
    //        printf("%u ",i->arr[0]);

    //    printf("\n\n");

    //    its.clear();
    //    t.clear();
    //}
    //}

    //{
    //Tree<unsigned>::a t;
    //yuki::Vector<unsigned> us(yuki::reserve_tag,30);
    //for(size_t i=100;i!=0;--i){
    //    unsigned u = distrib1(gen);
    //    unsigned u2 = distrib2(gen);
    //    if(u2==0)
    //        us.emplace_back(u);
    //    t.emplace(u);
    //}
    //for(auto i = t.begin();i!=t.end();++i)
    //    printf("%u ",*i);
    //printf("\n\n");

    //Tree<unsigned>::a t2 = t;;

    //for(auto i = t.begin();i!=t.end();++i)
    //    printf("%u ",*i);
    //printf("\n\n");
    //for(auto i = t2.begin();i!=t2.end();++i)
    //    printf("%u ",*i);
    //printf("\n\n");

    //t2 = std::move(t);

    //for(auto i = t.begin();i!=t.end();++i)
    //    printf("%u ",*i);
    //printf("\n\n");
    //for(auto i = t2.begin();i!=t2.end();++i)
    //    printf("%u ",*i);
    //printf("\n\n");

    //t2.emplace(100U);
    //for(auto i = t2.begin();i!=t2.end();++i)
    //    printf("%u ",*i);
    //printf("\n\n");
    //t2.insert_unique(100U);
    //for(auto i = t2.begin();i!=t2.end();++i)
    //    printf("%u ",*i);
    //printf("\n\n");
    //t2.emplace(100U);
    //for(auto i = t2.begin();i!=t2.end();++i)
    //    printf("%u ",*i);
    //printf("\n\n");
    //}

    //{
    //Tree<unsigned>::a t;
    //yuki::Vector<unsigned> us(yuki::reserve_tag,30);
    //for(size_t i=100;i!=0;--i){
    //    unsigned u = distrib1(gen);
    //    unsigned u2 = distrib2(gen);
    //    if(u2==0)
    //        us.emplace_back(u);
    //    t.emplace(u);
    //}
    //for(auto i = t.begin();i!=t.end();++i)
    //    printf("%u ",*i);
    //printf("\n\n");
    //for(auto i = us.begin();i!=us.end();++i){
    //    printf("%u ",*i);
    //    auto it = t.first_greater(*i);
    //    if(it!=t.end())
    //        printf("%u",*it);
    //    printf("\n");
    //}
    //printf("\n");

    //for(size_t i=20;i!=0;--i){
    //    unsigned u = distrib1(gen);
    //    printf("%u ",u);
    //    auto it = t.first_greater(u);
    //    if(it!=t.end())
    //        printf("%u",*it);
    //    printf("\n");
    //}
    //printf("\n");

    //for(auto i = us.begin();i!=us.end();++i){
    //    printf("%u ",*i);
    //    auto it = t.first_equiv_greater(*i);
    //    if(it!=t.end())
    //        printf("%u",*it);
    //    printf("\n");
    //}
    //printf("\n");

    //for(size_t i=20;i!=0;--i){
    //    unsigned u = distrib1(gen);
    //    printf("%u ",u);
    //    auto it = t.first_equiv_greater(u);
    //    if(it!=t.end())
    //        printf("%u",*it);
    //    printf("\n");
    //}
    //printf("\n");
    //}

    {
    yuki::Map<std::string,std::string> map;
    map.insert_or_assign("abc","def");
    map["abc"]="efg";
    printf("%s",map["abc"].c_str());
    yuki::Map<std::string,std::string> map2=std::move(map);
    printf(" %zu %zu",map.size(),map2.size());
    const yuki::Map<std::string,std::string> map3(yuki::from_ordered_tag,map2.tree_base().isomorphic_copy(yuki::Iden{},[](yuki::KM_Pair<const std::string,std::string>& kmp) static {kmp.mapped.push_back('0');}));
    printf("\n%s",map3["abc"].c_str());
    struct Traverser{
        size_t total=0;
        void operator()(const yuki::KM_Pair<const std::string,std::string>& kmp) {(total+=kmp.key.size())+=kmp.mapped.size();}
    } traverser;
    map3.tree_base().unordered_traverse(traverser);
    printf("\n%zu",traverser.total);
    printf("\n");
    }

    //{
    //yuki::VMultiSet<Big> vms;
    //vms.emplace(10);
    //vms.emplace(10);
    //printf("%zu %zu %zu",vms.bucket_count(),vms.size(),vms.count(10));
    //}

    {
    Tree<unsigned>::a t;
    t.emplace(1); t.emplace(3); t.emplace(5); t.emplace(7); t.emplace(9);
    Tree<unsigned>::a t2;
    t2.emplace(2); t2.emplace(4); t2.emplace(6); t2.emplace(8); t2.emplace(9); t2.emplace(10);

    Tree<unsigned>::a t3 = t;
    t3.merge(t2);

    for(auto i = t3.begin();i!=t3.end();++i)
        printf("%u ",*i);
    printf("\n");
    for(auto i = t2.begin();i!=t2.end();++i)
        printf("%u ",*i);
    printf("\n");

    t3.merge(std::move(t2));

    for(auto i = t3.begin();i!=t3.end();++i)
        printf("%u ",*i);
    printf("\n");
    for(auto i = t2.begin();i!=t2.end();++i)
        printf("%u ",*i);
    printf("\n");
    }
}