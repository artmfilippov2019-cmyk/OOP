#ifndef LAB3_HASHTABLE_TPP
#define LAB3_HASHTABLE_TPP

#include <algorithm>
#include <utility>
#include <cmath>
#include <new>
#include <type_traits>
#include <stdexcept>

static_assert(std::bidirectional_iterator<HashTableIterator<float, int, false>>);
static_assert(std::bidirectional_iterator<HashTableIterator<std::string, int, true>>);

template<typename Key, typename Value, bool is_const>
HashTableIterator<Key, Value, is_const>::HashTableIterator() noexcept
    : current_node(nullptr) {}

template<typename Key, typename Value, bool is_const>
template<bool other_const>
HashTableIterator<Key, Value, is_const>::HashTableIterator(
    const HashTableIterator<Key, Value, other_const>& o) noexcept
    requires (is_const >= other_const)
    : current_node(o.current_node) {}

template<typename Key, typename Value, bool is_const>
template<bool other_const>
HashTableIterator<Key, Value, is_const>&
HashTableIterator<Key, Value, is_const>::operator=(
    const HashTableIterator<Key, Value, other_const>& o) noexcept
    requires (is_const >= other_const) {
    current_node = o.current_node;
    return *this;
}

template<typename Key, typename Value, bool is_const>
HashTableIterator<Key, Value, is_const>::reference
HashTableIterator<Key, Value, is_const>::operator*() const noexcept {
    return current_node->val;
}

template<typename Key, typename Value, bool is_const>
HashTableIterator<Key, Value, is_const>::pointer
HashTableIterator<Key, Value, is_const>::operator->() const noexcept {
    return &(current_node->val);
}

template<typename Key, typename Value, bool is_const>
template<bool other_const>
bool HashTableIterator<Key, Value, is_const>::operator==(
    const HashTableIterator<Key, Value, other_const>& o) const noexcept {
    return current_node == o.current_node;
}

static bool is_end_state(NodeState s) {
	return s == NodeState::END_EMPTY || s == NodeState::END_OCCUPIED || s == NodeState::END_DELETED;
}

static bool is_begin_state(NodeState s) {
	return s == NodeState::BEGIN_EMPTY || s == NodeState::BEGIN_OCCUPIED || s == NodeState::BEGIN_DELETED;
}

static bool is_occupied_state(NodeState s) {
	return s == NodeState::CURRENT_OCCUPIED || s == NodeState::BEGIN_OCCUPIED;
}

static bool is_empty_state(NodeState s) {
	return s == NodeState::CURRENT_EMPTY || s == NodeState::BEGIN_EMPTY;
}

template<typename Key, typename Value, bool is_const>
HashTableIterator<Key, Value, is_const>&
HashTableIterator<Key, Value, is_const>::operator++() noexcept {
	if (!current_node) return *this;
	if (is_end_state(current_node->state)) return *this;

	++current_node;
	while (!is_end_state(current_node->state) && !is_occupied_state(current_node->state)) {
		++current_node;
	}
	return *this;
}

template<typename Key, typename Value, bool is_const>
HashTableIterator<Key, Value, is_const>
HashTableIterator<Key, Value, is_const>::operator++(int) noexcept {
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<typename Key, typename Value, bool is_const>
HashTableIterator<Key, Value, is_const>&
HashTableIterator<Key, Value, is_const>::operator--() noexcept {
	if (!current_node) return *this;

	auto node = current_node;
	while (!is_begin_state(node->state)) {
		--node;
		if (is_occupied_state(node->state)) {
			current_node = node;
			return *this;
		}
	}

	return *this;
}

template<typename Key, typename Value, bool is_const>
HashTableIterator<Key, Value, is_const>
HashTableIterator<Key, Value, is_const>::operator--(int) noexcept {
    auto tmp = *this;
    --(*this);
    return tmp;
}

template<typename Key, typename Value, bool is_const>
HashTableIterator<Key, Value, is_const>::HashTableIterator(node_ptr_t cur)
    : current_node(cur) {}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::HashTable() noexcept(
    std::is_nothrow_default_constructible_v<Key> &&
    std::is_nothrow_default_constructible_v<Value>)
    : buckets(allocate_buckets(INITIAL_CAPACITY)), capacity(INITIAL_CAPACITY) {
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::HashTable(const HashTable& o)
    : hasher(o.hasher) {
	if (!o.capacity) {
		buckets = nullptr;
		capacity = 0;
		table_size = 0;
		return;
	}
	node_t* tmp = allocate_buckets(o.capacity);
	try {
		for (size_t i = 0; i < o.capacity; ++i) {
			if (is_occupied_state(o.buckets[i].state)) {
				new (&tmp[i].val) value_type(o.buckets[i].val);
				tmp[i].state = o.buckets[i].state;
			} else {
				tmp[i].state = o.buckets[i].state;
			}
		}
	} catch (...) {
		for (size_t i = 0; i < o.capacity; ++i) {
			if (is_occupied_state(tmp[i].state)) {
				tmp[i].val.~value_type();
			}
		}
		::operator delete(tmp, static_cast<std::align_val_t>(alignof(node_t)));
		throw;
	}
	buckets = tmp;
	capacity = o.capacity;
	table_size = o.table_size;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::const_iterator
HashTable<Key, Value, Hash>::begin() const noexcept {
    return cbegin();
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::const_iterator
HashTable<Key, Value, Hash>::end() const noexcept {
    return cend();
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
bool HashTable<Key, Value, Hash>::operator==(const HashTable& o) const {
    if (table_size != o.table_size) return false;
    for (auto it = begin(); it != end(); ++it) {
        auto o_it = o.find(it->first);
        if (o_it == o.end() || o_it->second != it->second) {
            return false;
        }
    }
    return true;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::size_type
HashTable<Key, Value, Hash>::size() const noexcept {
    return table_size;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::size_type
HashTable<Key, Value, Hash>::max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(node_t);
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
bool HashTable<Key, Value, Hash>::empty() const noexcept {
    return table_size == 0;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>& HashTable<Key, Value, Hash>::operator=(std::initializer_list<value_type> il) {
	HashTable tmp(il);
	swap(tmp);
	return *this;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
template<std::input_iterator It>
void HashTable<Key, Value, Hash>::insert(It i, It j) {
	size_t new_elements = 0;
	for (It it = i; it != j; ++it) {
		auto idx_found = find_insert_index(it->first);
		if (!idx_found.second) ++new_elements;
	}

	if (static_cast<float>(table_size + new_elements) / capacity > MAX_LOAD_FACTOR) {
		rehash(std::max(capacity * 2, static_cast<size_t>((table_size + new_elements) / MAX_LOAD_FACTOR)));
	}

	for (It it = i; it != j; ++it) {
		emplace(*it);
	}
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
void HashTable<Key, Value, Hash>::insert(std::initializer_list<value_type> il) {
    insert(il.begin(), il.end());
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::iterator
HashTable<Key, Value, Hash>::erase(const_iterator q) noexcept {
	if (q.current_node == nullptr || !is_occupied_state(q.current_node->state)) {
		return end();
	}
	auto index = static_cast<size_t>(q.current_node - buckets);
	buckets[index].val.~value_type();
	buckets[index].state = (index == 0 ? NodeState::BEGIN_DELETED : NodeState::CURRENT_DELETED);
	--table_size;

	auto next_it = q;
	++next_it;
	return iterator(const_cast<node_t*>(next_it.current_node));
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::size_type
HashTable<Key, Value, Hash>::erase(const Key& key) noexcept {
	size_t index = find_index(key);
	if (index == capacity) {
		return 0;
	}
	buckets[index].val.~value_type();
	buckets[index].state = (index == 0 ? NodeState::BEGIN_DELETED : NodeState::CURRENT_DELETED);
	--table_size;
	return 1;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
void HashTable<Key, Value, Hash>::clear() noexcept {
	if (!buckets) {
		table_size = 0;
		return;
	}
	for (size_t i = 0; i < capacity; ++i) {
		if (is_occupied_state(buckets[i].state)) {
			buckets[i].val.~value_type();
		}
		buckets[i].state = (i == 0 ? NodeState::BEGIN_EMPTY : NodeState::CURRENT_EMPTY);
	}
	table_size = 0;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::iterator
HashTable<Key, Value, Hash>::find(const Key& key) {
	size_t index = find_index(key);
	if (index == capacity) {
		return end();
	}
	return iterator(buckets + index);
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::const_iterator
HashTable<Key, Value, Hash>::find(const Key& key) const {
	size_t index = find_index(key);
	if (index == capacity) {
		return cend();
	}
	return const_iterator(buckets + index);
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
Value& HashTable<Key, Value, Hash>::operator[](const Key& key) {
    auto it = find(key);
    if (it != end()) {
        return it->second;
    }
    auto [inserted_it, success] = insert({key, Value{}});
    return inserted_it->second;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
Value& HashTable<Key, Value, Hash>::operator[](Key&& key) {
    auto it = find(key);
    if (it != end()) {
        return it->second;
    }
    auto [inserted_it, success] = insert({std::move(key), Value{}});
    return inserted_it->second;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
Value& HashTable<Key, Value, Hash>::at(const Key& key) {
    auto it = find(key);
    if (it == end()) {
        throw std::out_of_range("Key not found");
    }
    return it->second;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
const Value& HashTable<Key, Value, Hash>::at(const Key& key) const {
    auto it = find(key);
    if (it == cend()) {
        throw std::out_of_range("Key not found");
    }
    return it->second;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
size_t HashTable<Key, Value, Hash>::find_index(const Key& key) const {
	if (capacity == 0) return capacity;
	size_t start = hasher(key) % capacity;
	for (size_t i = 0; i < capacity; ++i) {
		size_t idx = (start + i) % capacity;
		if (is_empty_state(buckets[idx].state)) {
			return capacity;
		}
		if (is_occupied_state(buckets[idx].state) && buckets[idx].val.first == key) {
			return idx;
		}
	}
	return capacity;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::HashTable(HashTable&& o) noexcept
    : buckets(o.buckets), table_size(o.table_size),
      capacity(o.capacity), hasher(std::move(o.hasher)) {
    o.buckets = nullptr;
    o.table_size = 0;
    o.capacity = 0;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::HashTable(std::initializer_list<value_type> il) {
    HashTable tmp;
    tmp.capacity = std::max(INITIAL_CAPACITY,
        static_cast<size_t>(std::ceil(il.size() / MAX_LOAD_FACTOR)));
    tmp.buckets = allocate_buckets(tmp.capacity);
    for (const auto& v : il) tmp.insert(v);
    swap(tmp);
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>&
HashTable<Key, Value, Hash>::operator=(const HashTable& o) {
    if (this == &o) return *this;
    HashTable tmp(o);
    swap(tmp);
    return *this;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>&
HashTable<Key, Value, Hash>::operator=(HashTable&& o) noexcept {
    if (this != &o) {
        clear();
        deallocate_buckets(buckets, capacity);
        buckets = o.buckets;
        table_size = o.table_size;
        capacity = o.capacity;
        hasher = std::move(o.hasher);
        o.buckets = nullptr;
        o.table_size = 0;
        o.capacity = 0;
    }
    return *this;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::~HashTable() {
    clear();
    deallocate_buckets(buckets, capacity);
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::iterator
HashTable<Key, Value, Hash>::begin() noexcept {
	if (!buckets) return end();
	auto p = buckets;
	while (p != buckets + capacity && !is_occupied_state(p->state)) ++p;
	return iterator(p);
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::iterator
HashTable<Key, Value, Hash>::end() noexcept {
	return iterator(buckets + capacity);
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::const_iterator
HashTable<Key, Value, Hash>::cbegin() const noexcept {
	if (!buckets) return cend();
	auto p = buckets;
	while (p != buckets + capacity && !is_occupied_state(p->state)) ++p;
	return const_iterator(p);
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::const_iterator
HashTable<Key, Value, Hash>::cend() const noexcept {
	return const_iterator(buckets + capacity);
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
void HashTable<Key, Value, Hash>::swap(HashTable& o) noexcept {
    std::swap(buckets, o.buckets);
    std::swap(table_size, o.table_size);
    std::swap(capacity, o.capacity);
    std::swap(hasher, o.hasher);
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
template<typename... Args>
std::pair<typename HashTable<Key, Value, Hash>::iterator, bool>
HashTable<Key, Value, Hash>::emplace(Args&&... args)
    requires std::constructible_from<value_type, Args...> {
	value_type tmp(std::forward<Args>(args)...);
	auto [idx, found] = find_insert_index(tmp.first);
	if (found) {
		return {iterator(buckets + idx), false};
	}
	if (static_cast<float>(table_size + 1) / capacity > MAX_LOAD_FACTOR) {
		rehash(capacity * 2);
		idx = find_insert_index(tmp.first).first;
	}
	new (&buckets[idx].val) value_type(std::move(tmp));
	buckets[idx].state = NodeState::CURRENT_OCCUPIED;
	++table_size;
	return {iterator(buckets + idx), true};
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
std::pair<typename HashTable<Key, Value, Hash>::iterator, bool>
HashTable<Key, Value, Hash>::insert(const value_type& v) {
    return emplace(v);
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
std::pair<typename HashTable<Key, Value, Hash>::iterator, bool>
HashTable<Key, Value, Hash>::insert(value_type&& v) {
    return emplace(std::move(v));
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
void HashTable<Key, Value, Hash>::rehash(size_t new_capacity) {
	node_t* new_buckets = allocate_buckets(new_capacity);
	try {
		for (size_t i = 0; i < capacity; ++i) {
			if (is_occupied_state(buckets[i].state)) {
				const auto& key = buckets[i].val.first;
				size_t start = hasher(key) % new_capacity;
				size_t idx = new_capacity;
				for (size_t j = 0; j < new_capacity; ++j) {
					size_t cur = (start + j) % new_capacity;
					if (is_empty_state(new_buckets[cur].state)) {
						idx = cur;
						break;
					}
				}
				new (&new_buckets[idx].val)
				value_type(std::move_if_noexcept(buckets[i].val));
				new_buckets[idx].state = (idx == 0 ? NodeState::BEGIN_OCCUPIED : NodeState::CURRENT_OCCUPIED);
			}
		}
	} catch (...) {
		for (size_t i = 0; i < new_capacity; ++i) {
			if (is_occupied_state(new_buckets[i].state))
				new_buckets[i].val.~value_type();
			new_buckets[i].~node_t();
		}
		::operator delete(new_buckets, static_cast<std::align_val_t>(alignof(node_t)));
		throw;
	}
	for (size_t i = 0; i < capacity; ++i) {
		if (is_occupied_state(buckets[i].state))
			buckets[i].val.~value_type();
		buckets[i].~node_t();
	}
	::operator delete(buckets, static_cast<std::align_val_t>(alignof(node_t)));
	buckets = new_buckets;
	capacity = new_capacity;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
std::pair<size_t, bool>
HashTable<Key, Value, Hash>::find_insert_index(const Key& key) {
	size_t start = hasher(key) % capacity;
	size_t first_deleted = capacity;
	for (size_t i = 0; i < capacity; ++i) {
		size_t idx = (start + i) % capacity;
		if (is_empty_state(buckets[idx].state))
			return {first_deleted != capacity ? first_deleted : idx, false};
		if ((buckets[idx].state == NodeState::CURRENT_DELETED || buckets[idx].state == NodeState::BEGIN_DELETED) && first_deleted == capacity)
			first_deleted = idx;
		if (is_occupied_state(buckets[idx].state) &&
			buckets[idx].val.first == key)
			return {idx, true};
	}
	return {first_deleted, false};
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
HashTable<Key, Value, Hash>::node_t*
HashTable<Key, Value, Hash>::allocate_buckets(size_t n) {
	auto p = static_cast<node_t*>(operator new((n + 1) * sizeof(node_t), static_cast<std::align_val_t>(alignof(node_t))));
	size_t i = 0;
	try {
		for (; i < n; ++i) new (&p[i]) node_t();
		new (&p[n]) node_t();
		p[n].state = NodeState::END_EMPTY;
		if (n > 0) {
			p[0].state = NodeState::BEGIN_EMPTY;
		}
	} catch (...) {
		for (size_t j = 0; j < i; ++j) {
			p[j].~node_t();
		}
		if (i == n) {
			p[n].~node_t();
		}
		::operator delete(p, static_cast<std::align_val_t>(alignof(node_t)));
		throw;
	}
	return p;
}

template <std::default_initializable Key, std::default_initializable Value, typename Hash>
void HashTable<Key, Value, Hash>::deallocate_buckets(node_t* p, size_t n) noexcept {
	if (!p) return;
	for (size_t i = 0; i <= n; ++i) {
		if (is_occupied_state(p[i].state)) {
			p[i].val.~value_type();
		}
	}
	::operator delete(p, static_cast<std::align_val_t>(alignof(node_t)));
}

#endif
