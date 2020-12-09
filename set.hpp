#pragma once
#include<cassert>
#include<set>
#include<algorithm>
#ifdef YUKI_UTILS_DEV
#include"utils.hpp"
#else
#include<yuki/utils.hpp>
#endif
namespace yuki{
    template<
        typename Key,
        typename Compare = std::less<Key>,
        typename Equal = std::conditional_t<(requires(const Key& key1,const Key& key2){std::equal_to<Key>{}(key1,key2);}),std::equal_to<Key>,yuki::equiv<Key,Compare>>,
        typename Allocator = std::allocator<Key>
    >
    struct multiset : private std::multiset<Key,Compare,Allocator> {
        private:
            typedef std::multiset<Key,Compare,Allocator> Base_;
            constexpr void flag_true_() noexcept {has_equiv_=true;has_equal_=true;}
            constexpr void flag_false_() noexcept {has_equiv_=false;has_equal_=false;}
        public:
            using typename Base_::key_type;
            using typename Base_::size_type;
            using typename Base_::difference_type;
            using typename Base_::key_compare;
            using typename Base_::allocator_type;
            using typename Base_::reference;
            using typename Base_::const_reference;
            using typename Base_::pointer;
            using typename Base_::const_pointer;
            using typename Base_::iterator;
            using typename Base_::const_iterator;
            using typename Base_::reverse_iterator;
            using typename Base_::const_reverse_iterator;
            using typename Base_::node_type;
            typedef Equal key_equal;
        protected:
            Compare key_compare_;
            Equal key_equal_;
            Allocator alloc_;
            bool has_equiv_ = false;
            bool has_equal_ = false;
        public:
            multiset() : multiset(Compare(),Equal()) {}

            explicit multiset(const Compare& comp, const Equal& eq, const Allocator& alloc = Allocator()) : Base_(comp,alloc),key_compare_(comp),key_equal_(eq),alloc_(alloc){}

            template<typename InputIterator>
            multiset(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Equal& eq = Equal(), const Allocator& alloc = Allocator()) : Base_(first,last,comp,alloc),key_compare_(comp),key_equal_(eq),alloc_(alloc) {}

            multiset(const multiset&) = default;

            multiset(multiset&&) = default;

            explicit multiset(const Allocator& alloc) : multiset(Compare(),Equal(),alloc) {}

            multiset(const multiset& other, const Allocator& alloc) : Base_(other,alloc), key_equal_(other.key_equal_), alloc_(alloc) {}

            multiset(multiset&& other, const Allocator& alloc) : Base_(other,alloc), key_equal_(other.key_equal_), alloc_(alloc) {}

            multiset(std::initializer_list<key_type> il, const Compare& comp = Compare(), const Equal& eq = Equal(), const Allocator& alloc = Allocator()) : Base_(il,comp,alloc), key_compare_(comp),key_equal_(eq),alloc_(alloc) {}

            template<typename InputIterator>
            multiset(InputIterator first, InputIterator last, const Allocator& alloc) : multiset(first,last,Compare(),Equal(),alloc) {}

            multiset(std::initializer_list<key_type> il, const Allocator& alloc) : multiset(il,Compare(),Equal(),alloc) {}

            ~multiset() = default;

            multiset& operator=(const multiset&) = default;

            multiset& operator=(multiset&&)
                noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_move_assignable_v<Compare> && std::is_nothrow_move_assignable_v<Equal>)
                = default;

            multiset& operator=(std::initializer_list<key_type> il){Base_::operator=(il); has_equiv_=true; has_equal_=true; return *this;}

            using Base_::get_allocator;

            using Base_::begin;
            using Base_::end;
            using Base_::rbegin;
            using Base_::rend;
            using Base_::cbegin;
            using Base_::cend;
            using Base_::crbegin;
            using Base_::crend;

            using Base_::empty;
            using Base_::size;
            using Base_::max_size;

            using Base_::extract;

            using Base_::erase;
            size_type erase(const key_type& x) = delete;
            // There are `erase_equiv` and `erase_equal`, see later.

            void swap(multiset& other)
                noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable_v<Compare> && std::is_nothrow_swappable_v<Equal>)
            {
                Base_::swap(other);
                using std::swap;
                swap(key_compare_,other.key_compare_);
                swap(key_equal_,other.key_equal_);
                swap(alloc_,other.alloc_);
                swap(has_equiv_,other.has_equiv_);
                swap(has_equal_,other.has_equal_);
            }

            void clear() noexcept {flag_false_();Base_::clear();}

            template<typename C2,typename E2>
            void merge(multiset<Key,C2,E2,Allocator>& other){
                has_equiv_ = has_equiv_ || other.has_equiv_;
                has_equal_ = has_equal_ || other.has_equal_;
                Base_::merge(other);
            }
            template<typename C2,typename E2>
            void merge(multiset<Key,C2,E2,Allocator>&& other){
                has_equiv_ = has_equiv_ || other.has_equiv_;
                has_equal_ = has_equal_ || other.has_equal_;
                Base_::merge(std::move(other));
            }

            using Base_::key_comp;

            key_equal key_eq() const {return key_equal_;}

            bool equiv(const Key& lhs, const Key& rhs){return !key_compare_(lhs,rhs) && !key_compare_(rhs,lhs);}
            bool equal(const Key& lhs, const Key& rhs){return key_equal_(lhs,rhs);}

            using Base_::lower_bound;
            //iterator lower_bound(const Key& key);
            //const_iterator lower_bound(const Key& key) const;
            //template<typename K> requires requires{typename Compare::is_transparent;}
            //iterator lower_bound(const K& key);
            //template<typename K> requires requires{typename Compare::is_transparent;}
            //const_iterator lower_bound(const K& key) const;

            using Base_::upper_bound;
            //iterator upper_bound(const Key& key);
            //const_iterator upper_bound(const Key& key) const;
            //template<typename K> requires requires{typename Compare::is_transparent;}
            //iterator upper_bound(const K& key);
            //template<typename K> requires requires{typename Compare::is_transparent;}
            //const_iterator upper_bound(const K& key) const;

            std::pair<iterator,iterator> equiv_range(const Key& key){return {lower_bound(key),upper_bound(key)};}
            std::pair<const_iterator,const_iterator> equiv_range(const Key& key) const {return {lower_bound(key),upper_bound(key)};}
            template<typename K> requires requires{typename Compare::is_transparent;}
            std::pair<iterator,iterator> equiv_range(const K& key){return {lower_bound(key),upper_bound(key)};}
            template<typename K> requires requires{typename Compare::is_transparent;}
            std::pair<const_iterator,const_iterator> equiv_range(const K& key) const {return {lower_bound(key),upper_bound(key)};}

            iterator find_equiv(const Key& key){return Base_::find(key);}
            const_iterator find_equiv(const Key& key) const {return Base_::find(key);}
            template<typename K> requires requires{typename Compare::is_transparent;}
            iterator find_equiv(const K& key){return Base_::find(key);}
            template<typename K> requires requires{typename Compare::is_transparent;}
            const_iterator find_equiv(const K& key) const {return Base_::find(key);}

            iterator find_equal(const Key& key){
                return std::find_if(lower_bound(key),upper_bound(key),[this,&key](const Key& key2){return key_equal_(key,key2);});
            }
            const_iterator find_equal(const Key& key) const {
                return std::find_if(lower_bound(key),upper_bound(key),[this,&key](const Key& key2){return key_equal_(key,key2);});
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            iterator find_equal(const K& key){
                return std::find_if(lower_bound(key),upper_bound(key),[this,&key](const Key& key2){return key_equal_(key,key2);});
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            const_iterator find_equal(const K& key) const {
                return std::find_if(lower_bound(key),upper_bound(key),[this,&key](const Key& key2){return key_equal_(key,key2);});
            }

            size_type count_equiv(const Key& key) const {return Base_::count(key);}
            template<typename K> requires requires{typename Compare::is_transparent;}
            size_type count_equiv(const K& key) const {return Base_::count(key);}

            size_type count_equal(const Key& key) const {
                return yuki::count_if(lower_bound(key),upper_bound(key),[this,&key](const Key& key2){return key_equal_(key,key2);});
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            size_type count_equal(const K& key) const {
                return yuki::count_if(lower_bound(key),upper_bound(key),[this,&key](const Key& key2){return key_equal_(key,key2);});
            }

            bool contains_equiv(const Key& key) const {return Base_::contains(key);}
            template<typename K> requires requires{typename Compare::is_transparent;}
            bool contains_equiv(const K& key) const {return Base_::contains(key);}

            bool contains_equal(const Key& key) const {
                auto lb=lower_bound(key);
                auto ub=upper_bound(key);
                for(;lb!=ub;++lb){
                    if(key_equal_(key,*lb))
                        return true;
                }
                return false;
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            bool contains_equal(const K& key) const {
                auto lb=lower_bound(key);
                auto ub=upper_bound(key);
                for(;lb!=ub;++lb){
                    if(key_equal_(key,*lb))
                        return true;
                }
                return false;
            }

            size_type erase_equiv(const Key& key){return Base_::erase(key);}
            size_type erase_equal(const Key& key){
                auto lb=lower_bound(key);
                auto ub=upper_bound(key);
                size_type n=0;
                for(;lb!=ub;++lb){
                    if(key_equal_(key,*lb)){
                        erase(lb);
                        ++n;
                    }
                }
                return n;
            }

            iterator insert(const key_type& key){flag_true_(); return Base_::insert(key);}
            iterator insert(key_type&& key){flag_true_(); return Base_::insert(std::move(key));}
            iterator insert(const_iterator hint, const key_type& key){flag_true_(); return Base_::insert(hint,key);}
            iterator insert(const_iterator hint, key_type&& key){flag_true_(); return Base_::insert(hint,std::move(key));}
            template<typename InputIt>
            void insert(InputIt first, InputIt last){flag_true_(); return Base_::insert(first,last);}
            void insert(std::initializer_list<key_type> il){flag_true_(); return Base_::insert(il);}
            iterator insert(node_type&& nh){flag_true_(); return Base_::insert(std::move(nh));}
            iterator insert(const_iterator hint, node_type&& nh){flag_true_(); return Base_::insert(hint,std::move(nh));}

            template<bool signal_suc>
            auto insert_equiv(const key_type& key) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                auto ub = upper_bound(key);
                if(ub==begin() || key_compare_(*(--ub),key)){
                    YUKI_CONDITIONAL_RETURN(flag,YUKI_PROTECT({insert(ub,key),true}),YUKI_PROTECT(insert(ub,key)))
                }else{
                    if constexpr(signal_suc)
                        return {--ub,false};
                    else
                        return --ub;
                }
            }
            template<bool signal_suc>
            auto insert_equiv(key_type&& key) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                auto ub = upper_bound(key);
                if(ub==begin() || key_compare_(*(--ub),key)){
                    if constexpr(signal_suc)
                        return {insert(ub,std::move(key)),true};
                    else
                        return insert(ub,std::move(key));
                }else{
                    if constexpr(signal_suc)
                        return {--ub,false};
                    else
                        return --ub;
                }
            }
            iterator insert_equiv(const_iterator hint, const key_type& key){
                if(key_compare_(key,*hint) && key_compare_(*--hint,key))
                    return Base_::insert(hint,key)
                else if()
            }
            iterator insert_equiv(const_iterator hint, key_type&& key);
            template<typename InputIt>
            void insert_equiv(InputIt first, InputIt last);
            void insert_equiv(std::initializer_list<key_type> il);
            iterator insert_equiv(node_type&& nh);
            iterator insert_equiv(const_iterator hint, node_type&& nh);

            iterator insert_equal(const key_type& key);
            iterator insert_equal(key_type&& key);
            iterator insert_equal(const_iterator hint, const key_type& key);
            iterator insert_equal(const_iterator hint, key_type&& key);
            template<typename InputIt>
            void insert_equal(InputIt first, InputIt last);
            void insert_equal(std::initializer_list<key_type> il);
            iterator insert_equal(node_type&& nh);
            iterator insert_equal(const_iterator hint, node_type&& nh);

            template<typename... Args>
            iterator emplace(Args&&... args);

            template<typename... Args>
            iterator emplace_equiv(Args&&... args);

            template<typename... Args>
            iterator emplace_equal(Args&&... args);

            template<typename... Args>
            iterator emplace_hint(const_iterator hint,Args&&... args);

            template<typename... Args>
            iterator emplace_hint_equiv(const_iterator hint,Args&&... args);

            template<typename... Args>
            iterator emplace_hint_equal(const_iterator hint,Args&&... args);

            constexpr bool potentially_has_equivs() const noexcept {return has_equiv_;}
            constexpr bool potentially_has_equals() const noexcept {assert(!has_equal_ || has_equiv_); return has_equal_;}
    };

    template<typename Key,typename Compare,typename Equal,typename Allocator>
    void swap(multiset<Key,Compare,Equal,Allocator>& lhs,multiset<Key,Compare,Equal,Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs))) {lhs.swap(rhs);}

    template<typename Key,typename Compare,typename Equal,typename Alloc,typename Pred>
    auto erase_if(multiset<Key,Compare,Equal,Alloc>& ms,Pred pred) -> typename multiset<Key,Compare,Equal,Alloc>::size_type {
        typename multiset<Key,Compare,Equal,Alloc>::size_type old_size = ms.size();
        for (auto i = ms.begin(), last = ms.end(); i != last; ){
            if (pred(*i)){
              i = ms.erase(i);
            }else{
              ++i;
            }
        }
        return old_size - ms.size();
    }
}