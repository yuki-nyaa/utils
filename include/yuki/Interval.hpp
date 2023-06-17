#pragma once
#include<yuki/algorithm.hpp>
#include<yuki/Ordered_Vector.hpp>
#include<limits>

namespace yuki{
template<typename N>
struct CInterval{
    N lb;
    N ub;

    constexpr bool empty() const {return lb>ub;}

    constexpr bool contains(const N n) const {return n>=lb && n<=ub;}

    /// @note This only makes sense when `N` is an "integer-like" type.
    template<typename D=N>
    constexpr D size() const {return ub>=lb ? D(ub)-D(lb)+D(1) : D(0);}

    friend constexpr bool operator==(CInterval,CInterval) noexcept = default;

    /// @note The comparison operators are only meant to compare non-overlapping intervals.
    friend constexpr bool operator<(const CInterval lhs,const CInterval rhs) {return lhs.lb<rhs.lb;}

    /// @note The comparison operators are only meant to compare non-overlapping intervals.
    friend constexpr bool operator>(const CInterval lhs,const CInterval rhs) {return lhs.ub>rhs.ub;}
};

/// @pre The cintervals should be non-overlapping and should be arranged in the order of either `operator<` or `operator>`.
template<typename It,typename N_arg,typename KV,typename Comp>
constexpr It find_in_cintervals(It first,const It end,const N_arg i,KV&& kv,Comp&& comp){
    typedef decltype(std::forward<KV>(kv)(*first).lb) N;
    It fg = first_greater(first,end,CInterval<N>{i,i},std::forward<KV>(kv),std::forward<Comp>(comp));
    if(fg!=first){
        if(std::forward<KV>(kv)(*--fg).contains(i))
            return fg;
    }
    return end;
}

/// @pre The cintervals should be non-overlapping and should be arranged in the order of either `operator<` or `operator>`.
template<typename It,typename N_arg>
constexpr It find_in_cintervals(It first,const It end,const N_arg i){
    typedef decltype(first->lb) N;
    It fg = first_greater(first,end,CInterval<N>{i,i});
    if(fg!=first){
        if((--fg)->contains(i))
            return fg;
    }
    return end;
}

/// @pre The cintervals should be non-overlapping and should be arranged in the order of either `operator<` or `operator>`.
template<typename It,typename N_arg,typename KV,typename Comp>
constexpr bool contained_in_cintervals(It first,const It end,const N_arg i,KV&& kv,Comp&& comp){
    typedef decltype(std::forward<KV>(kv)(*first).lb) N;
    It fg = first_greater(first,end,CInterval<N>{i,i},std::forward<KV>(kv),std::forward<Comp>(comp));
    if(fg!=first){
        if(std::forward<KV>(kv)(*--fg).contains(i))
            return true;
    }
    return false;
}

/// @pre The cintervals should be non-overlapping and should be arranged in the order of either `operator<` or `operator>`.
template<typename It,typename N_arg>
constexpr bool contained_in_cintervals(It first,const It end,const N_arg i){
    typedef decltype(first->lb) N;
    It fg = first_greater(first,end,CInterval<N>{i,i});
    if(fg!=first){
        if((--fg)->contains(i))
            return true;
    }
    return false;
}

template<typename It,typename KV>
constexpr size_t cintervals_number_count(It first,const It end,KV&& kv){
    size_t count=0;
    for(;first!=end;++first)
        count += std::forward<KV>(kv)(*first).size();
    return count;
}

template<typename It>
constexpr size_t cintervals_number_count(It first,const It end){
    size_t count=0;
    for(;first!=end;++first)
        count += first->size();
    return count;
}

template<typename It>
struct number_iterator{
  private:
    decltype(It{}->lb) n;
    CInterval<decltype(It{}->lb)> ci;
    It i;
    It i_end;
  public:
    number_iterator() noexcept = default;

    constexpr number_iterator(const It ip,const It iep) noexcept :
        i(ip),
        i_end(iep)
    {
        if(i!=i_end){
            ci=*i;
            n=ci.lb;
        }
    }

    constexpr auto operator*() const {return n;}

    constexpr bool is_end() const {return i==i_end;}

    constexpr number_iterator& operator++(){
        ++n;
        if(n>ci.ub){
            ++i;
            if(i!=i_end){
                ci=*i;
                n=ci.lb;
            }
        }
        return *this;
    }

    constexpr number_iterator operator++(int){
        const number_iterator orig = *this;
        operator++();
        return orig;
    }
};




/// Store integers as closed intervals. For simplicity currently the comparator is fixed to be `yuki::Less<CInterval<I>>`.
template<typename I,typename A=yuki::Allocator<CInterval<I>>,typename EC=yuki::Default_EC<2,1,4>>
struct IntegralCIs_OV : private yuki::Ordered_Vector<CInterval<I>,yuki::Less<CInterval<I>>,A,EC> {
  private:
    typedef yuki::Ordered_Vector<CInterval<I>,yuki::Less<CInterval<I>>,A,EC> OV_;
  public:
    using typename OV_::vector_type;
    using typename OV_::key_type;
    using typename OV_::value_type;
    using typename OV_::key_of_value_type;
    using typename OV_::key_compare;
    using typename OV_::allocator_type;
    using typename OV_::ec_type;

    using typename OV_::pointer;
    using typename OV_::const_pointer;
    using typename OV_::size_type;
    using typename OV_::difference_type;

    using OV_::OV_;

    IntegralCIs_OV(const CInterval<I> ci) noexcept :
        OV_(1,ci)
    {}

    IntegralCIs_OV& operator=(const CInterval<I> ci) noexcept {
        OV_::assign(1,ci);
        return *this;
    }

    using OV_::assign;

    friend void swap(IntegralCIs_OV& lhs,IntegralCIs_OV& rhs) noexcept {swap(static_cast<OV_&>(lhs),static_cast<OV_&>(rhs));}

    using OV_::clear;
    using OV_::reset;
    using OV_::empty;
    using OV_::size;
    using OV_::capacity;

    using OV_::vector;
    OV_& ordered_vector() {return static_cast<OV_&>(*this);}
    const OV_& ordered_vector() const {return static_cast<OV_&>(*this);}

    using OV_::front;
    using OV_::back;
    using OV_::operator[];

    using typename OV_::non_const_iterator;
    using typename OV_::const_iterator;
    using OV_::begin;
    using OV_::end;

    typedef number_iterator<const_iterator> const_number_iterator;
    const_number_iterator begin_number() const {return {begin(),end()};}

    using OV_::reserve;
    using OV_::pop_back;
    using OV_::pop_back_v;
    using OV_::recapacity;
    using OV_::shrink_to_fit;

    using OV_::min;
    using OV_::max;
    /// @pre `!empty()`
    I min_number() const {assert(!empty()); return min()->lb;}
    /// @pre `!empty()`
    I max_number() const {assert(!empty()); return max()->ub;}
    using OV_::first_equiv_greater;
    using OV_::first_greater;
    using OV_::last_less;
    using OV_::last_less_equiv;
    const_iterator find(const CInterval<I> ci) const {return OV_::find_any(ci);}
    using OV_::contains;
    using OV_::count;

    size_type total_numbers() const {
        return cintervals_number_count(begin(),end());
    }

    const_iterator find(const I n) const {
        return yuki::find_in_cintervals(begin(),end(),n);
    }

    bool contains(const I n) const {
        return yuki::contained_in_cintervals(begin(),end(),n);
    }

    yuki::IB_Pair<const_iterator> insert(const I n){
        const typename vector_type::iterator b = vector_type::begin();
        using yuki::const_kast;
        const typename vector_type::iterator fg = const_kast(first_greater({n,n}));

        if(fg==b){
            if(b!=end() && b->lb==n+1){ // `n+1` will not overflow, since `fg->lb > n` by definition of `fg`.
                --b->lb;
                return {b,true};
            }else
                return {vector_type::emplace(b,n,n),true};
        }else{
            const auto prev = fg-1;
            if(prev->ub<n){
                unsigned flag=0;
                if(prev->ub+1==n) // `prev->ub+1` will not overflow since `prev->ub<n`.
                    flag |= 0b10U;
                if(fg!=end() && fg->lb==n+1) // `n+1` will not overflow, since `fg->lb > n` by definition of `fg`.
                    flag |= 1U;
                switch(flag){
                    case 0:
                        return {vector_type::emplace(fg,n,n),true};
                    case 1:
                        --fg->lb; // Will not overflow.
                        return {fg,true};
                    case 2:
                        ++prev->ub; // Will not overflow.
                        return {prev,true};
                    case 3:
                        return {vector_type::erase_then_emplace(prev,2,CInterval<I>{prev->lb,fg->ub}),true};
                    default: assert(false); std::unreachable();
                }
            }
            return {prev,false};
        }
    }

    /// @pre `!ci.empty()`
    yuki::IB_Pair<const_iterator> insert(const CInterval<I> ci){
        assert(!ci.empty());

        if(ci.lb==ci.ub)
            return insert(ci.lb);

        const const_iterator b=begin(), e=end();
        const const_iterator left_fg = first_greater({ci.lb,ci.lb});
        const const_iterator right_fg = first_greater({ci.ub,ci.ub});

        const_iterator left=e, right=e;

        if(left_fg!=b && (left_fg-1)->ub >= ci.lb)
            left = left_fg-1;

        if(right_fg!=b && (right_fg-1)->ub >= ci.ub)
            right = right_fg-1;

        CInterval<I> ci_new;

        if(left!=e){
            if(right!=e){ // 1
                if(left!=right)
                    ci_new={left->lb,right->ub}, ++right;
                else // Same as the previous branch, actually.
                    return {left,false};
            }else{ // 2
                ci_new={left->lb,ci.ub}, right=right_fg;
                if(right!=e){
                    if(ci_new.ub+1==right->lb) // `ci_new.ub+1` i.e. `ci.ub+1` will not overflow, since `right_fg->lb > ci.ub` by definition.
                        ci_new.ub=right->ub, ++right;
                }
            }
        }else{
            if(right!=e){ // 3 (2 with left and right reversed)
                ci_new={ci.lb,right->ub}, left=left_fg, ++right;
                if(left!=b){
                    if((left-1)->ub+1 == ci_new.lb) // `(left(_fg)-1)->ub+1` will not overflow. By definition we have `left==e` iff. `left_fg==b || (left_fg-1)->ub < ci.lb`, but since we already have `left(_fg)!=b` then it must be the case that `(left_fg-1)->ub < ci.lb`.
                        --left, ci_new.lb=left->lb;
                }
            }else{ // 4
                ci_new=ci;
                left=left_fg, right=right_fg;
                if(right!=e){
                    if(ci_new.ub+1==right->lb) // Will not overflow as in case 2.
                        ci_new.ub=right->ub, ++right;
                }
                if(left!=b){
                    if((left-1)->ub+1 == ci_new.lb) // Will not overflow as in case 3.
                        --left, ci_new.lb=left->lb;
                }
            }
        }
        return {vector_type::erase_then_emplace(left,right-left,ci_new),true};
    } // void insert(const C32_Interval ci)

    friend IntegralCIs_OV operator*<I,A,EC>(const IntegralCIs_OV&,const IntegralCIs_OV&);
    friend IntegralCIs_OV operator+<I,A,EC>(const IntegralCIs_OV&,const IntegralCIs_OV&);
    friend IntegralCIs_OV operator-<I,A,EC>(const IntegralCIs_OV&,const IntegralCIs_OV&);
    friend IntegralCIs_OV operator!<I,A,EC>(const IntegralCIs_OV&);

    template<CInterval universe,typename I2,typename A2,typename EC2>
    friend IntegralCIs_OV<I2,A2,EC2> negate(const IntegralCIs_OV<I2,A2,EC2>&);

    template<typename It,typename IsUniverse=yuki::False>
    void clear_and_merge(It,size_t,IsUniverse&&=yuki::False{});
}; // struct IntegralCIs_OV


// A*(B0+...+Bm) = (A*B0)+...+(A*Bm)
template<typename I,typename A,typename EC>
IntegralCIs_OV<I,A,EC> operator*(const IntegralCIs_OV<I,A,EC>&lhs_p,const IntegralCIs_OV<I,A,EC>&rhs_p){
    typedef typename IntegralCIs_OV<I,A,EC>::vector_type::iterator iterator;
    typedef typename IntegralCIs_OV<I,A,EC>::const_iterator const_iterator;

    const IntegralCIs_OV<I,A,EC>* lhs = &lhs_p;
    const IntegralCIs_OV<I,A,EC>* rhs = &rhs_p;
    if(lhs_p.size()<rhs_p.size())
        lhs=&rhs_p, rhs=&lhs_p;

    if(rhs->empty())
        return {};

    auto append_back = [](IntegralCIs_OV<I,A,EC>& cc_p,const const_iterator l,const const_iterator r) -> iterator {
        const typename IntegralCIs_OV<I,A,EC>::size_type s = r-l;
        assert(cc_p.capacity()>=cc_p.size()+s);
        const iterator e = cc_p.vector_type::end();
        yuki::uninitialized_copy_no_overlap(e,l,s);
        cc_p.force_resize(cc_p.size()+s);
        return e;
    };

    IntegralCIs_OV<I,A,EC> cc(yuki::reserve_tag,lhs->size()+rhs->size()-1);

    for(const CInterval<I> ci : *rhs){
        const const_iterator b = lhs->begin();
        const const_iterator e = lhs->end();
        const const_iterator left_fg = lhs->first_greater({ci.lb,ci.lb});
        const const_iterator right_fg = lhs->first_greater({ci.ub,ci.ub});

        const_iterator left = e;
        const_iterator right = e;

        if(left_fg!=b && (left_fg-1)->ub >= ci.lb)
            left = left_fg-1;

        if(right_fg!=b && (right_fg-1)->ub >= ci.ub)
            right = right_fg-1;

        // `left==e` iff. everything in `lhs` is greater than `ci.lb`, or the previous interval of `left_fg` does not contain `ci.lb`.
        // Otherwise `left` denotes the interval that contains `ci.lb`.
        // Ditto for `right`.

        if(left!=e){
            if(right!=e){ // 1
                if(left==right){
                    assert(cc.size()!=cc.capacity());
                    cc.vector_type::emplace_back(ci);
                }else{ // Same as the previous branch, actually.
                    ++right;
                    append_back(cc,left,right)->lb = ci.lb;
                    cc.vector_type::back().ub = ci.ub;
                }
            }else{ // 2
                right = right_fg;
                append_back(cc,left,right)->lb = ci.lb;
            }
        }else{
            if(right!=e){ // 3 (2 with left and right reversed)
                left=left_fg, ++right;
                append_back(cc,left,right);
                cc.vector_type::back().ub = ci.ub;
            }else{ // 4
                left=left_fg, right=right_fg;
                if(left!=right){ // Same without this `if`, actually. `append_back` will do nothing if `left==right`.
                    append_back(cc,left,right);
                }
                // else do nothing
            }
        }
    } // for(const CInterval<I> ci : *rhs)
    return cc;
} // operator*


template<typename I,typename A,typename EC>
IntegralCIs_OV<I,A,EC> operator+(const IntegralCIs_OV<I,A,EC>& lhs,const IntegralCIs_OV<I,A,EC>& rhs){
    typedef typename IntegralCIs_OV<I,A,EC>::const_iterator const_iterator;

    IntegralCIs_OV<I,A,EC> cc(yuki::reserve_tag,lhs.size()+rhs.size());

    const_iterator l = lhs.begin();
    const_iterator r = rhs.begin();
    const const_iterator le = lhs.end();
    const const_iterator re = rhs.end();

    CInterval<I> ci;

    while(1){
        if(l==le){
            if(r==re)
                return cc;
            else
               ci = *r++;
        }else{
            if(r==re)
                ci = *l++;
            else{
                if(r->lb < l->lb)
                    ci = *r++;
                else
                    ci = *l++;
            }
        }

        if(!cc.empty()){
            CInterval<I>& cib = cc.vector_type::back();
            if(ci.ub>cib.ub){
                if(ci.lb<=cib.ub+1) // Obviously this will not overflow.
                    cib.ub=ci.ub;
                else
                    cc.vector_type::emplace_back(ci);
            }
        }else
            cc.vector_type::emplace_back(ci);
    }
} // operator+


// (A0+...+An)-B = (A0-B)+...+(An-B)
template<typename I,typename A,typename EC>
IntegralCIs_OV<I,A,EC> operator-(const IntegralCIs_OV<I,A,EC>& lhs,const IntegralCIs_OV<I,A,EC>& rhs){
    typedef typename IntegralCIs_OV<I,A,EC>::const_iterator const_iterator;

    IntegralCIs_OV<I,A,EC> cc(yuki::reserve_tag,lhs.size()+rhs.size());

    for(const CInterval<I> ci : lhs){
        const const_iterator b = rhs.begin();
        const const_iterator e = rhs.end();
        const const_iterator left_fg = rhs.first_greater({ci.lb,ci.lb});
        const const_iterator right_fg = rhs.first_greater({ci.ub,ci.ub});

        const_iterator left = e;
        const_iterator right = e;

        if(left_fg!=b && (left_fg-1)->ub >= ci.lb)
            left = left_fg-1;

        if(right_fg!=b && (right_fg-1)->ub >= ci.ub)
            right = right_fg-1;

        // `left==e` iff. everything in `lhs` is greater than `ci.lb`, or the previous interval of `left_fg` does not contain `ci.lb`.
        // Otherwise `left` denotes the interval that contains `ci.lb`.
        // Ditto for `right`.

        if(left!=e){
            if(right!=e){ // 1
                assert(cc.capacity()>=cc.size()+(right-left));
                for(const_iterator i=left+1; left!=right; ++left,++i)
                    cc.vector_type::emplace_back(CInterval<I>{left->ub+1,i->lb-1}); // By definition we have `i->lb > left->ub` so both expressions will not overflow. Since all insertion methods merge adjacent intervals whenever possible, this interval can never be empty.
            }else{ // 2
                right = right_fg;
                assert(cc.capacity()>=cc.size()+(right-left));
                for(const_iterator i=left+1; left!=right; ++left,++i)
                    cc.vector_type::emplace_back(CInterval<I>{left->ub+1,i->lb-1}); // Will not overflow as in case 1.
                cc.vector_type::back().ub = ci.ub;
            }
        }else{
            if(right!=e){ // 3 (2 with left and right reversed)
                left=left_fg;
                assert(cc.capacity()>=cc.size()+(right-left+1));
                cc.vector_type::emplace_back(CInterval<I>{ci.lb,left->lb-1}); // It's trivial to see that `left->lb-1` will not overflow since `ci.lb < left(_fg)->lb` by definition. What's not so obvious is that `left(_fg)` can be safely dereferenced. Because all insertion methods of `IntegralCIs_OV` do not accept an empty interval, we have `left_fg<=right_fg`. So it suffices to show that `left_fg==right_fg` implies `right_fg!=e`. Now suppose otherwise that `left_fg==right_fg==e`. There could be several possiblities but it's impossible to make `left==e && right!=e` in this case, given that `ci` is not an empty interval.
                for(const_iterator i=left+1; left!=right; ++left,++i)
                    cc.vector_type::emplace_back(CInterval<I>{left->ub+1,i->lb-1}); // Will not overflow as in case 1.
            }else{ // 4
                left=left_fg, right=right_fg;
                assert(cc.capacity()>=cc.size()+(right-left+1));
                if(left!=right){
                    cc.vector_type::emplace_back(CInterval<I>{ci.lb,left->lb-1});
                    for(const_iterator i=left+1; left!=right; ++left,++i)
                        cc.vector_type::emplace_back(CInterval<I>{left->ub+1,i->lb-1}); // Will not overflow as in case 1.
                    cc.vector_type::back().ub = ci.ub;
                }else
                    cc.vector_type::emplace_back(ci);
            }
        }
    } // for(const CInterval<I> ci : lhs)
    return cc;
} // operator-


/// @pre `iciov` should be a subset of `universe`.
template<CInterval universe,typename I,typename A,typename EC>
IntegralCIs_OV<I,A,EC> negate(const IntegralCIs_OV<I,A,EC>& iciov){
    if(iciov.empty()) // This is necessary, not just an optimization.
        return IntegralCIs_OV<I,A,EC>(CInterval<I>{universe.lb,universe.ub});

    typedef typename IntegralCIs_OV<I,A,EC>::const_iterator const_iterator;

    IntegralCIs_OV<I,A,EC> cc(yuki::reserve_tag,iciov.size()+1);

    const_iterator i1 = iciov.begin();
    const const_iterator e = iciov.end();

    assert(i1->lb >= universe.lb);
    if(i1->lb > universe.lb)
        cc.vector_type::emplace_back(CInterval<I>{0,(i1->lb)-1}); // Obviously this will not overflow.

    for(const_iterator i2=i1+1; i2!=e; ++i1,++i2)
        cc.vector_type::emplace_back(CInterval<I>{i1->ub+1,i2->lb-1}); // Obviously this will not overflow.

    assert(i1->ub <= universe.ub);
    if(i1->ub < universe.ub)
        cc.vector_type::emplace_back(CInterval<I>{(i1->ub)+1,universe.ub}); // Obviously this will not overflow.
    return cc;
}


template<typename I,typename A,typename EC>
IntegralCIs_OV<I,A,EC> operator!(const IntegralCIs_OV<I,A,EC>& iciov){
    return negate<std::numeric_limits<I>::min(),std::numeric_limits<I>::max()>(iciov);
}


template<typename I,typename A,typename EC>
template<typename It,typename IsUniverse>
void IntegralCIs_OV<I,A,EC>::clear_and_merge(It it,const size_t count,IsUniverse&& is_universe){
    static yuki::Vector<const_iterator> is(yuki::reserve_tag,64), es(yuki::reserve_tag,64);

    clear();
    is.clear();
    es.clear();

    for(size_t i=0;i<count;++i,++it){
        is.emplace_back(it->begin());
        es.emplace_back(it->end());
    }

    const_iterator* i_min = nullptr;
    CInterval<I> ci;
    bool ci_set = false;

    while(!std::forward<IsUniverse>(is_universe)(*this)){
        ci_set = false;
        for(size_t i=0;i<count;++i){
            if(is[i]!=es[i] && (!ci_set || is[i]->lb < ci.lb)){
                ci_set = true;
                i_min = &(is[i]);
                ci = **i_min;
            }
        }

        if(!ci_set)
            return;

        ++*i_min;

        if(!empty()){
            CInterval<I>& cib = vector_type::back();
            if(ci.ub>cib.ub){
                if(ci.lb<=cib.ub+1) // Obviously this will not overflow.
                    cib.ub=ci.ub;
                else
                    vector_type::emplace_back(ci);
            }
        }else
            vector_type::emplace_back(ci);
    }
} // void IntegralCIs_OV<I,A,EC>::clear_and_merge(It it,const size_t count)
} // namespace yuki




namespace yuki{
inline constexpr yuki::CInterval<char32_t> cc_dot_ascii[] = {{0,U'\n'-1},{U'\n'+1,U'\r'-1},{U'\r'+1,127}};
inline constexpr yuki::CInterval<char32_t> cc_dot_byte[] = {{0,U'\n'-1},{U'\n'+1,U'\r'-1},{U'\r'+1,255}};
inline constexpr yuki::CInterval<char32_t> cc_all_byte[] = {{0,255}};
inline constexpr yuki::CInterval<char32_t> cc_newline[] = {{U'\n',U'\n'},{U'\r',U'\r'}};
// POSIX catetories
inline constexpr yuki::CInterval<char32_t> cc_ascii[] = {{0,127}};
inline constexpr yuki::CInterval<char32_t> cc_space[] = {{U'\t',U'\r'},{U' ',U' '}};
inline constexpr yuki::CInterval<char32_t> cc_xdigit[] = {{U'0',U'9'},{U'A',U'F'},{U'a',U'f'}};
inline constexpr yuki::CInterval<char32_t> cc_cntrl[] = {{0,0x1F},{127,127}};
inline constexpr yuki::CInterval<char32_t> cc_print[] = {{0x20,0x7E}};
inline constexpr yuki::CInterval<char32_t> cc_alnum[] = {{U'0',U'9'},{U'A',U'Z'},{U'a',U'z'}};
inline constexpr yuki::CInterval<char32_t> cc_alpha[] = {{U'A',U'Z'},{U'a',U'z'}};
inline constexpr yuki::CInterval<char32_t> cc_blank[] = {{U'\t',U'\t'},{U' ',U' '}};
inline constexpr yuki::CInterval<char32_t> cc_digit[] = {{U'0',U'9'}};
inline constexpr yuki::CInterval<char32_t> cc_graph[] = {{0x21,0x7E}};
inline constexpr yuki::CInterval<char32_t> cc_lower[] = {{U'a',U'z'}};
inline constexpr yuki::CInterval<char32_t> cc_punct[] = {{0x21,0x2F},{0x3A,0x40},{0x5B,0x60},{0x7B,0x7E}};
inline constexpr yuki::CInterval<char32_t> cc_upper[] = {{U'A',U'Z'}};
inline constexpr yuki::CInterval<char32_t> cc_word[] = {{U'0',U'9'},{U'A',U'Z'},{U'_',U'_'},{U'a',U'z'}};
namespace unicode{
    inline constexpr yuki::CInterval<char32_t> All[1] = {{0,0x10FFFF}};
    inline constexpr yuki::CInterval<char32_t> Dot[3] = {{0,U'\n'-1},{U'\n'+1,U'\r'-1},{U'\r'+1,0x10FFFF}};
}
}
