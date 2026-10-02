#ifndef LAB3_HASHTABLE_H
#define LAB3_HASHTABLE_H

#include <iostream>
#include <vector>
#include <functional>
#include <utility>
#include <concepts>
#include <iterator>

template <std::default_initializable Key, std::default_initializable Value, typename Hash = std::hash<Key>>
class HashTable;

enum class NodeState {
    BEGIN_EMPTY,
    BEGIN_OCCUPIED,
    BEGIN_DELETED,

    CURRENT_EMPTY,
    CURRENT_OCCUPIED,
    CURRENT_DELETED,

    END_EMPTY,
    END_OCCUPIED,
    END_DELETED
};

template<typename Key, typename Value>
struct HashNode {
	union {
		std::pair<const Key, Value> val;
	};
    NodeState state = NodeState::CURRENT_EMPTY;

	HashNode() noexcept {}
	~HashNode() noexcept {}
};

template<typename Key, typename Value, bool is_const>
class HashTableIterator {
public:
    using difference_type = ptrdiff_t;
    using value_type = std::pair<const Key, Value>;
    using pointer   = std::conditional_t<is_const, const value_type, value_type>*;
    using reference = std::conditional_t<is_const, const value_type, value_type>&;
    using iterator_category = std::bidirectional_iterator_tag;

    HashTableIterator() noexcept;

    template<bool other_const>
    HashTableIterator(const HashTableIterator<Key, Value, other_const>& o) noexcept
        requires (is_const >= other_const);

    template<bool other_const>
    HashTableIterator& operator = (const HashTableIterator<Key, Value, other_const>& o) noexcept
        requires (is_const >= other_const);

    reference operator * () const noexcept;

    pointer operator -> () const noexcept;

    template<bool other_const>
    bool operator == (const HashTableIterator<Key, Value, other_const>& o) const noexcept;

    template<bool other_const>
    bool operator != (const HashTableIterator<Key, Value, other_const>& o) const noexcept {
        return !(*this == o);
    }

    HashTableIterator& operator ++ () noexcept;
    HashTableIterator operator ++ (int) noexcept;

    HashTableIterator& operator -- () noexcept;
    HashTableIterator operator -- (int) noexcept;

private:
    using node_ptr_t = std::conditional_t<is_const, const HashNode<Key, Value>, HashNode<Key, Value>>*;

    node_ptr_t current_node;

    explicit HashTableIterator(node_ptr_t current);

    friend class HashTable<Key, Value>;
    friend class HashTableIterator<Key, Value, !is_const>;
};

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
class HashTable {
public:
    using value_type = std::pair<const Key, Value>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = size_t;

    using iterator = HashTableIterator<Key, Value, false>;
    using const_iterator = HashTableIterator<Key, Value, true>;

    HashTable() noexcept(std::is_nothrow_default_constructible_v<Key> &&
                         std::is_nothrow_default_constructible_v<Value>);

    HashTable(const HashTable& o);

    HashTable(HashTable&& o) noexcept;

    HashTable(std::initializer_list<value_type> il);

    HashTable& operator = (const HashTable& o);

    HashTable& operator = (HashTable&& o) noexcept;

    ~HashTable();

    iterator begin() noexcept;

    iterator end() noexcept;

    const_iterator begin() const noexcept;

    const_iterator end() const noexcept;

    const_iterator cbegin() const noexcept;

    const_iterator cend() const noexcept;

    bool operator==(const HashTable& o) const;

    void swap(HashTable& o) noexcept;

    [[nodiscard]] size_type size() const noexcept;

    [[nodiscard]] size_type max_size() const noexcept;

    [[nodiscard]] bool empty() const noexcept;

    HashTable& operator = (std::initializer_list<value_type> il);

    std::pair<iterator, bool> insert(const value_type& val);
    std::pair<iterator, bool> insert(value_type&& val);

    template<std::input_iterator It>
    void insert(It i, It j);

    void insert(std::initializer_list<value_type> il);

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args)
        requires std::constructible_from<value_type, Args...>;

    iterator erase(const_iterator q) noexcept;
    size_type erase(const Key& key) noexcept;

    void clear() noexcept;

    iterator find(const Key& key);
    const_iterator find(const Key& key) const;

    Value& operator[](const Key& key);
    Value& operator[](Key&& key);

    Value& at(const Key& key);
    const Value& at(const Key& key) const;

private:
    using node_t = HashNode<Key, Value>;

    node_t* buckets = nullptr;
    size_t table_size = 0;
    size_t capacity = 0;
    Hash hasher;

    static constexpr float MAX_LOAD_FACTOR = 0.7f;
    static constexpr size_t INITIAL_CAPACITY = 8;

    void rehash(size_t new_capacity);
    size_t find_index(const Key& key) const;
    std::pair<size_t, bool> find_insert_index(const Key& key);

    node_t* allocate_buckets(size_t n);
    void deallocate_buckets(node_t* p, size_t n) noexcept;
};

#include "HashTable.tpp"

#endif
