#include<cstdio>
#include<type_traits>
#include<yuki/Allocator.hpp>

template<typename T,typename A=yuki::Allocator<T>,typename A::size_type dynamic_threshold_p=(sizeof(typename A::pointer)+sizeof(typename A::size_type))/sizeof(T)>
struct SmallVector : private A{
    static_assert(std::is_same_v<T,typename A::value_type>);
    static_assert(std::is_same_v<T*,typename A::pointer>);
    static_assert(std::is_same_v<const T*,typename A::const_pointer>);
    static_assert(dynamic_threshold_p>=(sizeof(typename A::pointer)+sizeof(typename A::size_type))/sizeof(T));

    typedef T value_type;
    typedef A allocator_type;
    using typename A::pointer;
    using typename A::const_pointer;
    using typename A::size_type;
    using typename A::difference_type;

    static constexpr size_type dynamic_threshold = dynamic_threshold_p;
  private:
    union{
    struct{pointer ptr; size_type cap;} dynamic_;
    struct{struct{alignas(alignof(T)) unsigned char data[sizeof(T)];} arr[dynamic_threshold];} static_;
    };
    size_type size_=0;
  public:
    SmallVector() noexcept = default;

    ~SmallVector() noexcept {
        if(size_>dynamic_threshold)
            A::deallocate(dynamic_.ptr,dynamic_.cap);
    }

    bool empty() const {return size_==0;};
    size_type size() const {return size_;}

    typedef pointer iterator;
    typedef const_pointer const_iterator;

    iterator begin(){
        return size_>dynamic_threshold ? dynamic_.ptr : std::launder(reinterpret_cast<T*>(static_.arr));
    }
    iterator end() {return begin()+size_;}
    const_iterator begin() const {
        return size_>dynamic_threshold ? dynamic_.ptr : std::launder(reinterpret_cast<T*>(static_.arr));
    }
    const_iterator end() const {return begin()+size_;}

    T& operator[](const size_type i) {assert(i<size_); return begin()[i];}
    const T& operator[](const size_type i) const {assert(i<size_); return begin()[i];}

    void push_back(const T& t){
        if(size_>dynamic_threshold){
            if(size_==dynamic_.cap){
                const pointer ptr_new = A::allocate(2*dynamic_.cap);
                yuki::move_no_overlap(ptr_new,dynamic_.ptr,size_);
                A::deallocate(dynamic_.ptr,dynamic_.cap);
                dynamic_.ptr=ptr_new;
                dynamic_.cap*=2;
            }
            ::new(dynamic_.ptr+size_) T(t);
        }else{
            if(size_==dynamic_threshold){
                const pointer ptr_new = A::allocate(2*dynamic_threshold);
                yuki::move_no_overlap(ptr_new,static_.arr,dynamic_threshold);
                dynamic_.ptr=ptr_new;
                dynamic_.cap=2*dynamic_threshold;
                ::new(dynamic_.ptr+size_) T(t);
            }else
                ::new(static_.arr+size_) T(t);
        }
        ++size_;
    }
};

static_assert(sizeof(SmallVector<unsigned short>)==24);
static_assert(SmallVector<unsigned short>::dynamic_threshold==8);

int main(){
    SmallVector<unsigned short> foo;

    foo.push_back(2);
    foo.push_back(3);
    foo.push_back(5);
    foo.push_back(7);
    foo.push_back(11);
    foo.push_back(13);
    foo.push_back(17);
    foo.push_back(19);
    foo.push_back(23);
    //foo.push_back(29);

    for(const unsigned short s : foo)
        printf("%u ",(unsigned)s);
}
