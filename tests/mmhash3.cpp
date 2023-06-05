#include"../include/yuki/mmhash3.hpp"

#include<string_view>

constexpr bool test(){
    constexpr std::string_view tests[24] = {
    	"uhadrighea",
    	"iuhaeruigh",
    	"qeirugh",
    	"iuqwehgiu",
    	"hwdfiug",
    	"h845gwhr89gh3j6w",
    	"8978erthg",
    	"aes5y$%JU^WRS^&*EJ%YGHIA",
    	"jegAEJ%YJ",
    	"WR(^HJ",
    	"SFJN(",
    	"ERJ^HJKDR^YJHSDI*F",
    	"JB*SEJTHI",
    	"$^JK",
    	"top7djSRTIJAS",
    	"E%TJCA#(ARHKFYO<<>PJKU",
    	"OZ,/TDKJSR56./,YJRTKJOPSJ$%HI",
    	"SCJ%IGJAEFGJ",
    	"ETH87,9\\[p[",
    	"ISRTJHIS",
    	"ROTNJH",
    	"ISEN%GIQ^",
    	"IURT^UIOEMR^Y",
    	"N3564w57E%&IDTK7mrynjwR^KJ",
    };
    for(const std::string_view sv : tests)
        if(yuki::mmhash3<unsigned>(sv.data(),sv.size())!=yuki::mmhash3<unsigned long long>(sv.data(),sv.size()))
            return false;
    return true;
}

static_assert(test());

int main() {}
