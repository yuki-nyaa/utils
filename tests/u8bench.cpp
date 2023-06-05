#include<cstdio>
#include<random>
#include<string>
constexpr unsigned u8_length_byte(unsigned char c){
    if(c<0x80)
        return 1;
    else if(c<0xE0)
        return 2;
    else if(c<0xF0)
        return 3;
    else
        return 4;
}

template<typename T>
constexpr char to_char(T t) {return static_cast<unsigned char>(t);}

struct U8Char1{
  private:
    char32_t bytes;
  public:
    U8Char1() noexcept = default;

    constexpr U8Char1(char32_t c) noexcept : bytes(0) {operator=(c);}

    constexpr U8Char1(char32_t c0,char32_t c1,char32_t c2,char32_t c3) noexcept :
        bytes( (c0<<24) | (c1<<16) | (c2<<8) | c3 )
    {}

    explicit U8Char1(const char* str) noexcept : bytes(0) {set(str);}

    explicit U8Char1(const unsigned char* str) noexcept : bytes(0) {set(str);}

    constexpr U8Char1& operator=(char32_t c) noexcept {
        if(c < 0x80)
            bytes=c;
        else if(c < 0x0800)
            bytes = ((0xC0 | (c >> 6)) << 8) | (0x80 | (c & 0x3F));
        else if(c < 0x010000)
            bytes = ((0xE0 | (c >> 12)) << 16) | ((0x80 | ((c >> 6) & 0x3F)) << 8) | (0x80 | (c & 0x3F));
        else
            bytes = ((0xF0 | (c >> 18)) << 24) | ((0x80 | ((c >> 12) & 0x3F)) << 16) | ((0x80 | ((c >> 6) & 0x3F)) << 8) | (0x80 | (c & 0x3F));
        return *this;
    }

    constexpr void set(char32_t c0,char32_t c1,char32_t c2,char32_t c3) {bytes=(c0<<24) | (c1<<16) | (c2<<8) | c3;}

    void set(const char* str){
        switch(u8_length_byte(*str)){
            case 1 : bytes=static_cast<unsigned char>(*str);
            case 2 : set(0,0,static_cast<unsigned char>(str[0]),static_cast<unsigned char>(str[1]));
            case 3 : set(0,static_cast<unsigned char>(str[0]),static_cast<unsigned char>(str[1]),static_cast<unsigned char>(str[2]));
            case 4 : set(static_cast<unsigned char>(str[0]),static_cast<unsigned char>(str[1]),static_cast<unsigned char>(str[2]),static_cast<unsigned char>(str[3]));
        }
    }

    void set(const unsigned char* str){
        switch(u8_length_byte(*str)){
            case 1 : bytes=*str;
            case 2 : set(0,0,str[0],str[1]);
            case 3 : set(0,str[0],str[1],str[2]);
            case 4 : set(str[0],str[1],str[2],str[3]);
        }
    }

    explicit constexpr operator char32_t() const noexcept {
        if(bytes<0x100)
            return bytes;
        else if(bytes<0x10000)
            return (bytes & 0x3F) | ((bytes & 0x1F00) >> 2);
        else if(bytes<0x1000000)
            return (bytes & 0x3F) | ((bytes & 0x3F00) >> 2) | ((bytes & 0xF0000) >> 4);
        else
            return (bytes & 0x3F) | ((bytes & 0x3F00) >> 2) | ((bytes & 0x3F0000) >> 4) | ((bytes & 0x7000000) >> 6);
    }

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

    template<typename C>
    constexpr C* write_to(C* s) const {
        static_assert(std::is_same_v<C,char> || std::is_same_v<C,unsigned char>);
        char arr[4] = {
            to_char(bytes>>24),
            to_char((bytes>>16)&0xFF),
            to_char((bytes>>8)&0xFF),
            to_char(bytes&0xFF)
        };
        for(unsigned i=0;i<4;++i)
            if(arr[i]!=0)
                *s++ = arr[i];
        return s;
    }

    void write_to(std::string& str){
        char arr[4] = {
            to_char(bytes>>24),
            to_char((bytes>>16)&0xFF),
            to_char((bytes>>8)&0xFF),
            to_char(bytes&0xFF)
        };
        for(unsigned i=0;i<4;++i){
            if(arr[i]!=0){
                str.append(arr+i,4-i);
                return;
            }
        }
    }

    friend constexpr std::strong_ordering operator<=>(U8Char1,U8Char1) noexcept = default;
    friend constexpr bool operator==(U8Char1,U8Char1) noexcept = default;
}; // struct U8Char1

struct U8Char2{
  private:
    char bytes[4];
  public:
    U8Char2() noexcept = default;

    constexpr U8Char2(char32_t c) noexcept : bytes{} {operator=(c);}

    explicit U8Char2(const char* str) noexcept : bytes{} {set(str);}

    explicit U8Char2(const unsigned char* str) noexcept : bytes{} {set(str);}

    constexpr U8Char2& operator=(char32_t c) noexcept {
        if(c < 0x80){
            bytes[0]=0;
            bytes[1]=0;
            bytes[2]=0;
            bytes[3]=to_char(c);
        }else if(c < 0x0800){
            bytes[0]=0;
            bytes[1]=0;
            bytes[2]=to_char(0xC0 | (c >> 6));
            bytes[3]=to_char(0x80 | (c & 0x3F));
        }else if(c < 0x010000){
            bytes[0]=0;
            bytes[1]=to_char(0xE0 | (c >> 12));
            bytes[2]=to_char((0x80 | ((c >> 6) & 0x3F)));
            bytes[3]=to_char(0x80 | (c & 0x3F));
        }else{
            bytes[0]=to_char(0xF0 | (c >> 18));
            bytes[1]=to_char((0x80 | ((c >> 12) & 0x3F)));
            bytes[2]=to_char((0x80 | ((c >> 6) & 0x3F)));
            bytes[3]=to_char(0x80 | (c & 0x3F));
        }
        return *this;
    }

    explicit constexpr operator char32_t() const noexcept {
        if(bytes<0x100)
            return bytes;
        else if(bytes<0x10000)
            return (bytes & 0x3F) | ((bytes & 0x1F00) >> 2);
        else if(bytes<0x1000000)
            return (bytes & 0x3F) | ((bytes & 0x3F00) >> 2) | ((bytes & 0xF0000) >> 4);
        else
            return (bytes & 0x3F) | ((bytes & 0x3F00) >> 2) | ((bytes & 0x3F0000) >> 4) | ((bytes & 0x7000000) >> 6);
    }

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

    template<typename C>
    constexpr C* write_to(C* s) const {
        static_assert(std::is_same_v<C,char> || std::is_same_v<C,unsigned char>);
        unsigned char arr[4] = {
            static_cast<unsigned char>(bytes>>24),
            static_cast<unsigned char>((bytes>>16)&0xFF),
            static_cast<unsigned char>((bytes>>8)&0xFF),
            static_cast<unsigned char>(bytes&0xFF)
        };
        for(unsigned i=0;i<4;++i)
            if(arr[i]!=0)
                *s++ = arr[i];
        return s;
    }

    friend constexpr std::strong_ordering operator<=>(U8Char2,U8Char2) noexcept = default;
    friend constexpr bool operator==(U8Char2,U8Char2) noexcept = default;
}; // struct U8Char1
int main(){
    size_t less = 0;
    size_t equal = 0;
    size_t greater = 0;
    constexpr unsigned arr_total = 10000;
    constexpr unsigned test_total = 10000000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> arr_distrib(0,0x10FFFF);
    std::uniform_int_distribution<unsigned> index_distrib(0,arr_total-1);
    U8Char1 arr[arr_total];
    for(auto i=arr_total;i!=0;--i)
        arr[i-1] = arr_distrib(gen);
    unsigned ia;
    unsigned ib;
    for(auto i = test_total;i!=0;--i){
        ia = index_distrib(gen);
        ib = index_distrib(gen);
        std::strong_ordering result = arr[ia]<=>arr[ib];
        if(result==std::strong_ordering::less)
            ++less;
        else if(result==std::strong_ordering::equal)
            ++equal;
        else if(result==std::strong_ordering::greater)
            ++greater;
    }
    printf("%zu %zu %zu",less,equal,greater);
}