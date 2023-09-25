#include<cstdio>
#include<cstdlib>
#include<string>
#include<cassert>
#include<yuki/Interval.hpp>

constexpr char32_t UNICODE_TOTAL = 0x110000;

#ifndef IND
#define IND "    "
#endif

#define YUKI_UGEN_HEADER_PROLOGUE \
    "#pragma once\n" \
    "#include<yuki/unicode_base.hpp>\n" \
    "\n" \
    "namespace yuki::unicode{\n" \
    "\n"

#define YUKI_UGEN_CPP_SIMPLE_PROLOGUE \
    "#include<yuki/Interval.hpp>\n" \
    "\n" \
    "namespace yuki::unicode{\n" \
    "\n"

#define YUKI_UGEN_EPILOGUE "} // namespace yuki::unicode\n"

struct Buf{
    char data[105]={};
    unsigned size=0;
    #ifdef YUKI_UGEN_DBG
    static unsigned max_size;
    #endif

    void push_back(const char c){
        assert(size<sizeof(data)); // Increase `data`'s extent if it fires.
        data[size++]=c;
        #ifdef YUKI_UGEN_DBG
        if(size>max_size)
            max_size=size;
        #endif
    }

    void remove_trailing_spaces(){
        while(size && data[size-1]==' ')
            --size;
    }
} buf;

#ifdef YUKI_UGEN_DBG
unsigned Buf::max_size=0;
#endif

unsigned cat=0;
const char* cat_name="";
yuki::IntegralCIs_OV<char32_t> cc(yuki::reserve_tag,1024), total(yuki::reserve_tag,1024);
size_t total_cp=0, total_total_cp=0;
unsigned char char_table[UNICODE_TOTAL] = {};
size_t char_table_max_nonzero=0;

void write_c32(FILE* const out,const char32_t c){
    switch(c){
        case U'\'' : fputs("U\'\\\'\'",out); return; // Rather cryptic...
        case U'\"' : fputs("U\'\\\"\'",out); return; // Rather cryptic...
        case U'\\' : fputs("U\'\\\\\'",out); return; // Rather cryptic...
        case U'\a' : fputs("U\'\\a\'",out); return;
        case U'\b' : fputs("U\'\\b\'",out); return;
        case U'\f' : fputs("U\'\\f\'",out); return;
        case U'\n' : fputs("U\'\\n\'",out); return;
        case U'\r' : fputs("U\'\\r\'",out); return;
        case U'\t' : fputs("U\'\\t\'",out); return;
        case U'\v' : fputs("U\'\\v\'",out); return;
        default:{
            if(c<0x80U && isprint(c))
                fprintf(out,"U\'%c\'",static_cast<char>(static_cast<unsigned char>(c)));
            else
                fprintf(out,"0x%lX",static_cast<unsigned long>(c));
            return;
        }
    }
}

#ifndef YUKI_UGEN_MAX_LINE_ITEMS
#define YUKI_UGEN_MAX_LINE_ITEMS 8
#endif

void write_cc(FILE* const out,const yuki::IntegralCIs_OV<char32_t>& cc){
    unsigned line_items=0;
    for(const yuki::CInterval<char32_t> ci : cc){
        if(line_items==0)
            fputs(IND,out);
        fputc(static_cast<unsigned char>('{'),out);
        write_c32(out,ci.lb);
        fputc(static_cast<unsigned char>(','),out);
        write_c32(out,ci.ub);
        fputs("},",out);
        if(line_items==YUKI_UGEN_MAX_LINE_ITEMS-1){
            fputc(static_cast<unsigned char>('\n'),out);
            line_items=0;
        }else
            ++line_items;
    }
    if(line_items!=0)
        fputc(static_cast<unsigned char>('\n'),out);
}

void write_cc(FILE* const out_h,FILE* const out_cpp,const char* const name,const yuki::IntegralCIs_OV<char32_t>& cc,const size_t total_cp,const size_t header_threshold){
    if(*name=='\0')
        return;
    if(cc.size()<=header_threshold){
        fprintf(out_h,"inline constexpr yuki::CInterval<char32_t> %s[%zu]={\n",name,cc.size());
        write_cc(out_h,cc);
        fputs("};\n",out_h);
    }else{
        fprintf(out_h,"extern const yuki::CInterval<char32_t> %s[%zu];\n",name,cc.size());
        fprintf(out_cpp,"extern const yuki::CInterval<char32_t> %s[%zu]={\n",name,cc.size());
        write_cc(out_cpp,cc);
        fprintf(out_cpp,"}; // extern const yuki::CInterval<char32_t> %s[%zu]\n\n\n",name,cc.size());
    }
    fprintf(out_h,"inline constexpr size_t %s_total=%zu;\n\n\n",name,total_cp);
};

void write_char_table(FILE* const out){
    #ifndef YUKI_UGEN_MAX_LINE_TINY_ITEMS
    #define YUKI_UGEN_MAX_LINE_TINY_ITEMS 32
    #endif
    unsigned line_tiny_items=0;
    for(unsigned i=0;i<char_table_max_nonzero+1;++i){
        if(line_tiny_items==0)
            fputs(IND,out);
        fprintf(out,"%u,",static_cast<unsigned>(char_table[i]));
        if(line_tiny_items==YUKI_UGEN_MAX_LINE_TINY_ITEMS-1){
            fputc(static_cast<unsigned char>('\n'),out);
            line_tiny_items=0;
        }else
            ++line_tiny_items;
    }
    if(line_tiny_items!=0)
        fputc(static_cast<unsigned char>('\n'),out);
}




constexpr char32_t PARSE_EOF = UNICODE_TOTAL;

yuki::CInterval<char32_t> parse_line(FILE* const in){
    assert(buf.size==0);
    char32_t lb=0, ub=0;
  line:
    switch(const int c=fgetc(in);c){
        case EOF: return{PARSE_EOF,0};
        case static_cast<unsigned char>('#'):
            while(1){
                switch(const int c=fgetc(in);c){
                    case EOF: return{PARSE_EOF,0};
                    case static_cast<unsigned char>('\n'): goto line;
                }
            }
        case static_cast<unsigned char>('\n'):
        case static_cast<unsigned char>('\r'):
        case static_cast<unsigned char>(' '): goto line;
        default: assert(isxdigit(c)); buf.push_back(static_cast<unsigned char>(c)); break;
    }
    while(1){
        switch(const char c=static_cast<unsigned char>(fgetc(in));c){
            case '.':
                buf.data[buf.size]=0;
                lb=strtoul(buf.data,nullptr,16);
                buf.size=0;
                fgetc(in);
                while(1){
                    switch(const char c1=static_cast<unsigned char>(fgetc(in));c1){
                        case ' ':
                            continue;
                        case ';':
                            buf.data[buf.size]=0;
                            ub=strtoul(buf.data,nullptr,16);
                            buf.size=0;
                            fgetc(in);
                            goto second_field;
                        default:
                            assert(isxdigit(static_cast<unsigned char>(c1))); buf.push_back(c1); continue;
                    }
                }
            case ' ': continue;
            case ';':
                buf.data[buf.size]=0;
                lb=strtoul(buf.data,nullptr,16);
                buf.size=0;
                ub=lb;
                fgetc(in);
                goto second_field;
            default: assert(isxdigit(static_cast<unsigned char>(c))); buf.push_back(c); continue;
        }
    }
  second_field:
    assert(buf.size==0);
    while(1){
        switch(const char c=static_cast<unsigned char>(fgetc(in));c){
            case '#':
                while(1){
                    switch(const int c=fgetc(in);c){
                        case EOF:
                        case static_cast<unsigned char>('\n'): return {lb,ub};
                    }
                }
            case '\r': fgetc(in); return {lb,ub};
            case '\n': return {lb,ub};
            default: buf.push_back(c); continue;
        }
    }
}




void general_category(std::string& dir_ucd,std::string& dir_h,std::string& dir_cpp){
    #ifndef YUKI_UGEN_GC_HEADER_THRESHOLD
    #define YUKI_UGEN_GC_HEADER_THRESHOLD 128
    #endif

    dir_ucd.append("extracted/DerivedGeneralCategory.txt");
    FILE* const in = fopen(dir_ucd.c_str(),"r");
    if(!in){
        fprintf(stderr,"Error: Input \"%s\" cannot be opened!\n",dir_ucd.c_str());
        exit(EXIT_FAILURE);
    }

    dir_h.append("general_category.h");
    FILE* const out_h = fopen(dir_h.c_str(),"w");
    if(!out_h){
        fprintf(stderr,"Error: Header output \"%s\" cannot be opened!\n",dir_h.c_str());
        exit(EXIT_FAILURE);
    }

    //const size_t dir_cpp_size = dir_cpp.size();
    dir_cpp.append("general_category.cpp");
    FILE* const out_cpp = fopen(dir_cpp.c_str(),"w");
    if(!out_cpp){
        fprintf(stderr,"Error: Cpp output \"%s\" cannot be opened!\n",dir_cpp.c_str());
        exit(EXIT_FAILURE);
    }

    fputs(YUKI_UGEN_HEADER_PROLOGUE,out_h);
    fputs(YUKI_UGEN_CPP_SIMPLE_PROLOGUE,out_cpp);

    struct{
        yuki::IntegralCIs_OV<char32_t> cc;
        size_t total_cp=0;
    } big_gcs[8];
    unsigned current_big_gc=0;
    bool current_big_gc_lc=false;

    cat=0;
    cat_name="";
    cc.clear();
    total_cp=0;

    while(1){
        const yuki::CInterval<char32_t> ci=parse_line(in);
        if(ci.lb==PARSE_EOF)
            break;

        #define GC_Case(num,name,big_gc,big_gc_lc) \
        if(cat!=num){ \
            big_gcs[current_big_gc].cc = big_gcs[current_big_gc].cc + cc; \
            big_gcs[current_big_gc].total_cp += total_cp; \
            if(current_big_gc_lc){ \
                big_gcs[7].cc = big_gcs[7].cc + cc; \
                big_gcs[7].total_cp += total_cp; \
            } \
            write_cc(out_h,out_cpp,cat_name,cc,total_cp,YUKI_UGEN_GC_HEADER_THRESHOLD); \
            cc.clear(); \
            cat=num; \
            cat_name=name; \
            total_cp=0; \
            current_big_gc=big_gc; \
            current_big_gc_lc=big_gc_lc;\
        } \
        break;

        assert(buf.size>=2);

        switch(buf.data[0]){
            case 'C':
                switch(buf.data[1]){
                    case 'c': GC_Case(1,"Cc",0,false)
                    case 'f': GC_Case(2,"Cf",0,false)
                    case 'n': GC_Case(3,"Cn",0,false)
                    case 'o': GC_Case(4,"Co",0,false)
                    case 's': GC_Case(5,"Cs",0,false)
                }
                break;
            case 'L':
                switch(buf.data[1]){
                    case 'l': GC_Case(6,"Ll",1,true)
                    case 'm': GC_Case(7,"Lm",1,false)
                    case 'o': GC_Case(8,"Lo",1,false)
                    case 't': GC_Case(9,"Lt",1,true)
                    case 'u': GC_Case(10,"Lu",1,true)
                }
                break;
            case 'M':
                switch(buf.data[1]){
                    case 'c': GC_Case(11,"Mc",2,false)
                    case 'e': GC_Case(12,"Me",2,false)
                    case 'n': GC_Case(13,"Mn",2,false)
                }
                break;
            case 'N':
                switch(buf.data[1]){
                    case 'd': GC_Case(14,"Nd",3,false)
                    case 'l': GC_Case(15,"Nl",3,false)
                    case 'o': GC_Case(16,"No",3,false)
                }
                break;
            case 'P':
                switch(buf.data[1]){
                    case 'c': GC_Case(17,"Pc",4,false)
                    case 'd': GC_Case(18,"Pd",4,false)
                    case 'e': GC_Case(19,"Pe",4,false)
                    case 'f': GC_Case(20,"Pf",4,false)
                    case 'i': GC_Case(21,"Pi",4,false)
                    case 'o': GC_Case(22,"Po",4,false)
                    case 's': GC_Case(23,"Ps",4,false)
                }
                break;
            case 'S':
                switch(buf.data[1]){
                    case 'c': GC_Case(24,"Sc",5,false)
                    case 'k': GC_Case(25,"Sk",5,false)
                    case 'm': GC_Case(26,"Sm",5,false)
                    case 'o': GC_Case(27,"So",5,false)
                }
                break;
            case 'Z':
                switch(buf.data[1]){
                    case 'l': GC_Case(28,"Zl",6,false)
                    case 'p': GC_Case(29,"Zp",6,false)
                    case 's': GC_Case(30,"Zs",6,false)
                }
                break;
        }
        buf.size=0;
        #undef GC_Case
        assert(!ci.empty());
        cc.insert(ci);
        total_cp += ci.size();
        //memset(char_table+lb,cat-1,ci.size());
    } // while(1)

    big_gcs[current_big_gc].cc = big_gcs[current_big_gc].cc + cc;
    big_gcs[current_big_gc].total_cp += total_cp;
    if(current_big_gc_lc){
        big_gcs[7].cc = big_gcs[7].cc + cc;
        big_gcs[7].total_cp += total_cp;
    }
    write_cc(out_h,out_cpp,cat_name,cc,total_cp,YUKI_UGEN_GC_HEADER_THRESHOLD);
    cc.clear();
    write_cc(out_h,out_cpp,"C",big_gcs[0].cc,big_gcs[0].total_cp,YUKI_UGEN_GC_HEADER_THRESHOLD);
    write_cc(out_h,out_cpp,"L",big_gcs[1].cc,big_gcs[1].total_cp,YUKI_UGEN_GC_HEADER_THRESHOLD);
    write_cc(out_h,out_cpp,"M",big_gcs[2].cc,big_gcs[2].total_cp,YUKI_UGEN_GC_HEADER_THRESHOLD);
    write_cc(out_h,out_cpp,"N",big_gcs[3].cc,big_gcs[3].total_cp,YUKI_UGEN_GC_HEADER_THRESHOLD);
    write_cc(out_h,out_cpp,"P",big_gcs[4].cc,big_gcs[4].total_cp,YUKI_UGEN_GC_HEADER_THRESHOLD);
    write_cc(out_h,out_cpp,"S",big_gcs[5].cc,big_gcs[5].total_cp,YUKI_UGEN_GC_HEADER_THRESHOLD);
    write_cc(out_h,out_cpp,"Z",big_gcs[6].cc,big_gcs[6].total_cp,YUKI_UGEN_GC_HEADER_THRESHOLD);
    write_cc(out_h,out_cpp,"LC",big_gcs[7].cc,big_gcs[7].total_cp,YUKI_UGEN_GC_HEADER_THRESHOLD);

    fprintf(out_h,
        "enum struct GCategory : unsigned char {Cc,Cf,Cn,Co,Cs,Ll,Lm,Lo,Lt,Lu,Mc,Me,Mn,Nd,Nl,No,Pc,Pd,Pe,Pf,Pi,Po,Ps,Sc,Sk,Sm,So,Zl,Zp,Zs,C,L,M,N,P,S,Z,LC};\n"
        "\n"
        //"extern const unsigned char general_category[%zu];\n\n"
        "inline constexpr Name_CC_Num<GCategory> gcategory_table[38]={\n"
        IND "{\"Cc\",Cc,0},{\"Cf\",Cf,1},{\"Cn\",Cn,2},{\"Co\",Co,3},{\"Cs\",Cs,4},\n"
        IND "{\"Ll\",Ll,5},{\"Lm\",Lm,6},{\"Lo\",Lo,7},{\"Lt\",Lt,8},{\"Lu\",Lu,9},\n"
        IND "{\"Mc\",Mc,10},{\"Me\",Me,11},{\"Mn\",Mn,12},\n"
        IND "{\"Nd\",Nd,13},{\"Nl\",Nl,14},{\"No\",No,15},\n"
        IND "{\"Pc\",Pc,16},{\"Pd\",Pd,17},{\"Pe\",Pe,18},{\"Pf\",Pf,19},{\"Pi\",Pi,20},{\"Po\",Po,21},{\"Ps\",Ps,22},\n"
        IND "{\"Sc\",Sc,23},{\"Sk\",Sk,24},{\"Sm\",Sm,25},{\"So\",So,26},\n"
        IND "{\"Zl\",Zl,27},{\"Zp\",Zp,28},{\"Zs\",Zs,29},\n"
        IND "{\"C\",C,30},{\"L\",L,31},{\"M\",M,32},{\"N\",N,33},{\"P\",P,34},{\"S\",S,35},{\"Z\",Z,36},{\"LC\",LC,37},\n"
        "};\n"
        YUKI_UGEN_EPILOGUE
        //,static_cast<size_t>(UNICODE_TOTAL)
    );

    fputs(YUKI_UGEN_EPILOGUE,out_cpp);

    fclose(in);
    fclose(out_h);
    fclose(out_cpp);

    //dir_cpp.resize(dir_cpp_size);
    //dir_cpp.append("general_category_table.cpp");
    //FILE* const out_gc_table = fopen(dir_cpp.c_str(),"w");
    //if(!out_gc_table){
    //    fprintf(stderr,"Error: Cpp output \"%s\" cannot be opened!\n",dir_cpp.c_str());
    //    exit(EXIT_FAILURE);
    //}

    //fprintf(out_gc_table,
    //    "namespace yuki{\n"
    //    "extern const unsigned char general_category[%zu]={\n",
    //    static_cast<size_t>(UNICODE_TOTAL)
    //);
    //unsigned line_items=0;
    //for(char32_t i=0;i<UNICODE_TOTAL;++i){
    //    fprintf(out_gc_table,"%u,",char_table[i]);
    //    if(line_items==31){
    //        fputc(static_cast<unsigned char>('\n'),out_gc_table);
    //        line_items=0;
    //    }else
    //        ++line_items;
    //}
    //fprintf(out_gc_table,
    //    "}; // extern const unsigned char general_category[%zu]\n"
    //    "} // namespace yuki\n",
    //    static_cast<size_t>(UNICODE_TOTAL)
    //);
    //fclose(out_gc_table);
} // general_category




#include<yuki/CHashTable_Str.hpp>
#include<yuki/functional.hpp>
struct Name_Alias_Num{
    std::string name;
    std::string alias;
    unsigned number=0;
    bool duplicated=false;

    struct Name{
        template<typename NA>
        static auto& operator()(NA& na) {return na.name;}
        template<typename NA>
        static const auto& operator()(const NA& na) {return na.name;}
    };

    struct Alias{
        template<typename NA>
        static auto& operator()(NA& na) {return na.alias;}
        template<typename NA>
        static const auto& operator()(const NA& na) {return na.alias;}
    };

    struct Nil{
        template<typename NA>
        static bool operator()(const NA& na) {return na.alias.empty();}
    };
};

struct Alias_Num{
    std::string_view alias;
    unsigned number=0;
    bool* duplicated=nullptr;

    constexpr Alias_Num() noexcept = default;

    Alias_Num(Name_Alias_Num& na) noexcept :
        alias(na.alias),
        number(na.number),
        duplicated(&na.duplicated)
    {}

    Alias_Num& operator=(Name_Alias_Num& na) noexcept {
        alias=na.alias;
        number=na.number;
        duplicated=&na.duplicated;
        return *this;
    }
};

struct Alias_Table{
    yuki::CHashTable_Str<Name_Alias_Num,Name_Alias_Num::Name,Name_Alias_Num::Nil,1024> blocks;
    yuki::CHashTable_Str<Name_Alias_Num,Name_Alias_Num::Name,Name_Alias_Num::Nil,1024> scripts;
    yuki::CHashTable_Str<Name_Alias_Num,Name_Alias_Num::Name,Name_Alias_Num::Nil,1024> properties;
    yuki::CHashTable_Str<Alias_Num,Name_Alias_Num::Alias,Name_Alias_Num::Nil,1024> scripts_by_alias;
    yuki::CHashTable_Str<Alias_Num,Name_Alias_Num::Alias,Name_Alias_Num::Nil,1024> blocks_by_alias;

    static constexpr size_t blocks_linear_capacity = 328;
    static constexpr size_t scripts_linear_capacity = 163;
    Alias_Num blocks_linear[blocks_linear_capacity];
    Alias_Num scripts_linear[scripts_linear_capacity];
    unsigned blocks_linear_size = 0;
    unsigned scripts_linear_size = 0;

    Alias_Table(std::string& dir_ucd) noexcept;

    void make_properties(std::string& dir_ucd);
};

Alias_Table::Alias_Table(std::string& dir_ucd) noexcept {
    const size_t dir_ucd_size = dir_ucd.size();
    dir_ucd.append("PropertyValueAliases.txt");
    FILE* const in = fopen(dir_ucd.c_str(),"r");
    if(!in){
        fprintf(stderr,"Error: Input \"%s\" cannot be opened!\n",dir_ucd.c_str());
        exit(EXIT_FAILURE);
    }

    Buf buf1;

  line:
    switch(const int c=fgetc(in);c){
        case EOF:
            end:
            assert(blocks.size()==blocks_linear_size && scripts.size()==scripts_linear_size && blocks_by_alias.size()==blocks_linear_size && scripts_by_alias.size()==scripts_linear_size);
            fclose(in);
            dir_ucd.resize(dir_ucd_size);
            #ifdef YUKI_UGEN_DBG
            fprintf(stderr,"Total blocks: %u\n",blocks_linear_size);
            fprintf(stderr,"Total scripts: %u\n",scripts_linear_size);
            #endif
            make_properties(dir_ucd);
            return;
        case static_cast<unsigned char>('#'):
            while(1){
                switch(const int c=fgetc(in);c){
                    case EOF: goto end;
                    case static_cast<unsigned char>('\n'): goto line;
                }
            }
        case static_cast<unsigned char>('\n'):
        case static_cast<unsigned char>('\r'):
        case static_cast<unsigned char>(' '): goto line;
        case static_cast<unsigned char>('b'): goto lk;
        case static_cast<unsigned char>('s'): goto c;
    }
  lk:
    if(fgetc(in)==static_cast<unsigned char>('l') && fgetc(in)==static_cast<unsigned char>('k')){
        fgetc(in);
        fgetc(in);
        assert(buf.size==0 && buf1.size==0);
        while(1){
            switch(const char c1=static_cast<unsigned char>(fgetc(in));c1){
                case ' ':
                    continue;
                case ';':
                    fgetc(in);
                    while(1){
                        switch(const char c2=static_cast<unsigned char>(fgetc(in));c2){
                            case ';':
                            case ' ':
                                assert(scripts.size()==0);
                                if(buf1.size!=2 || buf1.data[0]!='N' || buf1.data[1]!='B'){
                                    assert(blocks_linear_size<blocks_linear_capacity); // Increase `blocks_linear_capacity` if it fires.
                                    Name_Alias_Num* const inserted = blocks.emplace(Name_Alias_Num{std::string(buf.data,buf.size),std::string(buf1.data,buf1.size),blocks_linear_size+1});
                                    blocks_linear[blocks_linear_size] = *inserted;
                                    blocks_by_alias.emplace(Alias_Num(*inserted));
                                    ++blocks_linear_size;
                                }
                                buf.size=0;
                                buf1.size=0;
                                while(fgetc(in)!=static_cast<unsigned char>('\n'))
                                    ;
                                goto line;
                            case '\r':
                                fgetc(in);
                                [[fallthrough]];
                            case '\n':
                                assert(scripts.size()==0);
                                if(buf1.size!=2 || buf1.data[0]!='N' || buf1.data[1]!='B'){
                                    assert(blocks_linear_size<blocks_linear_capacity); // Increase `blocks_linear_capacity` if it fires.
                                    Name_Alias_Num* const inserted = blocks.emplace(Name_Alias_Num{std::string(buf.data,buf.size),std::string(buf1.data,buf1.size),blocks_linear_size+1});
                                    blocks_linear[blocks_linear_size] = *inserted;
                                    blocks_by_alias.emplace(Alias_Num(*inserted));
                                    ++blocks_linear_size;
                                }
                                buf.size=0;
                                buf1.size=0;
                                goto line;
                            default:
                                assert(isalnum(static_cast<unsigned char>(c2)) || c2=='_');
                                buf.push_back(c2);
                                continue;
                        }
                    }
                default:
                    assert(isalnum(static_cast<unsigned char>(c1)) || c1=='_');
                    buf1.push_back(c1);
                    continue;
            } // switch(const char c1=static_cast<unsigned char>(fgetc(in));c1)
        } // while(1)
    } // if(fgetc(in)==static_cast<unsigned char>('l') && fgetc(in)==static_cast<unsigned char>('k'))
    while(fgetc(in)!=static_cast<unsigned char>('\n'))
        ;
    goto line;
  c:
    if(fgetc(in)==static_cast<unsigned char>('c')){
        fgetc(in);
        fgetc(in);
        assert(buf.size==0 && buf1.size==0);
        while(1){
            switch(const char c1=static_cast<unsigned char>(fgetc(in));c1){
                case ' ':
                    continue;
                case ';':
                    fgetc(in);
                    while(1){
                        switch(const char c2=static_cast<unsigned char>(fgetc(in));c2){
                            case ';':
                            case ' ':{
                                assert(buf1.size==4);
                                // Funnily "PropertyValueAliases.txt" defines the alias "Hrkt" for "Katakana_Or_Hiragana" but this script is not listed in "Scripts.txt". Probably this is a bug.
                                if((buf1.data[0]!='Z' || buf1.data[1]!='z') && (buf1.data[0]!='H' || buf1.data[1]!='r')){
                                    assert(scripts_linear_size<scripts_linear_capacity); // Increase `scripts_linear_capacity` if it fires.
                                    if(Name_Alias_Num* const na_block=const_cast<Name_Alias_Num*>(blocks.find(std::string_view(buf1.data,buf1.size))); na_block){
                                        na_block->duplicated = true;
                                        Name_Alias_Num* const inserted = scripts.emplace(Name_Alias_Num{std::string(buf.data,buf.size),std::string(buf1.data,buf1.size),scripts_linear_size+1,true});
                                        scripts_linear[scripts_linear_size] = *inserted;
                                        scripts_by_alias.emplace(Alias_Num(*inserted));
                                    }else{
                                        Name_Alias_Num* const inserted = scripts.emplace(Name_Alias_Num{std::string(buf.data,buf.size),std::string(buf1.data,buf1.size),scripts_linear_size+1,false});
                                        scripts_linear[scripts_linear_size] =  *inserted;
                                        scripts_by_alias.emplace(Alias_Num(*inserted));
                                    }
                                    ++scripts_linear_size;
                                }
                                buf.size=0;
                                buf1.size=0;
                                while(fgetc(in)!=static_cast<unsigned char>('\n'))
                                    ;
                                goto line;
                            }
                            case '\r':
                                fgetc(in);
                                [[fallthrough]];
                            case '\n':{
                                assert(buf1.size==4);
                                if((buf1.data[0]!='Z' || buf1.data[1]!='z') && (buf1.data[0]!='H' || buf1.data[1]!='r')){
                                    assert(scripts_linear_size<scripts_linear_capacity); // Increase `scripts_linear_capacity` if it fires.
                                    if(Name_Alias_Num* const na_block=const_cast<Name_Alias_Num*>(blocks.find(std::string_view(buf1.data,buf1.size))); na_block){
                                        na_block->duplicated = true;
                                        Name_Alias_Num* const inserted = scripts.emplace(Name_Alias_Num{std::string(buf.data,buf.size),std::string(buf1.data,buf1.size),scripts_linear_size+1,true});
                                        scripts_linear[scripts_linear_size] = *inserted;
                                        scripts_by_alias.emplace(Alias_Num(*inserted));
                                    }else{
                                        Name_Alias_Num* const inserted = scripts.emplace(Name_Alias_Num{std::string(buf.data,buf.size),std::string(buf1.data,buf1.size),scripts_linear_size+1,false});
                                        scripts_linear[scripts_linear_size] =  *inserted;
                                        scripts_by_alias.emplace(Alias_Num(*inserted));
                                    }
                                    ++scripts_linear_size;
                                }
                                buf.size=0;
                                buf1.size=0;
                                goto line;
                            }
                            default:
                                assert(isalnum(static_cast<unsigned char>(c2)) || c2=='_');
                                buf.push_back(c2);
                                continue;
                        }
                    }
                default:
                    assert(isalnum(static_cast<unsigned char>(c1)) || c1=='_');
                    buf1.push_back(c1);
                    continue;
            } // switch(const char c1=static_cast<unsigned char>(fgetc(in));c1)
        } // while(1)
    } // if(fgetc(in)==static_cast<unsigned char>('c'))
    while(fgetc(in)!=static_cast<unsigned char>('\n'))
        ;
    goto line;
} // Alias_Table::Alias_Table(std::string& dir_ucd)




void Alias_Table::make_properties(std::string& dir_ucd){
    assert(buf.size==0);

    Buf buf1;

    dir_ucd.append("PropertyAliases.txt");
    FILE* const in = fopen(dir_ucd.c_str(),"r");
    if(!in){
        fprintf(stderr,"Error: Input \"%s\" cannot be opened!\n",dir_ucd.c_str());
        exit(EXIT_FAILURE);
    }

    cat=0;

  line:
    switch(const int c=fgetc(in);c){
        case EOF:
            end:
            fclose(in);
            #ifdef YUKI_UGEN_DBG
            fprintf(stderr,"Total properties: %zu\n",properties.size());
            #endif
            return;
        case static_cast<unsigned char>('#'):
            while(1){
                switch(const int c=fgetc(in);c){
                    case EOF: goto end;
                    case static_cast<unsigned char>('\n'): goto line;
                }
            }
        case static_cast<unsigned char>('\n'):
        case static_cast<unsigned char>('\r'):
        case static_cast<unsigned char>(' '): goto line;
        default: assert(isalpha(c)); buf.push_back(static_cast<unsigned char>(c)); break;
    }
    while(1){
        switch(const char c=static_cast<unsigned char>(fgetc(in));c){
            case ' ':
                while(fgetc(in)!=static_cast<unsigned char>(';'))
                    ;
                fgetc(in);
                goto second_field;
            case ';':
                fgetc(in);
                goto second_field;
            default:
                assert(isalnum(static_cast<unsigned char>(c)) || c=='_');
                buf.push_back(c);
                continue;
        }
    }
  second_field:
    assert(buf1.size==0);
    while(1){
        switch(const char c=static_cast<unsigned char>(fgetc(in));c){
            case ' ':
            case '#':{
                const std::string alias(buf.data,buf.size);
                bool duplicated = false;
                if(Alias_Num* const na_block=const_cast<Alias_Num*>(blocks_by_alias.find(alias)); na_block){
                    *na_block->duplicated=true;
                    duplicated=true;
                }
                if(Alias_Num* const na_script=const_cast<Alias_Num*>(scripts_by_alias.find(alias)); na_script){
                    *na_script->duplicated=true;
                    duplicated=true;
                }
                properties.emplace(Name_Alias_Num{std::string(buf1.data,buf1.size),std::move(alias),++cat,duplicated});
                buf.size=0;
                buf1.size=0;
                while(1){
                    switch(const int c=fgetc(in);c){
                        case EOF:
                        case static_cast<unsigned char>('\n'): goto line;
                    }
                }
            }
            case '\r':{
                const std::string alias(buf.data,buf.size);
                bool duplicated = false;
                if(Alias_Num* const na_block=const_cast<Alias_Num*>(blocks_by_alias.find(alias)); na_block){
                    *na_block->duplicated=true;
                    duplicated=true;
                }
                if(Alias_Num* const na_script=const_cast<Alias_Num*>(scripts_by_alias.find(alias)); na_script){
                    *na_script->duplicated=true;
                    duplicated=true;
                }
                properties.emplace(Name_Alias_Num{std::string(buf1.data,buf1.size),std::move(alias),++cat,duplicated});
                buf.size=0;
                buf1.size=0;
                fgetc(in);
                goto line;
            }
            case '\n':{
                const std::string alias(buf.data,buf.size);
                bool duplicated = false;
                if(Alias_Num* const na_block=const_cast<Alias_Num*>(blocks_by_alias.find(alias)); na_block){
                    *na_block->duplicated=true;
                    duplicated=true;
                }
                if(Alias_Num* const na_script=const_cast<Alias_Num*>(scripts_by_alias.find(alias)); na_script){
                    *na_script->duplicated=true;
                    duplicated=true;
                }
                properties.emplace(Name_Alias_Num{std::string(buf1.data,buf1.size),std::move(alias),++cat,duplicated});
                buf.size=0;
                buf1.size=0;
                goto line;
            }
            default: assert(isalnum(static_cast<unsigned char>(c)) || c=='_'); buf1.push_back(c);
        }
    } // while(1)
} // Alias_Table::make_properties




#ifndef YUKI_UGEN_MAX_LINE_SMALL_ITEMS
#define YUKI_UGEN_MAX_LINE_SMALL_ITEMS 16
#endif
#ifndef YUKI_UGEN_MAX_LINE_BIG_ITEMS
#define YUKI_UGEN_MAX_LINE_BIG_ITEMS 4
#endif

void write_enum_table(
    FILE* const out_h,
    FILE* const out_cpp,
    const char* const enum_name,
    const char* const table_name,
    const char* const disamb,
    const char* const zero,
    const Alias_Num* const ans,
    const unsigned ans_size,
    const unsigned header_threshold,
    const unsigned enum_line_max,
    const unsigned table_line_max)
{
    fprintf(out_h,
        "enum struct %s : %s {\n"
        IND "%s,",
        enum_name, ans_size+1<256?"unsigned char":"unsigned short",
        zero
    );

    unsigned line_small_items=1;
    for(unsigned i=0;i<ans_size;++i){
        if(line_small_items==0)
            fputs(IND,out_h);
        fprintf(out_h,"%s,",ans[i].alias.data());
        if(line_small_items==enum_line_max-1){
            fputc(static_cast<unsigned char>('\n'),out_h);
            line_small_items=0;
        }else{
            fputc(static_cast<unsigned char>(' '),out_h);
            ++line_small_items;
        }
    }
    fprintf(out_h,"%stotal_\n}; // enum struct %s\n\n\n",line_small_items!=0?"":IND,enum_name);

    line_small_items=1;
    if(ans_size<=header_threshold){
        fprintf(out_h,
            "inline constexpr Name_CC_Num<%s> %s[%u]={\n"
            IND "{\"%s\",%s,0},",
            enum_name,table_name,ans_size+1,
            zero,zero
        );
        for(unsigned i=0;i<ans_size;++i){
            if(line_small_items==0)
                fputs(IND,out_h);
            fprintf(out_h,"{\"%s%s\",%s%s,%u},",ans[i].alias.data(),*(ans[i].duplicated)?disamb:"",ans[i].alias.data(),*(ans[i].duplicated)?disamb:"",ans[i].number);
            if(line_small_items==table_line_max-1){
                fputc(static_cast<unsigned char>('\n'),out_h);
                line_small_items=0;
            }else
                ++line_small_items;
        }
        fprintf(out_h,"%s};\n\n\n",line_small_items!=0?"\n":"");
    }else{
        fprintf(out_h,"extern const Name_CC_Num<%s> %s[%u];\n\n\n",enum_name,table_name,ans_size+1);
        fprintf(out_cpp,
            "extern const Name_CC_Num<%s> %s[%u]={\n"
            IND "{\"%s\",%s,0},",
            enum_name,table_name,ans_size+1,
            zero,zero
        );
        for(unsigned i=0;i<ans_size;++i){
            if(line_small_items==0)
                fputs(IND,out_cpp);
            fprintf(out_cpp,"{\"%s%s\",%s%s,%u},",ans[i].alias.data(),*(ans[i].duplicated)?disamb:"",ans[i].alias.data(),*(ans[i].duplicated)?disamb:"",ans[i].number);
            if(line_small_items==table_line_max-1){
                fputc(static_cast<unsigned char>('\n'),out_cpp);
                line_small_items=0;
            }else
                ++line_small_items;
        }
        fprintf(out_cpp,"%s}; // extern const Name_CC_Num<%s> %s[%u]\n\n\n",line_small_items!=0?"\n":"",enum_name,table_name,ans_size+1);
    }
} // write_enum_table




template<const Alias_Table& alias_table>
void scripts(std::string& dir_ucd,std::string& dir_h,std::string& dir_cpp){
    #ifndef YUKI_UGEN_SCRIPTS_HEADER_THRESHOLD
    #define YUKI_UGEN_SCRIPTS_HEADER_THRESHOLD 173
    #endif

    dir_h.append("scripts.h");
    FILE* const out_h = fopen(dir_h.c_str(),"w");
    if(!out_h){
        fprintf(stderr,"Error: Header output \"%s\" cannot be opened!\n",dir_h.c_str());
        exit(EXIT_FAILURE);
    }

    dir_cpp.append("scripts.cpp");
    FILE* const out_cpp = fopen(dir_cpp.c_str(),"w");
    if(!out_cpp){
        fprintf(stderr,"Error: Cpp output \"%s\" cannot be opened!\n",dir_cpp.c_str());
        exit(EXIT_FAILURE);
    }

    fputs(YUKI_UGEN_HEADER_PROLOGUE,out_h);

    fputs(
        "#include<yuki/unicode/scripts.h>\n"
        "\n"
        "namespace yuki::unicode{\n"
        "\n",
        out_cpp
    );

    assert(alias_table.scripts_linear_size<256);

    struct Scs{
        unsigned char storage[21];
        unsigned char size=0;

        void insert_back(const unsigned char sc){
            assert(size<sizeof(storage)); // Increase `storage`'s extent if it fires.
            assert(size==0 || storage[size-1]<=sc);
            storage[size++]=sc;
        }
        //void insert(const unsigned char sc){
        //    unsigned char* const fg = yuki::first_greater(storage,storage+size,sc);
        //    if(fg==storage || *(fg-1)!=sc){
        //        assert(size<sizeof(storage)); // Increase `storage`'s extent if it fires.
        //        yuki::move_overlap(fg+1,fg,(storage+size)-fg);
        //        *fg=sc;
        //        ++size;
        //    }
        //}
    };

    struct{
        Scs storage[63];
        unsigned size=0;
        unsigned max_scs_size=0;

        void insert_back(const Scs scs){
            auto scs_eq = [](const Scs lhs,const Scs rhs)->bool
            {
                if(lhs.size==rhs.size){
                    for(unsigned i=0;i<lhs.size;++i)
                        if(lhs.storage[i]!=rhs.storage[i])
                            return false;
                    return true;
                }
                return false;
            };
            if(size==0 || !scs_eq(storage[size-1],scs)){
                assert(size < sizeof(storage)/sizeof(Scs)); // Increase `storage`'s extent if it fires.
                storage[size++]=scs;
                if(scs.size>max_scs_size)
                    max_scs_size=scs.size;
            }
        }
    } scss;

    struct{
        struct{
            char32_t c;
            unsigned scs_i;
        } storage[600];
        unsigned size=0;

        void insert(const char32_t c,const char32_t scs_i){
            assert(size < sizeof(storage)/sizeof(storage[0])); // Increase `storage`'s extent if it fires.
            auto* const fg = yuki::first_greater(storage,storage+size,c,[](const auto v){return v.c;},yuki::Less<char32_t>{});
            assert(fg==storage || (fg-1)->c!=c);
            yuki::move_overlap(fg+1,fg,(storage+size)-fg);
            fg->c = c;
            fg->scs_i = scs_i;
            ++size;
        }
    } scs_by_char;

    struct{
        struct{
            unsigned sc;
            yuki::IntegralCIs_OV<char32_t> cc;
        } storage[68];
        unsigned size=0;

        void insert(const unsigned sc,const yuki::CInterval<char32_t> ci){
            auto* const fg = yuki::first_greater(storage,storage+size,sc,[](const auto& v){return v.sc;},yuki::Less<unsigned>{});
            if(fg==storage || (fg-1)->sc!=sc){
                assert(size < sizeof(storage)/sizeof(storage[0]));  // Increase `storage`'s extent if it fires.
                yuki::move_overlap(fg+1,fg,(storage+size)-fg);
                fg->sc = sc;
                fg->cc.insert(ci);
                ++size;
            }else
                (fg-1)->cc.insert(ci);
        }

        const yuki::IntegralCIs_OV<char32_t>* find(const unsigned sc) const {
            const auto* const feg = yuki::first_equiv_greater(storage,storage+size,sc,[](const auto& v){return v.sc;},yuki::Less<unsigned>{});
            return feg!=storage+size && feg->sc==sc ? &feg->cc : nullptr;
        }
    } exts;

    const size_t dir_ucd_orig = dir_ucd.size();
    dir_ucd.append("ScriptExtensions.txt");
    FILE* const in_ext = fopen(dir_ucd.c_str(),"r");
    if(!in_ext){
        fprintf(stderr,"Error: Input \"%s\" cannot be opened!\n",dir_ucd.c_str());
        exit(EXIT_FAILURE);
    }

    while(1){
        const yuki::CInterval<char32_t> ci=parse_line(in_ext);
        if(ci.lb==PARSE_EOF)
            break;

        buf.remove_trailing_spaces();

        Scs current_scs;
        const unsigned buf_e = buf.size;
        for(unsigned i=0; i<buf_e; ){
            unsigned j=i;
            while(j<buf_e && buf.data[j]!=' ')
                ++j;
            const Alias_Num* const an = alias_table.scripts_by_alias.find(std::string_view(buf.data+i,buf.data+j));
            buf.size=0;
            assert(an && an->number!=0);
            current_scs.insert_back(an->number);
            exts.insert(an->number,ci);
            if(j<buf_e)
                ++j;
            i=j;
        }

        scss.insert_back(current_scs);

        for(char32_t c=ci.lb;c<=ci.ub;++c)
            scs_by_char.insert(c,scss.size-1);
    } // while(1)

    #ifdef YUKI_UGEN_DBG
    fprintf(stderr,
        "scss.size=%u\n"
        "scss.max_scs_size=%u\n"
        "scs_by_char.size=%u\n"
        "exts.size=%u\n",
        scss.size,
        scss.max_scs_size,
        scs_by_char.size,
        exts.size
    );
    #endif

    fclose(in_ext);

    dir_ucd.resize(dir_ucd_orig);
    dir_ucd.append("Scripts.txt");
    FILE* const in = fopen(dir_ucd.c_str(),"r");
    if(!in){
        fprintf(stderr,"Error: Input \"%s\" cannot be opened!\n",dir_ucd.c_str());
        exit(EXIT_FAILURE);
    }

    cat=0;
    cat_name="";
    cc.clear();
    total.clear();
    total_cp=0;
    total_total_cp=0;

    memset(char_table,0,sizeof(char_table));
    char_table_max_nonzero=0;

    yuki::IntegralCIs_OV<char32_t> common,inherited;
    size_t common_total_cp=0, inherited_total_cp=0;
    bool current_duplicated=false;

    while(1){
        const yuki::CInterval<char32_t> ci=parse_line(in);
        if(ci.lb==PARSE_EOF)
            break;

        buf.remove_trailing_spaces();

        const Name_Alias_Num* const na = alias_table.scripts.find(std::string_view(buf.data,buf.size));
        buf.size=0;
        assert(na && na->number!=0);
        if(na->number!=cat){
            if(cat!=0){
                total = total+cc;
                total_total_cp+=total_cp;

                if(*cat_name=='Z'){
                    if(cat_name[1]=='y'){
                        common = common+cc;
                        common_total_cp += total_cp;
                    }else if(cat_name[1]=='i'){
                        inherited = inherited+cc;
                        inherited_total_cp += total_cp;
                    }else
                        goto normal;
                }else{
                    normal:
                    for(auto i=cc.begin_number();!i.is_end();++i){
                        assert(char_table[*i]==0);
                        char_table[*i]=cat;
                    }
                    if(const char32_t current_max=cc.max_number(); current_max>char_table_max_nonzero)
                        char_table_max_nonzero = current_max;
                    if(const yuki::IntegralCIs_OV<char32_t>* ext=exts.find(cat); ext){
                        for(const yuki::CInterval<char32_t> ci1 : *ext){
                            cc.insert(ci1);
                            total_cp += ci1.size();
                        }
                    }
                }

                if(current_duplicated){
                    memcpy(buf.data,cat_name,4);
                    memcpy(buf.data+4,"_sc",sizeof("_sc"));
                    write_cc(out_h,out_cpp,buf.data,cc,total_cp,YUKI_UGEN_SCRIPTS_HEADER_THRESHOLD);
                }else
                    write_cc(out_h,out_cpp,cat_name,cc,total_cp,YUKI_UGEN_SCRIPTS_HEADER_THRESHOLD);
            }
            cc.clear();
            total_cp=0;
            cat_name=na->alias.data();
            cat=na->number;
            current_duplicated=na->duplicated;
        }
        cc.insert(ci);
        total_cp+=ci.size();
    } // while(1)

    total = total+cc;
    total_total_cp+=total_cp;
    if(*cat_name=='Z'){
        if(cat_name[1]=='y'){
            common = common+cc;
            common_total_cp += total_cp;
        }else if(cat_name[1]=='i'){
            inherited = inherited+cc;
            inherited_total_cp += total_cp;
        }else
            goto normal_final;
    }else{
        normal_final:
        for(auto i=cc.begin_number();!i.is_end();++i){
            assert(char_table[*i]==0);
            char_table[*i]=cat;
        }
        if(const char32_t current_max=cc.max_number(); current_max>char_table_max_nonzero)
            char_table_max_nonzero = current_max;
        if(const yuki::IntegralCIs_OV<char32_t>* ext=exts.find(cat); ext){
            for(const yuki::CInterval<char32_t> ci1 : *ext){
                cc.insert(ci1);
                total_cp += ci1.size();
            }
        }
    }
    if(current_duplicated){
        memcpy(buf.data,cat_name,4);
        memcpy(buf.data+4,"_sc",sizeof("_sc"));
        write_cc(out_h,out_cpp,buf.data,cc,total_cp,YUKI_UGEN_SCRIPTS_HEADER_THRESHOLD);
    }else
        write_cc(out_h,out_cpp,cat_name,cc,total_cp,YUKI_UGEN_SCRIPTS_HEADER_THRESHOLD);

    write_cc(out_h,out_cpp,"Zzzz",yuki::negate<yuki::CInterval<char32_t>{0,UNICODE_TOTAL-1}>(total),UNICODE_TOTAL-total_total_cp,YUKI_UGEN_SCRIPTS_HEADER_THRESHOLD);

    write_enum_table(out_h,out_cpp,"Script","script_table","_sc","Zzzz",alias_table.scripts_linear,alias_table.scripts_linear_size,YUKI_UGEN_SCRIPTS_HEADER_THRESHOLD,YUKI_UGEN_MAX_LINE_SMALL_ITEMS,YUKI_UGEN_MAX_LINE_ITEMS);

    fprintf(out_h,
        "extern const unsigned char script_explicit[%zu];\n"
        "\n"
        "inline Script script(const char32_t c){\n"
        IND "assert(c<0x%zX);\n"
        IND "if(yuki::contained_in_cintervals(Zyyy,Zyyy+sizeof(Zyyy)/sizeof(Zyyy[0]),c))\n"
        IND IND "return Script::Zyyy;\n"
        IND "else if(yuki::contained_in_cintervals(Zinh,Zinh+sizeof(Zinh)/sizeof(Zinh[0]),c))\n"
        IND IND "return Script::Zinh;\n"
        IND "else if(yuki::contained_in_cintervals(Zzzz,Zzzz+sizeof(Zzzz)/sizeof(Zzzz[0]),c))\n"
        IND IND "return Script::Zzzz;\n"
        IND "else\n"
        IND IND "return static_cast<Script>(script_explicit[c]);\n"
        "}\n\n\n",
        char_table_max_nonzero+1,
        static_cast<size_t>(UNICODE_TOTAL)
    );

    fprintf(out_h,
        "typedef Basic_ScriptExt<Script,%u> ScriptExt;\n"
        "\n"
        "ScriptExt script_ext(const char32_t);\n\n\n",
        scss.max_scs_size
    );
    fprintf(out_cpp,
        "ScriptExt script_ext(const char32_t c){\n"
        IND "assert(c<0x%zX);\n"
        IND "static constexpr ScriptExt::Core core[%u]={\n",
        static_cast<size_t>(UNICODE_TOTAL),
        scss.size
    );
    for(unsigned i=0;i<scss.size;++i){
        fputs(IND IND "{",out_cpp);
        const Scs scs = scss.storage[i];
        for(unsigned j=0;j<scs.size;++j)
            fprintf(out_cpp,"%u,",static_cast<unsigned>(scs.storage[j]));
        fputs("},\n",out_cpp);
    }
    fprintf(out_cpp,
        IND "}; // static constexpr ScriptExt::Core core[%u]\n"
        "\n"
        IND "switch(c){\n",
        scss.size
    );
    for(unsigned i=0;i<scs_by_char.size;++i){
        const auto scsbc = scs_by_char.storage[i];
        if(common.contains(scsbc.c))
            fprintf(out_cpp,IND IND "case 0x%lX: return {core[%u],Script::Zyyy};\n",static_cast<unsigned long>(scsbc.c),scsbc.scs_i);
        else if(inherited.contains(scsbc.c))
            fprintf(out_cpp,IND IND "case 0x%lX: return {core[%u],Script::Zinh};\n",static_cast<unsigned long>(scsbc.c),scsbc.scs_i);
        else{
            assert(scsbc.c<=char_table_max_nonzero);
            assert(char_table[scsbc.c]!=0);
            fprintf(out_cpp,IND IND "case 0x%lX: return {core[%u],%u};\n",static_cast<unsigned long>(scsbc.c),scsbc.scs_i,static_cast<unsigned>(char_table[scsbc.c]));
        }
    }
    fputs(
        IND IND "default: return script(c);\n"
        IND "} // switch(c)\n"
        "} // ScriptExt script_ext(const char32_t c)\n\n\n",
        out_cpp
    );

    fprintf(out_cpp,"extern const unsigned char script_explicit[%zu]={\n",char_table_max_nonzero+1);
    write_char_table(out_cpp);
    fprintf(out_cpp,"}; // extern const unsigned char script_explicit[%zu]\n\n\n",char_table_max_nonzero+1);

    fputs(YUKI_UGEN_EPILOGUE,out_h);
    fputs(YUKI_UGEN_EPILOGUE,out_cpp);

    fclose(in);
    fclose(out_h);
    fclose(out_cpp);
} // scripts




template<const Alias_Table& alias_table>
void blocks(std::string& dir_ucd,std::string& dir_hpp){
    dir_ucd.append("Blocks.txt");
    FILE* const in = fopen(dir_ucd.c_str(),"r");
    if(!in){
        fprintf(stderr,"Error: Input \"%s\" cannot be opened!\n",dir_ucd.c_str());
        exit(EXIT_FAILURE);
    }

    dir_hpp.append("blocks.hpp");
    FILE* const out_hpp = fopen(dir_hpp.c_str(),"w");
    if(!out_hpp){
        fprintf(stderr,"Error: Header output \"%s\" cannot be opened!\n",dir_hpp.c_str());
        exit(EXIT_FAILURE);
    }

    fputs(YUKI_UGEN_HEADER_PROLOGUE,out_hpp);

    cc.clear();
    total_cp=0;

    yuki::CInterval<char32_t> previous_ci = {0,0};

    while(1){
        const yuki::CInterval<char32_t> ci=parse_line(in);
        if(ci.lb==PARSE_EOF)
            break;

        assert(ci.lb>previous_ci.ub || ci.lb==0);
        previous_ci=ci;

        // The Unicode Consortium is just being too sloppy. The names given in "PropertyValueAliases.txt" and "Blocks.txt" are not exactly the same. I really don't see any advantages of doing so.
        for(unsigned i=0;i<buf.size;++i){
            if(buf.data[i]==' ' || buf.data[i]=='-'){
                buf.data[i++]='_';
                assert(i<buf.size);
                buf.data[i]=toupper(static_cast<unsigned char>(buf.data[i]));
            }
        }

        //buf.data[buf.size]=0;
        //fprintf(stderr,"%u %s\n",buf.size,buf.data);
        const Name_Alias_Num* const na = alias_table.blocks.find(std::string_view(buf.data,buf.size));
        buf.size=0;
        assert(na && na->number!=0);
        fprintf(out_hpp,"inline constexpr yuki::CInterval<char32_t> %s%s[1] = {",na->alias.data(),na->duplicated?"_blk":"");
        write_c32(out_hpp,ci.lb);
        fputc(static_cast<unsigned char>(','),out_hpp);
        write_c32(out_hpp,ci.ub);
        fputs("};\n",out_hpp);

        cc.insert(ci);
        total_cp+=ci.size();
    }

    write_cc(out_hpp,nullptr,"NB",yuki::negate<{char32_t(0),char32_t(UNICODE_TOTAL-1)}>(cc),UNICODE_TOTAL-total_cp,-1);

    write_enum_table(out_hpp,nullptr,"Block","block_table","_blk","NB",alias_table.blocks_linear,alias_table.blocks_linear_size,-1,YUKI_UGEN_MAX_LINE_ITEMS,YUKI_UGEN_MAX_LINE_BIG_ITEMS);

    fputs(
        "constexpr Block block(const char32_t c){\n"
        IND "const Name_CC_Num<Block>* const p = yuki::find_in_cintervals(\n"
        IND IND "block_table+1,\n"
        IND IND "block_table+sizeof(block_table)/sizeof(Name_CC_Num<Block>),\n"
        IND IND "c,\n"
        IND IND "[](const Name_CC_Num<Block> nccn){return *nccn.cc;},\n"
        IND IND "yuki::Less<yuki::CInterval<char32_t>>{}\n"
        IND ");\n"
        IND "return p!=block_table+sizeof(block_table)/sizeof(Name_CC_Num<Block>) ? p->num : Block::NB;\n"
        "}\n",
        out_hpp
    );

    fputs(YUKI_UGEN_EPILOGUE,out_hpp);

    fclose(in);
    fclose(out_hpp);
} // blocks




template<const Alias_Table& alias_table>
void binary_properties(std::string& dir_ucd,std::string& dir_h,std::string& dir_cpp){
    #ifndef YUKI_UGEN_BP_HEADER_THRESHOLD
    #define YUKI_UGEN_BP_HEADER_THRESHOLD 157
    #endif

    const size_t dir_ucd_size = dir_ucd.size();
    dir_ucd.append("PropList.txt");
    FILE* in = fopen(dir_ucd.c_str(),"r");
    if(!in){
        fprintf(stderr,"Error: Input \"%s\" cannot be opened!\n",dir_ucd.c_str());
        exit(EXIT_FAILURE);
    }

    dir_h.append("binary_properties.h");
    FILE* const out_h = fopen(dir_h.c_str(),"w");
    if(!out_h){
        fprintf(stderr,"Error: Header output \"%s\" cannot be opened!\n",dir_h.c_str());
        exit(EXIT_FAILURE);
    }

    dir_cpp.append("binary_properties.cpp");
    FILE* const out_cpp = fopen(dir_cpp.c_str(),"w");
    if(!out_cpp){
        fprintf(stderr,"Error: Cpp output \"%s\" cannot be opened!\n",dir_cpp.c_str());
        exit(EXIT_FAILURE);
    }

    fputs(YUKI_UGEN_HEADER_PROLOGUE,out_h);
    fputs(YUKI_UGEN_CPP_SIMPLE_PROLOGUE,out_cpp);

    cc.clear();
    total_cp=0;
    cat=0;
    cat_name="";
    bool current_duplicated=false;

    std::string bp_table_buf;
    bp_table_buf.reserve(4096);
    unsigned bp_table_total=0;

    bool derived=false;

    while(1){
        const yuki::CInterval<char32_t> ci=parse_line(in);
        if(ci.lb==PARSE_EOF){
            if(!derived){
                derived=true;
                fclose(in);
                dir_ucd.resize(dir_ucd_size);
                dir_ucd.append("DerivedCoreProperties.txt");
                in = fopen(dir_ucd.c_str(),"r");
                if(!in){
                    fprintf(stderr,"Error: Input \"%s\" cannot be opened!\n",dir_ucd.c_str());
                    exit(EXIT_FAILURE);
                }
                continue;
            }else
                break;
        }

        buf.remove_trailing_spaces();

        const Name_Alias_Num* const nan = alias_table.properties.find(std::string_view(buf.data,buf.size));
        if(!nan){
            // There is one "Enumerated Property" in "DerivedCoreProperties.txt".
            assert(buf.size>4 && buf.data[0]=='I' && buf.data[1]=='n' && buf.data[2]=='C' && buf.data[3]=='B' && buf.data[4]==';');
            buf.size=0;
            continue;
        }
        assert(nan->number!=0);

        buf.size=0;

        if(nan->number!=cat){
            if(cat!=0){
                if(bp_table_total%YUKI_UGEN_MAX_LINE_ITEMS==0)
                    bp_table_buf.append(IND);

                if(current_duplicated){
                    unsigned i=0;
                    while(cat_name[i]!=0){
                        buf.data[i]=cat_name[i];
                        ++i;
                    }
                    memcpy(buf.data+i,"_bp",sizeof("_bp"));
                    write_cc(out_h,out_cpp,buf.data,cc,total_cp,YUKI_UGEN_BP_HEADER_THRESHOLD);
                    bp_table_buf.append("{\"").append(buf.data).append("\",").append(buf.data).append("},");
                }else{
                    write_cc(out_h,out_cpp,cat_name,cc,total_cp,YUKI_UGEN_BP_HEADER_THRESHOLD);
                    bp_table_buf.append("{\"").append(cat_name).append("\",").append(cat_name).append("},");
                }

                if(bp_table_total%YUKI_UGEN_MAX_LINE_ITEMS==YUKI_UGEN_MAX_LINE_ITEMS-1)
                    bp_table_buf.push_back('\n');
                ++bp_table_total;
            }
            cc.clear();
            total_cp=0;
            cat = nan->number;
            cat_name = nan->alias.c_str();
            current_duplicated = nan->duplicated;
        }

        cc.insert(ci);
        total_cp+=ci.size();
    }

    assert(cat!=0);
    if(bp_table_total%YUKI_UGEN_MAX_LINE_ITEMS==0)
        bp_table_buf.append(IND);
    if(current_duplicated){
        unsigned i=0;
        while(cat_name[i]!=0){
            buf.data[i]=cat_name[i];
            ++i;
        }
        memcpy(buf.data+i,"_bp",sizeof("_bp"));
        write_cc(out_h,out_cpp,buf.data,cc,total_cp,YUKI_UGEN_BP_HEADER_THRESHOLD);
        bp_table_buf.append("{\"").append(buf.data).append("\",").append(buf.data).append("},");
    }else{
        write_cc(out_h,out_cpp,cat_name,cc,total_cp,YUKI_UGEN_BP_HEADER_THRESHOLD);
        bp_table_buf.append("{\"").append(cat_name).append("\",").append(cat_name).append("},");
    }
    if(bp_table_total%YUKI_UGEN_MAX_LINE_ITEMS==YUKI_UGEN_MAX_LINE_ITEMS-1)
        bp_table_buf.push_back('\n');
    ++bp_table_total;

    if(bp_table_total%YUKI_UGEN_MAX_LINE_ITEMS!=0)
        bp_table_buf.push_back('\n');

    fprintf(out_h,
        "inline constexpr Name_CC bproperty_table[%u]={\n%s};\n"
        "\n"
        "\n"
        "template<typename C>\n"
        "constexpr bool is_WSpace(const C c){\n"
        IND "return yuki::contained_in_cintervals(yuki::unicode::WSpace,std::end(yuki::unicode::WSpace),static_cast<char32_t>(c));\n"
        "}\n",
        bp_table_total,bp_table_buf.c_str()
    );

    fputs(YUKI_UGEN_EPILOGUE,out_h);
    fputs(YUKI_UGEN_EPILOGUE,out_cpp);

    fclose(in);
    fclose(out_h);
    fclose(out_cpp);
}




void write_case(std::string& dir_ucd,std::string& dir_cpp){
    const size_t dir_ucd_size = dir_ucd.size();
    dir_ucd.append("UnicodeData.txt");
    FILE* in = fopen(dir_ucd.c_str(),"r");
    if(!in){
        fprintf(stderr,"Error: Input \"%s\" cannot be opened!\n",dir_ucd.c_str());
        exit(EXIT_FAILURE);
    }

    dir_cpp.append("case.cpp");
    FILE* const out_cpp = fopen(dir_cpp.c_str(),"w");
    if(!out_cpp){
        fprintf(stderr,"Error: Cpp output \"%s\" cannot be opened!\n",dir_cpp.c_str());
        exit(EXIT_FAILURE);
    }

    fputs(
        "namespace yuki::unicode{\n"
        "char32_t to_upper(const char32_t c){\n"
        "switch(c){\n"
        "default: return c;\n",
        out_cpp
    );

    struct UL_Pair{
        unsigned long key;
        unsigned long mapped;
    };

    #ifndef YUKI_UGEN_CASE_RESERVE
    #define YUKI_UGEN_CASE_RESERVE 4526
    #endif
    yuki::Vector<UL_Pair> to_lower(yuki::reserve_tag,YUKI_UGEN_CASE_RESERVE), to_title(yuki::reserve_tag,YUKI_UGEN_CASE_RESERVE);

  line:
    {
    assert(buf.size==0);
    while(1){
        switch(const int c=fgetc(in); c){
            case EOF: goto write;
            case static_cast<unsigned char>(';'): goto key_process;
            default: assert(isxdigit(c)); buf.push_back(c); break;
        }
    }
  key_process:
    buf.data[buf.size]=0;
    const unsigned long key = strtoul(buf.data,nullptr,16);
    buf.size=0;

    for(unsigned semicolon_count=1; semicolon_count<12; ){
        const int c = fgetc(in);
        assert(c!=EOF);
        if(c==static_cast<unsigned char>(';'))
            ++semicolon_count;
    }

    unsigned long upper=UNICODE_TOTAL, lower=UNICODE_TOTAL, title=UNICODE_TOTAL;
    while(1){
        if(const char c=static_cast<unsigned char>(fgetc(in)); c!=';')
            buf.push_back(c);
        else
            break;
    }
    if(buf.size!=0){
        buf.data[buf.size]=0;
        upper=strtoul(buf.data,nullptr,16);
        buf.size=0;
    }
    while(1){
        if(const char c=static_cast<unsigned char>(fgetc(in)); c!=';')
            buf.push_back(c);
        else
            break;
    }
    if(buf.size!=0){
        buf.data[buf.size]=0;
        lower=strtoul(buf.data,nullptr,16);
        buf.size=0;
    }
    while(1){
        switch(const int c=fgetc(in); c){
            case EOF:
            case static_cast<unsigned char>('\n'):
                if(buf.size!=0){
                    buf.data[buf.size]=0;
                    title=strtoul(buf.data,nullptr,16);
                    buf.size=0;
                }
                goto case_process;
            case static_cast<unsigned char>('\r'):
                fgetc(in);
                if(buf.size!=0){
                    buf.data[buf.size]=0;
                    title=strtoul(buf.data,nullptr,16);
                    buf.size=0;
                }
                goto case_process;
            default: assert(isxdigit(c)); buf.push_back(c); break;
        }
    }
  case_process:
    if(upper!=UNICODE_TOTAL)
        fprintf(out_cpp,"case 0x%lX: return 0x%lX;\n",key,upper);
    if(lower!=UNICODE_TOTAL)
        to_lower.emplace_back(key,lower);
    if(title!=UNICODE_TOTAL)
        to_title.emplace_back(key,title);
    goto line;
    } // line
  write:
    fputs(
        "} // switch(c)\n"
        "} // char32_t to_upper(const char32_t c)\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "char32_t to_lower(const char32_t c){\n"
        "switch(c){\n"
        "default: return c;\n",
        out_cpp
    );
    for(const UL_Pair ulp : to_lower)
        fprintf(out_cpp,"case 0x%lX: return 0x%lX;\n",ulp.key,ulp.mapped);
    fputs(
        "} // switch(c)\n"
        "} // char32_t to_lower(const char32_t c)\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "char32_t to_title(const char32_t c){\n"
        "switch(c){\n"
        "default: return c;\n",
        out_cpp
    );
    for(const UL_Pair ulp : to_title)
        fprintf(out_cpp,"case 0x%lX: return 0x%lX;\n",ulp.key,ulp.mapped);
    fputs(
        "} // switch(c)\n"
        "} // char32_t to_title(const char32_t c)\n"
        "\n"
        "\n"
        "\n"
        "\n",
        out_cpp
    );


    fclose(in);
    dir_ucd.resize(dir_ucd_size);
    dir_ucd.append("CaseFolding.txt");
    in = fopen(dir_ucd.c_str(),"r");
    if(!in){
        fprintf(stderr,"Error: Input \"%s\" cannot be opened!\n",dir_ucd.c_str());
        exit(EXIT_FAILURE);
    }

    fputs(YUKI_UGEN_EPILOGUE,out_cpp);

    fclose(in);
    fclose(out_cpp);
}




void mirror(std::string& dir_ucd,std::string& dir_hpp){
    dir_ucd.append("BidiMirroring.txt");
    FILE* const in = fopen(dir_ucd.c_str(),"r");
    if(!in){
        fprintf(stderr,"Error: Input \"%s\" cannot be opened!\n",dir_ucd.c_str());
        exit(EXIT_FAILURE);
    }

    dir_hpp.append("mirror.hpp");
    FILE* const out_hpp = fopen(dir_hpp.c_str(),"w");
    if(!out_hpp){
        fprintf(stderr,"Error: Cpp output \"%s\" cannot be opened!\n",dir_hpp.c_str());
        exit(EXIT_FAILURE);
    }

    fputs(
        "#pragma once\n"
        "namespace yuki::unicode{\n"
        "constexpr char32_t mirror(const char32_t c){\n"
        "switch(c){\n"
        "default: return c;\n",
        out_hpp
    );

  line:
    assert(buf.size==0);
    while(1){
        switch(const int c=fgetc(in); c){
            case EOF:
                end:
                fputs(
                    "} // switch(c)\n"
                    "} // constexpr char32_t mirror(const char32_t c)\n"
                    YUKI_UGEN_EPILOGUE,
                    out_hpp
                );
                fclose(in);
                fclose(out_hpp);
                return;
            case static_cast<unsigned char>('#'):
                while(1){
                    switch(const int c=fgetc(in);c){
                        case EOF: goto end;
                        case static_cast<unsigned char>('\n'): goto line;
                    }
                }
            case static_cast<unsigned char>(';'): goto key_process;
            case static_cast<unsigned char>('\n'): goto line;
            case static_cast<unsigned char>('\r'): fgetc(in); goto line;
            default: assert(isxdigit(c)); buf.push_back(c); break;
        }
    }
  key_process:
    buf.data[buf.size]=0;
    const unsigned long key = strtoul(buf.data,nullptr,16);
    buf.size=0;
    fgetc(in);
    while(1){
        switch(const char c=static_cast<unsigned char>(fgetc(in)); c){
            case ' ':
                buf.data[buf.size]=0;
                fprintf(out_hpp,"case 0x%lX: return 0x%lX;\n",key,strtoul(buf.data,nullptr,16));
                buf.size=0;
                while(1){
                    switch(const int c=fgetc(in);c){
                        case EOF:
                        case static_cast<unsigned char>('\n'): goto line;
                    }
                }
            default: assert(isxdigit(c)); buf.push_back(c); break;
        }
    }
    goto line;
}




int main(const int argc,const char*const*const argv){
    if(argc!=4){
        fputs("Error: Wrong number of arguments!\n",stderr);
        fputs("--Note: The first argument is the UCD path. The second is the header out path and the third is the cpp output path.\n",stderr);
        return EXIT_FAILURE;
    }

    std::string dir_ucd=argv[1], dir_h=argv[2], dir_cpp=argv[3];

    switch(dir_ucd.back()){
        case '/': case '\\': break;
        default: dir_ucd.push_back('/'); break;
    }
    switch(dir_h.back()){
        case '/': case '\\': break;
        default: dir_h.push_back('/'); break;
    }
    switch(dir_cpp.back()){
        case '/': case '\\': break;
        default: dir_cpp.push_back('/'); break;
    }

    const size_t dir_ucd_size=dir_ucd.size(), dir_h_size=dir_h.size(), dir_cpp_size=dir_cpp.size();

    general_category(dir_ucd,dir_h,dir_cpp);
    dir_ucd.resize(dir_ucd_size);
    dir_h.resize(dir_h_size);
    dir_cpp.resize(dir_cpp_size);

    static const Alias_Table alias_table(dir_ucd);
    dir_ucd.resize(dir_ucd_size);

    #ifdef YUKI_UGEN_DBG
    {
    const Name_Alias_Num* const cham_blk = alias_table.blocks.find("Cham");
    const Name_Alias_Num* const cham_sc = alias_table.scripts.find("Cham");
    const Alias_Num cham_sc_sv = *alias_table.scripts_by_alias.find("Cham");
    // Cham 41 1
    fprintf(stderr,"%s %u %u\n",cham_blk->alias.c_str(),cham_blk->number,static_cast<unsigned>(cham_blk->duplicated));
    // Cham 22 1
    fprintf(stderr,"%s %u %u\n",cham_sc->alias.c_str(),cham_sc->number,static_cast<unsigned>(cham_sc->duplicated));
    // Cham 22 1
    fprintf(stderr,"%s %u %u\n",cham_sc_sv.alias.data(),cham_sc_sv.number,static_cast<unsigned>(*cham_sc_sv.duplicated));

    const Alias_Num beng_sc_sv = *alias_table.scripts_by_alias.find("Beng");
    // Beng 12 0
    fprintf(stderr,"%s %u %u\n",beng_sc_sv.alias.data(),beng_sc_sv.number,static_cast<unsigned>(*beng_sc_sv.duplicated));

    const Name_Alias_Num* const latn_sc = alias_table.scripts.find("Latin");
    // Latn 70 0
    // Note: "HrKt" is not recognized as an "atomic" script category by this program so there will be a seeming number-dismatch for all script categories after "HrKt".
    fprintf(stderr,"%s %u %u\n",latn_sc->alias.data(),latn_sc->number,static_cast<unsigned>(latn_sc->duplicated));

    // ODI
    fprintf(stderr,"%s\n",alias_table.properties["Other_Default_Ignorable_Code_Point"].alias.c_str());
    // Term
    fprintf(stderr,"%s\n",alias_table.properties["Terminal_Punctuation"].alias.c_str());
    // 1 1
    fprintf(stderr,"%u %u\n",static_cast<unsigned>(alias_table.properties["Variation_Selector"].duplicated),static_cast<unsigned>(alias_table.properties["ID_Continue"].duplicated));
    // 1 1
    fprintf(stderr,"%u %u\n",static_cast<unsigned>(*alias_table.blocks_by_alias["VS"].duplicated),static_cast<unsigned>(*alias_table.blocks_by_alias["IDC"].duplicated));
    }
    #endif

    scripts<alias_table>(dir_ucd,dir_h,dir_cpp);
    dir_ucd.resize(dir_ucd_size);
    dir_h.resize(dir_h_size);
    dir_cpp.resize(dir_cpp_size);

    blocks<alias_table>(dir_ucd,dir_h);
    dir_ucd.resize(dir_ucd_size);
    dir_h.resize(dir_h_size);

    binary_properties<alias_table>(dir_ucd,dir_h,dir_cpp);
    dir_ucd.resize(dir_ucd_size);
    dir_h.resize(dir_h_size);
    dir_cpp.resize(dir_cpp_size);

    write_case(dir_ucd,dir_cpp);
    dir_ucd.resize(dir_ucd_size);
    dir_cpp.resize(dir_cpp_size);

    mirror(dir_ucd,dir_h);
    dir_ucd.resize(dir_ucd_size);
    dir_h.resize(dir_h_size);

    #ifdef YUKI_UGEN_DBG
    fprintf(stderr,"Buf::max_size=%u\n",Buf::max_size);
    #endif
}
