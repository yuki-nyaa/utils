#include<yuki/print.hpp>
int main(){
    FILE* f1 = fopen("cat1.txt","a");
    FILE* f2 = fopen("cat2.txt","r");
    yuki::concat_file(f1,f2);
}