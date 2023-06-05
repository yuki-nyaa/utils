// Experimental
#pragma once
#include<type_traits>
#include<atomic>
namespace yuki{
template<typename T>
struct default_delete{
    void operator()(const T* const p) {delete p;}
};
template<typename T>
struct default_delete<T[]>{
    void operator()(const T* const p) {delete[] p;}
};
template<typename T,typename D = default_delete<T>>
struct shared_ptr : private D{
    static_assert(std::is_empty_v<D>);
  private:
    T* p;
    std::atomic<unsigned>* r;
  public:
    shared_ptr(T* const p2 = nullptr) noexcept : p(p2),r(nullptr) {
        if(p2){
            r = new std::atomic<unsigned>;
            r->store(1);
        }
    }
    shared_ptr(const shared_ptr& other) noexcept : p(other.p), r(other.r) {
        if(r)
            r->fetch_add(1);
    }
    shared_ptr(shared_ptr&& other) noexcept : p(other.p), r(other.r) {
        other.p = nullptr;
        other.r = nullptr;
    }
    ~shared_ptr() noexcept {
        if(r){
            r->fetch_sub(1);
            if(r->load()==0){
                this->operator()(p);
                delete r;
                r = nullptr;
            }
        }
    }

    shared_ptr& operator=(T* const p2) noexcept {
        this->~shared_ptr();
        if((p=p2)){
            r = new std::atomic<unsigned>;
            r->store(1);
        }
    }
    shared_ptr& operator=(const shared_ptr& other) noexcept {
        if(this!=&other)
            this->operator=(other.p);
    }
    shared_ptr& operator=(shared_ptr&& other) noexcept {
        if(this!=&other){
            this->~shared_ptr();
            p = other.p;
            r = other.r;
            other.p = nullptr;
            other.r = nullptr;
        }
    }

    unsigned use_count() const noexcept {
        if(r)
            return r->load();
        else
            return 0;
    }
    bool is_null() const noexcept {return !static_cast<bool>(r);}
    T& operator*() const noexcept {return *p;}
    T* operator->() const noexcept {return p;}
    T& operator[](const size_t i) const noexcept {return p[i];}
}; // struct shared_ptr_<T,D>
} // namespace yuki
