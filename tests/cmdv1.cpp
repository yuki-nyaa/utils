#include"../include/yuki/cmd.cpp"

constexpr void action0(int& i,const char*const*,size_t) {i=1;}
constexpr void action1(int& i,const char*const*,size_t) {i=2;}
constexpr void action2(int& i,const char*const*,size_t) {i=3;}
constexpr void action3(int& i,const char*const*,size_t) {i=4;}
constexpr void action4(int& i,const char*const*,size_t) {i=5;}

constexpr bool test0(){
    constexpr yuki::Cmd_Option<int> arr[]={
        {"action0",0,yuki::SO_Cat::N,&action0},
        {"action1",U'1',yuki::SO_Cat::a,&action1},
        {"action2",0,yuki::SO_Cat::N,&action2},
        {"action3",U'3',yuki::SO_Cat::a,&action3},
        {"action4",0,yuki::SO_Cat::N,&action4},
    };
    int i=0;
    constexpr yuki::Cmd_Option_Table cot(arr);
    cot["action0"](i,nullptr,0);
    assert(i==1);
    cot["action1"](i,nullptr,0);
    assert(i==2);
    cot["action2"](i,nullptr,0);
    assert(i==3);
    cot["action3"](i,nullptr,0);
    assert(i==4);
    cot["action4"](i,nullptr,0);
    assert(i==5);
    assert(cot["action5"]==nullptr);

    cot[U'1'].action(i,nullptr,0);
    assert(i==2);
    cot[U'3'].action(i,nullptr,0);
    assert(i==4);
    assert(cot[U'9'].action==nullptr);
    return true;
}

static_assert(test0());


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
    {"신진서",U's',yuki::SO_Cat::a,[](Foo& foo,const char*const*,size_t){foo.sjs=true;},"Top go player."},
    {"煌めけ",0,yuki::SO_Cat::N,[](Foo& foo,const char*const*,size_t){foo.krmk=true;},"Tiny Stars"},
    {"covfefe",0,yuki::SO_Cat::N,[](Foo& foo,const char*const*,size_t){foo.covfefe=true;},"Despite the negative press covfefe,"},
    {"",U'ク',yuki::SO_Cat::a,[](Foo& foo,const char*const*,size_t){foo.ku=true;},"Ku"},
    {"",U'ー',yuki::SO_Cat::a,[](Foo& foo,const char*const*,size_t){++foo.top;},"Long vowel"},
    {"",U'カ',yuki::SO_Cat::a,[](Foo& foo,const char*const*,size_t){foo.ka=true;},"Kanon"},
    {"",U'可',yuki::SO_Cat::b,[](Foo& foo,const char*const* p,size_t s){if(s>0) foo.kac=yuki::ystrlen(p[0]);}}
};

constexpr yuki::Cmd_Option_Table cot(coarr);

constexpr void foo_first(Foo& foo,const char* const first) {foo.first=first;}

template<typename CPEH>
constexpr bool test3(){
    Foo foo;
    constexpr const char* test_argv[] = {"foo","foo",/*"--신진서",*/"blah","blah","-クーカーs","blah","-可可","blah"};
    yuki::cmd_parse<cot,foo_first,CPEH>(foo,sizeof(test_argv)/sizeof(const char*),test_argv);
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

static_assert(test3<yuki::Noop_CPEH>());


int main(const int argc,const char*const*const argv){
    constexpr const char* test_argv[] = {"foo","foo","foo","--신진서","blah","blah","-クーカーs","blah","-可可","blah","-ab可c","blah","blah","--abc"};
    Foo foo;
    yuki::cmd_parse<cot,foo_first>(foo,sizeof(test_argv)/sizeof(const char*),test_argv);
    yuki::cmd_help(stderr,coarr);
}
