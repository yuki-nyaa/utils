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
struct Long_Option{
    std::string_view name; ///< Should be null-terminated.
    typedef void(*action_t)(Cmd_Data&,const char*const*,size_t);
    action_t action;
    yuki::U8Char shorthand;
    std::string_view description; ///< Should be null-terminated.
};

template<typename Cmd_Data>
struct Short_Option{
    yuki::U8Char c;
    SO_Cat cat;
    typedef void(*action_t)(Cmd_Data&,const char*const*,size_t);
    action_t action;
    std::string_view description; ///< Should be null-terminated.
};


namespace cmd_impl_{
    struct Long_Option_SV{
        template<typename Cmd_Data>
        static constexpr std::string_view operator()(const Long_Option<Cmd_Data> opt) {return opt.name;}
    };
    struct MMHash3_SV{
        static constexpr size_t operator()(const std::string_view sv) {return mmhash3(sv.data(),sv.size());}
    };
    struct Long_Option_NIL{
        template<typename Cmd_Data>
        static constexpr bool operator()(const Long_Option<Cmd_Data> opt) {return opt.action==nullptr;}
    };
} // namespace cmd_impl_

template<typename Cmd_Data,size_t slots>
struct Long_Option_Table:
    private CHashTable<std::string_view,Long_Option<Cmd_Data>,cmd_impl_::Long_Option_SV,cmd_impl_::MMHash3_SV,yuki::Equal_To<std::string_view>,cmd_impl_::Long_Option_NIL,slots>
{
  private:
    typedef CHashTable<std::string_view,Long_Option<Cmd_Data>,cmd_impl_::Long_Option_SV,cmd_impl_::MMHash3_SV,yuki::Equal_To<std::string_view>,cmd_impl_::Long_Option_NIL,slots> CHT;
  public:
    static Cmd_Data Cmd_Data_t();
    using CHT::CHT;
    using CHT::operator[];
};

template<typename Cmd_Data,size_t N>
Long_Option_Table(const Long_Option<Cmd_Data>(&)[N]) -> Long_Option_Table<Cmd_Data,std::bit_ceil(N)>;


template<typename Cmd_Data,size_t slots>
struct Short_Option_Table{
  private:
    Short_Option<Cmd_Data> storage[slots] = {};
    size_t size = 0;

    static constexpr Short_Option<Cmd_Data> nil = {};
  public:
    constexpr Short_Option_Table() noexcept = default;

    template<typename It>
    constexpr Short_Option_Table(It it,const size_t s) noexcept :
        storage{}
    {
        struct{
            unsigned i;
            yuki::U8Char c;
        } lab[slots]{};
        size_t data_end = 0;
        for(size_t i=0;i<s;++it,++i){
            assert(it->c.raw()!=0 && it->c<=yuki::UNICODE_MAX_U8);
            auto* const fg = yuki::first_greater(lab,lab+data_end,it->c,[](const auto l){return l.c;},yuki::Less<yuki::U8Char>{});
            if(fg==lab || (fg-1)->c!=it->c){
                yuki::move_overlap(fg+1,fg,(lab+data_end)-fg);
                fg->i = i;
                fg->c = it->c;
                ++data_end;
            }
        }
        it-=s;
        for(size_t i=0;i<data_end;++i)
            storage[i]=it[lab[i].i];
        size = data_end;
    }

    template<size_t N>
    constexpr Short_Option_Table(const Short_Option<Cmd_Data>(&arr)[N]) noexcept :
        Short_Option_Table(arr,N)
    {}

    constexpr Short_Option_Table(std::initializer_list<Short_Option<Cmd_Data>> il) noexcept :
        Short_Option_Table(il.begin(),il.size())
    {}

    constexpr const Short_Option<Cmd_Data>& operator[](const yuki::U8Char u8c) const {
        const Short_Option<Cmd_Data>* const feg = yuki::first_equiv_greater(storage,storage+size,u8c,[](const auto so){return so.c;},yuki::Less<yuki::U8Char>{});
        return feg!=storage+size && feg->c==u8c ? *feg : nil;
    }
}; // struct Short_Option_Table

template<typename Cmd_Data,size_t N>
Short_Option_Table(const Short_Option<Cmd_Data>(&)[N]) -> Short_Option_Table<Cmd_Data,N>;


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


/// @note There's no checking that all `lo(:los).shorthand`s are present in `sos`.
template<typename Cmd_Data,size_t Nl,size_t Ns>
void cmd_help(FILE* const out,const Long_Option<Cmd_Data>(&los)[Nl],const Short_Option<Cmd_Data>(&sos)[Ns]){
    yuki::U8Char so_with_long[Nl<Ns?Nl:Ns] = {};
    size_t so_with_long_end = 0;

    for(const Long_Option<Cmd_Data> lo : los){
        fprintf(out,"--%s    ",lo.name.data());
        if(lo.shorthand.raw()!=0){
            assert(lo.shorthand<=yuki::UNICODE_MAX_U8);
            yuki::U8Char* const fg = yuki::first_greater(so_with_long,so_with_long+so_with_long_end,lo.shorthand);
            if(fg==so_with_long || *(fg-1)!=lo.shorthand){
                yuki::move_overlap(fg+1,fg,(so_with_long+so_with_long_end)-fg);
                *fg=lo.shorthand;
                ++so_with_long_end;
            }
            char buf[5] = {};
            lo.shorthand.write_to(buf);
            fprintf(out,"-%s    ",buf);
        }
        fprintf(out,"%s\n",lo.description.data());
    }

    assert(so_with_long_end<Ns); // If not, i.e. the number of different shorthands in `los` exceeds the number of short options, then there must be at least one shorthand that is not present in `sos`. That is, either the shorthand is misspelled or a short option is mistakenly left out.

    for(const Short_Option<Cmd_Data> so : sos){
        if(!yuki::bcontains(so_with_long,so_with_long+so_with_long_end,so.c)){
            assert(so.c.raw()!=0 && so.c<=yuki::UNICODE_MAX_U8);
            char buf[5] = {};
            so.c.write_to(buf);
            fprintf(out,"-%s    %s\n",buf,so.description.data());
        }
    }
}


template<typename Cmd_Data>
void default_cmd_first(Cmd_Data&,const char* const first){
    Normal_CPEH::dangling_argument(1,first);
}


/// @pre Each `argv[i]` should be null-terminated.
template<const auto& los,const auto& sos,void(*action_first)(decltype(los.Cmd_Data_t())&,const char*)=default_cmd_first,typename CPEH=Normal_CPEH,size_t arg_slots=64,typename Cmd_Data>
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
                const typename Long_Option<Cmd_Data>::action_t action = los[sv].action;
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
                    const Short_Option<Cmd_Data> so = sos[u8c];
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
