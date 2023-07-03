#include<yuki/Span.hpp>
#include<string>

using namespace yuki::literals;
constexpr yuki::String_View span1 = "abcde"_sp;
constexpr yuki::String_View span11 = "abcde"_sp;
static_assert(span1==span11);

int main(){
    char arr[6] = "abcde";
    yuki::Span span("abcde");
    yuki::Span span2(arr);
    std::string str;
    yuki::Span span3 = std::as_const(str);
}
