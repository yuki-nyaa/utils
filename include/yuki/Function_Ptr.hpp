#pragma once
#include<yuki/tmp.hpp>
#include<cassert>

namespace yuki{
enum struct FP_Storage_Type {FP,N,VP};

template<typename T>
struct Function_Ref;

template<typename T>
struct Function_Ptr;

template<typename R,typename... Args>
struct Function_Ptr<R(Args...)>{
  private:
    union storage_t{
        R(*fp_)(Args...);
        void* v_;
    } storage_;
    R(*fwd_)(storage_t,Args...);

    static constexpr R fp_action(const storage_t s,Args... args) {return s.fp_(args...);}
  public:
    Function_Ptr() noexcept = default;

    template<typename U,typename=std::enable_if_t<!std::is_same_v<std::remove_cvref_t<U>,Function_Ptr<R(Args...)>>>>
    constexpr Function_Ptr(U&& u) noexcept :
        storage_{.v_=const_cast<void*>(static_cast<const void*>(&u))},
        fwd_([](const storage_t s,Args... args)->R {return static_cast<U&&>((*static_cast<std::remove_reference_t<U>*>(s.v_)))(args...);})
    {}

    template<typename U,typename=std::enable_if_t<!std::is_same_v<std::remove_cvref_t<U>,Function_Ptr<R(Args...)>>>> requires std::is_convertible_v<U&&,R(*)(Args...)>
    constexpr Function_Ptr(U&& u) noexcept :
        storage_{.fp_=static_cast<R(*)(Args...)>(std::forward<U>(u))},
        fwd_(fp_action)
    {}

    constexpr Function_Ptr(R(*fp)(Args...)) noexcept :
        storage_{.fp_=fp},
        fwd_(fp_action)
    {}

    constexpr Function_Ptr(std::nullptr_t) noexcept :
        storage_{.fp_=nullptr},
        fwd_(nullptr)
    {}

    constexpr Function_Ptr(const Function_Ptr<R(Args...)noexcept> fp_noexcept) noexcept : Function_Ptr(fp_noexcept.fp_except) {}

    constexpr Function_Ptr(const Function_Ref<R(Args...)> fr) noexcept : Function_Ptr(static_cast<const Function_Ptr&>(fr)) {}
    constexpr Function_Ptr(const Function_Ref<R(Args...)noexcept> fr) noexcept : Function_Ptr(static_cast<const Function_Ptr<R(Args...)noexcept>&>(fr).fp_except) {}

    /// Member pointers are not supported. You should wrap them in lambdas to achieve the same effect.
    template<typename M,typename C>
    Function_Ptr(M C::*) = delete;

    constexpr R operator()(Args... args) const {assert(fwd_); return fwd_(storage_,args...);}

    explicit constexpr operator bool() const noexcept {return static_cast<bool>(fwd_);}

    friend constexpr bool operator==(const Function_Ptr lhs,const Function_Ptr rhs) noexcept {
        if(lhs.fwd_==rhs.fwd_){
            if(lhs.fwd_==fp_action)
                return lhs.storage_.fp_==rhs.storage_.fp_;
            if(lhs.fwd_==nullptr)
                return true;
            return lhs.storage_.v_==rhs.storage_.v_;
        }
        return false;
    }

    /// For debugging only.
    constexpr FP_Storage_Type storage_type() const {
        if(fwd_==fp_action)
            return FP_Storage_Type::FP;
        if(fwd_==nullptr)
            return FP_Storage_Type::N;
        return FP_Storage_Type::VP;
    }
};

template<typename R,typename... Args>
struct Function_Ptr<R(Args...)noexcept>{
    friend struct Function_Ptr<R(Args...)>;
  private:
    Function_Ptr<R(Args...)> fp_except;
  public:
    Function_Ptr() noexcept = default;

    template<typename U,typename=std::enable_if_t<!std::is_same_v<std::remove_cvref_t<U>,Function_Ptr<R(Args...)noexcept>>>>
    constexpr Function_Ptr(U&& u) noexcept :
        fp_except(std::forward<U>(u))
    {static_assert(noexcept(static_cast<U&&>(u)(yuki::declval<Args>()...)));}

    Function_Ptr(R(*fp)(Args...)) = delete;

    constexpr Function_Ptr(std::nullptr_t) noexcept : fp_except(nullptr) {}

    Function_Ptr(Function_Ptr<R(Args...)>) = delete;

    template<typename M,typename C>
    Function_Ptr(M C::*) = delete;

    constexpr R operator()(Args... args) const noexcept {return fp_except(args...);}

    explicit constexpr operator bool() const noexcept {return static_cast<bool>(fp_except);}

    friend constexpr bool operator==(Function_Ptr,Function_Ptr) noexcept = default;

    friend constexpr bool operator==(const Function_Ptr<R(Args...)> lhs,const Function_Ptr<R(Args...)noexcept> rhs) noexcept {return lhs==rhs.fp_except;}

    constexpr FP_Storage_Type storage_type() const {return fp_except.storage_type();}
};

//template<typename R,typename... Args>
//struct Function_Ptr<R(Args...)noexcept> : private Function_Ptr<R(Args...)>{
//    friend struct Function_Ptr<R(Args...)>;
//  private:
//   typedef Function_Ptr<R(Args...)> FP_EXCEPT;
//  public:
//    using FP_EXCEPT::FP_EXCEPT;

//    Function_Ptr(R(*fp)(Args...)) = delete;

//    Function_Ptr(Function_Ptr<R(Args...)>) = delete;

//    using FP_EXCEPT::operator();
//    using FP_EXCEPT::operator bool;

//    using FP_EXCEPT::storage_type;

//    friend constexpr bool operator==(Function_Ptr,Function_Ptr) noexcept = default;

//    friend constexpr bool operator==(const Function_Ptr<R(Args...)> lhs,const Function_Ptr<R(Args...)noexcept> rhs) noexcept {return lhs==static_cast<Function_Ptr<R(Args...)>>(rhs);}
//};

static_assert(std::is_trivial_v<Function_Ptr<int(int)>>);
static_assert(std::is_standard_layout_v<Function_Ptr<int(int)>>);
static_assert(std::is_trivial_v<Function_Ptr<int(int)noexcept>>);
static_assert(std::is_standard_layout_v<Function_Ptr<int(int)noexcept>>);


template<typename T>
struct Function_Ref : private Function_Ptr<T>{
    template<typename U>
    friend struct Function_Ptr;
  private:
    typedef Function_Ptr<T> FP_;
  public:
    using FP_::FP_;

    Function_Ref() = delete;
    Function_Ref(std::nullptr_t) = delete;

    constexpr Function_Ref(const Function_Ptr<T> fp) noexcept : Function_Ptr<T>(fp) {}

    constexpr Function_Ref(const Function_Ref&) noexcept = default;
    Function_Ref& operator=(const Function_Ref&) = delete;

    using FP_::operator();
    using FP_::storage_type;
};
static_assert(std::is_trivially_copyable_v<Function_Ref<int(int)>>);
static_assert(std::is_standard_layout_v<Function_Ref<int(int)>>);
static_assert(std::is_trivially_copyable_v<Function_Ref<int(int)noexcept>>);
static_assert(std::is_standard_layout_v<Function_Ref<int(int)noexcept>>);
} // namespace yuki
