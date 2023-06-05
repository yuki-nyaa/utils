#include<yuki/Ordered_Vector.hpp>
#include<cstdio>
#include<string>
int main(){
    {
    yuki::Vector<int> v0;
    v0.emplace_back(4);
    v0.emplace_back(8);
    v0.emplace_back(2);
    v0.emplace_back(4);
    v0.emplace_back(16);
    v0.emplace_back(8);
    v0.emplace_back(32);
    v0.emplace_back(16);
    v0.emplace_back(64);
    printf("%zu %zu\n",v0.size(),v0.capacity());
    for(const int i:v0)
        printf("%d ",i);

    printf("\n");

    yuki::Vector<int> v00 = v0;
    printf("%zu %zu\n",v00.size(),v00.capacity());
    for(const int i:v00)
        printf("%d ",i);

    printf("\n");

    v00.emplace_back(0);
    printf("%zu %zu\n",v00.size(),v00.capacity());
    for(const int i:v00)
        printf("%d ",i);

    printf("\n");

    v00.clear();
    v00=v0;
    printf("%zu %zu\n",v00.size(),v00.capacity());
    for(const int i:v00)
        printf("%d ",i);

    printf("\n");

    v00=std::move(v0);
    printf("%zu %zu\n",v00.size(),v00.capacity());
    for(const int i:v00)
        printf("%d ",i);

    printf("\nTEST0 DONE!\n\n");
    }

    yuki::Vector<int> v(100);
    yuki::Vector<int,yuki::List_Allocator<int>> v2{1,2,3,4,5};
    yuki::List_Allocator<int> la1;
    yuki::Vector<int,yuki::List_Allocator<int>> v3(std::move(la1));
    v2=v3;
    yuki::Vector<int,yuki::List_Allocator<int>> v4{1,2,3,4,5};
    for(int i: v4)
        printf("%d ",i);
    printf("\n");

    yuki::Vector<int> v5;
    v5.emplace_back(100);
    v5.emplace_back(101);
    v5.pop_back();
    v5.emplace_back(101);
    for(int i: v5)
        printf("%d ",i);
    printf("\n");
    for(int i = 0;i<=10;++i)
        v5.emplace(v5.begin()+1,102+i);
    for(int i: v5)
        printf("%d ",i);
    printf("\n");
    int arr[32];
    for(int i = 0;i<32;++i)
        arr[i] = 200+i;
    v5.insert(v5.begin()+1,arr,32);
    for(int i: v5)
        printf("%d ",i);
    printf("\n");
    printf("%zu %zu\n",v5.size(),v5.capacity());
    v5.pop_back();
    printf("%zu %zu\n",v5.size(),v5.capacity());

    printf("TEST1 DONE!\n\n");

    std::string arr_str[32];
    for(int i = 0;i<32;++i)
        arr_str[i] = "foo";
    yuki::Vector<std::string> v6;
    v6.emplace_back("str0");
    v6.emplace_back("str1");
    v6.insert(v6.begin()+1,std::make_move_iterator(arr_str),32);
    for(const std::string& s : v6)
        printf("%s ",s.c_str());
    printf("\n");
    for(int i = 0;i<32;++i)
        printf("%zu ",arr_str[i].size());
    printf("\n");

    std::string arr1_str[16];
    for(int i = 0;i<16;++i)
        arr1_str[i] = "bar";
    v6.erase_then_insert(v6.begin()+1,8,std::make_move_iterator(arr1_str),16);
    for(const std::string& s : v6)
        printf("%s ",s.c_str());
    printf("\n");
    printf("TEST2 DONE!\n\n");

    yuki::Vector<int> v7{1,3,5,7,9};
    for(int i:v7) printf("%d ",i);
    printf("\n");
    printf("TEST3 DONE!\n\n");

    yuki::Ordered_Vector<int> ov(yuki::from_ordered_tag,{1,3,5,7,9});
    yuki::Ordered_Vector<int> ov1(yuki::from_ordered_tag,{2,4,6,8,10});
    for(int i:ov) printf("%d ",i);
    printf("\n");
    yuki::Ordered_Vector<int> ov2 = ov+ov1;
    for(int i:ov2) printf("%d ",i);
    printf("\n");
    printf("TEST4 DONE!\n\n");

    yuki::Vector<std::string> v8(yuki::from_variadic_tag,"aaa","bbb","ccc");
    for(const std::string& s : v8)
        printf("%s ",s.c_str());
    printf("\n%zu,%zu\n",v8.size(),v8.capacity());
    printf("TEST5 DONE!\n\n");

    yuki::Ordered_Vector<int> ov3(yuki::from_ordered_tag,{1,3,5,7,9,11,13,15});
    printf("%d\n",*(ov3.last_less(11)));
    printf("TEST6 DONE!\n\n");

    yuki::Ordered_Vector<int> ov4 = ov3;
    for(int i : ov4)
        printf("%d ",i);
    printf("\n\n");
    for(int i : ov3)
        printf("%d ",i);
    printf("\n\n");
    yuki::Ordered_Vector<int> ov5 = std::move(ov4);
    for(int i : ov4)
        printf("%d ",i);
    printf("\n\n");
    for(int i : ov5)
        printf("%d ",i);
    printf("\n\n");
    ov5 = ov3;
    for(int i : ov3)
        printf("%d ",i);
    printf("\n\n");
    for(int i : ov5)
        printf("%d ",i);
    printf("\n\n");
    printf("TEST7 DONE!\n\n");

    yuki::Ordered_Vector<int> ov6(yuki::from_ordered_tag,{1,3,5,7,9});
    yuki::Ordered_Vector<int> ov7(yuki::from_ordered_tag,{2,4,6,8,9});
    yuki::Ordered_Vector<int> ov8=ov6;
    ov8.merge(std::move(ov7));
    for(auto i : ov7)
        printf("%u ",i);
    printf("\n\n");
    for(int i : ov8)
        printf("%d ",i);
    printf("\n\n");

    //yuki::Vector<const std::string> v9;
    //v9.emplace_back("zero");
    //v9.emplace_back("one");
    //v9.emplace_back("two");
    //v9.pop_back(2);
}