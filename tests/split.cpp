#include"../utils.hpp"
int main(){
    const char* str_a="nsdiopvan.cnaspc.sss.csacsad.eeas";
    auto [lhs1,rhs1]=yuki::split_first(str_a,'.');
    auto [lhs2,rhs2]=yuki::split_last(str_a,'.');
    fmt::print("{} ---------- {}\n",lhs1,rhs1);
    fmt::print("{} ---------- {}\n",lhs2,rhs2);

    auto [lhs3,rhs3]=yuki::vsplit_first(str_a,'.');
    auto [lhs4,rhs4]=yuki::vsplit_last(str_a,'.');
    fmt::print("{} ---------- {}\n",lhs3,rhs3);
    fmt::print("{} ---------- {}\n",lhs4,rhs4);
}