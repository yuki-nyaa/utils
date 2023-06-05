#pragma once
#include<yuki/CHashTable.hpp>
#include<yuki/mmhash3.hpp>
#include<yuki/algorithm.hpp>
#include<yuki/uchar.hpp>
#include<yuki/Allocator.hpp>
#include<string_view>
#include<bit>
#include<cstdio>

namespace yuki{
enum struct SO_Cat : unsigned {N=0,a,b};

template<typename Cmd_Data>
struct Cmd_Option{
    std::string_view name; ///< Should be null-terminated.
    yuki::U8Char shorthand;
    SO_Cat so_cat;
    void(*action)(Cmd_Data&,const char*const*,size_t);
    const char* description;

    typedef decltype(action) action_t;
    constexpr bool valid() const {
        return (!name.empty() && shorthand.raw()==0)
            && (name.empty() || name!="help")
            && (shorthand.raw()==0 || (shorthand<=yuki::UNICODE_MAX_U8 && shorthand!=yuki::U8Char(U'?') && so_cat!=SO_Cat::N))
            && action;
    }
};

template<typename Cmd_Data,size_t long_slots,size_t short_slots>
struct Cmd_Option_Table{
  private:
    struct Long_Option{
        std::string_view name;
        void(*action)(Cmd_Data&,const char*const*,size_t);
    };
    struct Short_Option{
        yuki::U8Char name;
        SO_Cat cat;
        void(*action)(Cmd_Data&,const char*const*,size_t);
    };
    struct Long_Option_SV{
        static constexpr std::string_view operator()(const Long_Option opt) {return opt.name;}
    };
    struct MMHash3_SV{
        static constexpr size_t operator()(const std::string_view sv) {return mmhash3(sv.data(),sv.size());}
    };
    struct Long_Option_NIL{
        static constexpr bool operator()(const Long_Option opt) {return opt.action==nullptr;}
    };

    CHashTable<std::string_view,Long_Option,Long_Option_SV,MMHash3_SV,yuki::Equal_To<std::string_view>,Long_Option_NIL,long_slots> los;
    Short_Option sos[short_slots];
    size_t sos_end;
  public:
    template<size_t N>
    constexpr Cmd_Option_Table(const Cmd_Option<Cmd_Data>(&arr)[N]) noexcept :
        los(),
        sos{},
        sos_end(0)
    {
        for(const Cmd_Option<Cmd_Data> co : arr){
            assert(co.valid());
            if(!co.name.empty())
                los.insert(Long_Option{co.name,co.action});
            if(co.shorthand.raw()!=0){
                Short_Option* const fg = yuki::first_greater(sos,sos+sos_end,co.shorthand,[](const Short_Option so){return so.name;},yuki::Less<yuki::U8Char>{});
                if(fg==sos || (fg-1)->name!=co.shorthand){
                    yuki::move_overlap(fg+1,fg,(sos+sos_end)-fg);
                    fg->name = co.shorthand;
                    fg->cat = co.so_cat;
                    fg->action = co.action;
                    ++sos_end;
                }
            } // if(co.shorthand.raw()!=0)
        } // for(const Cmd_Option<Cmd_Data> co : arr)
    }

    constexpr typename Cmd_Option<Cmd_Data>::action_t operator[](const std::string_view lo) const {
        return los[lo].action;
    }

    constexpr Short_Option operator[](const yuki::U8Char so) const {
        const Short_Option* const feg = yuki::first_equiv_greater(sos,sos+sos_end,so,[](const Short_Option so){return so.name;},yuki::Less<yuki::U8Char>{});
        return feg!=sos+sos_end && feg->name==so ? *feg : Short_Option{};
    }

    static Cmd_Data Cmd_Data_t();
}; // struct Cmd_Option_Table

template<typename Cmd_Data,size_t N>
Cmd_Option_Table(const Cmd_Option<Cmd_Data>(&arr)[N]) -> Cmd_Option_Table<Cmd_Data,std::bit_ceil(N),N>;


struct Noop_CPEH{
    static constexpr void unknown_long_option(size_t,std::string_view) {}
    static constexpr void unknown_short_option(size_t,yuki::U8Char) {}
    static constexpr void dangling_argument(size_t,const char*) {}
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
    static void dangling_argument(const size_t i,const char* const arg){
        fprintf(stderr,"YCMD Error: Dangling argument \"%s\" at position %zu!\n",arg,i);
    }
};


template<typename Cmd_Data,size_t N>
void cmd_help(FILE* const out,const Cmd_Option<Cmd_Data>(&cos)[N],const char* const prologue="Usage:\n",const char* const epilogue=""){
    fprintf(out,"%s",prologue);
    for(const Cmd_Option<Cmd_Data> co : cos){
        if(!co.name.empty())
            fprintf(out,"--%s    ",co.name.data());
        if(co.shorthand.raw()!=0){
            char buf[5] = {};
            co.shorthand.write_to(buf);
            fprintf(out,"-%s    ",buf);
        }
        fprintf(out,"%s\n",co.description);
    }
    fprintf(out,"-?    --help    Print help.\n%s",epilogue);
}


template<typename Cmd_Data>
void default_cmd_first(Cmd_Data&,const char* const first){
    Normal_CPEH::dangling_argument(1,first);
}


/// @pre Each `argv[i]` should be null-terminated.
template<const auto& cot,void(*action_first)(decltype(cot.Cmd_Data_t())&,const char*)=default_cmd_first,typename CPEH=Normal_CPEH,size_t arg_slots=64,typename Cmd_Data>
constexpr void cmd_parse(Cmd_Data& cmd_data,const size_t argc,const char*const*const argv){
    struct{
        const char* data[arg_slots] = {};
        size_t end = 0;

        constexpr void emplace_back(const char* const p){
            assert(end<arg_slots); // `arg_slots` should be increased in this case.
            data[end++]=p;
        }
        constexpr void clear(){
            while(end!=0)
                data[--end]=nullptr;
        }
    } args;

    size_t i=1;

    if(argc>0 && argv[1][0]!='-'){
        action_first(cmd_data,argv[1]);
        i=2;
    }

    while(i<argc){
        const char* current = argv[i];
        if(current[0]=='-'){
            if(current[1]=='-'){
                const std::string_view sv = current+2;
                const typename Cmd_Option<Cmd_Data>::action_t action = cot[sv];
                if(action){
                    args.clear();
                    ++i;
                    for(;i<argc && argv[i][0]!='-'; ++i)
                        args.emplace_back(argv[i]);
                    action(cmd_data,args.data,args.end);
                }else{
                    CPEH::unknown_long_option(i,sv);
                    ++i;
                    for(;i<argc && argv[i][0]!='-'; ++i)
                        ;
                }
            }else{
                ++current;
                bool is_sa = true;
                while(is_sa){
                    const yuki::U8Char u8c(current);
                    const auto so = cot[u8c];
                    switch(so.cat){
                        case SO_Cat::N:{
                            CPEH::unknown_short_option(i,u8c);
                            current += u8c.length();
                            if(*current=='\0'){
                                ++i;
                                for(;i<argc && argv[i][0]!='-'; ++i)
                                    ;
                                is_sa = false;
                            }
                            break;
                        }
                        case SO_Cat::a:{
                            current += u8c.length();
                            if(*current=='\0'){
                                args.clear();
                                ++i;
                                for(;i<argc && argv[i][0]!='-'; ++i)
                                    args.emplace_back(argv[i]);
                                so.action(cmd_data,args.data,args.end);
                                is_sa = false;
                            }else
                                so.action(cmd_data,nullptr,0);
                            break;
                        }
                        case SO_Cat::b:{
                            args.clear();
                            if(*(current+u8c.length())!='\0')
                                args.emplace_back(current+u8c.length());
                            ++i;
                            for(;i<argc && argv[i][0]!='-'; ++i)
                                args.emplace_back(argv[i]);
                            so.action(cmd_data,args.data,args.end);
                            is_sa = false;
                            break;
                        }
                        default: assert(false); std::unreachable();
                    } // switch(so.cat)
                } // while(is_sa)
            } // if(current[1]=='-') else
        }else{ // if(current[0]=='-')
            CPEH::dangling_argument(i,argv[i]);
            ++i;
        }
    } // while(i<argc)
}
} // namespace yuki
