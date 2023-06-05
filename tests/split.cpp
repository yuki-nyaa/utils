#include<yuki/string_split.hpp>
#include<fmt/core.h>
int main(){
    constexpr const char str_a[34] = "nsdiopvan.cnaspc.sss.csacsad.eeas";

    auto [lhs1,rhs1]=yuki::split_first(str_a,'.');
    auto [lhs2,rhs2]=yuki::split_last(str_a,'.');
    fmt::print("{} ---------- {}\n",lhs1,rhs1);
    fmt::print("{} ---------- {}\n",lhs2,rhs2);

    auto [lhs3,rhs3]=yuki::vsplit_first(str_a,'.');
    auto [lhs4,rhs4]=yuki::vsplit_last(str_a,'.');
    fmt::print("{} ---------- {}\n",lhs3,rhs3);
    fmt::print("{} ---------- {}\n",lhs4,rhs4);

    constexpr const char str_b[6] = "12345";

    auto [stem1,ext1]=yuki::split_filename(str_a);
    fmt::print("{} ---------- {}\n",stem1,ext1);
    auto [stem2,ext2]=yuki::vsplit_filename(str_a);
    fmt::print("{} ---------- {}\n",stem2,ext2);
    auto [stem3,ext3]=yuki::split_filename(str_b);
    fmt::print("{} ---------- {}\n",stem3,ext3);
    auto [stem4,ext4]=yuki::vsplit_filename(str_b);
    fmt::print("{} ---------- {}\n",stem4,ext4);
}