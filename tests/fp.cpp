#include<yuki/Function_Ptr.hpp>

constexpr auto lambda1 = [i=100](const int p){return p+i;};
static_assert(!noexcept(lambda1(100)));
constexpr yuki::Function_Ptr<int(int)> fp1 = lambda1;
static_assert(fp1);
static_assert(fp1(100)==200);
static_assert(fp1.storage_type()==yuki::FP_Storage_Type::VP);
static_assert(!noexcept(fp1(100)));
//constexpr yuki::Function_Ptr<int(int)> fp1_1 = [i=100](const int p){return p+i;}; // Error. The lifetime of the lambda ends after the initialization of `fp1_1` is finished.
constexpr yuki::Function_Ptr<int(int)> fp1_2 = fp1;
static_assert(fp1_2==fp1);

constexpr yuki::Function_Ptr<int(int)noexcept> fp2 = [](const int p)noexcept{return p+100;};
static_assert(fp2.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fp2(100)==200);
static_assert(noexcept(fp2(100)));

constexpr int func1(int p) noexcept {return p*=2;}
constexpr yuki::Function_Ptr<int(int)> fp3 = func1;
static_assert(fp3(200)==400);
static_assert(fp3.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fp3!=fp1);
static_assert(fp3==func1);

constexpr yuki::Function_Ptr<int(int)noexcept> fp4 = func1;
static_assert(fp4(200)==400);
static_assert(fp4.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fp4==fp3);
constexpr yuki::Function_Ptr<int(int)noexcept> fp4_1 = &func1;
static_assert(fp4_1.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fp4_1==fp4);
static_assert(fp4_1==func1);
constexpr yuki::Function_Ptr<int(int)noexcept> fp4_2 = fp4;
static_assert(fp4_2.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fp4_2==fp4);
static_assert(fp4_2==func1);

constexpr int func2(int p){return p*=2;}
//constexpr yuki::Function_Ptr<int(int)noexcept> fp5 = func2; // Error.

constexpr auto lambda3 = [i=100](const int p)noexcept(false){return p+i;};
//constexpr yuki::Function_Ptr<int(int)noexcept> fp6 = lambda3; // Error.


constexpr auto lambda4 = [i=100](const auto p){return p+i;};
constexpr yuki::Function_Ptr<int(int)> fp7 = lambda1;
static_assert(fp7(100)==200);

template<typename T>
constexpr auto func3 (T t) noexcept {return t*=2;}
constexpr yuki::Function_Ptr<int(int)> fp8 = func3;
static_assert(fp8(200)==400);

constexpr yuki::Function_Ptr<int(int)> fp9([](const auto p){return p+123;});
static_assert(fp9(100)==223);
static_assert(fp9.storage_type()==yuki::FP_Storage_Type::FP);


constexpr yuki::Function_Ptr<int(int)> fp10 = nullptr;
static_assert(!fp10);
constexpr yuki::Function_Ptr<int(int)noexcept> fp11 = nullptr;
static_assert(!fp11);


static_assert(std::is_same_v<yuki::Function_Ptr<int(int)>,yuki::Function_Ptr<int(const int)>>);


struct NonCopyNonMove{
    int member;
    constexpr NonCopyNonMove() noexcept : member(123) {}
    NonCopyNonMove(const NonCopyNonMove&) = delete;
    NonCopyNonMove(NonCopyNonMove&&) = delete;
};

auto lambda6 = []()->NonCopyNonMove {return NonCopyNonMove();};
constexpr yuki::Function_Ptr<NonCopyNonMove()> fp14 = lambda6;
static_assert(fp14().member==123);

int int1=234;
constexpr yuki::Function_Ptr<void()> fp15 = [](){++int1;};




constexpr yuki::Function_Ref<int(int)> fr1 = lambda1;
static_assert(fr1(100)==200);
static_assert(fr1.storage_type()==yuki::FP_Storage_Type::VP);
static_assert(!noexcept(fr1(100)));
//constexpr yuki::Function_Ref<int(int)> fr1_1 = [i=100](const int p){return p+i;}; // Error. The lifetime of the lambda ends after the initialization of `fp1_1` is finished.

constexpr yuki::Function_Ref<int(int)noexcept> fr2 = [](const int p)noexcept{return p+100;};
static_assert(fr2.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fr2(100)==200);
static_assert(noexcept(fr2(100)));

constexpr yuki::Function_Ref<int(int)> fr3 = func1;
static_assert(fr3(200)==400);
static_assert(fr3.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fr3!=fr1);
static_assert(fr3==func1);
static_assert(fr3==fp3);

constexpr yuki::Function_Ref<int(int)> fr4 = fp3;
static_assert(fr4(200)==400);
static_assert(fr4.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fr4==fp3);

constexpr yuki::Function_Ptr<int(int)> fp_fr4 = fr4;
static_assert(fp_fr4(200)==400);
static_assert(fp_fr4.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fp_fr4==fr4);

constexpr yuki::Function_Ref<int(int)noexcept> fr5 = func1;
static_assert(fr5(200)==400);
static_assert(fr5.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fr5==fp3);

constexpr yuki::Function_Ptr<int(int)noexcept> fp_fr5 = fr5;
static_assert(fp_fr5(200)==400);
static_assert(fp_fr5.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fp_fr5==fr5);
static_assert(fr5==fr4);
static_assert(fr5==fp_fr4);
static_assert(fp_fr5==fr4);
static_assert(fp_fr5==fp_fr4);

constexpr yuki::Function_Ref<int(int)> fr6 = fp_fr5;
static_assert(fr6(200)==400);
static_assert(fr6.storage_type()==yuki::FP_Storage_Type::FP);
static_assert(fr6==fp3);
static_assert(fr6==fr4);
static_assert(fr6==fp_fr4);


struct Foo{
    int foo(int) const;
};

// All error.
//constexpr yuki::Function_Ptr<int(const Foo&,int)> fp16 = &Foo::foo;
//constexpr yuki::Function_Ptr<int(const Foo&,int)noexcept> fp17 = &Foo::foo;
//constexpr yuki::Function_Ref<int(const Foo&,int)> fr7 = &Foo::foo;
//constexpr yuki::Function_Ref<int(const Foo&,int)noexcept> fr8 = &Foo::foo;


struct Callable{
    constexpr int operator()(int) & noexcept {return 1;}
    constexpr int operator()(int) const& noexcept {return 2;}
    constexpr int operator()(int) && noexcept {return 3;}
    constexpr int operator()(int) const&& noexcept {return 4;}
};
constexpr Callable callable;
constexpr yuki::Function_Ref<int(int)noexcept> fr7_2(callable);
static_assert(fr7_2(0)==2);
constexpr yuki::Function_Ref<int(int)noexcept> fr7_4(static_cast<const Callable&&>(callable));
static_assert(fr7_4(0)==4);


constexpr yuki::Function_Ref<int(void)noexcept> fr9([]()noexcept{return 123;});
static_assert(fr9()==123);

constexpr yuki::Function_Ref<int(int func(int))noexcept> fr10([](int func(int))noexcept{return func(1);});
static_assert(fr10(func1)==2);




#include<string>
struct MoveCount{
    static size_t mcount;
    static size_t dcount;

    std::string str;

    MoveCount() noexcept = default;
    MoveCount(MoveCount&& other) noexcept : str(std::move(other.str)) {++mcount;}
    ~MoveCount() noexcept {++dcount;}
};
size_t MoveCount::mcount=0;
size_t MoveCount::dcount=0;




#include<cstdio>
int main(){
    fp15();
    assert(int1==235);

    yuki::Function_Ptr<int(int)> fp12 = nullptr;
    assert(fp12.storage_type()==yuki::FP_Storage_Type::N);
    //fp12(0); // Assertion failure
    yuki::Function_Ptr<int(int)noexcept> fp13 = nullptr;
    assert(fp13.storage_type()==yuki::FP_Storage_Type::N);
    //fp13(0); // Assertion failure
    fp12=fp3;
    assert(fp12.storage_type()==yuki::FP_Storage_Type::FP);
    assert(fp12(100)==200);
    fp12=fp13;
    assert(!fp12);
    assert(fp12.storage_type()==yuki::FP_Storage_Type::N);
    assert(fp12==fp13);
    assert(fp13==fp12);
    //fp13=fp12; // Error

    Callable callable_nc;
    const yuki::Function_Ref<int(int)noexcept> fr7_1(callable_nc);
    assert(fr7_1(0)==1);
    const yuki::Function_Ref<int(int)noexcept> fr7_3(std::move(callable_nc));
    assert(fr7_3(0)==3);

    const yuki::Function_Ref<void(MoveCount)noexcept> fr_mc([](MoveCount mc)noexcept{mc.str.push_back('a');});
    fr_mc(MoveCount{});
    printf("%zu %zu\n",MoveCount::mcount,MoveCount::dcount);

    fputs("TEST DONE!",stdout);
}
