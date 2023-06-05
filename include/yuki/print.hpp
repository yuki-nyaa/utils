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
void print_line(const T& message,FILE* const fp=stdout) {fmt::print(fp,"{}\n",message);}

template<char sep = ' ',typename... Ts>
void print_space(FILE* const fp,const Ts&... messages) {(...,(fmt::print(fp,"{}{}",messages,sep))); fputc(static_cast<unsigned char>('\n'),fp);}

template<typename... Args>
void try_print(FILE* const fp,const std::string_view format,const Args&... args){
    if(fp)
        fmt::vprint(fp,format,fmt::make_format_args(args...));
}

inline int try_fprintf(FILE* const fp,const char* const format){
    if(fp)
        return fprintf(fp,format);
    else
        return 0;
}

// The standard does not say what would happen with `fclose(NULL/nullptr)`. It even does not say it's undefined.
inline int try_fclose(FILE* const fp){
    if(fp)
        return fclose(fp);
    else
        return 0;
}

inline void print_loc(FILE* const fp,const std::string_view filename,const size_t line,const size_t col){
    fmt::print(fp,"{}:{}:{}: ",filename,line,col);
}

template<typename... Args>
void print_error(FILE* const fp,const std::string_view format,const Args&... args){
    fputs("ERROR: ",fp);
    fmt::vprint(fp,format,fmt::make_format_args(args...));
    fflush(fp);
}
template<typename... Args>
void print_warning(FILE* const fp,const std::string_view format,const Args&... args){
    fputs("WARNING: ",fp);
    fmt::vprint(fp,format,fmt::make_format_args(args...));
    fflush(fp);
}
template<typename... Args>
void print_note(FILE* const fp,const std::string_view format,const Args&... args){
    fputs("--NOTE: ",fp);
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
} // namespace yuki
