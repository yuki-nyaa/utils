#pragma once
#include<yuki/Allocator.hpp>
#include<yuki/functional.hpp>

namespace yuki{

template<typename T,typename A=Allocator<T>,typename EC=yuki::Default_EC<2,1,4>>
struct Vector : protected A, protected EC{
    static_assert(std::is_same_v<T,typename A::value_type>);
    using typename A::value_type;
    using typename A::pointer;
    using typename A::const_pointer;
    using typename A::size_type;
    using typename A::difference_type;
    typedef A allocator_type;
    typedef EC ec_type;

    constexpr Vector() noexcept = default;

    Vector(const Vector& other) noexcept:
        A(yuki::select_on_container_copy_construction<A>(other)),
        EC(yuki::select_on_container_copy_construction<EC>(other)),
        cap(other.e-other.b)
    {
        b = A::allocate(cap);
        yuki::uninitialized_copy_no_overlap(b,other.b,cap);
        e = b+cap;
    }

    constexpr Vector(Vector&& other) noexcept:
        A(static_cast<A&&>(*this)),
        EC(static_cast<EC&&>(*this)),
        b(other.b),
        e(other.e),
        cap(other.cap)
    {
        other.b = nullptr;
        other.e = nullptr;
        other.cap = 0;
    }

    Vector& operator=(const Vector& other) noexcept {
        if(this!=&other){
            EC::operator=(other);
            if constexpr(!std::is_trivially_destructible_v<T>)
                yuki::destroy(b,e);
            const size_type other_size = other.e-other.b;
            if constexpr(A::propagate_on_container_copy_assignment){
                A::deallocate(b,cap);
                yuki::clear_alloc<A>(*this);
                A::operator=(other);
                cap = other_size;
                b = A::allocate(cap);
            }else{
                if(cap<other_size){
                    A::deallocate(b,cap);
                    yuki::clear_alloc<A>(*this);
                    cap = other_size;
                    b = A::allocate(cap);
                }
            }
            yuki::uninitialized_copy_no_overlap(b,other.b,other_size);
            e = b+other_size;
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if(this!=&other){
            EC::operator=(static_cast<EC&&>(other));
            if constexpr(!std::is_trivially_destructible_v<T>)
                yuki::destroy(b,e);
            A::deallocate(b,cap);
            yuki::clear_alloc<A>(*this);
            if constexpr(A::propagate_on_container_move_assignment){
                A::operator=(static_cast<A&&>(other));
            }else{
                if(static_cast<const A&>(*this)!=static_cast<const A&>(other)){
                    cap = other.e-other.b;
                    b = A::allocate(cap);
                    yuki::uninitialized_move_no_overlap(b,other.b,cap);
                    e = b+cap;
                    other.reset();
                    return *this;
                }
            }
            b = other.b;
            e = other.e;
            cap = other.cap;
            other.b = nullptr;
            other.e = nullptr;
            other.cap = 0;
        }
        return *this;
    }

    friend constexpr void swap(Vector& lhs,Vector& rhs) noexcept {
        using std::swap;

        if constexpr(A::propagate_on_container_swap)
            swap(static_cast<A&>(lhs),static_cast<A&>(rhs));
        else{
            if(static_cast<const A&>(lhs)!=static_cast<const A&>(rhs)){
                Vector temp=std::move(rhs);
                rhs=std::move(lhs);
                lhs=std::move(temp);
                return;
            }
        }

        swap(static_cast<EC&>(lhs),static_cast<EC&>(rhs));
        swap(lhs.b,rhs.b);
        swap(lhs.e,rhs.e);
        swap(lhs.cap,rhs.cap);
    }

    template<typename Ap = A&&,typename ECp = EC&&,typename = std::enable_if_t<std::is_constructible_v<A,Ap&&>>>
    explicit constexpr Vector(Ap&& ap, ECp&& ecp = EC{}) noexcept :
        A(std::forward<Ap>(ap)),
        EC(std::forward<ECp>(ecp)),
        b(nullptr),
        e(nullptr),
        cap(0)
    {}

    template<typename Ap = A&&,typename ECp = EC&&>
    Vector(yuki::reserve_tag_t,const size_type count,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        A(std::forward<Ap>(ap)),
        EC(std::forward<ECp>(ecp)),
        cap(count)
    {
        e=b=A::allocate(count);
    }

    template<typename Ap = A&&,typename ECp = EC&&>
    Vector(size_type count,const T t,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        Vector(yuki::reserve_tag,count,std::forward<Ap>(ap),std::forward<ECp>(ecp))
    {
        for(;count!=0;--count,++e){
            if constexpr(std::is_trivially_copyable_v<T> && std::is_same_v<pointer,T*>)
                memcpy(e,&t,sizeof(T));
            else{
                using yuki::iterator_unwrap;
                ::new(iterator_unwrap(e)) T(t);
            }
        }
    }

    template<typename Ap = A&&,typename ECp = EC&&,typename=std::enable_if_t<std::is_constructible_v<A,Ap&&>>>
    explicit Vector(size_type count,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        Vector(yuki::reserve_tag,count,std::forward<Ap>(ap),std::forward<ECp>(ecp))
    {
        if constexpr(std::is_trivially_default_constructible_v<T> && std::is_same_v<pointer,T*>){
            memset(e,0,count*sizeof(T));
            e+=count;
        }else
            for(;count!=0;--count,++e){
                using yuki::iterator_unwrap;
                ::new(iterator_unwrap(e)) T{};
            }
    }

    template<typename Ap = A&&,typename ECp = EC&&>
    Vector(yuki::memset_tag_t,const size_type count,const unsigned char c=0,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        Vector(yuki::reserve_tag,count,std::forward<Ap>(ap),std::forward<ECp>(ecp))
    {
        static_assert(std::is_trivial_v<T> && std::is_same_v<pointer,T*>);
        memset(b,c,cap*sizeof(T));
        e=b+cap;
    }

    template<typename It,typename Ap = A&&,typename ECp = EC&&,typename = decltype(*std::declval<It>())>
    Vector(const It p,const size_type count,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        Vector(yuki::reserve_tag,count,std::forward<Ap>(ap),std::forward<ECp>(ecp))
    {
        yuki::uninitialized_copy_no_overlap(b,p,count);
        e=b+cap;
    }

    template<typename Ap = A&&,typename ECp = EC&&>
    Vector(std::initializer_list<T> il,Ap&& ap = A{}, ECp&& ecp= EC{}) noexcept :
        Vector(il.begin(),il.size(),std::forward<Ap>(ap),std::forward<ECp>(ecp))
    {}

    template<typename... Args,typename Ap = A&&,typename ECp = EC&&,typename = std::enable_if_t<std::is_constructible_v<A,Ap&&>>>
    Vector(yuki::from_variadic_tag_t,Ap&& ap,ECp&& ecp,Args&&... args) noexcept :
        Vector(yuki::reserve_tag,sizeof...(args),std::forward<Ap>(ap),std::forward<ECp>(ecp))
    {
        using yuki::iterator_unwrap;
        (...,::new(iterator_unwrap(e++)) T(std::forward<Args>(args)));
    }

    template<typename... Args>
    Vector(yuki::from_variadic_tag_t,Args&&... args) noexcept :
        Vector(yuki::from_variadic_tag,A(),EC(),std::forward<Args>(args)...)
    {}

    ~Vector() noexcept {reset();}

    Vector& operator=(std::initializer_list<T> il) noexcept {
        clear();
        recapacity(il.size());
        yuki::uninitialized_copy_no_overlap(b,il.begin(),il.size());
        e=b+cap;
        return *this;
    }

    void clear(){
        if constexpr(!std::is_trivially_destructible_v<T>)
            yuki::destroy(b,e);
        e=b;
    }

    /// @note Make sure that all elements have been already destroyed.
    void deallocate(){
        A::deallocate(b,cap);
        yuki::clear_alloc<A>(*this);
        b=e=nullptr;
        cap=0;
    }

    void reset(){
        if constexpr(!std::is_trivially_destructible_v<T>)
            yuki::destroy(b,e);
        deallocate();
    }

    constexpr bool empty() const {return b==e;}
    constexpr size_type size() const {return e-b;}
    constexpr size_type capacity() const {return cap;}
    constexpr T& operator[](const size_type i) {assert(i<size_type(e-b)); return b[i];}
    constexpr const T& operator[](const size_type i) const {assert(i<size_type(e-b)); return b[i];}
    constexpr T& front() {assert(b!=e); return *b;}
    constexpr const T& front() const {assert(b!=e); return *b;}
    constexpr T& back() {assert(b!=e); return *(e-1);}
    constexpr const T& back() const {assert(b!=e); return *(e-1);}

    typedef pointer iterator;
    typedef const_pointer const_iterator;

    constexpr iterator begin() {return b;}
    constexpr const_iterator begin() const {return b;}
    constexpr iterator end() {return e;}
    constexpr const_iterator end() const {return e;}

    constexpr const_iterator cbegin() const {return b;}
    constexpr const_iterator cend() const {return e;}

  protected:
    /// @tparam expand True if `cap_new >(=) e-b`
    template<bool expand = true>
    void recap_(const size_type cap_new){
        const pointer b_new = A::allocate(cap_new);
        const size_type s_new = expand ? e-b : cap_new;
        yuki::uninitialized_move_no_overlap(b_new,b,s_new);
        if constexpr(!std::is_trivially_destructible_v<T>)
            yuki::destroy(b,e);
        A::deallocate(b,cap);
        yuki::clear_alloc<A>(*this);
        b = b_new;
        e = b+s_new;
        cap = cap_new;
    }
  public:
    void reserve(const size_type cap_new) {if(cap_new>cap) recap_<true>(cap_new);}

    /// @note Do not insert an element already in the vector, unless you are certain that the insertion would not trigger reallocation. (Actually there seems to be no PORTABLE way to check this.)
    template<typename... Args>
    void emplace_back(Args&&... args){
        if(size_type(e-b)==cap)
            recap_<true>(EC::operator()(cap,cap,1));
        using yuki::iterator_unwrap;
        ::new(iterator_unwrap(e)) T(std::forward<Args>(args)...);
        ++e;
    }

    /// @note Do not insert an element already in the vector, unless you are certain that the insertion would not trigger reallocation. (Actually there seems to be no PORTABLE way to check this.)
    template<typename... Args>
    void force_emplace_back(Args&&... args){
        assert(size_type(e-b)!=cap);
        using yuki::iterator_unwrap;
        ::new(iterator_unwrap(e)) T(std::forward<Args>(args)...);
        ++e;
    }

    void pop_back(const size_type n){
        assert(n<=size_type(e-b));
        e-=n;
        if constexpr(!std::is_trivially_destructible_v<T>)
            yuki::destroy(e,n);
    }

    void pop_back(){
        assert(e!=b);
        (--e)->~T();
    }

    T pop_back_v(){
        T t(std::move(*--e));
        e->~T();
        return t;
    }

    /// @note Unlike `resize`, no destruction or construction is performed. You have to do it yourself.
    constexpr void force_resize(const size_type s_new) {assert(s_new<=cap);e=b+s_new;}
    /// @note Unlike `resize`,no destruction or construction is performed. You have to do it yourself.
    constexpr void force_resize(const pointer e_new) {e=e_new;}
    /// @note Unlike `clear`,no destruction or construction is performed. You have to do it yourself.
    constexpr void force_clear() {e=b;}

    void resize(const size_type s_new){
        if(s_new<size_type(e-b)){
            if constexpr(!std::is_trivially_destructible_v<T>)
                yuki::destroy(b+s_new,e);
            e=b+s_new;
        }else if(s_new>size_type(e-b)){
            if(s_new>cap)
                recap_<true>(s_new);
            if constexpr(!std::is_trivially_default_constructible_v<T>){
                for(;size_type(e-b) < s_new;++e){
                    using yuki::iterator_unwrap;
                    ::new(iterator_unwrap(e)) T;
                }
            }else
                e=b+s_new;
        }
    }

    void recapacity(const size_type cap_new){
        if(cap_new!=cap){
            if(cap_new>size_type(e-b))
                recap_<true>(cap_new);
            else
                recap_<false>(cap_new);
        }
    }

    void shrink_to_fit() {e-b!=cap ? recap_<false>(e-b) : (void)0;}

    /// @note Do not insert an element already in the vector, unless you are certain that the insertion would not trigger reallocation. (Actually there seems to be no PORTABLE way to check this.)
    template<typename... Args>
    iterator emplace(const const_iterator cpos,Args&&... args){
        const size_type off = cpos-b;
        const iterator pos = b+off;
        if(size_type(e-b)==cap){
            const size_type cap_new = EC::operator()(cap,cap,1);
            const pointer b_new = A::allocate(cap_new);
            yuki::uninitialized_move_no_overlap(b_new,b,off);
            using yuki::iterator_unwrap;
            ::new(iterator_unwrap(b_new+off)) T(std::forward<Args>(args)...);
            yuki::uninitialized_move_no_overlap(b_new+off+1,pos,e-pos);
            if constexpr(!std::is_trivially_destructible_v<T>)
                yuki::destroy(b,e);
            A::deallocate(b,cap);
            yuki::clear_alloc<A>(*this);
            b = b_new;
            e = b+cap+1;
            cap = cap_new;
        }else{
            yuki::uninitialized_move_overlap(pos+1,pos,e-pos);
            using yuki::iterator_unwrap;
            ::new(iterator_unwrap(pos)) T(std::forward<Args>(args)...);
            ++e;
        }
        return b+off;
    }

    /// @note Do not insert an element already in the vector, unless you are certain that the insertion would not trigger reallocation. (Actually there seems to be no PORTABLE way to check this.)
    template<typename It>
    iterator erase_then_insert(const const_iterator cpos,const size_type count_erase,const It it,const size_type count){
        const size_type off = cpos-b;
        const iterator pos = b+off;
        const size_type s = e-b;
        if(s-count_erase+count>cap){
            const size_type cap_new = EC::operator()(s-count_erase,cap,count);
            const pointer b_new = A::allocate(cap_new);
            yuki::uninitialized_move_no_overlap(b_new,b,off);
            yuki::uninitialized_copy_no_overlap(b_new+off,it,count);
            yuki::uninitialized_move_no_overlap(b_new+off+count,pos+count_erase,e-(pos+count_erase));
            if constexpr(!std::is_trivially_destructible_v<T>)
                yuki::destroy(b,s);
            A::deallocate(b,cap);
            yuki::clear_alloc<A>(*this);
            b = b_new;
            e = b+s-count_erase+count;
            cap = cap_new;
        }else{
            if constexpr(!std::is_trivially_destructible_v<T>)
                yuki::destroy(pos,count_erase);
            yuki::uninitialized_move_overlap(pos+count,pos+count_erase,e-(pos+count_erase));
            yuki::uninitialized_copy_no_overlap(pos,it,count);
            e = e-count_erase+count;
        }
        return count==0 ? pos : b+off;
    }

    /// @note Do not insert an element already in the vector, unless you are certain that the insertion would not trigger reallocation. (Actually there seems to be no PORTABLE way to check this.)
    template<typename... Args>
    iterator erase_then_emplace(const const_iterator cpos,const size_type count_erase,Args&&... args){
        const size_type off = cpos-b;
        const iterator pos = b+off;
        const size_type s = e-b;
        if(s-count_erase+1>cap){
            const size_type cap_new = EC::operator()(s-count_erase,cap,1);
            const pointer b_new = A::allocate(cap_new);
            yuki::uninitialized_move_no_overlap(b_new,b,off);
            using yuki::iterator_unwrap;
            ::new(iterator_unwrap(b_new+off)) T(std::forward<Args>(args)...);
            yuki::uninitialized_move_no_overlap(b_new+off+1,pos+count_erase,e-(pos+count_erase));
            if constexpr(!std::is_trivially_destructible_v<T>)
                yuki::destroy(b,s);
            A::deallocate(b,cap);
            yuki::clear_alloc<A>(*this);
            b = b_new;
            e = b+s-count_erase+1;
            cap = cap_new;
        }else{
            if constexpr(!std::is_trivially_destructible_v<T>)
                yuki::destroy(pos,count_erase);
            yuki::uninitialized_move_overlap(pos+1,pos+count_erase,e-(pos+count_erase));
            ::new(iterator_unwrap(pos)) T(std::forward<Args>(args)...);
            e = e-count_erase+1;
        }
        return b+off;
    }

    /// @note Do not insert an element already in the vector, unless you are certain that the insertion would not trigger reallocation. (Actually there seems to be no PORTABLE way to check this.)
    template<typename It>
    iterator insert(const const_iterator pos,const It it,const size_type count) {return erase_then_insert<It>(pos,0,it,count);}

    iterator erase(const const_iterator cpos,const size_type count_erase=1){
        const size_type off = cpos-b;
        const iterator pos = b+off;
        if constexpr(!std::is_trivially_destructible_v<T>)
            yuki::destroy(pos,count_erase);
        yuki::uninitialized_move_overlap(pos,pos+count_erase,e-(pos+count_erase));
        e -= count_erase;
        return pos+count_erase;
    }

  private:
    pointer b = nullptr;
    pointer e = nullptr;
    size_type cap = 0;
}; // struct Vector<T,A,EC>
} // namespace yuki
