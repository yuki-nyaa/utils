#include<set>
namespace yuki{
    template<
        typename Key,
        typename Compare = std::less<Key>,
        typename Equal = std::equal_to<Key>,
        typename Allocator = std::allocator<Key>
    >
    struct multiset : private std::multiset<Key,Compare,Allocator> {
        private:
            typedef std::multiset<Key,Compare,Allocator> Base_;
        public:
            using typename Base_::key_type;
            using typename Base_::value_type;
            using typename Base_::size_type;
            using typename Base_::difference_type;
            using typename Base_::key_compare;
            using typename Base_::value_compare;
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
            typedef Equal value_equal;
        protected:
            Compare key_compare_;
            Equal key_equal_;
            Allocator alloc_;
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

            multiset(std::initializer_list<value_type> il, const Compare& comp = Compare(), const Equal& eq = Equal(), const Allocator& alloc = Allocator()) : Base_(il,comp,alloc), key_compare_(comp),key_equal_(eq),alloc_(alloc) {}

            template<typename InputIterator>
            multiset(InputIterator first, InputIterator last, const Allocator& alloc) : multiset(first,last,Compare(),Equal(),alloc) {}

            multiset(std::initializer_list<value_type> il, const Allocator& alloc) : multiset(il,Compare(),Equal(),alloc) {}

            ~multiset() = default;

            multiset& operator=(const multiset&) = default;

            multiset& operator=(multiset&&)
                noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_move_assignable_v<Compare> && std::is_nothrow_move_assignable_v<Equal>)
                = default;

            multiset& operator=(std::initializer_list<value_type> il){Base_::operator=(il); return *this;}

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

            using Base_::emplace;
            using Base_::emplace_hint;
            using Base_::insert;

            using Base_::extract;

            using Base_::erase;
            size_type erase(const key_type& x) = delete;

            void swap(multiset& other)
                noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable_v<Compare> && std::is_nothrow_swappable_v<Equal>)
            {
                Base_::swap(other);
                using std::swap;
                swap(key_compare_,other.key_compare_);
                swap(key_equal_,other.key_equal_);
                swap(alloc_,other.alloc_);
            }

            using Base_::clear;

            using Base_::merge;

            using Base_::key_comp;
            using Base_::value_comp;

            key_equal key_eq() const {return key_equal_;}
            value_equal value_eq() const {return key_equal_;}

            bool weak_equal(const Key& lhs, const Key& rhs){return !key_compare_(lhs,rhs) && !key_compare_(rhs,lhs);}
            bool strong_equal(const Key& lhs, const Key& rhs){return key_equal_(lhs,rhs);}

            iterator find(const Key& key);
            const_iterator find(const Key& key) const;
            template<typename K> requires requires{typename Compare::is_transparent;}
            iterator find(const K& key);
            template<typename K> requires requires{typename Compare::is_transparent;}
            const_iterator find(const K& key) const;

            size_type count(const Key& key) const;
            template<typename K> requires requires{typename Compare::is_transparent;}
            size_type count(const K& key) const;

            bool contains(const Key& key) const;
            template<typename K> requires requires{typename Compare::is_transparent;}
            bool contains(const K& key) const;

            iterator lower_bound(const Key& key);
            const_iterator lower_bound(const Key& key) const;
            template<typename K> requires requires{typename Compare::is_transparent;}
            iterator lower_bound(const K& key);
            template<typename K> requires requires{typename Compare::is_transparent;}
            const_iterator lower_bound(const K& key) const;

            iterator upper_bound(const Key& key);
            const_iterator upper_bound(const Key& key) const;
            template<typename K> requires requires{typename Compare::is_transparent;}
            iterator upper_bound(const K& key);
            template<typename K> requires requires{typename Compare::is_transparent;}
            const_iterator upper_bound(const K& key) const;

            std::pair<iterator,iterator> equal_range(const Key& key);
            std::pair<const_iterator,const_iterator> equal_range(const Key& key) const;
            template<typename K> requires requires{typename Compare::is_transparent;}
            std::pair<iterator,iterator> equal_range(const K& key);
            template<typename K> requires requires{typename Compare::is_transparent;}
            std::pair<const_iterator,const_iterator> equal_range(const K& key) const;
    };
    template<typename Key,typename Compare,typename Equal,typename Allocator>
    void swap(multiset<Key,Compare,Equal,Allocator>& lhs,multiset<Key,Compare,Equal,Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs))) {lhs.swap(rhs);}
}