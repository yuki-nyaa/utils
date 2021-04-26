#include"../utils.hpp"
int main(){
    fmt::print("{} {}",yuki::i_th_v<5,size_t,5,2,4,7,8>,yuki::i_th_v<5,std::make_index_sequence<10>>);
}