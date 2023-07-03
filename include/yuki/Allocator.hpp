#pragma once
#include<cstdlib>
#include<cstring>
#include<cassert>
#include<new>
#include<limits>
#include<yuki/tmp.hpp>

namespace yuki{

template<typename A>
constexpr typename A::size_type max_size(const A& a) noexcept {
    if constexpr(requires{a.max_size();})
        return a.max_size();
    else
        return std::numeric_limits<typename A::size_type>::max()/sizeof(typename A::value_type);
}




template<typename T>
struct Allocator{
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    static constexpr bool propagate_on_container_copy_assignment = false;
    static constexpr bool propagate_on_container_move_assignment = true;
    static constexpr bool propagate_on_container_swap = true;

    [[nodiscard]]
    static pointer allocate(const size_type n=1){
        pointer p = static_cast<pointer>(malloc(n*sizeof(T)));
        if(p==nullptr)
            throw std::bad_alloc{};
        return p;
    };

    //static pointer allocate(size_type n,std::size_t a){
    //    assert(a>=alignof(T));
    //    assert((n*sizeof(T))%a==0);
    //    pointer p = std::aligned_alloc(a,n*sizeof(T));
    //    if(p==nullptr)
    //        throw std::bad_alloc{};
    //    return p;
    //};

    static void deallocate(const const_pointer p,const size_type=1) noexcept {free(const_cast<pointer>(p));}

    friend constexpr void swap(Allocator&,Allocator&) noexcept {}
    friend constexpr bool operator==(const Allocator,const Allocator) noexcept {return true;}
}; // struct Allocator<T>




template<typename T>
constexpr T* iterator_unwrap(T* const p) noexcept {return p;}
constexpr std::nullptr_t iterator_unwrap(std::nullptr_t) noexcept {return nullptr;}
template<typename It>
constexpr auto iterator_unwrap(const It it) noexcept {
    return std::addressof(it.operator->());
}




template<typename P,typename... Args>
void construct(const P p,Args&&... args)
    noexcept(std::is_nothrow_constructible_v<decltype(*p),Args&&...>)
{
    typedef std::remove_reference_t<decltype(*p)> T;
    using yuki::iterator_unwrap;
    ::new(iterator_unwrap(p)) T(std::forward<Args>(args)...);
}

template<typename P,typename... Args>
void construct_brace(const P p,Args&&... args)
    noexcept(std::is_nothrow_constructible_v<decltype(*p),Args&&...>)
{
    typedef std::remove_reference_t<decltype(*p)> T;
    using yuki::iterator_unwrap;
    ::new(iterator_unwrap(p)) T{std::forward<Args>(args)...};
}

// The macro version.
#define YUKI_CONSTRUCT(p,...) \
    do{ \
        typedef std::remove_reference_t<decltype(*p)> T; \
        using yuki::iterator_unwrap; \
        ::new(iterator_unwrap(p)) T(__VA_ARGS__); \
    }while(0)

#define YUKI_CONSTRUCT_BRACE(p,...) \
    do{ \
        typedef std::remove_reference_t<decltype(*p)> T; \
        using yuki::iterator_unwrap; \
        ::new(iterator_unwrap(p)) T{__VA_ARGS__}; \
    }while(0)

#define YUKI_CONSTRUCT_FREE(p,...) \
    do{ \
        typedef std::remove_reference_t<decltype(*p)> T; \
        using yuki::iterator_unwrap; \
        ::new(iterator_unwrap(p)) T __VA_ARGS__ ; \
    }while(0)

template<typename P>
constexpr void destroy(const P p)
    noexcept(std::is_nothrow_destructible_v<decltype(*p)>)
{
    typedef std::remove_reference_t<decltype(*p)> T;
    p->~T();
}

template<typename P>
constexpr void destroy(P p,size_t n)
    noexcept(std::is_nothrow_destructible_v<decltype(*p)>)
{
    typedef std::remove_reference_t<decltype(*p)> T;
    for(;n!=0;--n,++p)
        p->~T();
}

template<typename P>
constexpr void destroy(P b,const P e)
    noexcept(std::is_nothrow_destructible_v<decltype(*b)>)
{
    typedef std::remove_reference_t<decltype(*b)> T;
    for(;b!=e;++b)
        b->~T();
}

template<typename A>
constexpr void clear_alloc(A& a) noexcept {
    if constexpr(requires{a.clear();})
        a.clear();
}

template<typename A>
constexpr A select_on_container_copy_construction(const A& a) noexcept {
    if constexpr(requires{a.select_on_container_copy_construction();})
        return a.select_on_container_copy_construction();
    else
        return A{};
}




/// @note Although it is named "copy", it can actually perform move. For example, when `Ps` is `std::move_iterator`.
template<typename Pd,typename Ps>
void uninitialized_copy_no_overlap(Pd dst,Ps src,size_t count)
    noexcept(std::is_nothrow_constructible_v<std::remove_reference_t<decltype(*dst)>,decltype(*src)>)
{
    typedef std::remove_reference_t<decltype(*dst)> dst_t;
    typedef std::remove_reference_t<decltype(*src)> src_t;
    static_assert(!std::is_const_v<dst_t>);

    using yuki::iterator_unwrap;
    if constexpr(std::is_same_v<Pd,dst_t*> && std::is_same_v<dst_t,std::remove_const_t<src_t>> && std::is_trivially_copyable_v<dst_t>)
        memcpy(dst,src,count*sizeof(dst_t));
    else
        for(;count!=0;--count)
            ::new(iterator_unwrap(dst++)) dst_t(*src++);
}

/// @note Although it is named "copy", it can actually perform move. For example, when `Ps` is `std::move_iterator`.
template<typename Pd,typename Ps>
constexpr void copy_no_overlap(Pd dst,Ps src,size_t count)
    noexcept(std::is_nothrow_assignable_v<std::remove_reference_t<decltype(*dst)>,decltype(*src)>)
{
    typedef std::remove_reference_t<decltype(*dst)> dst_t;
    typedef std::remove_reference_t<decltype(*src)> src_t;
    static_assert(!std::is_const_v<dst_t>);

    if constexpr(std::is_same_v<Pd,dst_t*> && std::is_same_v<dst_t,std::remove_const_t<src_t>> && std::is_trivially_copyable_v<dst_t>){
        if(!std::is_constant_evaluated())
            memcpy(dst,src,count*sizeof(dst_t));
        else
            for(;count!=0;--count)
                *dst++=*src++;
    }else
        for(;count!=0;--count)
            *dst++=*src++;
}

template<typename Pd,typename Ps>
void uninitialized_move_no_overlap(Pd dst,Ps src,size_t count)
    noexcept(std::is_nothrow_constructible_v<std::remove_reference_t<decltype(*dst)>,decltype(std::move(*src))>)
{
    typedef std::remove_reference_t<decltype(*dst)> dst_t;
    typedef std::remove_reference_t<decltype(*src)> src_t;
    static_assert(!std::is_const_v<dst_t>);
    static_assert(!std::is_const_v<src_t>);

    using yuki::iterator_unwrap;
    if constexpr(std::is_same_v<Pd,dst_t*> && std::is_same_v<dst_t,src_t> && std::is_trivially_copyable_v<dst_t>)
        memcpy(dst,src,count*sizeof(dst_t));
    else
        for(;count!=0;--count)
            ::new(iterator_unwrap(dst++)) dst_t(std::move(*src++));
}

template<typename Pd,typename Ps>
constexpr void move_no_overlap(Pd dst,Ps src,size_t count)
    noexcept(std::is_nothrow_assignable_v<std::remove_reference_t<decltype(*dst)>,decltype(std::move(*src))>)
{
    typedef std::remove_reference_t<decltype(*dst)> dst_t;
    typedef std::remove_reference_t<decltype(*src)> src_t;
    static_assert(!std::is_const_v<dst_t>);
    static_assert(!std::is_const_v<src_t>);

    if constexpr(std::is_same_v<Pd,dst_t*> && std::is_same_v<dst_t,src_t> && std::is_trivially_copyable_v<dst_t>){
        if(!std::is_constant_evaluated())
            memcpy(dst,src,count*sizeof(dst_t));
        else
            for(;count!=0;--count)
                *dst++=std::move(*src++);
    }else
        for(;count!=0;--count)
            *dst++=std::move(*src++);
}

// There is no `(uninitialized_)copy_overlap`, because the source objects will be overwritten anyway. (The number of overwrite, however, depends on how much they overlap.)

/// @note The non-overlapping objects will be destroyed if they are of non-trivially destructible type.
template<typename P>
void uninitialized_move_overlap(P dst,P src,size_t count)
    noexcept(std::is_nothrow_constructible_v<std::remove_reference_t<decltype(*dst)>,decltype(std::move(*src))>
        && std::is_nothrow_destructible_v<std::remove_reference_t<decltype(*src)>>)
{
    typedef std::remove_reference_t<decltype(*dst)> value_t;
    static_assert(!std::is_const_v<value_t>);

    using yuki::iterator_unwrap;
    if constexpr(std::is_same_v<P,value_t*> && std::is_trivially_copyable_v<value_t>)
        memmove(dst,src,count*sizeof(value_t));
    else{
        if(dst<src){ // From low to high.
            for(;count!=0;--count){
                ::new(iterator_unwrap(dst++)) value_t(std::move(*src));
                if constexpr(!std::is_trivially_destructible_v<value_t>)
                    src->~value_t();
                ++src;
            }
        }else if(dst>src){ // From high to low.
            dst+=count;
            src+=count;
            for(;count!=0;--count){
                ::new(iterator_unwrap(--dst)) value_t(std::move(*--src));
                if constexpr(!std::is_trivially_destructible_v<value_t>)
                    src->~value_t();
            }
        }
    }
}

/// @note The non-overlapping objects will be destroyed if they are of non-trivially destructible type.
template<typename P>
constexpr void move_overlap(P dst,P src,size_t count)
    noexcept(std::is_nothrow_assignable_v<std::remove_reference_t<decltype(*dst)>,decltype(std::move(*src))>
        && std::is_nothrow_destructible_v<std::remove_reference_t<decltype(*src)>>)
{
    typedef std::remove_reference_t<decltype(*dst)> value_t;
    static_assert(!std::is_const_v<value_t>);

    auto memmove_my = [](P dst_p,P src_p,size_t count_p)constexpr{
        if(dst_p<src_p){ // From low to high.
            const P src_nol = dst_p+count_p;
            for(;count_p!=0;--count_p)
                *dst_p++=std::move(*src_p++);
            if constexpr(!std::is_trivially_destructible_v<value_t>)
                destroy(src_nol,src_p-dst_p);
        }else if(dst_p>src_p){ // From high to low.
            const P src_nol = src_p;
            dst_p+=count_p;
            src_p+=count_p;
            for(;count_p!=0;--count_p)
                *--dst_p=std::move(*--src_p);
            if constexpr(!std::is_trivially_destructible_v<value_t>)
                destroy(src_nol,dst_p-src_p);
        }
    };

    if constexpr(std::is_same_v<P,value_t*> && std::is_trivially_copyable_v<value_t>){
        if(!std::is_constant_evaluated())
            memmove(dst,src,count*sizeof(value_t));
        else
            memmove_my(dst,src,count);
    }else
        memmove_my(dst,src,count);
}


template<typename P,typename T>
void uninitialized_fill_n(P p,size_t count,const T& value)
    noexcept(std::is_nothrow_constructible_v<std::remove_reference_t<decltype(*p)>,const T&>)
{
    typedef std::remove_reference_t<decltype(*p)> p_value_t;
    static_assert(!std::is_const_v<p_value_t>);
    static_assert(std::is_constructible_v<p_value_t,const T&>);

    using yuki::iterator_unwrap;
    for(;count!=0;--count,++p){
        if constexpr(std::is_same_v<P,p_value_t*> && std::is_same_v<p_value_t,T> && std::is_trivially_copyable_v<T>)
            memcpy(p,std::addressof(value),sizeof(T));
        else
            ::new(iterator_unwrap(p)) p_value_t(value);
    }
}

template<typename P,typename T>
constexpr void fill_n(P p,size_t count,const T& value)
    noexcept(std::is_nothrow_assignable_v<std::remove_reference_t<decltype(*p)>,const T&>)
{
    typedef std::remove_reference_t<decltype(*p)> p_value_t;
    static_assert(!std::is_const_v<p_value_t>);
    static_assert(std::is_assignable_v<p_value_t,const T&>);

    for(;count!=0;--count,++p){
        if constexpr(std::is_same_v<P,p_value_t*> && std::is_same_v<p_value_t,T> && std::is_trivially_copyable_v<T>)
            if(!std::is_constant_evaluated())
                memcpy(p,std::addressof(value),sizeof(T));
            else
               *p=value;
        else
            *p=value;
    }
}




template<typename T,size_t DEFAULT_BLOCK_SIZE_P = 32>
struct List_Allocator{
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    static constexpr bool propagate_on_container_copy_assignment = false;
    static constexpr bool propagate_on_container_move_assignment = true;
    static constexpr bool propagate_on_container_swap = true;

    static constexpr size_t DEFAULT_BLOCK_SIZE = DEFAULT_BLOCK_SIZE_P;

    [[nodiscard]]
    pointer allocate(const size_type n=1) noexcept {
        if(current==nullptr || n > current_info.total-current_info.next){
            Node* block = Allocator<Node>::allocate();
            current_info.total = n>DEFAULT_BLOCK_SIZE ? n : DEFAULT_BLOCK_SIZE;
            ::new(block) Node{Allocator<T>::allocate(current_info.total),nullptr};
            if(head==nullptr)
                head = block;
            else
                current->next_node  = block;
            current = block;
            current_info.next = n;
            return block->data;
        }else
            return (current->data)+(current_info.next+=n);
    }

    /// @note Deallocation is generally impossible, except for the last allocation.
    void deallocate(const const_pointer p,const size_type n=1) noexcept {
        if(current!=nullptr && n<=current_info.next && p==(current->data)+(current_info.next-n))
            current_info.next-=n;
    }

    bool empty() const noexcept {return head==nullptr;}
    size_t free_in_current_block() const noexcept {return current_info.total-current_info.next;}
    size_t current_block_size() const noexcept {return current_info.total;}

    void clear() noexcept {
        while(head!=nullptr){
            current = head->next_node;
            Allocator<T>::deallocate(head->data);
            Allocator<Node>::deallocate(head);
            head = current;
        }
    }

    ~List_Allocator() noexcept {clear();}

    List_Allocator() noexcept = default;
    List_Allocator(const List_Allocator&) = delete;
    List_Allocator(List_Allocator&& other) noexcept :
        head(other.head),
        current(other.current),
        current_info(other.current_info)
    {
        other.head = nullptr;
        other.current = nullptr;
        other.current_info.reset();
    }
    List_Allocator& operator=(const List_Allocator&) = delete;
    List_Allocator& operator=(List_Allocator&& other) noexcept {
        if(this!=&other){
            clear();
            head = other.head;
            current = other.current;
            current_info = other.current_info;
            other.head = nullptr;
            other.current = nullptr;
            other.current_info.reset();
        }
    }
    friend void swap(List_Allocator& lhs,List_Allocator& rhs) noexcept {
        using std::swap;
        swap(lhs.head,rhs.head);
        swap(lhs.current,rhs.current);
        swap(lhs.current_info,rhs.current_info);
    }
    friend bool operator==(const List_Allocator& lhs,const List_Allocator& rhs) noexcept
        {return lhs.head==rhs.head;}
  private:
    struct Node{
        T* data;
        Node* next_node = nullptr;
    };
    Node* head = nullptr; // 1
    Node* current = nullptr; // 2
    struct Current_Info{
        size_t total = 0;
        size_t next = 0;
        constexpr void reset() noexcept {total=0;next=0;}
    } current_info; // 3
}; // struct List_Allocator<T,s>




template<typename T,size_t SIZE_P = 32>
struct Array_Allocator_Dynamic{
    static constexpr size_t SIZE = SIZE_P;

    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    static constexpr bool propagate_on_container_copy_assignment = false;
    static constexpr bool propagate_on_container_move_assignment = true;
    static constexpr bool propagate_on_container_swap = true;

    pointer allocate(const size_type n=1) noexcept {
        if(n>1)
            return Allocator<T>::allocate(n);
        if(!p){
            p = Allocator<Tagged>::allocate(SIZE_P);
            stack.init();
        }
        if(stack.top>=SIZE_P){
            Tagged* p1 = Allocator<Tagged>::allocate(1);
            p1->flag = FROM_OTHER; // I'm not sure if this is UB or not.
            return to_value(p1);
        }else{
            Tagged* p1 = p+stack.pop();
            p1->flag = FROM_POOL; // I'm not sure if this is UB or not.
            return to_value(p1);
        }
    }

    void deallocate(const const_pointer p_arg,const size_type n = 1) noexcept {
        if(n>1)
            return Allocator<T>::deallocate(p_arg,n);
        const Tagged* p_block = to_tagged(p_arg);
        switch(p_block->flag){
            case FROM_POOL : return stack.push(p_block - p);
            case FROM_OTHER : return Allocator<Tagged>::deallocate(p_block,1);
        }
    }

    void clear() noexcept {Allocator<Tagged>::deallocate(p,SIZE_P);}

    Array_Allocator_Dynamic() noexcept = default;
    Array_Allocator_Dynamic(const Array_Allocator_Dynamic&) = delete;
    Array_Allocator_Dynamic(Array_Allocator_Dynamic&& other) noexcept :
        p(other.p)
    {
        memcpy(stack.data,other.stack.data,SIZE_P*sizeof(Stack::data_t));
        stack.top = other.stack.top;
        other.p = nullptr;
        other.stack.top=0;
    }
    Array_Allocator_Dynamic& operator=(const Array_Allocator_Dynamic&) = delete;
    Array_Allocator_Dynamic& operator=(Array_Allocator_Dynamic&& other) noexcept {
        if(this!=&other){
            clear();
            p = other.p;
            memcpy(stack.data,other.stack.data,SIZE_P*sizeof(typename Stack::data_t));
            stack.top = other.stack.top;
            other.p = nullptr;
            other.stack.top=0;
        }
    }
    ~Array_Allocator_Dynamic() noexcept {clear();}

    friend void swap(Array_Allocator_Dynamic& lhs,Array_Allocator_Dynamic& rhs) noexcept {
        using std::swap;
        swap(lhs.p,rhs.p);
        swap(lhs.stack.top,rhs.stack.top);
        static typename Stack::data_t temp[SIZE_P];
        memcpy(lhs.stack.data,temp,SIZE_P*sizeof(typename Stack::data_t));
        memcpy(rhs.stack.data,lhs.stack.data,SIZE_P*sizeof(typename Stack::data_t));
        memcpy(temp,rhs.stack.data,SIZE_P*sizeof(typename Stack::data_t));
    }

    friend bool operator==(const Array_Allocator_Dynamic& lhs,const Array_Allocator_Dynamic& rhs) noexcept {return lhs.p==rhs.p;}

    auto free_count() noexcept {return SIZE_P-stack.top;}

  private:
    enum : unsigned char {FROM_POOL, FROM_OTHER};

    template<typename U,bool = std::is_standard_layout_v<U>>
    struct Tagged_Impl_ : U {
        static constexpr bool is_standard_layout = false;
        unsigned char flag;
    };
    template<typename U>
    struct Tagged_Impl_<U,true>{
        static constexpr bool is_standard_layout = true;
        U value;
        unsigned char flag;
    };

    typedef Tagged_Impl_<T> Tagged;

    static constexpr T* to_value(Tagged* const pt){
        if constexpr(Tagged::is_standard_layout)
            return static_cast<T*>(static_cast<void*>(pt));
        else
            return static_cast<T*>(pt);
    }
    static constexpr const T* to_value(const Tagged* const pt){
        if constexpr(Tagged::is_standard_layout)
            return static_cast<const T*>(static_cast<const void*>(pt));
        else
            return static_cast<const T*>(pt);
    }
    static constexpr Tagged* to_tagged(T* const pv){
        if constexpr(Tagged::is_standard_layout)
            return static_cast<Tagged*>(static_cast<void*>(pv));
        else
            return static_cast<Tagged*>(pv);
    }
    static constexpr const Tagged* to_tagged(const T* const pv){
        if constexpr(Tagged::is_standard_layout)
            return static_cast<const Tagged*>(static_cast<const void*>(pv));
        else
            return static_cast<const Tagged*>(pv);
    }

    Tagged* p = nullptr;
    struct Stack{
        typedef uint_auto_t<SIZE_P> data_t;
        typedef uint_auto_t<SIZE_P,unsigned> top_t;
        data_t data[SIZE_P];
        top_t top = 0;

        void push(const data_t n) noexcept {data[--top]=n;}
        data_t pop() noexcept {return data[top++];}
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wtype-limits"
        void init() noexcept {for(size_t i=0;i<SIZE_P;++i) data[i]=i;}
        #pragma GCC diagnostic pop
    } stack;
};

template<typename T>
using Array_Allocator = Array_Allocator_Dynamic<T>;
} // namespace yuki




namespace yuki{
inline constexpr struct reserve_tag_t {} reserve_tag;
inline constexpr struct memset_tag_t {} memset_tag;
inline constexpr struct from_variadic_tag_t {} from_variadic_tag;
inline constexpr struct from_ordered_tag_t {} from_ordered_tag;

inline constexpr struct unique_tag_t {} unique_tag;
}
