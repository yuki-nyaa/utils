#pragma once
#include<yuki/Interval.hpp>
#include<string_view>

namespace yuki::unicode{
struct Name_CC{
    std::string_view name;
    const yuki::CInterval<char32_t>* cc=nullptr;
    size_t cc_size=0;

    constexpr Name_CC() noexcept = default;

    constexpr Name_CC(const std::string_view np,const yuki::CInterval<char32_t>* const ccp,const size_t ccsp) noexcept :
        name(np),
        cc(ccp),
        cc_size(ccsp)
    {}

    template<size_t N>
    constexpr Name_CC(const std::string_view np,const yuki::CInterval<char32_t>(&ccp)[N]) noexcept :
        name(np),
        cc(ccp),
        cc_size(N)
    {}

    struct Name{
        template<typename NCC>
        static constexpr std::string_view operator()(const NCC ncc) {return ncc.name;}
    };

    struct Nil{
        template<typename NCC>
        static constexpr bool operator()(const NCC ncc) {return ncc.cc==nullptr;}
    };
};

template<typename E>
struct Name_CC_Num : private Name_CC {
    using Name_CC::name;
    using Name_CC::cc;
    using Name_CC::cc_size;
    E num={};

    constexpr Name_CC_Num() noexcept = default;

    template<size_t N,typename Ep>
    constexpr Name_CC_Num(const std::string_view namep,const yuki::CInterval<char32_t>(&ccp)[N],const Ep nump={}) noexcept :
        Name_CC(namep,ccp),
        num(static_cast<E>(nump))
    {}
};


template<typename E,unsigned char max_scs_size>
struct ScriptExt_Core{
  private:
    E storage[max_scs_size] = {};
    unsigned char size_=0;
  public:
    constexpr ScriptExt_Core() noexcept = default;

    constexpr ScriptExt_Core(std::initializer_list<int> il) noexcept {
        for(const int i : il){
            assert(size_==0 || static_cast<int>(storage[size_-1])<i);
            storage[size_++]=static_cast<E>(i);
        }
    }

    constexpr unsigned char size() const {return size_;}

    constexpr const E* begin() const {return storage;}
    constexpr const E* end() const {return storage+size_;}

    constexpr bool contains(const E e) const {
        return yuki::bcontains(storage,storage+size_,e,yuki::Iden{},[](const E lhs,const E rhs){return std::to_underlying(lhs)<std::to_underlying(rhs);});
    }

    friend constexpr bool operator==(const ScriptExt_Core lhs,const ScriptExt_Core rhs) noexcept {
        if(lhs.size_==rhs.size_){
            for(unsigned char i=0;i<lhs.size_;++i)
                if(lhs.storage[i]!=rhs.storage[i])
                    return false;
            return true;
        }
        return false;
    }
};

template<typename E,unsigned char max_scs_size>
struct Basic_ScriptExt : private ScriptExt_Core<E,max_scs_size> {
  private:
    E primary_={};
  public:
    typedef ScriptExt_Core<E,max_scs_size> Core;

    constexpr Basic_ScriptExt() noexcept = default;

    constexpr Basic_ScriptExt(const E pp) noexcept :
        Core(),
        primary_(pp)
    {}

    constexpr Basic_ScriptExt(const Core core,const E pp) noexcept :
        Core(core),
        primary_(pp)
    {}

    constexpr Basic_ScriptExt(const Core core,const int pp) noexcept :
        Core(core),
        primary_(static_cast<E>(pp))
    {}

    constexpr E primary() const {return primary_;}

    using Core::size;

    using Core::begin;
    using Core::end;

    constexpr bool contains(const E e) const {return e==primary_ || Core::contains(e);}

    friend constexpr bool operator==(const Basic_ScriptExt,const Basic_ScriptExt) noexcept = default;
};
}
