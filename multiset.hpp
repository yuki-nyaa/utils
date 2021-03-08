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
        typename Equal = std::equal_to<Key>,
        // Not implemented by compiler:
        // typename Equal = std::conditional_t<(requires(const Key& key1,const Key& key2){std::equal_to<Key>{}(key1,key2);}),std::equal_to<Key>,yuki::equiv<Key,Compare>>,
        typename Allocator = std::allocator<Key>
    >
    struct multiset : private std::multiset<Key,Compare,Allocator> {
        protected:
            typedef std::multiset<Key,Compare,Allocator> std_multiset_base_;
        public:
            using typename std_multiset_base_::key_type;
            using typename std_multiset_base_::size_type;
            using typename std_multiset_base_::difference_type;
            using typename std_multiset_base_::key_compare;
            using typename std_multiset_base_::allocator_type;
            using typename std_multiset_base_::reference;
            using typename std_multiset_base_::const_reference;
            using typename std_multiset_base_::pointer;
            using typename std_multiset_base_::const_pointer;
            using typename std_multiset_base_::iterator;
            using typename std_multiset_base_::const_iterator;
            using typename std_multiset_base_::reverse_iterator;
            using typename std_multiset_base_::const_reverse_iterator;
            using typename std_multiset_base_::node_type;
            typedef Equal key_equal;
        protected:
            Compare key_compare_;
            Equal key_equal_;
            Allocator alloc_;
            // Note: The following two flags only represents POTENTIAl equiv/equal occurrence, i.e. if the flag is false, then one can be sure that there are no equivs/equals, but if the flag is true, then both are possible. For this reason, the 2 flags can appear seemingly inconsistent, i.e. `p_has_equal_=true` while `p_has_equiv_=false`, in which case one can be sure that there are no equivs, but nothing could be said about equals.
            // For this reason, it only makes sense to set them from `false` to `true`.
            mutable bool p_has_equiv_ = false;
            mutable bool p_has_equal_ = false;
            constexpr void flags_true_() noexcept {p_has_equal_=true;p_has_equiv_=true;}
            constexpr void flags_false_() noexcept {p_has_equiv_=false;p_has_equal_=false;}
        public:
            multiset() : multiset(Compare(),Equal()) {}

            explicit multiset(const Compare& comp, const Equal& eq, const Allocator& alloc = Allocator()) : std_multiset_base_(comp,alloc),key_compare_(comp),key_equal_(eq),alloc_(alloc){}

            template<typename InputIterator>
            multiset(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Equal& eq = Equal(), const Allocator& alloc = Allocator()) : std_multiset_base_(first,last,comp,alloc),key_compare_(comp),key_equal_(eq),alloc_(alloc),p_has_equiv_(true),p_has_equal_(true) {}

            multiset(const multiset&) = default;

            multiset(multiset&&) = default;

            explicit multiset(const Allocator& alloc) : multiset(Compare(),Equal(),alloc) {}

            multiset(const multiset& other, const Allocator& alloc) : std_multiset_base_(other,alloc), key_equal_(other.key_equal_), alloc_(alloc) {}

            multiset(multiset&& other, const Allocator& alloc) : std_multiset_base_(other,alloc), key_equal_(other.key_equal_), alloc_(alloc) {}

            multiset(std::initializer_list<key_type> il, const Compare& comp = Compare(), const Equal& eq = Equal(), const Allocator& alloc = Allocator()) : std_multiset_base_(il,comp,alloc), key_compare_(comp),key_equal_(eq),alloc_(alloc),p_has_equiv_(true),p_has_equal_(true) {}

            template<typename InputIterator>
            multiset(InputIterator first, InputIterator last, const Allocator& alloc) : multiset(first,last,Compare(),Equal(),alloc) {}

            multiset(std::initializer_list<key_type> il, const Allocator& alloc) : multiset(il,Compare(),Equal(),alloc) {}

            ~multiset() = default;

            multiset& operator=(const multiset&) = default;

            multiset& operator=(multiset&&)
                noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_move_assignable_v<Compare> && std::is_nothrow_move_assignable_v<Equal>)
                = default;

            multiset& operator=(std::initializer_list<key_type> il){std_multiset_base_::operator=(il); p_has_equiv_=true; p_has_equal_=true; return *this;}

            using std_multiset_base_::get_allocator;

            using std_multiset_base_::begin;
            using std_multiset_base_::end;
            using std_multiset_base_::rbegin;
            using std_multiset_base_::rend;
            using std_multiset_base_::cbegin;
            using std_multiset_base_::cend;
            using std_multiset_base_::crbegin;
            using std_multiset_base_::crend;

            using std_multiset_base_::empty;
            using std_multiset_base_::size;
            using std_multiset_base_::max_size;

            using std_multiset_base_::extract;

            using std_multiset_base_::erase;
            size_type erase(const key_type& x) = delete;
            // There are `erase_equiv` and `erase_equal`, see later.

            void swap(multiset& other)
                noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable_v<Compare> && std::is_nothrow_swappable_v<Equal>)
            {
                std_multiset_base_::swap(other);
                using std::swap;
                swap(key_compare_,other.key_compare_);
                swap(key_equal_,other.key_equal_);
                swap(alloc_,other.alloc_);
                swap(p_has_equiv_,other.p_has_equiv_);
                swap(p_has_equal_,other.p_has_equal_);
            }

            void clear() noexcept {flags_false_();std_multiset_base_::clear();}

            template<typename C2,typename E2>
            void merge(multiset<Key,C2,E2,Allocator>& other){
                p_has_equiv_ = p_has_equiv_ || other.p_has_equiv_;
                p_has_equal_ = p_has_equal_ || other.p_has_equal_;
                std_multiset_base_::merge(other);
            }
            template<typename C2,typename E2>
            void merge(multiset<Key,C2,E2,Allocator>&& other){
                p_has_equiv_ = p_has_equiv_ || other.p_has_equiv_;
                p_has_equal_ = p_has_equal_ || other.p_has_equal_;
                std_multiset_base_::merge(std::move(other));
            }

            using std_multiset_base_::key_comp;

            key_equal key_eq() const {return key_equal_;}

            bool equiv(const Key& lhs, const Key& rhs){return !key_compare_(lhs,rhs) && !key_compare_(rhs,lhs);}
            bool equal(const Key& lhs, const Key& rhs){return key_equal_(lhs,rhs);}

            // Summary of the various look-up functions.
            // "<=>" denotes "duality", `->` denotes derivation.
            // `first_greater`(primitive) <=> `last_smaller_equiv` -> `last_equiv`
            // `first_equiv_greater`(primitive) <=> `last_smaller`
            //      ... -> `first_equiv`

            // `upper_bound` are given a more straightforward and more appropriate name.
            iterator first_greater(const Key& key) {return std_multiset_base_::upper_bound(key);}
            const_iterator first_greater(const Key& key) const {return std_multiset_base_::upper_bound(key);}
            template<typename K> requires requires{typename Compare::is_transparent;}
            iterator first_greater(const K& key) {return std_multiset_base_::upper_bound(key);}
            template<typename K> requires requires{typename Compare::is_transparent;}
            const_iterator first_greater(const K& key) const {return std_multiset_base_::upper_bound(key);}

            // 'lower_bound' are also given a more straightforward and more appropriate name.
            // Although in English people usually say "greater than or equivalent to", I decide to make the word order conform to the actual order.
            iterator first_equiv_greater(const Key& key) {return std_multiset_base_::lower_bound(key);}
            const_iterator first_equiv_greater(const Key& key) const {return std_multiset_base_::lower_bound(key);}
            template<typename K> requires requires{typename Compare::is_transparent;}
            iterator first_equiv_greater(const K& key) {return std_multiset_base_::lower_bound(key);}
            template<typename K> requires requires{typename Compare::is_transparent;}
            const_iterator first_equiv_greater(const K& key) const {return std_multiset_base_::lower_bound(key);}

            // `equiv_range` are removed because they are rather useless. Why not just call `lower_bound`(`first_equiv_greater`) and `upper_bound`(`first_greater`) directly.

            // The "dual" of `first_greater`, so to speak.
            iterator last_smaller_equiv(const Key& key){
                iterator feg = first_greater(key);
                if(feg!=begin())
                    return --feg;
                else
                    return end();
            }
            const_iterator last_smaller_equiv(const Key& key) const {
                const_iterator feg = first_greater(key);
                if(feg!=cbegin())
                    return --feg;
                else
                    return cend();
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            iterator last_smaller_equiv(const K& key){
                iterator feg = first_greater(key);
                if(feg!=begin())
                    return --feg;
                else
                    return end();
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            const_iterator last_smaller_equiv(const K& key) const {
                const_iterator feg = first_greater(key);
                if(feg!=cbegin())
                    return --feg;
                else
                    return cend();
            }

            // The "dual" of `first_equiv_greater`, so to speak.
            iterator last_smaller(const Key& key){
                iterator feg = first_equiv_greater(key);
                if(feg!=begin())
                    return --feg;
                else
                    return end();
            }
            const_iterator last_smaller(const Key& key) const {
                const_iterator feg = first_equiv_greater(key);
                if(feg!=cbegin())
                    return --feg;
                else
                    return cend();
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            iterator last_smaller(const K& key){
                iterator feg = first_equiv_greater(key);
                if(feg!=begin())
                    return --feg;
                else
                    return end();
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            const_iterator last_smaller(const K& key) const {
                const_iterator feg = first_equiv_greater(key);
                if(feg!=cbegin())
                    return --feg;
                else
                    return cend();
            }

            // `find` split into 2 groups and are also given a more straightforward name. Note that the standard does not specify which element should be returned by `find` when there are multiple equivalent elements. Now I make that clear.
            iterator first_equiv(const Key& key){
                iterator feg = first_equiv_greater(key);
                if(feg!=end() && !key_compare_(key,*feg)) // Note that by definition `*feg>=key`, therefore `!key_compare_(*feg,key)`.
                    return feg;
                else
                    return end();
            }
            const_iterator first_equiv(const Key& key) const {
                const_iterator feg = first_equiv_greater(key);
                if(feg!=cend() && !key_compare_(key,*feg))
                    return feg;
                else
                    return cend();
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            iterator first_equiv(const K& key){
                iterator feg = first_equiv_greater(key);
                if(feg!=end() && !key_compare_(key,*feg))
                    return feg;
                else
                    return end();
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            const_iterator first_equiv(const K& key) const {
                const_iterator feg = first_equiv_greater(key);
                if(feg!=cend() && !key_compare_(key,*feg))
                    return feg;
                else
                    return cend();
            }

            iterator last_equiv(const Key& key){
                iterator lse = last_smaller_equiv(key);
                if(lse!=end() && !key_compare_(*lse,key)) // Note that by definition `*lse<=key`, therefore `!key_compare_(key,*lse)`.
                    return lse;
                else
                    return end();
            }
            const_iterator last_equiv(const Key& key) const {
                const_iterator lse = last_smaller_equiv(key);
                if(lse!=cend() && !key_compare_(*lse,key))
                    return lse;
                else
                    return cend();
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            iterator last_equiv(const K& key){
                iterator lse = last_smaller_equiv(key);
                if(lse!=end() && !key_compare_(*lse,key))
                    return lse;
                else
                    return end();
            }
            template<typename K> requires requires{typename Compare::is_transparent;}
            const_iterator last_equiv(const K& key) const {
                const_iterator lse = last_smaller_equiv(key);
                if(lse!=cend() && !key_compare_(*lse,key))
                    return lse;
                else
                    return cend();
            }


            iterator first_equal(const Key& key){
                iterator fequiv = first_equiv(key);
                if(fequiv!=end()){
                    iterator fg = first_greater(key);
                    // No need to check if `fg==end()`.
                    iterator index = std::find_if(fequiv,fg,[this,&key](const Key& rhs){return key_equal_(key,rhs);});
                    if(index==fg)
                        return end();
                    else
                        return index;
                }else{
                    return end();
                }
            }
            const_iterator first_equal(const Key& key) const {
                iterator fequiv = first_equiv(key);
                if(fequiv!=end()){
                    iterator fg = first_greater(key);
                    iterator index = std::find_if(fequiv,fg,[this,&key](const Key& rhs){return key_equal_(key,rhs);});
                    if(index==fg)
                        return end();
                    else
                        return index;
                }else{
                    return end();
                }
            }
            // TODO
            //template<typename K> requires requires{typename Compare::is_transparent;}
            //iterator first_equal(const K& key){
            //    iterator fequiv = first_equiv(key);
            //    if(fequiv!=end()){
            //        iterator fg = first_greater(key);
            //        iterator index = std::find_if(fequiv,fg,[this,&key](const Key& rhs){return key_equal_(key,rhs);});
            //        if(index==fg)
            //            return end();
            //        else
            //            return index;
            //    }else{
            //        return end();
            //    }
            //}
            //template<typename K> requires requires{typename Compare::is_transparent;}
            //const_iterator first_equal(const K& key) const {
            //    iterator fequiv = first_equiv(key);
            //    if(fequiv!=end()){
            //        iterator fg = first_greater(key);
            //        iterator index = std::find_if(fequiv,fg,[this,&key](const Key& rhs){return key_equal_(key,rhs);});
            //        if(index==fg)
            //            return end();
            //        else
            //            return index;
            //    }else{
            //        return end();
            //    }
            //}

            iterator last_equal(const Key& key){
                iterator lequiv = last_equiv(key);
                if(lequiv!=end()){
                    // Because there isn't a "before-begin" iterator, the loop below iterates through a CLOSED interval, which looks a bit unintuitive. Using reverse-iterators is also inelegant (you could try) and extremely error-prone since there is one offset.
                    iterator feg = first_equiv_greater(key);
                    // No need to check if `feg==end()` because there is at least one element greater or equiv to `key`, namely `lequiv`.
                    do{
                        if(key_equal_(key,*lequiv))
                            return lequiv;
                    }while((lequiv!=feg)?(--lequiv,true):false);
                    return end();
                }else{
                    return end();
                }
            }
            const_iterator last_equal(const Key& key) const {
                iterator lequiv = last_equiv(key);
                if(lequiv!=end()){
                    iterator feg = first_equiv_greater(key);
                    do{
                        if(key_equal_(key,*lequiv))
                            return lequiv;
                    }while((lequiv!=feg)?(--lequiv,true):false);
                    return end();
                }else{
                    return end();
                }
            }
            // TODO
            //template<typename K> requires requires{typename Compare::is_transparent;}
            //iterator last_equal(const K& key){
            //    iterator lequiv = last_equiv(key);
            //    if(lequiv!=end()){
            //        iterator ls = last_smaller(key);
            //        for(;lequiv!=ls;--lequiv){
            //            if(key_equal_(key,*lequiv))
            //                return lequiv;
            //        }
            //        return end();
            //    }else{
            //        return end();
            //    }
            //}
            //template<typename K> requires requires{typename Compare::is_transparent;}
            //const_iterator last_equal(const K& key) const {
            //    iterator lequiv = last_equiv(key);
            //    if(lequiv!=end()){
            //        iterator ls = last_smaller(key);
            //        for(;lequiv!=ls;--lequiv){
            //            if(key_equal_(key,*lequiv))
            //                return lequiv;
            //        }
            //        return end();
            //    }else{
            //        return end();
            //    }
            //}

            size_type count_equiv(const Key& key) const {return std_multiset_base_::count(key);}
            template<typename K> requires requires{typename Compare::is_transparent;}
            size_type count_equiv(const K& key) const {return std_multiset_base_::count(key);}

            size_type count_equal(const Key& key) const {
                iterator fequiv = first_equiv(key);
                if(fequiv!=end()){
                    return yuki::count_if(fequiv,first_greater(key),[this,&key](const Key& rhs){return key_equal_(key,rhs);});
                }else{
                    return 0;
                }
            }
            // TODO
            //template<typename K> requires requires{typename Compare::is_transparent;}
            //size_type count_equal(const K& key) const {
            //    iterator fequiv = first_equiv(key);
            //    if(fequiv!=end()){
            //        return yuki::count_if(fequiv,first_greater(key),[this,&key](const Key& rhs){return key_equal_(key,rhs);});
            //    }else{
            //        return 0;
            //    }
            //}

            bool contains_equiv(const Key& key) const {return std_multiset_base_::contains(key);}
            template<typename K> requires requires{typename Compare::is_transparent;}
            bool contains_equiv(const K& key) const {return std_multiset_base_::contains(key);}
            bool contains_equiv() const {
                iterator a = std::adjacent_find(begin(),end(),[this](const Key& lhs,const Key& rhs){return !key_compare_(lhs,rhs);}); // Note that `lhs<=rhs` by the property of `std::multiset`, therefore `!key_compare_(rhs,lhs)`.
                if(a!=end()){
                    p_has_equiv_=true;
                    return true;
                }else{
                    p_has_equiv_=false;
                    p_has_equal_=false;
                    return false;
                }
            }

            bool contains_equal(const Key& key) const {
                iterator fe = first_equal(key);
                if(fe!=end())
                    return true;
                else
                    return false;
            }
            // TODO
            //template<typename K> requires requires{typename Compare::is_transparent;}
            //bool contains_equal(const K& key) const {
            //    iterator fe = first_equal(key);
            //    if(fe!=end())
            //        return true;
            //    else
            //        return false;
            //}
            bool contains_equal() const {
                // A very stupid algorithm.
                //std::vector<iterator> seg_initials; seg_initials.reserve(size());
                //{
                //iterator seg_initial = begin();
                //iterator e = end();
                //while(seg_initial!=e)
                //    seg_initials.push_back(seg_initial);
                //    seg_initial = first_greater(*seg_initial);
                //}
                //seg_initials.push_back(std::move(e));
                //auto contains_equal_seg = [](const iterator& ini,const iterator& f){
                //    iterator first = ini;

                //}
                // TODO
                return true;
            }
            std::pair<bool,bool> contains_equiv_equal() const {
                // TODO
                return {true,true};
            }

            size_type erase_equiv(const Key& key){return std_multiset_base_::erase(key);}
            size_type erase_equal(const Key& key){
                iterator fequiv = first_equiv(key);
                if(fequiv!=end()){
                    iterator fg = first_greater(key);
                    size_type n=0;
                    for(;fequiv!=fg;){
                        if(key_equal_(key,*fequiv)){
                            fequiv=erase(fequiv);
                            ++n;
                        }else{
                            ++fequiv;
                        }
                    }
                    return n;
                }else{
                    return 0;
                }
            }


            iterator insert(const key_type& key) {flags_true_(); return std_multiset_base_::insert(key);}
            iterator insert(key_type&& key) {flags_true_(); return std_multiset_base_::insert(std::move(key));}
            iterator insert(const_iterator hint, const key_type& key) {flags_true_(); return std_multiset_base_::insert(hint,key);}
            iterator insert(const_iterator hint, key_type&& key) {flags_true_(); return std_multiset_base_::insert(hint,std::move(key));}
            template<typename InputIt>
            void insert(InputIt first, InputIt last) {flags_true_(); return std_multiset_base_::insert(first,last);}
            void insert(std::initializer_list<key_type> il) {flags_true_(); return std_multiset_base_::insert(il);}
            iterator insert(node_type&& nh) {flags_true_(); return std_multiset_base_::insert(std::move(nh));}
            iterator insert(const_iterator hint, node_type&& nh) {flags_true_(); return std_multiset_base_::insert(hint,std::move(nh));}

            template<bool signal_suc=false>
            auto insert_equiv(const key_type& key) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                iterator fg = first_greater(key);
                iterator le_candidate = --fg;
                if(fg==begin() || key_compare_(*le_candidate,key)){
                    p_has_equal_=true;
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(fg,key),true}),std_multiset_base_::insert(fg,key)); // Note that `std_multiset_base_::insert` does not change the flags.
                }else{
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({le_candidate,false}),le_candidate);
                }
            }
            template<bool signal_suc=false>
            auto insert_equiv(key_type&& key) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                iterator fg = first_greater(key);
                iterator le_candidate = --fg;
                if(fg==begin() || key_compare_(*le_candidate,key)){
                    p_has_equal_=true;
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(fg,std::move(key)),true}),std_multiset_base_::insert(fg,std::move(key)));
                }else{
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({le_candidate,false}),le_candidate);
                }
            }
            template<bool signal_suc=false>
            auto insert_equiv(const_iterator hint, const key_type& key) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                iterator equiv_i = first_equiv(key);
                if(equiv_i==end()){
                    p_has_equal_=true;
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(hint,key),true}),std_multiset_base_::insert(hint,key));
                }else{
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({equiv_i,false}),equiv_i);
                }
            }
            template<bool signal_suc>
            auto insert_equiv(const_iterator hint, key_type&& key) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                iterator equiv_i = first_equiv(key);
                if(equiv_i==end()){
                    p_has_equal_=true;
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(hint,std::move(key)),true}),std_multiset_base_::insert(hint,std::move(key)));
                }else{
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({equiv_i,false}),equiv_i);
                }
            }
            template<typename InputIt>
            void insert_equiv(InputIt first, InputIt last){
                for(;first!=last;++first)
                    insert_equiv(*first);
            }
            void insert_equiv(std::initializer_list<key_type> il){
                for(const key_type& key : il)
                    insert_equiv(key);
            }
            template<bool signal_suc>
            auto insert_equiv(node_type&& nh) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                if(nh.empty()){
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({end(),false}),end());
                }else{
                    // Same as `insert_equiv(key_type&&)`.
                    key_type& key = nh.value();
                    iterator fg = first_greater(key);
                    iterator le_candidate = --fg;
                    if(fg==begin() || key_compare_(*le_candidate,key)){
                        p_has_equal_=true;
                        YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(fg,std::move(nh)),true}),std_multiset_base_::insert(fg,std::move(nh)));
                    }else{
                        YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({le_candidate,false}),le_candidate);
                    }
                }
            }
            template<bool signal_suc>
            auto insert_equiv(const_iterator hint, node_type&& nh) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                if(nh.empty()){
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({end(),false}),end());
                }else{
                    // Same as `insert_equiv(const_iterator,key_type&&)`.
                    iterator fe = first_equiv(nh.value());
                    if(fe==end()){
                        p_has_equal_=true;
                        YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(hint,std::move(nh)),true}),std_multiset_base_::insert(hint,std::move(nh)));
                    }else{
                        YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({fe,false}),fe);
                    }
                }
            }


            template<bool signal_suc=false>
            auto insert_equal(const key_type& key) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                iterator lequal = last_equal(key);
                if(lequal==end()){
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(first_greater(key),key),true}),std_multiset_base_::insert(first_greater(key),key));
                }else{
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({lequal,false}),lequal);
                }
            }
            template<bool signal_suc=false>
            auto insert_equal(key_type&& key) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                iterator lequal = last_equal(key);
                if(lequal==end()){
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(first_greater(key),std::move(key)),true}),std_multiset_base_::insert(first_greater(key),std::move(key)));
                }else{
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({lequal,false}),lequal);
                }
            }
            template<bool signal_suc=false>
            auto insert_equal(const_iterator hint, const key_type& key) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                iterator lequal = last_equal(key);
                if(lequal==end()){
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(hint,key),true}),std_multiset_base_::insert(hint,key));
                }else{
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({lequal,false}),lequal);
                }
            }
            template<bool signal_suc>
            auto insert_equal(const_iterator hint, key_type&& key) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                iterator lequal = last_equal(key);
                if(lequal==end()){
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(hint,std::move(key)),true}),std_multiset_base_::insert(hint,std::move(key)));
                }else{
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({lequal,false}),lequal);
                }
            }
            template<typename InputIt>
            void insert_equal(InputIt first, InputIt last){
                for(;first!=last;++first)
                    insert_equal(*first);
            }
            void insert_equal(std::initializer_list<key_type> il){
                for(const key_type& key : il)
                    insert_equal(key);
            }
            template<bool signal_suc>
            auto insert_equal(node_type&& nh) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                if(nh.empty()){
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({end(),false}),end());
                }else{
                    // Same as `insert_equiv(key_type&&)`.
                    iterator lequal = last_equal(nh.value());
                    if(lequal==end()){
                        YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(first_greater(nh.value()),std::move(nh)),true}),std_multiset_base_::insert(first_greater(nh.value()),std::move(nh)));
                    }else{
                        YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({lequal,false}),lequal);
                    }
                }
            }
            template<bool signal_suc>
            auto insert_equal(const_iterator hint, node_type&& nh) -> std::conditional_t<signal_suc,std::pair<iterator,bool>,iterator> {
                if(nh.empty()){
                    YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({end(),false}),end());
                }else{
                    // Same as `insert_equiv(const_iterator,key_type&&)`.
                    iterator lequal = last_equal(nh.value());
                    if(lequal==end()){
                        YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({std_multiset_base_::insert(hint,std::move(nh)),true}),std_multiset_base_::insert(hint,std::move(nh)));
                    }else{
                        YUKI_CONDITIONAL_RETURN(signal_suc,YUKI_PROTECT({lequal,false}),lequal);
                    }
                }
            }

            template<typename... Args>
            iterator emplace(Args&&... args) {flags_true_(); return std_multiset_base_::emplace(std::forward<Args>(args)...);}

            template<typename... Args>
            iterator emplace_equiv(Args&&... args) {return insert_equiv(key_type(std::forward<Args>(args)...));}

            template<typename... Args>
            iterator emplace_equal(Args&&... args) {return insert_equal(key_type(std::forward<Args>(args)...));}

            template<typename... Args>
            iterator emplace_hint(const_iterator hint,Args&&... args) {flags_true_(); return std_multiset_base_::emplace(hint,std::forward<Args>(args)...);}

            template<typename... Args>
            iterator emplace_hint_equiv(const_iterator hint,Args&&... args) {return insert_equiv(hint,key_type(std::forward<Args>(args)...));}

            template<typename... Args>
            iterator emplace_hint_equal(const_iterator hint,Args&&... args) {return insert_equal(hint,key_type(std::forward<Args>(args)...));}

            constexpr bool potentially_has_equivs() const noexcept {return p_has_equiv_;}
            constexpr bool potentially_has_equals() const noexcept {return p_has_equal_;}
    }; // struct multiset : private std::multiset<Key,Compare,Allocator>

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

    // TODO deduction guides

    // using all
    //template<
    //    typename Key,
    //    typename Compare = std::less<Key>,
    //    typename Equal = std::equal_to<Key>,
    //    typename Allocator = std::allocator<Key>
    //> struct Foo : protected yuki::multiset<Key,Compare,Equal,Allocator> {
    //    protected:
    //        typedef yuki::multiset<Key,Compare,Allocator> yuki_multiset_base_;
    //    public:
    //        using typename yuki_multiset_base_::key_type;
    //        using typename yuki_multiset_base_::size_type;
    //        using typename yuki_multiset_base_::difference_type;
    //        using typename yuki_multiset_base_::key_compare;
    //        using typename yuki_multiset_base_::allocator_type;
    //        using typename yuki_multiset_base_::reference;
    //        using typename yuki_multiset_base_::const_reference;
    //        using typename yuki_multiset_base_::pointer;
    //        using typename yuki_multiset_base_::const_pointer;
    //        using typename yuki_multiset_base_::iterator;
    //        using typename yuki_multiset_base_::const_iterator;
    //        using typename yuki_multiset_base_::reverse_iterator;
    //        using typename yuki_multiset_base_::const_reverse_iterator;
    //        using typename yuki_multiset_base_::node_type;
    //        using typename yuki_multiset_base_::key_equal;
    //    //protected:
    //    //    Compare key_compare_;
    //    //    Equal key_equal_;
    //    //    Allocator alloc_;
    //    //    mutable bool p_has_equiv_ = false;
    //    //    mutable bool p_has_equal_ = false;
    //    //    constexpr void flags_true_() noexcept {p_has_equal_=true;p_has_equiv_=true;}
    //    //    constexpr void flags_false_() noexcept {p_has_equiv_=false;p_has_equal_=false;}
    //    public:
    //        Foo() : Foo(key_compare(),key_equal()) {}

    //        explicit Foo(const key_compare& comp, const key_equal& eq, const allocator_type& alloc = allocator_type()) : yuki_multiset_base_(comp,eq,alloc) {}

    //        template<typename InputIterator>
    //        Foo(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const key_equal& eq = key_equal(), const allocator_type& alloc = allocator_type()) : yuki_multiset_base_(first,last,comp,eq,alloc) {}

    //        Foo(const Foo&) = default;

    //        Foo(Foo&&) = default;

    //        explicit Foo(const allocator_type& alloc) : Foo(key_compare(),key_equal(),alloc) {}

    //        Foo(const Foo& other, const allocator_type& alloc) : yuki_multiset_base_(other,alloc) {}

    //        Foo(Foo&& other, const allocator_type& alloc) : yuki_multiset_base_(other,alloc) {}

    //        Foo(std::initializer_list<key_type> il, const key_compare& comp = key_compare(), const key_equal& eq = key_equal(), const allocator_type& alloc = allocator_type()) : yuki_multiset_base_(il,comp,eq,alloc) {}

    //        template<typename InputIterator>
    //        Foo(InputIterator first, InputIterator last, const allocator_type& alloc) : Foo(first,last,key_compare(),key_equal(),alloc) {}

    //        Foo(std::initializer_list<key_type> il, const allocator_type& alloc) : Foo(il,key_compare(),key_equal(),alloc) {}

    //        ~Foo() = default;

    //        Foo& operator=(const Foo&) = default;

    //        Foo& operator=(Foo&&)
    //            noexcept(std::allocator_traits<allocator_type>::is_always_equal::value && std::is_nothrow_move_assignable_v<key_compare> && std::is_nothrow_move_assignable_v<key_equal>)
    //            = default;

    //        Foo& operator=(std::initializer_list<key_type> il){yuki_multiset_base_::operator=(il); return *this;}

    //        using yuki_multiset_base_::get_allocator;

    //        using yuki_multiset_base_::begin;
    //        using yuki_multiset_base_::end;
    //        using yuki_multiset_base_::rbegin;
    //        using yuki_multiset_base_::rend;
    //        using yuki_multiset_base_::cbegin;
    //        using yuki_multiset_base_::cend;
    //        using yuki_multiset_base_::crbegin;
    //        using yuki_multiset_base_::crend;

    //        using yuki_multiset_base_::empty;
    //        using yuki_multiset_base_::size;
    //        using yuki_multiset_base_::max_size;

    //        using yuki_multiset_base_::extract;

    //        using yuki_multiset_base_::erase;

    //        using yuki_multiset_base_::swap;

    //        using yuki_multiset_base_::clear;

    //        using yuki_multiset_base_::merge;

    //        using yuki_multiset_base_::key_comp;
    //        using yuki_multiset_base_::key_eq;

    //        using yuki_multiset_base_::equiv;
    //        using yuki_multiset_base_::equal;

    //        using yuki_multiset_base_::first_greater;
    //        using yuki_multiset_base_::first_equiv_greater;
    //        using yuki_multiset_base_::last_smaller_equiv;
    //        using yuki_multiset_base_::last_smaller;

    //        using yuki_multiset_base_::first_equiv;
    //        using yuki_multiset_base_::last_equiv;
    //        using yuki_multiset_base_::first_equal;
    //        using yuki_multiset_base_::last_equal;

    //        using yuki_multiset_base_::count_equiv;
    //        using yuki_multiset_base_::count_equal;

    //        using yuki_multiset_base_::contains_equiv;
    //        using yuki_multiset_base_::contains_equal;
    //        using yuki_multiset_base_::contains_equiv_equal;

    //        using yuki_multiset_base_::erase_equiv;
    //        using yuki_multiset_base_::erase_equak;

    //        using yuki_multiset_base_::insert;
    //        using yuki_multiset_base_::insert_equiv;
    //        using yuki_multiset_base_::insert_equal;

    //        using yuki_multiset_base_::emplace;
    //        using yuki_multiset_base_::emplace_equiv;
    //        using yuki_multiset_base_::emplace_equal;
    //        using yuki_multiset_base_::emplace_hint;
    //        using yuki_multiset_base_::emplace_hint_equiv;
    //        using yuki_multiset_base_::emplace_hint_equal;

    //        using yuki_multiset_base_::potentially_has_equivs;
    //        using yuki_multiset_base_::potentially_has_equals;
    //}; // struct Foo : protected yuki::multiset<Key,Compare,Equal,Allocator>
    //template<typename Key,typename Compare,typename Equal,typename Allocator>
    //void swap(Foo<Key,Compare,Equal,Allocator>& lhs,Foo<Key,Compare,Equal,Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs))) {lhs.swap(rhs);}

    //template<typename Key,typename Compare,typename Equal,typename Alloc,typename Pred>
    //auto erase_if(Foo<Key,Compare,Equal,Alloc>& foo,Pred pred) -> typename Foo<Key,Compare,Equal,Alloc>::size_type {
    //    typename Foo<Key,Compare,Equal,Alloc>::size_type old_size = foo.size();
    //    for (auto i = foo.begin(), last = foo.end(); i != last; ){
    //        if (pred(*i)){
    //          i = foo.erase(i);
    //        }else{
    //          ++i;
    //        }
    //    }
    //    return old_size - foo.size();
    //}
} // namespace yuki