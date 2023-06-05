#pragma once
#include<cassert>
#include<compare>
#include<cstring>
#include<string>
#include<cstdio>
#include<utility>

namespace yuki{

constexpr size_t ystrlen(const char* str){
    assert(str);
    if(!std::is_constant_evaluated())
        return strlen(str);
    else{
        for(size_t i=0;;++i,++str)
            if(*str=='\0')
                return i;
    }
}

constexpr int ystrcmp(const char* lhs,const char* rhs){
    assert(lhs&&rhs);
    if(!std::is_constant_evaluated())
        return strcmp(lhs,rhs);
    else{
        for(;*lhs==*rhs;++lhs,++rhs)
            if(*lhs=='\0')
                return 0;
        return static_cast<unsigned char>(*lhs)-static_cast<unsigned char>(*rhs); // They are to be promoted to `int` anyway so there's no need to worry about overflow.
    }
}




template<typename T>
constexpr char to_char(const T t) {return static_cast<unsigned char>(t);}




inline constexpr char32_t UNICODE_MAX_32 = 0x10FFFF;
inline constexpr char32_t EOF_32 = UNICODE_MAX_32+1;
inline constexpr char32_t EOF_U8_UNDERLYING = 0xF4908080;
inline constexpr char32_t U8CHAR_MAX_REPRESENTABLE_32 = 0x1FFFFF;
inline constexpr char32_t U8CHAR_MAX_REPRESENTABLE_UNDERLYING = 0xF7BFBFBF;




inline constexpr struct U8_Length_Byte_Table{
    unsigned char arr[256];
    constexpr U8_Length_Byte_Table() noexcept :
        arr{}
    {
        for(unsigned i=0;i<0x80;++i)
            arr[i]=1;
        unsigned i=0b11000000;
        for(;i<0b11100000;++i)
            arr[i]=2;
        for(;i<0b11110000;++i)
            arr[i]=3;
        for(;i<0b11111000;++i)
            arr[i]=4;
        for(;i<0b11111100;++i)
            arr[i]=5;
        for(;i<0b11111110;++i)
            arr[i]=6;
        arr[255]=7;
    }
} u8_length_byte_table;

/// @note For invalid utf-8 byte (>=0b11111000) the function returns 0.
constexpr unsigned u8_length_byte(const unsigned char c) {return u8_length_byte_table.arr[c];}


inline constexpr struct U8_Length_C32_Table{
    unsigned char arr[U8CHAR_MAX_REPRESENTABLE_32+1];
    constexpr U8_Length_C32_Table() noexcept :
        arr{}
    {
        char32_t i=0;
        for(;i<0x80;++i)
            arr[i]=(unsigned char)-3;
        for(;i<0x800;++i)
            arr[i]=(unsigned char)-2;
        for(;i<0x10000;++i)
            arr[i]=(unsigned char)-1;
    }
} u8_length_c32_table;

/// @pre c<=U8CHAR_MAX_REPRESENTABLE_32
constexpr unsigned u8_length_c32(const char32_t c) {assert(c<=U8CHAR_MAX_REPRESENTABLE_32); return u8_length_c32_table.arr[c]+4;}


template<typename It>
constexpr unsigned u8_length_byte_reverse(It end){
    if(static_cast<unsigned char>(*--end)<0x80)
        return 1;
    assert(u8_length_byte(*end)==0);
    if(u8_length_byte(*--end)==2)
        return 2;
    assert(u8_length_byte(*end)==0);
    if(u8_length_byte(*--end)==3)
        return 3;
    assert(u8_length_byte(*end)==0);
    if(u8_length_byte(*--end)==4)
        return 4;
    assert(u8_length_byte(*end)==0);
    if(u8_length_byte(*--end)==5)
        return 5;
    assert(u8_length_byte(*end)==0);
    if(u8_length_byte(*--end)==6)
        return 6;
    assert(u8_length_byte(*end)==0);
    if(static_cast<unsigned char>(*--end)==255)
        return 7;
    assert(false);
    std::unreachable();
}


/// @note For little endian utf16 you should pass the second byte, of course.
/// @note No cheking for `c` being a high-surrogate.
constexpr unsigned u16_length_byte(const unsigned char c){
    assert(c<0xDC00 || c>0xE000);
    return (c>=0xD8 && c<0xDC) ? 4 : 2;
}


inline constexpr struct U16_Length_C32_Table{
    unsigned char arr[UNICODE_MAX_32+1];
    constexpr U16_Length_C32_Table() noexcept :
        arr{}
    {
        char32_t i=0;
        for(;i<0xD800;++i)
            arr[i]=(unsigned char)-2;
        for(;i<0xE000;++i)
            arr[i]=(unsigned char)-4;
        for(;i<0x10000;++i)
            arr[i]=(unsigned char)-2;
    }
} u16_length_c32_table;

/// @pre c<=UNICODE_MAX_32
/// @note For uncodable characters (0xD800-0xDFFF) the function returns 0.
constexpr unsigned u16_length_c32(const char32_t c) {assert(c<=UNICODE_MAX_32); return u16_length_c32_table.arr[c]+4;}




constexpr bool is_valid_utf8_bytes(const unsigned char,const unsigned char,const unsigned char,const unsigned char){
    // TODO
    return true;
}

template<typename It>
constexpr bool is_valid_utf8_bytes(It const it){
    typedef std::remove_cvref_t<decltype(*it)> C;
    static_assert(std::is_same_v<C,char> || std::is_same_v<C,unsigned char>);
    // TODO
    return true;
}

constexpr bool is_valid_utf16be_bytes(const unsigned char,const unsigned char,const unsigned char,const unsigned char){
    // TODO
    return true;
}

template<typename It>
constexpr bool is_valid_utf16be_bytes(It const it){
    typedef std::remove_cvref_t<decltype(*it)> C;
    static_assert(std::is_same_v<C,char> || std::is_same_v<C,unsigned char>);
    // TODO
    return true;
}

constexpr bool is_valid_utf16le_bytes(const unsigned char c0,const unsigned char c1,const unsigned char c2,const unsigned char c3){
    return is_valid_utf16be_bytes(c1,c0,c3,c2);
}

template<typename It>
constexpr bool is_valid_utf16le_bytes(It const it){
    typedef std::remove_cvref_t<decltype(*it)> C;
    static_assert(std::is_same_v<C,char> || std::is_same_v<C,unsigned char>);
    // TODO
    return true;
}




struct U8Char{
  private:
    char32_t bytes;
  public:
    U8Char() noexcept = default;

    constexpr U8Char(const char32_t c) noexcept : bytes(0) {operator=(c);}

    constexpr U8Char(const unsigned char c0,const unsigned char c1,const unsigned char c2,const unsigned char c3) noexcept :
        bytes(((char32_t)c0<<24)|((char32_t)c1<<16)|((char32_t)c2<<8)|((char32_t)c3))
    {assert(is_valid_utf8_bytes(c0,c1,c2,c3));}

    template<typename It,typename=decltype(*It{})>
    explicit constexpr U8Char(It const str) noexcept : bytes(0) {set(str);}

    template<typename It,typename=decltype(*It{})>
    constexpr U8Char(It const str,const unsigned l) noexcept : bytes(0) {set(str,l);}

    U8Char(FILE* const in) noexcept : bytes(0) {set(in);}

    static constexpr struct raw_tag_t{} raw_tag = {};
    constexpr U8Char(raw_tag_t,const char32_t c) noexcept :
        bytes(c)
    {
        assert(c<=U8CHAR_MAX_REPRESENTABLE_UNDERLYING);
        assert(is_valid_utf8_bytes((c>>24)&0xFFU,(c>>16)&0xFFU,(c>>8)&0xFFU,c&0xFFU));
    }

    constexpr U8Char& operator=(const char32_t c) noexcept {
        assert(c<=U8CHAR_MAX_REPRESENTABLE_32);
        switch(u8_length_c32_table.arr[c]){
            case (unsigned char)-3: bytes=c; return *this; // if(c<0x80)
            case (unsigned char)-2: bytes=0b11000000'10000000+(c&0b111111U)+((c&0b11111000000U)<<2); return *this; // if(c<0x800)
            case (unsigned char)-1: bytes=0b11100000'10000000'10000000+(c&0b111111U)+((c&0b111111000000U)<<2)+((c&0b1111000000000000U)<<4); return *this; // if(c<0x10000)
            default: bytes=0b11110000'10000000'10000000'10000000+(c&0b111111U)+((c&0b111111000000U)<<2)+((c&0b111111000000000000U)<<4)+((c&0b111000000000000000000U)<<6); return *this;
        }
    }

    constexpr void set(const unsigned char c0,const unsigned char c1,const unsigned char c2,const unsigned char c3){
        assert(is_valid_utf8_bytes(c0,c1,c2,c3));
        bytes=((char32_t)c0<<24)|((char32_t)c1<<16)|((char32_t)c2<<8)|((char32_t)c3);
    }

    template<typename It,typename=decltype(*It{})>
    constexpr It set(It const str,const unsigned l){
        static_assert(std::is_same_v<std::remove_cvref_t<decltype(*str)>,char>||std::is_same_v<std::remove_cvref_t<decltype(*str)>,unsigned char>);
        assert(l==u8_length_byte(*str) || l==0);
        assert(is_valid_utf8_bytes(str));
        switch(l){
            case 0: break;
            case 1: bytes=static_cast<unsigned char>(*str);break;
            case 2: set(0,0,str[0],str[1]);break;
            case 3: set(0,str[0],str[1],str[2]);break;
            case 4: set(str[0],str[1],str[2],str[3]);break;
            default: assert(false); std::unreachable();
        }
        return str+l;
    }

    /// @note This will do the conversion even if `*str==0`, in which case the converted value would be 0 of course.
    template<typename It,typename=decltype(*It{})>
    constexpr It set(It const str) {return set(str,u8_length_byte(*str));}

    unsigned set(FILE* const in){
        if(const int c=fgetc(in); c!=EOF){
            switch(const unsigned u8l=u8_length_byte(c); u8l){
                case 0:
                    assert(false);
                    std::unreachable();
                case 1:
                    bytes=static_cast<unsigned char>(c);
                    return 1;
                default:{
                    unsigned char buf[4] = {static_cast<unsigned char>(c)};
                    const size_t read = fread(buf+1,1,u8l-1,in);
                    assert(read==u8l-1);
                    set(buf,u8l);
                    return u8l;
                }
            }
        }
        bytes=EOF_U8_UNDERLYING;
        return 0;
    }

    explicit constexpr operator char32_t() const noexcept {
        if(bytes<0x100)
            return bytes;
        else if(bytes<0x10000)
            return (bytes&0b111111U)+((bytes&0b11111'00000000U)>>2);
        else if(bytes<0x1000000)
            return (bytes&0b111111U)+((bytes&0b111111'00000000U)>>2)+((bytes&0b1111'00000000'00000000U)>>4);
        else
            return (bytes&0b111111U)+((bytes&0b111111'00000000U)>>2)+((bytes&0b111111'00000000'00000000U)>>4)+((bytes&0b111'00000000'00000000'00000000U)>>6);
    }

    /// Useful for `switch`.
    constexpr char32_t raw() const {return bytes;}

    constexpr unsigned length() const {
        if(bytes<0x100)
            return 1;
        else if(bytes<0x10000)
            return 2;
        else if(bytes<0x1000000)
            return 3;
        else
            return 4;
    }

    constexpr unsigned char* write_to(unsigned char* s) const {
        const unsigned char arr[4] = {
            static_cast<unsigned char>(bytes>>24),
            static_cast<unsigned char>((bytes>>16)&0xFFU),
            static_cast<unsigned char>((bytes>>8)&0xFFU),
            static_cast<unsigned char>(bytes&0xFFU)
        };
        for(unsigned i=0;i<3;++i) // It's 3, NOT 4!!
            if(arr[i]!=0)
                *s++ = arr[i];
        *s++=arr[3];
        return s;
    }

    constexpr char* write_to(char* s) const {
        const char arr[4] = {
            to_char(bytes>>24),
            to_char((bytes>>16)&0xFF),
            to_char((bytes>>8)&0xFF),
            to_char(bytes&0xFF)
        };
        for(unsigned i=0;i<3;++i) // It's 3, NOT 4!!
            if(arr[i]!=0)
                *s++ = arr[i];
        *s++=arr[3];
        return s;
    }

    void write_to(std::string& str) const {
        const char arr[4] = {
            to_char(bytes>>24),
            to_char((bytes>>16)&0xFF),
            to_char((bytes>>8)&0xFF),
            to_char(bytes&0xFF)
        };
        for(unsigned i=0;i<3;++i){ // It's 3, NOT 4!!
            if(arr[i]!=0){
                str.append(arr+i,4-i);
                return;
            }
        }
        str.push_back(arr[3]);
    }

    friend constexpr std::strong_ordering operator<=>(U8Char,U8Char) noexcept = default;
    friend constexpr bool operator==(U8Char,U8Char) noexcept = default;
}; // struct U8Char

static_assert(std::is_trivial_v<U8Char>);
static_assert(std::is_standard_layout_v<U8Char>);

static_assert(U8Char(EOF_32).raw()==EOF_U8_UNDERLYING);
static_assert(U8Char(U8CHAR_MAX_REPRESENTABLE_32).raw()==U8CHAR_MAX_REPRESENTABLE_UNDERLYING);

inline constexpr U8Char UNICODE_MAX_U8 = UNICODE_MAX_32;
inline constexpr U8Char EOF_U8 = EOF_32;


namespace literals{
    constexpr U8Char operator""_u8(const char32_t c) {return U8Char(c);}
    constexpr U8Char operator""_u8(const char c) {return U8Char(static_cast<unsigned char>(c));}
    constexpr U8Char operator""_u8(const unsigned long long c) {return U8Char(c);}

    constexpr U8Char operator""_u8r(const unsigned long long c) {return U8Char(U8Char::raw_tag,c);}

    constexpr unsigned char operator""_uc(const char c) {return c;}
    constexpr unsigned char operator""_uc(const unsigned long long c) {return c;}
}




template<typename It,typename F>
constexpr size_t remove_trailing_u8(const It begin,It end,F&& f){
    while(end!=begin)
        if(const unsigned u8l=u8_length_byte_reverse(end); !std::forward<F>(f)(U8Char(end-=u8l,u8l)))
            return (end+=u8l)-begin;
    return end-begin;
}




enum struct encoding : unsigned char {
    utf8, ///< @note Naturally this also covers plain ASCII
    utf16be,
    utf16le,
    utf32be,
    utf32le,

    latin1,

    custom,

    auto_detect, ///< Reserved for programs that supports auto encoding detection.

    cp437, ///< DOS CP 437
    cp850, ///< DOS CP 850
    cp858, ///< DOS CP 858
    ebcdic,
    cp1250, ///< Windows CP 1250
    cp1251, ///< Windows CP 1251
    cp1252, ///< Windows CP 1252
    cp1253, ///< Windows CP 1253
    cp1254, ///< Windows CP 1254
    cp1255, ///< Windows CP 1255
    cp1256, ///< Windows CP 1256
    cp1257, ///< Windows CP 1257
    cp1258, ///< Windows CP 1258
    iso8859_2, ///< ISO-8859-2, Latin-2
    iso8859_3, ///< ISO-8859-3, Latin-3
    iso8859_4, ///< ISO-8859-4, Latin-4
    iso8859_5, ///< ISO-8859-5, Cyrillic
    iso8859_6, ///< ISO-8859-6, Arabic
    iso8859_7, ///< ISO-8859-7, Greek
    iso8859_8, ///< ISO-8859-8, Hebrew
    iso8859_9, ///< ISO-8859-9, Latin-5
    iso8859_10, ///< ISO-8859-10, Latin-6
    iso8859_11, ///< ISO-8859-11, Thai
    iso8859_13, ///< ISO-8859-13, Latin-7
    iso8859_14, ///< ISO-8859-14, Latin-8
    iso8859_15, ///< ISO-8859-15, Latin-9
    iso8859_16, ///< ISO-8859-16
    macroman, ///< Macintosh Roman with CR to LF translation
    koi8_r,
    koi8_u,
    koi8_ru,
};




// Note: For all single byte conversion functions (i.e. excluding "utf16", "utf32", etc.) that takes a `const char*` as the argument, the conversion will be performed as usual even if the pointed value is 0.

template<encoding>
char32_t to_u32(const unsigned char);

template<encoding enc>
char32_t to_u32(const char* const in) {return to_u32<enc>(static_cast<unsigned char>(*in));}

template<encoding enc>
char32_t to_u32(FILE* const in) {
    const int c = fgetc(in);
    return c!=EOF ? to_u32<enc>(c) : EOF_32;
}

template<encoding enc>
constexpr char32_t to_u32(const unsigned char,const unsigned char,const unsigned char,const unsigned char);


template<>
constexpr char32_t to_u32<encoding::latin1>(const unsigned char c) {return c;}


template<>
constexpr char32_t to_u32<encoding::utf8>(const unsigned char c0,const unsigned char c1,const unsigned char c2,const unsigned char c3){
    assert(is_valid_utf8_bytes(c0,c1,c2,c3));
    return static_cast<char32_t>(U8Char(c0,c1,c2,c3));
}

template<>
constexpr char32_t to_u32<encoding::utf8>(const char* const in) {return static_cast<char32_t>(U8Char(in));}

template<>
inline char32_t to_u32<encoding::utf8>(FILE* const in){
    if(const int c = fgetc(in); c!=EOF){
        switch(const unsigned u8l=u8_length_byte(c); u8l){
            case 0: assert(false); std::unreachable();
            case 1: return static_cast<unsigned char>(c);
            default:{
                unsigned char buf[4] = {static_cast<unsigned char>(c)};
                const size_t read = fread(buf+1,1,u8l-1,in);
                assert(read==u8l-1);
                return static_cast<char32_t>(U8Char(buf,u8l));
            }
        }
    }
    return EOF_32;
}


template<>
constexpr char32_t to_u32<encoding::utf16be>(const unsigned char c0,const unsigned char c1,const unsigned char c2,const unsigned char c3){
    assert(is_valid_utf16be_bytes(c0,c1,c2,c3));
    return c0==0
        ? ((char32_t)c2<<8)+(char32_t)c3
        : ((((char32_t)c0<<8)+(char32_t)c1-0xD800)<<10) + (((char32_t)c2<<8)+(char32_t)c3-0xDC00) + 0x10000;
}

template<>
constexpr char32_t to_u32<encoding::utf16be>(const char* const in){
    switch(u16_length_byte(*in)){
        case 2: return to_u32<encoding::utf16be>(0,0,in[0],in[1]);
        case 4: return to_u32<encoding::utf16be>(in[0],in[1],in[2],in[3]);
        default: assert(false); std::unreachable();
    }
}

template<>
inline char32_t to_u32<encoding::utf16be>(FILE* const in){
    if(const int c = fgetc(in); c!=EOF){
        switch(const unsigned u16l=u16_length_byte(c); u16l){
            case 2:{
                const int c2 = fgetc(in);
                assert(c2!=EOF);
                return ((char32_t)c<<8)+(char32_t)c2;
            }
            case 4:{
                unsigned char buf[4] = {static_cast<unsigned char>(c)};
                const size_t read = fread(buf+1,1,3,in);
                assert(read==3);
                return to_u32<encoding::utf16be>(buf[0],buf[1],buf[2],buf[3]);
            }
            default: assert(false); std::unreachable();
        }
    }
    return EOF_32;
}


template<>
constexpr char32_t to_u32<encoding::utf16le>(const unsigned char c0,const unsigned char c1,const unsigned char c2,const unsigned char c3){
    assert(is_valid_utf16le_bytes(c0,c1,c2,c3));
    return c0==0
        ? ((char32_t)c3<<8)+(char32_t)c2
        : ((((char32_t)c1<<8)+(char32_t)c0-0xD800)<<10) + (((char32_t)c3<<8)+(char32_t)c2-0xDC00) + 0x10000;
}

template<>
constexpr char32_t to_u32<encoding::utf16le>(const char* const in){
    switch(u16_length_byte(in[1])){
        case 2: return to_u32<encoding::utf16le>(0,0,in[0],in[1]);
        case 4: return to_u32<encoding::utf16le>(in[0],in[1],in[2],in[3]);
        default: assert(false); std::unreachable();
    }
}

template<>
inline char32_t to_u32<encoding::utf16le>(FILE* const in){
    unsigned char buf[4] = {};
    if(const size_t read=fread(buf,1,2,in); read==2){
        switch(const unsigned u16l=u16_length_byte(buf[1]); u16l){
            case 2: return ((char32_t)buf[1]<<8)+(char32_t)buf[0];
            case 4:{
                const size_t read = fread(buf+2,1,2,in);
                assert(read==2);
                return to_u32<encoding::utf16le>(buf[0],buf[1],buf[2],buf[3]);
            }
            default: assert(false); std::unreachable();
        }
    }
    return EOF_32;
}


template<>
constexpr char32_t to_u32<encoding::utf32be>(const unsigned char c0,const unsigned char c1,const unsigned char c2,const unsigned char c3){
    return ((char32_t)c0<<24) | ((char32_t)c1<<16) | ((char32_t)c2<<8) | (char32_t)c3;
}

template<>
constexpr char32_t to_u32<encoding::utf32be>(const char* const in){
    return to_u32<encoding::utf32be>(in[0],in[1],in[2],in[3]);
}

template<>
inline char32_t to_u32<encoding::utf32be>(FILE* const in){
    unsigned char buf[4]={};
    const size_t read = fread(buf,1,4,in);
    return read==4 ? to_u32<encoding::utf32be>(buf[0],buf[1],buf[2],buf[3]) : EOF_32;
}


template<>
constexpr char32_t to_u32<encoding::utf32le>(const unsigned char c0,const unsigned char c1,const unsigned char c2,const unsigned char c3){
    return ((char32_t)c3<<24) | ((char32_t)c2<<16) | ((char32_t)c1<<8) | (char32_t)c0;
}

template<>
constexpr char32_t to_u32<encoding::utf32le>(const char* const in){
    return to_u32<encoding::utf32le>(in[0],in[1],in[2],in[3]);
}

template<>
inline char32_t to_u32<encoding::utf32le>(FILE* const in){
    unsigned char buf[4]={};
    const size_t read = fread(buf,1,4,in);
    return read==4 ? to_u32<encoding::utf32le>(buf[0],buf[1],buf[2],buf[3]) : EOF_32;
}


typedef char32_t codepage_t(unsigned char);

inline char32_t to_u32_custom(const unsigned char c,codepage_t* const cp) {return cp(c);}

inline char32_t to_u32_custom(const char* const in,codepage_t* const cp) {return cp(*in);}

inline char32_t to_u32_custom(FILE* const in,codepage_t* const cp){
    const int c = fgetc(in);
    return c!=EOF ? cp(c) : EOF_32;
}
} // namespace yuki
