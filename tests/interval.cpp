#include<yuki/Interval.hpp>

template<typename I,typename A,typename EC>
void print_cc(const yuki::IntegralCIs_OV<I,A,EC>& cc){
    for(const yuki::CInterval<I> ci : cc)
        printf("[%llx,%llx] ",(long long)ci.lb,(long long)ci.ub);
    printf("\n");
}

int main(){
    {
    yuki::IntegralCIs_OV<int> cc0{yuki::from_ordered_tag,{{0x6c,0x6c},{0x44,0x44},{0x79,0x79},{0x7d,0x7d}}};
    yuki::IntegralCIs_OV<int>cc1{yuki::from_ordered_tag,{{0,0x9},{0xb,0xc},{0xe,0x10ffff}}};
    print_cc(cc1-cc0);
    }
    {
    yuki::IntegralCIs_OV<int> cc;
    cc.insert({0,100});
    cc.insert({200,300});
    cc.insert({400,500});

    cc.insert({50,120});

    // [0 120] [200 300] [400 500]
    print_cc(cc);

    const auto i = cc.find(300);
    printf("%d %d\n",i->lb,i->ub);

    cc.insert({600,700});

    // [0 120] [200 300] [400 500] [600 700]
    print_cc(cc);

    cc.insert({250,650});

    // [0 120] [200 700]
    print_cc(cc);

    cc.insert({800,900});
    cc.insert({1000,1100});
    cc.insert({250,950});

    // [0 120] [200 950] [1000,1100]
    print_cc(cc);

    cc.insert({130,250});

    // [0 120] [130 950] [1000 1100]
    print_cc(cc);

    cc.insert({121,999});

    // [0 1100]
    print_cc(cc);

    cc.clear();

    cc.insert({0,100});
    cc.insert({200,300});
    cc.insert({400,500});

    cc.insert({101,101});

    // [0 101] [200 300] [400 500]
    print_cc(cc);

    cc.insert({100,199});

    // [0 300] [400 500]
    print_cc(cc);
    }

    printf("TEST1 DONE!\n\n");

    {
    yuki::IntegralCIs_OV<int> cc;
    cc.insert({0,300});
    cc.insert({400,450});

    // [100 300] [400 450]
    {
    yuki::IntegralCIs_OV<int> cc2({100,450});
    print_cc(cc*cc2);
    }

    // [101 102] [400 400]
    {
    yuki::IntegralCIs_OV<int> cc2(yuki::from_ordered_tag,{{101,102},{400,400}});
    print_cc(cc*cc2);
    }

    cc.assign(yuki::from_ordered_tag,{{100,300},{400,450}});

    // [150 300]
    {
    yuki::IntegralCIs_OV<int> cc2({150,350});
    print_cc(cc=cc*cc2);
    }

    cc.insert({320,330});

    // [150 350]
    {
    yuki::IntegralCIs_OV<int> cc2({301,350});
    print_cc(cc=cc+cc2);
    }

    // [0 149] [351 1114111]
    // [150 350]
    {
    yuki::IntegralCIs_OV<int> cc2 = yuki::negate<{0,0x10FFFF}>(cc);
    print_cc(cc2);
    print_cc(yuki::negate<{0,0x10FFFF}>(cc2));
    }

    {
    print_cc(cc);
    yuki::IntegralCIs_OV<int> cc2({0,0x10FFFF});
    print_cc(cc2-cc);
    }
    }

    printf("TEST2 DONE!\n\n");

    {
    yuki::IntegralCIs_OV<int> ccs[10];
    ccs[0].insert({100,120});
    ccs[0].insert({200,220});

    ccs[1].insert({110,150});
    ccs[1].insert({210,210});
    ccs[1].insert({300,300});

    // [100 150] [200 220] [300 300]
    yuki::IntegralCIs_OV<int> merged; merged.clear_and_merge(ccs,2);
    print_cc(merged);
    }

    printf("TEST3 DONE!\n\n");

    {
    yuki::IntegralCIs_OV<int> cc;
    cc.insert({10,15});
    cc.insert({100,100});
    cc.insert({120,130});
    for(yuki::IntegralCIs_OV<int>::const_number_iterator i = cc.begin_number();!i.is_end();++i)
        printf("%d ",*i);
    printf("\n");
    }

    printf("TEST4 DONE!\n\n");

    {
    yuki::IntegralCIs_OV<char32_t> cc({0,0x10FFFF});
    cc = cc-yuki::IntegralCIs_OV<char32_t>({U'\n',U'\n'});
    print_cc(cc);

    yuki::IntegralCIs_OV<char32_t> cc2 = cc-yuki::IntegralCIs_OV<char32_t>({U'\"',U'\"'});
    print_cc(cc2);

    yuki::IntegralCIs_OV<char32_t> cc3({10,34});
    print_cc(cc3-cc2);

    yuki::IntegralCIs_OV<char32_t> cc4 = cc+cc;
    print_cc(cc4);
    print_cc(yuki::IntegralCIs_OV<char32_t>({U'\"',U'\"'})-cc4);
    }
    printf("TEST5 DONE!\n\n");

    {
    yuki::IntegralCIs_OV<int> cc;
    cc.insert(100);
    cc.insert(102);
    cc.insert(101);
    cc.insert(50);
    cc.insert(51);
    cc.insert(99);
    cc.insert(70);
    cc.insert(200);
    cc.insert(25);
    print_cc(cc);
    }
    printf("TEST6 DONE!\n\n");
}
