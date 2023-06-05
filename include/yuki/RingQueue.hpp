#pragma once
#include<type_traits>
#include<iterator>
#include<yuki/Allocator.hpp>
#include<yuki/functional.hpp>

namespace yuki{

template<typename RQ,typename P>
struct RQ_Iterator_;

template<typename T,typename A=yuki::Allocator<T>,typename EC=yuki::Default_EC<2,1,4>>
struct RingQueue : private A, private EC{
    static_assert(std::is_same_v<T,typename A::value_type>);
    using typename A::value_type;
    using typename A::pointer;
    using typename A::const_pointer;
    using typename A::size_type;
    using typename A::difference_type;

    constexpr RingQueue() noexcept = default;

  private:
    void destroy_(){
        if constexpr(!std::is_trivially_destructible_v<T>){
            if(b){
                if(b<e)
                    yuki::destroy(b,e);
                else{
                    yuki::destroy(b,B+cap);
                    yuki::destroy(B,e);
                }
            }
        }
    }

    template<typename RQ>
    void copy_other_(RQ&& other){
        if(other.b<other.e){
            cap=other.e-other.b;
            e=b=B=A::allocate(cap);
            if constexpr(!std::is_rvalue_reference_v<RQ&&>)
                yuki::uninitialized_copy_no_overlap(B,other.b,cap);
            else
               yuki::uninitialized_move_no_overlap(B,other.b,cap);
        }else{
            cap = other.cap-(other.b-other.e);
            e=b=B=A::allocate(cap);
            const size_type seg1 = (other.B+other.cap)-other.b;
            if constexpr(!std::is_rvalue_reference_v<RQ&&>){
                yuki::uninitialized_copy_no_overlap(B,other.b,seg1);
                yuki::uninitialized_copy_no_overlap(B+seg1,other.B,other.e-other.B);
            }else{
                yuki::uninitialized_move_no_overlap(B,other.b,seg1);
                yuki::uninitialized_move_no_overlap(B+seg1,other.B,other.e-other.B);
            }
        }
    }

  public:
    RingQueue(const RingQueue& other) noexcept:
        A(yuki::select_on_container_copy_construction<A>(other)),
        EC(yuki::select_on_container_copy_construction<EC>(other)),
        B(nullptr),
        b(nullptr),
        e(nullptr),
        cap(0)
    {if(other.b) copy_other_(other);}

    constexpr RingQueue(RingQueue&& other) noexcept:
        A(static_cast<A&&>(*this)),
        EC(static_cast<EC&&>(*this)),
        B(other.B),
        b(other.b),
        e(other.e),
        cap(other.cap)
    {
        other.B = nullptr;
        other.b = nullptr;
        other.e = nullptr;
        other.cap = 0;
    }

    RingQueue& operator=(const RingQueue& other) noexcept {
        if(this!=&other){
            EC::operator=(other);
            destroy_();
            if constexpr(A::propagate_on_container_copy_assignment){
                A::deallocate(B,cap);
                yuki::clear_alloc<A>(*this);
                A::operator=(other);
                if(other.b)
                    copy_other_(other);
                else{
                    b=nullptr;
                    e=B;
                }
            }else{
                if(other.b){
                    if(other.b<other.e){
                        const size_type other_size = other.e-other.b;
                        if(cap<other_size){
                            A::deallocate(B,cap);
                            yuki::clear_alloc<A>(*this);
                            cap=other_size;
                            B=A::allocate(other_size);
                        }
                        b=B;
                        e=B+other_size;
                        yuki::uninitialized_copy_no_overlap(B,other.b,other_size);
                    }else{
                        const size_type other_size = other.cap-(other.b-other.e);
                        if(cap<other_size){
                            A::deallocate(B,cap);
                            yuki::clear_alloc<A>(*this);
                            cap=other_size;
                            B=A::allocate(other_size);
                        }
                        b=B;
                        e=B+other_size;
                        const size_type seg1 = (other.B+other.cap)-other.b;
                        yuki::uninitialized_copy_no_overlap(B,other.b,seg1);
                        yuki::uninitialized_copy_no_overlap(B+seg1,other.B,other.e-other.B);
                    }
                }else{
                    b=nullptr;
                    e=B;
                }
            }
        }
        return *this;
    }

    RingQueue& operator=(RingQueue&& other) noexcept {
        if(this!=&other){
            EC::operator=(static_cast<EC&&>(other));
            reset();
            if constexpr(A::propagate_on_container_move_assignment){
                A::operator=(static_cast<A&&>(other));
            }else{
                if(static_cast<const A&>(*this)!=static_cast<const A&>(other)){
                    if(other.b)
                        copy_other_(std::move(other));
                    other.reset();
                    return *this;
                }
            }
            B = other.B;
            b = other.b;
            e = other.e;
            cap = other.cap;
            other.B = nullptr;
            other.b = nullptr;
            other.e = nullptr;
            other.cap = 0;
        }
        return *this;
    }

    friend constexpr void swap(RingQueue& lhs,RingQueue& rhs) noexcept {
        using std::swap;

        if constexpr(A::propagate_on_container_swap)
            swap(static_cast<A&>(lhs),static_cast<A&>(rhs));
        else{
            if(static_cast<const A&>(lhs)!=static_cast<const A&>(rhs)){
                RingQueue temp=std::move(rhs);
                rhs=std::move(lhs);
                lhs=std::move(temp);
                return;
            }
        }

        swap(static_cast<EC&>(lhs),static_cast<EC&>(rhs));
        swap(lhs.B,rhs.B);
        swap(lhs.b,rhs.b);
        swap(lhs.e,rhs.e);
        swap(lhs.cap,rhs.cap);
    }

    template<typename Ap = A&&,typename ECp = EC&&,typename = std::enable_if_t<std::is_constructible_v<A,Ap&&>>>
    constexpr explicit RingQueue(Ap&& ap, ECp&& ecp = EC{}) noexcept :
        A(std::forward<Ap>(ap)),
        EC(std::forward<ECp>(ecp)),
        B(nullptr),
        b(nullptr),
        e(nullptr),
        cap(0)
    {}

    template<typename Ap = A&&,typename ECp = EC&&>
    RingQueue(yuki::reserve_tag_t,const size_type count,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        A(std::forward<Ap>(ap)),
        EC(std::forward<ECp>(ecp)),
        b(nullptr),
        cap(count)
    {e=B=A::allocate(count);}

    template<typename Ap = A&&,typename ECp = EC&&>
    RingQueue(size_type count,const T t,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        RingQueue(yuki::reserve_tag,count,std::forward<Ap>(ap),std::forward<ECp>(ecp))
    {
        for(;count!=0;--count,++e){
            if constexpr(std::is_trivially_copyable_v<T> && std::is_same_v<pointer,T*>)
                memcpy(e,&t,sizeof(T));
            else{
                using yuki::iterator_unwrap;
                ::new(iterator_unwrap(e)) T(t);
            }
        }
        e=b=B;
    }

    template<typename Ap = A&&,typename ECp = EC&&>
    explicit RingQueue(size_type count,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        RingQueue(yuki::reserve_tag,count,std::forward<Ap>(ap),std::forward<ECp>(ecp))
    {
        if constexpr(std::is_trivially_default_constructible_v<T> && std::is_same_v<pointer,T*>)
            memset(e,0,count*sizeof(T));
        else{
            for(;count!=0;--count,++e){
                using yuki::iterator_unwrap;
                ::new(iterator_unwrap(e)) T{};
            }
            e=B;
        }
        b=B;
    }

    template<typename Ap = A&&,typename ECp = EC&&>
    RingQueue(yuki::memset_tag_t,const size_type count,const unsigned char c=0,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        RingQueue(yuki::reserve_tag,count,std::forward<Ap>(ap),std::forward<ECp>(ecp))
    {
        static_assert(std::is_trivial_v<T> && std::is_same_v<pointer,T*>);
        memset(B,c,cap*sizeof(T));
        e=b=B;
    }

    template<typename It,typename Ap = A&&,typename ECp = EC&&,typename = decltype(*std::declval<It>())>
    RingQueue(const It p,const size_type count,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        RingQueue(yuki::reserve_tag,count,std::forward<Ap>(ap),std::forward<ECp>(ecp))
    {
        yuki::uninitialized_copy_no_overlap(B,p,count);
        e=b=B;
    }

    template<typename Ap = A&&,typename ECp = EC&&>
    RingQueue(std::initializer_list<T> il,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        RingQueue(il.begin(),il.size(),std::forward<Ap>(ap),std::forward<ECp>(ecp))
    {}

    ~RingQueue() noexcept {reset();}

  protected:
    constexpr pointer add(const pointer p,size_type n) const {
        n %= cap;
        const size_type after = cap-(p-B);
        return n<after ? p+n : B+(n-after);
    }
    constexpr pointer sub(const pointer p,size_type n) const {
        n %= cap;
        const size_type before = p-B;
        return n<before ? p-n : B+(cap-(n-before));
    }
    constexpr pointer add_or_sub(const pointer p,const difference_type n) const {return n<0 ? sub(p,-n) : add(p,n);}
    constexpr pointer& pre_inc(pointer& p) const {return size_type(p-B+1)==cap ? (p=B) : ++p;}
    constexpr const_pointer& pre_inc(const_pointer& p) const {return size_type(p-B+1)==cap ? (p=B) : ++p;}
    constexpr pointer post_inc(pointer& p) const {const pointer p_orig = p; pre_inc(p); return p_orig;}
    constexpr const_pointer post_inc(const_pointer& p) const {const const_pointer p_orig = p; pre_inc(p); return p_orig;}
    constexpr pointer& pre_dec(pointer& p) const {return p==B ? (p=B+cap-1) : --p;}
    constexpr const_pointer& pre_dec(const_pointer& p) const {return p==B ? (p=B+cap-1) : --p;}
    constexpr pointer post_dec(pointer& p) const {const pointer p_orig = p; pre_dec(p); return p_orig;}
    constexpr const_pointer post_dec(const_pointer& p) const {const const_pointer p_orig = p; pre_dec(p); return p_orig;}

    void expand_(const size_type cap_new){
        const pointer B_new = A::allocate(cap_new);
        if(b){
            if(b<e){
                yuki::uninitialized_move_no_overlap(B_new,b,e-b);
                if constexpr(!std::is_trivially_destructible_v<T>)
                    yuki::destroy(b,e);
            }else{
                const size_type seg1 = (B+cap)-b;
                yuki::uninitialized_move_no_overlap(B_new,b,seg1);
                yuki::uninitialized_move_no_overlap(B_new+seg1,B,e-B);
                if constexpr(!std::is_trivially_destructible_v<T>){
                    yuki::destroy(b,B+cap);
                    yuki::destroy(B,e);
                }
            }
        }
        A::deallocate(B,cap);
        yuki::clear_alloc<A>(*this);
        b=B=B_new;
        e=b+cap;
        cap=cap_new;
    }

  public:
    constexpr bool empty() const {return !static_cast<bool>(b);}
    /// @note A rq with capacity 0 is both full and empty. This is because the primary purpose of this function is to detect whether an insert would need an expansion.
    constexpr bool full() const {return cap==0 || (b==e && b);}
    constexpr size_type capacity() const {return cap;}
    constexpr size_type size() const {
        if(!b) return 0;
        if(b<e) return e-b;
        else return cap-(b-e);
    }

    constexpr T& operator[](const size_type i) {return *add(b,i);}
    constexpr const T& operator[](const size_type i) const {return *add(b,i);}
    constexpr T& front() {assert(b); return *b;}
    constexpr const T& front() const {assert(b); return *b;}
    constexpr T& back() {assert(b); const pointer p=e; return *pre_dec(p);}
    constexpr const T& back() const {assert(b); const pointer p=e; return *pre_dec(p);}

    template<typename... Args>
    void emplace_back(Args&&... args){
        if(full())
            expand_(EC::operator()(cap,cap,1));
        if(!b) b=B;
        using yuki::iterator_unwrap;
        ::new(iterator_unwrap(e)) T(std::forward<Args>(args)...);
        pre_inc(e);
    }

    /// @note Call this on a zero-capacity queue is UB.
    template<typename... Args>
    void emplace_back_overwrite(Args&&... args){
        assert(B);
        if(e==b && b)
            if constexpr(!std::is_trivially_destructible_v<T>)
                e->~T();
        using yuki::iterator_unwrap;
        ::new(iterator_unwrap(e)) T(std::forward<Args>(args)...);
        if(!b)
            b=e;
        else if(e==b)
            pre_inc(b);
        pre_inc(e);
    }

    template<typename... Args>
    void emplace_front(Args&&... args){
        if(full()){
            expand_(EC::operator()(cap,cap,1));
            using yuki::iterator_unwrap;
            ::new(iterator_unwrap(B)) T(std::forward<Args>(args)...);
        }else{
            if(!b)
                b=B+(cap-1);
            else
                pre_dec(b);
            using yuki::iterator_unwrap;
            ::new(iterator_unwrap(b)) T(std::forward<Args>(args)...);
        }
    }

    /// @note Call this on a zero-capacity queue is UB.
    template<typename... Args>
    void emplace_front_overwrite(Args&&... args){
        assert(B);
        if(!b){
            b=e;
            pre_inc(e);
        }else{
            if(e==b){
                pre_dec(e);
                if constexpr(!std::is_trivially_destructible_v<T>)
                    e->~T();
            }
            pre_dec(b);
        }
        using yuki::iterator_unwrap;
        ::new(iterator_unwrap(b)) T(std::forward<Args>(args)...);
    }

    /// @note pop more than size is UB.
    void pop_back(const size_type n){
        const size_type size_orig = size();
        assert(n<=size_orig);
        if constexpr(std::is_trivially_destructible_v<T>)
            e = sub(e,n);
        else
            for(size_type i=n;i!=0;--i)
                pre_dec(e)->~T();
        if(n>=size_orig){
            b=nullptr;
            e=B;
        }
    }

    /// @note pop more than size is UB.
    void pop_back(){
        const size_type size_orig = size();
        assert(size_orig>=1);
        pre_dec(e)->~T();
        if(size_orig<=1){
            b=nullptr;
            e=B;
        }
    }

    /// @note pop more than size is UB.
    T pop_back_v(){
        const size_type size_orig = size();
        assert(size_orig>=1);
        pre_dec(e);
        T t(std::move(*e));
        e->~T();
        if(size_orig<=1){
            b=nullptr;
            e=B;
        }
        return t;
    }

    /// @note pop more than size is UB.
    void pop_front(const size_type n){
        const size_type size_orig = size();
        assert(n<=size_orig);
        if constexpr(std::is_trivially_destructible_v<T>)
            b = add(b,n);
        else
            for(size_type i=n;i!=0;--i)
                post_inc(b)->~T();
        if(n>=size_orig){
            b=nullptr;
            e=B;
        }
    }

    /// @note pop more than size is UB.
    void pop_front(){
        const size_type size_orig = size();
        assert(size_orig>=1);
        b->~T();
        pre_inc(b);
        if(size_orig<=1){
            b=nullptr;
            e=B;
        }
    }

    /// @note pop more than size is UB.
    T pop_front_v(){
        const size_type size_orig = size();
        assert(size_orig>=1);
        T t(std::move(*b));
        b->~T();
        pre_inc(b);
        if(size_orig<=1){
            b=nullptr;
            e=B;
        }
        return t;
    }

    void clear(){
        destroy_();
        b=nullptr;
        e=B;
    }

    /// @note Make sure that all elements have been already destroyed.
    void deallocate(){
        A::deallocate(B,cap);
        yuki::clear_alloc<A>(*this);
        B=b=e=nullptr;
        cap=0;
    }

    void reset(){
        destroy_();
        deallocate();
    }

    void reserve(const size_type cap_new) {if(cap_new>cap) expand_(cap_new);}

    typedef RQ_Iterator_<RingQueue,pointer> iterator;
    typedef RQ_Iterator_<RingQueue,const_pointer> const_iterator;

    iterator begin() {return {this,b};}
    const_iterator begin() const {return {this,b};}
    iterator end() {return {this,e};}
    const_iterator end() const {return {this,e};}

    // DANGEROUS ZONE BELOW!!
    pointer data() {return b;}
    const_pointer data() const {return b;}
    pointer Data() {return B;}
    const_pointer Data() const {return B;}

    /// @note Unlike `resize`, no destruction or construction is performed. You have to do it yourself.
    constexpr void force_resize(const size_type s_new){
        if(s_new==0){
            b=nullptr;
            e=B;
        }else{
            if(!b)
                b=B;
            e=add(b,s_new);
        }
    }
    /// @note No destruction or construction is performed. You have to do it yourself.
    constexpr void set_begin(const pointer b_new) {b=b_new;}
    /// @note No destruction or construction is performed. You have to do it yourself.
    constexpr void set_end(const pointer e_new) {e=e_new;}

    /// @note Unlike `clear`, no destruction or construction is performed. You have to do it yourself.
    constexpr void force_clear(){
        b=nullptr;
        e=B;
    }
    // DANGEROUS ZONE END!!
  private:
    pointer B = nullptr;
    pointer b = nullptr;
    pointer e = nullptr;
    size_type cap = 0;

    // The queue is empty iff. b==nullptr.
    // Other than that, the queue can have 2 forms: b<e or b>=e.
    // In the latter case, the contents split into two segments : [b,B+cap) and [B,e).
}; // struct RingQueue<T,A,EC>

template<typename RQ,typename P>
struct RQ_Iterator_{
    friend RQ;
  private:
    RQ* rq = nullptr;
    P p = nullptr;
    constexpr RQ_Iterator_(RQ* const rqp,const P pp) noexcept : rq(rqp),p(pp) {}

  public:
    typedef std::random_access_iterator_tag iterator_category;
    typedef std::remove_reference_t<decltype(*p)> value_type;
    typedef typename RQ::difference_type difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    constexpr RQ_Iterator_() noexcept = default;

    constexpr value_type& operator*() const {return *p;}
    constexpr P operator->() const {return p;}
    RQ_Iterator_& operator++() {rq->pre_inc(p);return *this;}
    RQ_Iterator_ operator++(int) {const RQ_Iterator_ orig=*this; rq->pre_inc(p); return orig;}
    RQ_Iterator_& operator--() {rq->pre_dec(p); return *this;}
    RQ_Iterator_ operator--(int) {const RQ_Iterator_ orig=*this; rq->pre_dec(p); return orig;}

    explicit constexpr operator bool() const noexcept {return static_cast<bool>(p);}

    friend constexpr bool operator==(RQ_Iterator_,RQ_Iterator_) = default;

    constexpr operator RQ_Iterator_<RQ,add_lconst_t<P>>() const noexcept {return {rq,p};}

    friend constexpr RQ_Iterator_<RQ,remove_lconst_t<P>> const_kast(const RQ_Iterator_<RQ,P> it) {using yuki::const_kast; return {it.rq,const_kast(it.p)};}

    RQ_Iterator_ operator+(const difference_type n) {return {rq,rq->add_or_sub(rq.p,n)};}
};

template<typename RQ,typename P>
struct add_lconst<RQ_Iterator_<RQ,P>> {typedef RQ_Iterator_<RQ,add_lconst_t<P>> type;};

} // namespace yuki
