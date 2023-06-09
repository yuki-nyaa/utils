#pragma once
#include<type_traits>
#include<yuki/Allocator.hpp>
#include<yuki/Pair.hpp>
#include<yuki/functional.hpp>
#include<iterator>

namespace yuki{
template<typename V>
struct RB_Tree_Node{
    typedef RB_Tree_Node* pointer;

    V value;
    bool is_black = false;
    RB_Tree_Node* parent = nullptr;
    RB_Tree_Node* left = nullptr;
    RB_Tree_Node* right = nullptr;
};

namespace tree_op{
    template<typename P>
    P min(P x){
        assert(x);
        while(x->left)
            x = x->left;
        return x;
    }

    template<typename P>
    P max(P x){
        assert(x);
        while(x->right)
            x = x->right;
        return x;
    }

    template<typename P>
    P next(P p){
        assert(p);
        if(p->right){
            return min(p->right);
        }else{
            P x = p;
            p = x->parent;
            while(p && x==p->right){
                x = p;
                p = p->parent;
            }
            return p;
        }
    }

    template<typename P>
    P prev(P p){
        assert(p);
        if(p->left){
            return max(p->left);
        }else{
            P x = p;
            p = x->parent;
            while(p && x==p->left){
                x = p;
                p = p->parent;
            }
            return p;
        }
    }

    template<typename P>
    constexpr bool is_black(const P p) {return !p || p->is_black;}

    template<typename P>
    constexpr bool is_red(const P p) {return p && !p->is_black;}
}; // namespace tree_op

template<typename P>
struct Tree_Iterator{
    P base;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef std::remove_reference_t<decltype((base->value))> value_type;
    typedef decltype(base-base) difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    constexpr Tree_Iterator() noexcept = default;
    constexpr Tree_Iterator(const P p) noexcept : base(p) {}

    explicit constexpr operator bool() const noexcept {return static_cast<bool>(base);}

    constexpr value_type& operator*() const {return base->value;}
    constexpr value_type* operator->() const {return std::addressof(base->value);}
    Tree_Iterator& operator++() {base = tree_op::next(base); return *this;}
    Tree_Iterator& operator--() {base = tree_op::prev(base); return *this;}
    Tree_Iterator operator++(int) {const Tree_Iterator tmp = *this; operator++(); return tmp;}
    Tree_Iterator operator--(int) {const Tree_Iterator tmp = *this; operator--(); return tmp;}

    friend constexpr bool operator==(Tree_Iterator,Tree_Iterator) = default;

    constexpr operator Tree_Iterator<add_lconst_t<P>>() const noexcept {return base;}

    friend constexpr Tree_Iterator<remove_lconst_t<P>> const_kast(const Tree_Iterator<P> it) {using yuki::const_kast; return {const_kast(it.base)};}
};

template<typename P>
struct add_lconst<Tree_Iterator<P>> {typedef Tree_Iterator<add_lconst_t<P>> type;};

template<typename K,typename V,typename KV,typename C=yuki::Less<K>,typename A=yuki::Allocator<RB_Tree_Node<V>>>
struct RB_Tree : protected KV,protected C,protected A{
    typedef K key_type;
    typedef V value_type;
    typedef KV key_of_value_type;
    typedef C key_compare;
    typedef A allocator_type;

    typedef RB_Tree_Node<V> node_type;
    static_assert(std::is_same_v<node_type,typename A::value_type>);

    typedef typename A::pointer pointer;
    typedef typename A::const_pointer const_pointer;
    typedef typename A::size_type size_type;
    typedef typename A::difference_type difference_type;

    static_assert(std::is_same_v<pointer,node_type*>);

    constexpr RB_Tree() noexcept = default;

    RB_Tree(const RB_Tree& other) noexcept :
        KV(yuki::select_on_container_copy_construction<KV>(other)),
        C(yuki::select_on_container_copy_construction<C>(other)),
        A(yuki::select_on_container_copy_construction<A>(other)),
        root_(nullptr),
        s_(other.s_)
    {
        root_ = clone(other.root_,nullptr);
    }

    constexpr RB_Tree(RB_Tree&& other) noexcept :
        KV(static_cast<KV&&>(other)),
        C(static_cast<C&&>(other)),
        A(static_cast<A&&>(other)),
        root_(other.root_),
        s_(other.s_)
    {
        other.root_ = nullptr;
        other.s_ = 0;
    }

    ~RB_Tree() noexcept {clear();}

    friend constexpr void swap(RB_Tree& lhs,RB_Tree& rhs) noexcept {
        using std::swap;

        if constexpr(A::propagate_on_container_swap)
            swap(static_cast<A&>(lhs),static_cast<A&>(rhs));
        else
            if(static_cast<const A&>(lhs)!=static_cast<const A&>(rhs)){
                RB_Tree temp=std::move(rhs);
                rhs=std::move(lhs);
                lhs=std::move(temp);
                return;
            }

        swap(static_cast<KV&>(lhs),static_cast<KV&>(rhs));
        swap(static_cast<C&>(lhs),static_cast<C&>(rhs));
        swap(lhs.root_,rhs.root_);
        swap(lhs.s_,rhs.s_);
    }

    RB_Tree& operator=(const RB_Tree& other) noexcept {
        if(this!=&other){
            KV::operator=(other);
            C::operator=(other);
            clear_at(root_);
            yuki::clear_alloc<A>(*this);
            if constexpr(A::propagate_on_container_copy_assignment)
                A::operator=(other);
            s_ = other.s_;
            root_ = clone(other.root_,nullptr);
        }
        return *this;
    }

    RB_Tree& operator=(RB_Tree&& other) noexcept {
        if(this!=&other){
            KV::operator=(static_cast<KV&&>(other));
            C::operator=(static_cast<C&&>(other));
            clear_at(root_);
            yuki::clear_alloc<A>(*this);
            if constexpr(A::propagate_on_container_move_assignment){
                A::operator=(static_cast<A&&>(other));
            }else{
                if(static_cast<const A&>(*this)!=static_cast<const A&>(other)){
                    s_ = other.s_;
                    root_ = take_other(other.root_,nullptr,other);
                    other.root_ = nullptr;
                    other.s_ = 0;
                    return *this;
                }
            }
            root_ = other.root_;
            s_ = other.s_;
            other.root_ = nullptr;
            other.s_ = 0;
        }
        return *this;
    }

    void clear() {clear_at(root_); yuki::clear_alloc<A>(*this); root_=nullptr; s_=0;}

    typedef Tree_Iterator<pointer> non_const_iterator;
    typedef Tree_Iterator<const_pointer> const_iterator;
    static_assert(std::is_same_v<typename const_iterator::value_type,const V>);

    constexpr bool empty() const {return !root_;}
    constexpr size_type size() const {return s_;}

    const_iterator begin() const {return min();}
    constexpr const_iterator end() const {return nullptr;}

    /// @note `f` should not change the keys.
    template<typename F>
    void unordered_traverse(F&& f){unordered_traverse_at(root_,std::forward<F>(f));}
    template<typename F>
    void unordered_traverse(F&& f) const {unordered_traverse_at(root_,std::forward<F>(f));}
  private:
    /// @note `f` should not change the keys.
    template<typename F>
    void unordered_traverse_at(const pointer p,F&& f){
        if(p){
            std::forward<F>(f)(p->value);
            unordered_traverse_at(p->left,std::forward<F>(f));
            unordered_traverse_at(p->right,std::forward<F>(f));
        }
    }
    template<typename F>
    void unordered_traverse_at(const const_pointer p,F&& f) const {
        if(p){
            std::forward<F>(f)(p->value);
            unordered_traverse_at(p->left,std::forward<F>(f));
            unordered_traverse_at(p->right,std::forward<F>(f));
        }
    }
  public:
    constexpr const_iterator root() const {return root_;}

    const_iterator min() const {return root_? tree_op::min(root_) : nullptr;}
    const_iterator max() const {return root_? tree_op::max(root_) : nullptr;}

    template<typename K2>
    const_iterator first_greater_tp(const K2& k) const {
        pointer last = nullptr;
        pointer current = root_;
        while(current){
            if(C::operator()(k,KV::operator()(current->value))){
                last = current;
                current = current->left;
            }else
                current = current->right;
        }
        return last;
    }
    const_iterator first_greater(const K& k) const {return first_greater_tp(k);}
    template<typename K2>
    const_iterator first_greater(const K2& k) const requires requires{typename C::is_transparent;}
        {return first_greater_tp(k);}

    template<typename K2>
    const_iterator first_equiv_greater_tp(const K2& k) const {
        pointer last = nullptr;
        pointer current = root_;
        while(current){
            if(!C::operator()(KV::operator()(current->value),k)){
                last = current;
                current = current->left;
            }else
                current = current->right;
        }
        return last;
    }
    const_iterator first_equiv_greater(const K& k) const {return first_equiv_greater_tp(k);}
    template<typename K2>
    const_iterator first_equiv_greater(const K2& k) const requires requires{typename C::is_transparent;}
        {return first_equiv_greater_tp(k);}

    template<typename K2>
    const_iterator find_any_tp(const K2& k) const {
        const yuki::IB_Pair<pointer> ibp = insert_leaf_unique(k);
        return !ibp.has_inserted ? ibp.iterator : nullptr;
    }
    const_iterator find_any(const K& k) const {return find_any_tp(k);}
    template<typename K2>
    const_iterator find_any(const K2& k) const requires requires{typename C::is_transparent;}
        {return find_any_tp(k);}

    template<typename K2>
    const_iterator find_first_tp(const K2& k) const {
        const const_iterator it = first_equiv_greater_tp(k);
        return (it.base && !C::operator()(k,KV::operator()(it.base->value))) ? it : const_iterator{nullptr};
    }
    const_iterator find_first(const K& k) const {return find_first_tp(k);}
    template<typename K2>
    const_iterator find_first(const K2& k) const requires requires{typename C::is_transparent;}
        {return find_first_tp(k);}

    template<typename K2>
    bool contains_tp(const K2& k) const {return !insert_leaf_unique(k).first;}
    bool contains(const K& k) const {return contains_tp(k);}
    template<typename K2>
    bool contains(const K2& k) const requires requires{typename C::is_transparent;}
        {return contains_tp(k);}

    template<typename K2>
    size_type count_tp(const K2& k) const {
        if(const_iterator feg=first_equiv_greater_tp(k); feg){
            const const_iterator fg = first_greater_tp(k);
            size_type c=0;
            for(;feg!=fg;++feg,++c)
                ;
            return c;
        }
        return 0;
    }
    size_type count(const K& k) const {return count_tp(k);}
    template<typename K2>
    size_type count(const K2& k) const requires requires{typename C::is_transparent;}
        {return count_tp(k);}

    template<typename... Args>
    const_iterator emplace(Args&&... args){
        const pointer z = A::allocate();
        using yuki::iterator_unwrap;
        ::new(iterator_unwrap(z)) node_type{{std::forward<Args>(args)...}};
        insert_at(z,insert_leaf(KV::operator()(z->value)));
        return z;
    }

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to first parameter.
    template<typename K2,typename... Args>
    const_iterator emplace_at_tp(const K2&,Args&&... args) {return emplace(std::forward<Args>(args)...);}
    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to first parameter.
    template<typename... Args>
    const_iterator emplace_at(const K&,Args&&... args) {return emplace(std::forward<Args>(args)...);}
    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to first parameter.
    template<typename K2,typename... Args>
    const_iterator emplace_at(const K2&,Args&&... args) requires requires{typename C::is_transparent;}
        {return emplace(std::forward<Args>(args)...);}

    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename K2,typename... Args>
    yuki::IB_Pair<const_iterator> emplace_unique_at_tp(const K2& k,Args&&... args){
        const yuki::IB_Pair<pointer> ibp = insert_leaf_unique(k);
        if(ibp.has_inserted){
            const pointer z = A::allocate();
            using yuki::iterator_unwrap;
            ::new(iterator_unwrap(z)) node_type{{std::forward<Args>(args)...}};
            insert_at(z,ibp.iterator);
            return {z,ibp.has_inserted};
        }else{
            return {ibp.iterator,ibp.has_inserted};
        }
    }
    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename... Args>
    yuki::IB_Pair<const_iterator> emplace_unique_at(const K& k,Args&&... args){
        return emplace_unique_at_tp(k,std::forward<Args>(args)...);
    }
    /// @pre `KV::operator()(V(std::forward<Args>(args)...))` should be equivalent to `k`.
    template<typename K2,typename... Args>
    yuki::IB_Pair<const_iterator> emplace_unique_at(const K2& k,Args&&... args) requires requires{typename C::is_transparent;}
        {return emplace_unique_at_tp(k,std::forward<Args>(args)...);}

    yuki::IB_Pair<const_iterator> insert_unique(const V& v) {return emplace_unique_at(KV::operator()(v),v);}

    yuki::IB_Pair<const_iterator> insert_unique(V&& v) {return emplace_unique_at(KV::operator()(v),std::move(v));}

    void erase(const_iterator zi){
        assert(zi.base);

        using yuki::const_kast;
        const pointer z = const_kast(zi.base);
        pointer y = z;
        pointer x;
        bool y_is_black = y->is_black;
        yuki::Pair<pointer,bool> xp_xr;
        if(!z->left){
            x = z->right;
            xp_xr = transplant(z,x);
        }else if(!z->right){
            x = z->left;
            xp_xr = transplant(z,x);
        }else{
            y = tree_op::min(z->right);
            y_is_black = y->is_black;
            x = y->right;
            pointer yp = y->parent;
            if(yp!=z){
                yp->left = x;
                if(x)
                    x->parent = yp;
                y->right = z->right;
                y->right->parent = y;
                xp_xr = {yp,false};
            }else{
                xp_xr = {y,true};
            }
            transplant(z,y);
            y->left = z->left;
            y->left->parent = y;
            y->is_black = z->is_black;
        }
        z->~node_type();
        A::deallocate(z);
        if(y_is_black)
            erase_fixup(x,xp_xr.zeroth,xp_xr.first);
        --s_;
    }

    template<typename K2>
    size_type erase_tp(const K2& k){
        const_iterator feg = first_equiv_greater_tp(k);
        const const_iterator fg = first_greater_tp(k);
        size_type s = 0;
        for(;feg!=fg;++feg,++s)
            erase(feg);
        return s;
    }
    size_type erase(const K& k) {return erase_tp(k);}
    template<typename K2>
    size_type erase(const K2& k) requires requires{typename C::is_transparent;}
        {return erase_tp(k);}

    template<typename K2>
    size_type erase_unique_tp(const K2& k){
        if(const const_iterator i=find_any_tp(k); i!=end()){
            erase(i);
            return 1;
        }
        return 0;
    }
    size_type erase_unique(const K& k) {return erase_unique_tp(k);}
    template<typename K2>
    size_type erase_unique(const K2& k) requires requires{typename C::is_transparent;}
        {return erase_unique_tp(k);}

    template<typename K2,typename V2,typename KV2,typename C2,typename A2>
    void merge(const RB_Tree<K2,V2,KV2,C2,A2>& other) {merge_copy<A2>(other.root_);}

    /// @pre `this!=&other`
    template<typename K2,typename V2,typename KV2,typename C2,typename A2>
    void merge(RB_Tree<K2,V2,KV2,C2,A2>&& other){
        assert(this!=&other);
        if constexpr(std::is_same_v<RB_Tree<K,V,KV,C,A>,RB_Tree<K2,V2,KV2,C2,A2>>){
            if((A::propagate_on_container_swap || static_cast<const A&>(*this)==static_cast<const A&>(other)) && s_<other.s_)
                swap(*this,other);
        }
        if constexpr(std::is_same_v<A,A2> && std::is_empty_v<A>)
            merge_splice(other.root_);
        else
            merge_move<A2>(other.root_,other);
        other.root_ = nullptr;
        other.s_ = 0;
    }

    template<typename K2,typename KV2,typename C2,typename A2,typename... M>
    void merge_unique(const RB_Tree<K2,V,KV2,C2,A2>& other,M&&... m){
        merge_copy_unique<A2>(other.root_,std::forward<M>(m)...);
    }

    /// @pre `this!=&other`
    template<typename K2,typename KV2,typename C2,typename A2,typename... M>
    void merge_unique(RB_Tree<K2,V,KV2,C2,A2>&& other,M&&... m){
        assert(this!=&other);
        if constexpr(std::is_same_v<RB_Tree<K,V,KV,C,A>,RB_Tree<K2,V,KV2,C2,A2>>){
            if((A::propagate_on_container_swap || static_cast<const A&>(*this)==static_cast<const A&>(other)) && s_<other.s_)
                swap(*this,other);
        }
        if constexpr(std::is_same_v<A,A2> && std::is_empty_v<A>)
            merge_splice_unique(other.root_,other,std::forward<M>(m)...);
        else
            merge_move_unique<A2>(other.root_,other,std::forward<M>(m)...);
        other.root_ = nullptr;
        other.s_ = 0;
    }

    template<typename K2,typename V2,typename KV2,typename C2,typename A2>
    friend struct RB_Tree;

    template<typename Target=RB_Tree,typename Before,typename... After>
    Target isomorphic_copy(Before&& before,After&&... after) const {
        Target ret;
        ret.s_ = s_;
        ret.root_ = ret.clone(root_,nullptr,std::forward<Before>(before),std::forward<After>(after)...);
        return ret;
    }
  private:
    pointer insert_leaf(const K& k) const {
        pointer y = nullptr;
        pointer x = root_;
        while(x){
            y = x;
            if(C::operator()(k,KV::operator()(x->value)))
                x = x->left;
            else
                x = x->right;
        }
        return y;
    }

    template<typename K2>
    yuki::IB_Pair<pointer> insert_leaf_unique(const K2& k) const {
        pointer y = nullptr;
        pointer x = root_;
        while(x){
            y = x;
            if(C::operator()(k,KV::operator()(x->value)))
                x = x->left;
            else if(C::operator()(KV::operator()(x->value),k))
                x = x->right;
            else
                return {y,false};
        }
        return {y,true};
    }

    void insert_at(const pointer z,const pointer y){
        z->parent = y;
        if(!y)
            root_ = z;
        else if(C::operator()(KV::operator()(z->value),KV::operator()(y->value)))
            y->left = z;
        else
            y->right = z;
        insert_fixup(z);
        ++s_;
    }

    void left_rotate(const pointer x){
        assert(x->right);
        const pointer y = x->right;
        x->right = y->left;
        if(y->left)
            y->left->parent = x;
        y->parent = x->parent;
        if(!x->parent)
            root_ = y;
        else if(x==x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void right_rotate(const pointer y){
        assert(y->left);
        const pointer x = y->left;
        y->left = x->right;
        if(x->right)
            x->right->parent = y;
        x->parent = y->parent;
        if(!y->parent)
            root_ = x;
        else if(y==y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        x->right = y;
        y->parent = x;
    }

    void insert_fixup(pointer z){
        pointer zp = z->parent;;
        while(tree_op::is_red(zp) && zp->parent){
            if(zp==zp->parent->left){
                pointer y = zp->parent->right;
                if(tree_op::is_red(y)){
                    zp->is_black = true;
                    y->is_black = true;
                    zp->parent->is_black = false;
                    z = zp->parent;
                }else{
                    if(z==zp->right){
                        z = zp;
                        left_rotate(z);
                        zp=z->parent;
                    }
                    zp->is_black = true;
                    zp->parent->is_black = false;
                    right_rotate(zp->parent);
                }
            }else{
                pointer y = zp->parent->left;
                if(tree_op::is_red(y)){
                    zp->is_black = true;
                    y->is_black = true;
                    zp->parent->is_black = false;
                    z = zp->parent;
                }else{
                    if(z==zp->left){
                        z = zp;
                        right_rotate(z);
                        zp=z->parent;
                    }
                    zp->is_black = true;
                    zp->parent->is_black = false;
                    left_rotate(zp->parent);
                }
            }
            zp=z->parent;
        }
        root_->is_black = true;
    }

    yuki::Pair<pointer,bool> transplant(const const_pointer u,const pointer v){
        const pointer up = u->parent;
        bool ur = false;
        if(!up)
            root_ = v;
        else if(u==up->left)
            up->left = v;
        else
            ur = true, up->right = v;

        if(v)
            v->parent = up;

        return {up,ur};
    }

    void erase_fixup(pointer x,pointer xp,bool xr){
        pointer w;
        while(x!=root_ && tree_op::is_black(x)){
            if(!xr){
                w = xp->right;
                if(!w->is_black){
                    w->is_black = true;
                    xp->is_black = false;
                    left_rotate(xp);
                    w = xp->right;
                }
                if(tree_op::is_black(w->left) && tree_op::is_black(w->right)){
                    w->is_black = false;
                    x = xp;
                    xp = x->parent;
                    xr = (x==xp->right);
                }else{
                    if(tree_op::is_black(w->right)){
                        w->left->is_black = true;
                        w->is_black = false;
                        right_rotate(w);
                        w = xp->right;
                    }
                    w->is_black = xp->is_black;
                    xp->is_black = true;
                    w->right->is_black = true;
                    left_rotate(xp);
                    x = root_;
                }
            }else{
                w = xp->left;
                if(!w->is_black){
                    w->is_black = true;
                    xp->is_black = false;
                    right_rotate(xp);
                    w = xp->left;
                }
                if(tree_op::is_black(w->left) && tree_op::is_black(w->right)){
                    w->is_black = false;
                    x = xp;
                    xp = x->parent;
                    xr = (x==xp->right);
                }else{
                    if(tree_op::is_black(w->left)){
                        w->right->is_black = true;
                        w->is_black = false;
                        left_rotate(w);
                        w = xp->left;
                    }
                    w->is_black = xp->is_black;
                    xp->is_black = true;
                    w->left->is_black = true;
                    right_rotate(xp);
                    x = root_;
                }
            }
        }
        x->is_black = true;
    }

    void clear_at(const const_pointer p){
        if(p){
            clear_at(p->left);
            clear_at(p->right);
            p->~node_type();
            A::deallocate(p);
        }
    }

    template<typename pointer2>
    pointer clone(const pointer2 other,const pointer parent){
        if(other){
            const pointer n = A::allocate();
            using yuki::iterator_unwrap;
            ::new(iterator_unwrap(n)) node_type{other->value,other->is_black,parent,clone(other->left,n),clone(other->right,n)};
            return n;
        }
        return nullptr;
    }

    template<typename pointer2,typename Before,typename... After>
    pointer clone(const pointer2 other,const pointer parent,Before&& before,After&&... after){
        if(other){
            const pointer n = A::allocate();
            using yuki::iterator_unwrap;
            ::new(iterator_unwrap(n)) node_type{std::forward<Before>(before)(other->value),other->is_black,parent,clone(other->left,n,std::forward<Before>(before),std::forward<After>(after)...),clone(other->right,n,std::forward<Before>(before),std::forward<After>(after)...)};
            (...,std::forward<After>(after)(n->value));
            return n;
        }
        return nullptr;
    }

    pointer take_other(const pointer other,const pointer parent,A& a_other){
        if(other){
            const pointer n = A::allocate();
            using yuki::iterator_unwrap;
            ::new(iterator_unwrap(n)) node_type{std::move(other->value),other->is_black,parent,take_other(other->left,n),take_other(other->right,n)};
            other->~node_type();
            a_other.deallocate(other);
            return n;
        }
        return nullptr;
    }

    template<typename A2>
    void merge_copy(const typename A2::const_pointer other){
        if(other){
            emplace(other->value);
            merge_copy<A2>(other->left);
            merge_copy<A2>(other->right);
        }
    }

    template<typename A2>
    void merge_move(const typename A2::pointer other,A2& a_other){
        if(other){
            emplace(std::move(other->value));
            merge_move<A2>(other->left,a_other);
            merge_move<A2>(other->right,a_other);
            typedef std::remove_reference_t<decltype(*other)> node_type2;
            other->~node_type2();
            a_other.deallocate(other);
        }
    }

    void merge_splice(const pointer other){
        if(other){
            merge_splice(other->left);
            merge_splice(other->right);
            other->left = nullptr;
            other->right = nullptr;
            other->is_black = false;
            insert_at(other,insert_leaf(KV::operator()(other->value)));
        }
    }

    template<typename V2,typename... M>
    void insert_or_merge(V2&& v,M&&... m){
        const yuki::IB_Pair<pointer> ibp = insert_leaf_unique(KV::operator()(std::forward<V2>(v)));
        if(ibp.has_inserted){
            const pointer z = A::allocate();
            using yuki::iterator_unwrap;
            ::new(iterator_unwrap(z)) node_type{std::forward<V2>(v)};
            insert_at(z,ibp.iterator);
        }else{
            (...,std::forward<M>(m)(ibp.iterator->value,std::forward<V2>(v)));
        }
    }

    template<typename A2,typename... M>
    void merge_copy_unique(const typename A2::const_pointer other,M&&... m){
        if(other){
            insert_or_merge(other->value,std::forward<M>(m)...);
            merge_copy_unique<A2>(other->left,std::forward<M>(m)...);
            merge_copy_unique<A2>(other->right,std::forward<M>(m)...);
        }
    }

    template<typename A2,typename... M>
    void merge_move_unique(const typename A2::pointer other,A2& a_other,M&&... m){
        if(other){
            insert_or_merge(std::move(other->value),std::forward<M>(m)...);
            merge_move_unique<A2>(other->left,a_other,std::forward<M>(m)...);
            merge_move_unique<A2>(other->right,a_other,std::forward<M>(m)...);
            typedef std::remove_reference_t<decltype(*other)> node_type2;
            other->~node_type2();
            a_other.deallocate(other);
        }
    }

    template<typename... M>
    void merge_splice_unique(const pointer other,A& a_other,M&&... m){
        if(other){
            merge_splice_unique(other->left,a_other,std::forward<M>(m)...);
            merge_splice_unique(other->right,a_other,std::forward<M>(m)...);
            const yuki::IB_Pair<pointer> ibp = insert_leaf_unique(KV::operator()(other->value));
            if(ibp.has_inserted){
                other->left = nullptr;
                other->right = nullptr;
                other->is_black = false;
                insert_at(other,ibp.iterator);
            }else{
                (...,std::forward<M>(m)(ibp.iterator->value,std::move(other->value)));
                other->~node_type();
                a_other.deallocate(other);
            }
        }
    }

  private:
    pointer root_ = nullptr;
    size_type s_ = 0;
}; // struct RB_Tree<K,V,KV,C,A>
} // namespace yuki
