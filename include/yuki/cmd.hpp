#pragma once
#include<yuki/CHashTable_Str.hpp>
#include<yuki/algorithm.hpp>
#include<yuki/uchar.hpp>
#include<yuki/Allocator.hpp>
#include<string_view>
#include<bit>
#include<cstdio>

namespace yuki{
 /* Command line option categories:
    1: Long option, which starts with "--". The string thereafter is treated as the option name and all later strings that do not begin with "-" are treated as arguments.
    2: Short option, consisting of a single character, preceded by "-". However, that the option ITSELF consists of a single character does not mean that only one character can follow "-". Depending on how the parser handles later characters, this category can be further divided into 2 sub-categories:
        B: The parser recognizes the string following the option character as the first argument to this option. E.g. "-Wall", in which "all" is recognized as the first argument to the short option "-W". All later strings that do not begin with "-" are treated as later arguments. (Note: Options like "pacman -Syu" are of this category too. "yu" will be treated as a single argument so you have to extract individual characters by yourself.)
        A: The parser recognizes the character immediately after it as another short option. E.g. "-abc" has the same effect as "-a -bc". How "c" will be treated depends on the category of "b". So for example if "b" is also of category A then "-abc" is equivalent to "-a -b -c". All later strings that do not begin with "-" are treated as arguments to "c".
    3. The default "--help" and "-?" option, either of which will print a help message and make the parse function return a non-zero value immediately.
 */
struct Short_Option{
    yuki::U8Char name;
    enum : unsigned {N=0,A,B} cat;

    Short_Option() noexcept = default;

    constexpr Short_Option(const yuki::U8Char u8c,const decltype(A) cat_p) noexcept :
        name(u8c),
        cat(cat_p)
    {}

    constexpr Short_Option(const yuki::U8Char u8c) noexcept :
        name(u8c),
        cat(u8c.raw()==0 ? N : A)
    {}

    constexpr Short_Option(const char32_t c32) noexcept :
        Short_Option(yuki::U8Char(c32))
    {}

    constexpr bool valid() const {
        return name.raw()==0 || (name<=yuki::UNICODE_MAX_U8 && cat!=N);
    }
};

template<typename Cmd_Data>
struct Cmd_Option{
    std::string_view name; ///< Should be null-terminated.
    Short_Option shorthand;
    void(*action)(Cmd_Data&,const char*const*,size_t);
    const char* description;

    typedef Cmd_Data Cmd_Data_t;
    typedef decltype(action) action_t;
    constexpr bool valid() const {
        return (!name.empty() || shorthand.name.raw()!=0)
            && shorthand.valid()
            && action;
    }
};


struct Noop_CPEH{
    static constexpr void unknown_long_option(size_t,std::string_view) {}
    static constexpr void unknown_short_option(size_t,yuki::U8Char) {}
};

struct Normal_CPEH{
    static void unknown_long_option(const size_t i,const std::string_view name){
        fprintf(stderr,"YCMD Error: Unknown long option \"%s\" at position %zu!\n",name.data(),i);
    }
    static void unknown_short_option(const size_t i,const yuki::U8Char c){
        char buf[5] = {};
        c.write_to(buf);
        fprintf(stderr,"YCMD Error: Unknown short option \"%s\" at position %zu!\n",buf,i);
    }
};


template<typename Cmd_Data,size_t N>
void cmd_help(FILE* const out,const Cmd_Option<Cmd_Data>(&cos)[N],const char* const prologue="Usage:\n",const char* const epilogue=""){
    fputs(prologue,out);
    bool helped = false;
    for(const Cmd_Option<Cmd_Data> co : cos){
        if(!co.name.empty()){
            if(!helped && co.name=="help")
                helped=true;
            fprintf(out,"--%s    ",co.name.data());
        }if(co.shorthand.name.raw()!=0){
            if(!helped && co.shorthand.name==U'?')
                helped=true;
            char buf[5] = {};
            co.shorthand.name.write_to(buf);
            fprintf(out,"-%s    ",buf);
        }
        if(co.description)
            fputs(co.description,out);
        fputc(static_cast<unsigned char>('\n'),out);
    }
    if(!helped)
        fputs("--help    -?    Print help.\n",out);
    fputs(epilogue,out);
}


template<typename Cmd_Data>
void default_positional_action(Cmd_Data&,const char*const* argv,const size_t argc){
    for(size_t i=0;i<argc;++argv,++i)
        fprintf(stderr,"YCMD Error: Dangling argument \"%s\" at position %zu!\n",*argv,i+1);
}


namespace cmd_impl_{
    struct COT_Size {
        size_t nl;
        size_t ns;
        bool has_sb;

        COT_Size() noexcept = default;

        template<typename Cmd_Data,size_t N>
        constexpr COT_Size(const Cmd_Option<Cmd_Data>(&coarr)[N]) noexcept :
            nl(0),
            ns(0),
            has_sb(false)
        {
            for(const Cmd_Option<Cmd_Data> co : coarr){
                if(!co.name.empty())
                    ++nl;
                if(co.shorthand.name.raw()!=0){
                    ++ns;
                    if(co.shorthand.cat==yuki::Short_Option::B)
                        has_sb=true;
                }
            }
            nl = std::bit_ceil(nl+1);
        }

        constexpr COT_Size(const size_t nlp,const size_t nsp) noexcept :
            nl(nlp),
            ns(nsp),
            has_sb(true)
        {}
    };
} // namespace cmd_impl_

template<const auto& coarr,cmd_impl_::COT_Size cot_size=coarr>
struct Cmd_Option_Table{
  private:
    typedef typename std::remove_cvref_t<decltype(coarr[0])>::Cmd_Data_t Cmd_Data;
    struct Long_Option{
        std::string_view name;
        void(*action)(Cmd_Data&,const char*const*,size_t);

        struct SV{
            static constexpr std::string_view operator()(const Long_Option opt) {return opt.name;}
        };

        struct Nil{
            static constexpr bool operator()(const Long_Option opt) {return opt.action==nullptr;}
        };
    };
    struct Short_Option_With_Action : private Short_Option{
        using Short_Option::name;
        using Short_Option::cat;
        void(*action)(Cmd_Data&,const char*const*,size_t);
    };

    CHashTable_Str<Long_Option,typename Long_Option::SV,typename Long_Option::Nil,cot_size.nl> los;
    Short_Option_With_Action sos[cot_size.ns];
    size_t sos_end;
  public:
    constexpr Cmd_Option_Table() noexcept :
        los(),
        sos{},
        sos_end(0)
    {
        for(const Cmd_Option<Cmd_Data> co : coarr){
            assert(co.valid());
            if(!co.name.empty())
                los.emplace(Long_Option{co.name,co.action});
            if(co.shorthand.name.raw()!=0){
                Short_Option_With_Action* const fg = yuki::first_greater(sos,sos+sos_end,co.shorthand.name,[](const Short_Option_With_Action so){return so.name;},yuki::Less<yuki::U8Char>{});
                if(fg==sos || (fg-1)->name!=co.shorthand.name){
                    yuki::move_overlap(fg+1,fg,(sos+sos_end)-fg);
                    fg->name = co.shorthand.name;
                    fg->cat = co.shorthand.cat;
                    fg->action = co.action;
                    ++sos_end;
                }
            } // if(co.shorthand.name.raw()!=0)
        } // for(const Cmd_Option<Cmd_Data> co : coarr)
    }

    constexpr const Long_Option* find(const std::string_view sv) const {return los.find(sv);}

    constexpr const Short_Option_With_Action* find(const yuki::U8Char so) const {
        const Short_Option_With_Action* const feg = yuki::first_equiv_greater(sos,sos+sos_end,so,[](const Short_Option_With_Action so){return so.name;},yuki::Less<yuki::U8Char>{});
        return feg!=sos+sos_end && feg->name==so ? feg : nullptr;
    }

    constexpr Long_Option operator[](const std::string_view sv) const {
        if(const Long_Option* const f=find(sv); f)
            return *f;
        else
            return Long_Option{};
    }

    constexpr Short_Option_With_Action operator[](const yuki::U8Char so) const {
        const Short_Option_With_Action* const feg = yuki::first_equiv_greater(sos,sos+sos_end,so,[](const Short_Option_With_Action so){return so.name;},yuki::Less<yuki::U8Char>{});
        return feg!=sos+sos_end && feg->name==so ? *feg : Short_Option_With_Action{};
    }

    /// @pre Each `argv[i]` should be non-null, non-empty, and null-terminated. The last one should be `nullptr`.
    /// @return Non-zero if the default "--help" or "-?" is encountered. 0 otherwise.
    template<void(*positional_action)(Cmd_Data&,const char*const*,size_t)=default_positional_action,typename CPEH=Normal_CPEH,size_t sbarg_slots=cot_size.has_sb?32:0>
    constexpr int parse(Cmd_Data& cmd_data,const size_t argc,const char*const*const argv,const char* const help_prologue="Usage:\n",const char* const help_epilogue="") const {
        struct{
            const char* data[sbarg_slots] = {};
            size_t end = 0;

            constexpr void emplace_back(const char* const p){
                assert(end<sbarg_slots); // `sbarg_slots` should be increased in this case.
                data[end++]=p;
            }
        } sbargs;

        size_t i=1;

        while(i<argc && *argv[i]!='-')
            ++i;

        if(i>1)
            positional_action(cmd_data,argv+1,i-1);

        while(i<argc){
            assert(argv[i] && *argv[i] != '\0' && *argv[i]=='-');
            const char* current = argv[i]+1;
            if(*current=='-'){
                const std::string_view sv = current+1;
                if(const Long_Option* const lo=los.find(sv); lo){
                    const size_t arg_begin = ++i;
                    while(i<argc && *argv[i]!='-')
                        ++i;
                    lo->action(cmd_data,argv+arg_begin,i-arg_begin);
                }else if(!std::is_constant_evaluated() && sv=="help"){
                    cmd_help(stderr,coarr,help_prologue,help_epilogue);
                    return 1;
                }else{
                    CPEH::unknown_long_option(i,sv);
                    ++i;
                    while(i<argc && *argv[i]!='-')
                        ++i;
                }
            }else{
                bool is_sa = true; // As a `constexpr` way to exit multiple loops. C++23 will lift the ban of `goto` in `constexpr` functions so I'm gonna remove this at that time.
                while(is_sa){
                    const yuki::U8Char u8c(current);
                    switch(const auto so=operator[](u8c); so.cat){
                        case Short_Option::N:{
                            if(!std::is_constant_evaluated() && *current=='?'){
                                cmd_help(stderr,coarr,help_prologue,help_epilogue);
                                return 1;
                            }else{
                                CPEH::unknown_short_option(i,u8c);
                                current += u8c.length();
                                if(*current=='\0'){
                                    ++i;
                                    while(i<argc && *argv[i]!='-')
                                        ++i;
                                    is_sa = false;
                                }
                            }
                            break;
                        }
                        case Short_Option::A:{
                            current += u8c.length();
                            if(*current=='\0'){
                                const size_t arg_begin = ++i;
                                while(i<argc && *argv[i]!='-')
                                    ++i;
                                so.action(cmd_data,argv+arg_begin,i-arg_begin);
                                is_sa = false;
                            }else
                                so.action(cmd_data,nullptr,0);
                            break;
                        }
                        case Short_Option::B:{
                            sbargs.end=0;
                            current += u8c.length();
                            if(*current!='\0')
                                sbargs.emplace_back(current);
                            ++i;
                            for(;i<argc && *argv[i]!='-'; ++i)
                                sbargs.emplace_back(argv[i]);
                            so.action(cmd_data,sbargs.data,sbargs.end);
                            is_sa = false;
                            break;
                        }
                        default: assert(false); std::unreachable();
                    } // switch(const auto so=find_so(u8c); so.cat)
                } // while(is_sa)
            } // if(*current=='-') else
        } // while(i<argc)
        return 0;
    } // cmd_parse
}; // struct Cmd_Option_Table
} // namespace yuki
