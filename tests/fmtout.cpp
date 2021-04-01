#include"../utils.hpp"
struct Foo{
    int a;
    const char* b;
};
template<>
struct fmt::formatter<Foo,char> : yuki::simple_formatter<Foo,char>{
    template<typename OutputIt>
    auto format(Foo const& foo,fmt::basic_format_context<OutputIt,char>& ctx) -> typename fmt::basic_format_context<OutputIt,char>::iterator {
        return fmt::format_to(ctx.out(),"({},{})",foo.a,foo.b);
    }
};
int main(){
    yuki::print_line(1);
    yuki::print_line("123");
    yuki::print_space<','>(stdout,1,"123");
    Foo foo1={1,"first"};
    Foo foo2={2,"second"};
    yuki::print_space(stdout,foo1,foo2);
}