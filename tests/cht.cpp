#include"../include/yuki/CHashTable.hpp"
#include"../include/yuki/functional.hpp"
#include<string_view>

struct StupidHash{
    static constexpr size_t operator()(const std::string_view sv) {return sv.empty() ? 0 : sv[0];}
};

struct V{
    std::string_view sv;
    int(*action)(int);
};

struct Get_SV{
    template<typename Vtp>
    static constexpr std::string_view operator()(const Vtp v) {return v.sv;}
};

struct VN{
    static constexpr bool operator()(const V v) {return v.action==nullptr;}
    static constexpr V nil = {};
};

constexpr int action0(int) {return 1;}
constexpr int action1(int) {return 2;}
constexpr int action2(int) {return 3;}
constexpr int action3(int) {return 4;}
constexpr int action4(int) {return 5;}

constexpr V arr[] = {
    {"action2",&action2},
    {"action3",&action3},
    {"bction0",&action0},
    {"action1",&action1},
};

constexpr yuki::CHashTable<std::string_view,V,Get_SV,StupidHash,yuki::Equal_To<std::string_view>,VN,4> cht(arr,sizeof(arr)/sizeof(V));

static_assert(cht.size()==4);
static_assert(cht["bction0"].action(100)==1);
static_assert(cht["action1"].action(100)==2);
static_assert(cht["action2"].action(100)==3);
static_assert(cht["action3"].action(100)==4);
static_assert(cht.find("action00")==nullptr);

static_assert(sizeof(decltype(cht))==96+8+8);

struct V1{
    std::string_view sv;
    int i;
};

struct V1N{
    static constexpr bool operator()(const V1 v) {return v.i==0;}
    static constexpr V1 nil = {};
};

constexpr V1 arr1[] = {
    {"action2",3},
    {"action3",4},
    {"bction0",5},
    {"action1",6},
    {"action1",7},
    {"action2",8},
    {"action3",9},
    {"bction0",10},
};

constexpr yuki::CHashTable<std::string_view,V1,Get_SV,StupidHash,yuki::Equal_To<std::string_view>,V1N,4> cht1(yuki::unique_tag,arr1,sizeof(arr1)/sizeof(V1));

static_assert(cht1.size()==4);
static_assert(cht1["action1"].i==6);
static_assert(cht1["action2"].i==3);
static_assert(cht1["action3"].i==4);
static_assert(cht1["bction0"].i==5);

int main(){

}
