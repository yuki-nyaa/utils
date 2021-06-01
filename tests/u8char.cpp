#include"../utils.hpp"
int main(){
    const char* str1 = "'好'";
    yuki::U8Char u8char1(str1,1);
    // 0 e5 a5 bd
    printf("%x %x %x %x\n",u8char1.bytes[0],u8char1.bytes[1],u8char1.bytes[2],u8char1.bytes[3]);
    const char* str2 = "zß水🍌";
    yuki::U8Char u8char2(str2,3);
    // f0 9f 8d 8c
    printf("%x %x %x %x\n",u8char2.bytes[0],u8char2.bytes[1],u8char2.bytes[2],u8char2.bytes[3]);
}