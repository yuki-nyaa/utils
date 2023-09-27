#pragma once
#include<cstdio>
#include<string_view>
#include<ctime>
#include<fmt/core.h>
#include<fmt/chrono.h>

namespace yuki{
template<typename T,typename CharT>
struct simple_formatter{ // Used only with empty format string.
    static constexpr auto parse(fmt::basic_format_parse_context<CharT>& ctx) -> typename fmt::basic_format_parse_context<CharT>::iterator {return ctx.begin();}
};

template<typename T>
void print_line(const T& message) {fmt::print("{}\n",message);}
template<typename T>
void print_line(FILE* const fp,const T& message) {fmt::print(fp,"{}\n",message);}

template<char sep = ' ',typename... Ts>
void print_space(FILE* const fp,const Ts&... messages) {(...,(fmt::print(fp,"{}{}",messages,sep))); fputc(static_cast<unsigned char>('\n'),fp);}

inline void print_loc(FILE* const fp,const std::string_view filename,const size_t line,const size_t col){
    fmt::print(fp,"{}:{}:{}: ",filename,line,col);
}

template<typename... Args>
void print_error(FILE* const fp,const std::string_view format,const Args&... args){
    fputs("ERROR: ",fp);
    fmt::vprint(fp,format,fmt::make_format_args(args...));
    fflush(fp);
}

inline std::time_t internal_time_;

template<typename... Ts>
void dbgout_base(FILE* const fp, const char* const program_name, const std::string_view format, const Ts&... messages){
    std::time(&internal_time_);
    fmt::print(fp,"YDBG - {} - {:%m-%d %H:%M:%S} - ",program_name,fmt::localtime(internal_time_));
    fmt::vprint(fp,format,fmt::make_format_args(messages...));
    fflush(fp);
}

/// Append `source` to `target`.
inline void concat_file(FILE* const target,FILE* const source){
    unsigned char buf[4096];
    int c;
    fpos_t pos;
    while(1){
        fgetpos(source,&pos);
        if(fread(buf,4096,1,source)==1)
            fwrite(buf,4096,1,target);
        else{
            fsetpos(source,&pos);
            break;
        }
    }
    while((c=fgetc(source))!=EOF)
        fputc(c,target);
}

template<typename It,typename F0,typename F,typename Fend>
void ind_printfixed_newline(It b,const It e,const size_t start,const size_t line_limit,F0&& f0,F&& f,Fend&& fend){
    size_t line_items=start;
    for(;b!=e;++b){
        if(line_items==0)
            std::forward<F0>(f0)();
        std::forward<F>(f)(*b);
        if(line_items==(line_limit-1)){
            std::forward<Fend>(fend)();
            line_items=0;
        }else
            ++line_items;
    }
    if(line_items!=0)
        std::forward<Fend>(fend)();
}
} // namespace yuki
