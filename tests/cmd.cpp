#include"../include/yuki/cmd.hpp"

constexpr void action0(int& i,const char*const*,size_t) {i=1;}
constexpr void action1(int& i,const char*const*,size_t) {i=2;}
constexpr void action2(int& i,const char*const*,size_t) {i=3;}
constexpr void action3(int& i,const char*const*,size_t) {i=4;}
constexpr void action4(int& i,const char*const*,size_t) {i=5;}

namespace test0{
constexpr yuki::Cmd_Option<int> arr[]={
    {"action0",0,&action0},
    {"action1",U'1',&action1},
    {"action2",0,&action2},
    {"action3",U'3',&action3},
    {"action4",0,&action4},
};

constexpr bool test(){
    int i=0;
    constexpr yuki::Cmd_Option_Table<arr> cot;
    cot["action0"].action(i,nullptr,0);
    assert(i==1);
    cot["action1"].action(i,nullptr,0);
    assert(i==2);
    cot["action2"].action(i,nullptr,0);
    assert(i==3);
    cot["action3"].action(i,nullptr,0);
    assert(i==4);
    cot["action4"].action(i,nullptr,0);
    assert(i==5);
    assert(cot["action5"].action==nullptr);

    cot[U'1'].action(i,nullptr,0);
    assert(i==2);
    cot[U'3'].action(i,nullptr,0);
    assert(i==4);
    assert(cot[U'9'].action==nullptr);
    return true;
}

static_assert(test());
} // namespace test0


namespace test1{
struct Foo{
    bool sjs = false;
    bool krmk = false;
    bool covfefe = false;
    bool ku = false;
    unsigned char top = 0;
    bool ka = false;
    unsigned char kac = 0;
    const char* first = nullptr;
};

constexpr yuki::Cmd_Option<Foo> coarr[] = {
    {"신진서",U's',[](Foo& foo,const char*const*,size_t){foo.sjs=true;},"Top go player."},
    {"煌めけ",0,[](Foo& foo,const char*const*,size_t){foo.krmk=true;},"Tiny Stars"},
    {"covfefe",0,[](Foo& foo,const char*const*,size_t){foo.covfefe=true;},"Despite the negative press covfefe,"},
    {"",U'ク',[](Foo& foo,const char*const*,size_t){foo.ku=true;},"Ku"},
    {"",U'ー',[](Foo& foo,const char*const*,size_t){++foo.top;},"Long vowel"},
    {"",U'カ',[](Foo& foo,const char*const*,size_t){foo.ka=true;},"Kanon"},
    {"",{U'可',yuki::Short_Option::B},[](Foo& foo,const char*const* p,size_t s){if(s>0) foo.kac=yuki::ystrlen(p[0]);}}
};

constexpr void foo_positional(Foo& foo,const char*const*const argv,const size_t) {foo.first=argv[0];}

template<typename CPEH>
constexpr bool test1(){
    Foo foo;
    constexpr yuki::Cmd_Option_Table<coarr> cot;
    constexpr const char* test_argv[] = {"foo","foo",/*"--신진서",*/"blah","blah","-クーカーs","blah","-可可","blah"};
    cot.parse<foo_positional,CPEH>(foo,sizeof(test_argv)/sizeof(const char*),test_argv);
    assert(foo.sjs);
    assert(!foo.krmk);
    assert(!foo.covfefe);
    assert(foo.ku);
    assert(foo.top==2);
    assert(foo.ka);
    assert(foo.kac==3);
    assert(yuki::ystrcmp(foo.first,"foo")==0);
    return true;
}

static_assert(test1<yuki::Noop_CPEH>());
} // namespace test1


int main(const int argc,const char*const*const argv){
    constexpr const char* test_argv[] = {"foo","foo","foo","--신진서","blah","blah","-クーカーs","blah","-可可","blah","-ab?可cc","blah","blah","--abc","--help","help","help"};
    test1::Foo foo;
    constexpr yuki::Cmd_Option_Table<test1::coarr> cot;
    cot.parse(foo,sizeof(test_argv)/sizeof(const char*),test_argv);
    assert(foo.sjs);
    assert(!foo.krmk);
    assert(!foo.covfefe);
    assert(foo.ku);
    assert(foo.top==2);
    assert(foo.ka);
    assert(foo.kac==3);
}
