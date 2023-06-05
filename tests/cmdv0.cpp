#include"../include/yuki/cmd.cpp"

constexpr void action0(int& i,const char*const*,size_t) {i=1;}
constexpr void action1(int& i,const char*const*,size_t) {i=2;}
constexpr void action2(int& i,const char*const*,size_t) {i=3;}
constexpr void action3(int& i,const char*const*,size_t) {i=4;}
constexpr void action4(int& i,const char*const*,size_t) {i=5;}

constexpr bool test0(){
    constexpr yuki::Long_Option<int> arr[]={
        {"action0",&action0},
        {"action1",&action1},
        {"action2",&action2},
        {"action3",&action3},
        {"action4",&action4},
    };
    int i=0;
    constexpr yuki::Long_Option_Table cot(arr);
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
    return true;
}

static_assert(test0());

constexpr yuki::Long_Option<int> arr1[]={
    {"action0",&action0},
    {"action1",&action1},
    {"action2",&action2},
    {"action3",&action3},
    {"action4",&action4},
};

constexpr yuki::Long_Option_Table cot1(arr1);

template<const auto& cot>
constexpr int test1(){
    int i=0;
    cot["action3"].action(i,nullptr,0);
    return i;
}

static_assert(test1<cot1>()==4);




constexpr bool test2(){
    constexpr yuki::Short_Option<int> arr[]={
        {U'ク',yuki::SO_Cat::a,&action0},
        {U'ー',yuki::SO_Cat::a,&action1},
        {U'カ',yuki::SO_Cat::a,&action2},
        {U'ー',yuki::SO_Cat::a,&action3},
        {U'k',yuki::SO_Cat::a,&action4},
    };
    int i=0;
    constexpr yuki::Short_Option_Table sos(arr);
    sos[U'ク'].action(i,nullptr,0);
    assert(i==1);
    sos[U'ー'].action(i,nullptr,0);
    assert(i==2);
    sos[U'カ'].action(i,nullptr,0);
    assert(i==3);
    sos[U'k'].action(i,nullptr,0);
    assert(i==5);
    assert(sos[U'a'].action==nullptr);
    return true;
}

static_assert(test2());




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

constexpr yuki::Long_Option<Foo> loarr[] = {
    {"신진서",[](Foo& foo,const char*const*,size_t){foo.sjs=true;},U'ク',"Top go player."},
    {"煌めけ",[](Foo& foo,const char*const*,size_t){foo.krmk=true;},0,"Tiny Stars"},
    {"covfefe",[](Foo& foo,const char*const*,size_t){foo.covfefe=true;},0,"Despite the negative press covfefe,"}
};

constexpr yuki::Short_Option<Foo> soarr[] = {
    {U'ク',yuki::SO_Cat::a,[](Foo& foo,const char*const*,size_t){foo.ku=true;}},
    {U'ー',yuki::SO_Cat::a,[](Foo& foo,const char*const*,size_t){++foo.top;},"Long vowel"},
    {U'カ',yuki::SO_Cat::a,[](Foo& foo,const char*const*,size_t){foo.ka=true;},"Kanon"},
    {U'可',yuki::SO_Cat::b,[](Foo& foo,const char*const* p,size_t s){if(s>0) foo.kac=yuki::ystrlen(p[0]);}}
};

constexpr yuki::Long_Option_Table lotfoo(loarr);
constexpr yuki::Short_Option_Table sotfoo(soarr);

constexpr void foo_first(Foo& foo,const char* const first) {foo.first=first;}

template<typename CPEH>
constexpr bool test3(){
    Foo foo;
    constexpr const char* test_argv[] = {"foo","foo","--신진서","blah","blah","-クーカー","blah","-可可","blah"};
    yuki::cmd_parse<lotfoo,sotfoo,foo_first,CPEH>(foo,sizeof(test_argv)/sizeof(const char*),test_argv);
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
    constexpr const char* test_argv[] = {"foo","foo","foo","--신진서","blah","blah","-クーカー","blah","-可可","blah","-ab可c","blah","blah","--abc"};
    Foo foo;
    yuki::cmd_parse<lotfoo,sotfoo,foo_first>(foo,sizeof(test_argv)/sizeof(const char*),test_argv);
    yuki::print_help(stderr,loarr,soarr);
   //test3();
}
