#include "HashTable/HashTable.hpp"
#include <gtest/gtest.h>
#include <vector>

struct MyType {
    static int constructions;
    static int destructions;
    int* p = nullptr;

    MyType() {
        p = new int(42);
        ++constructions;
    }
    explicit MyType(int v) {
        p = new int(v);
        ++constructions;
    }
    MyType(const MyType& o) {
        if (o.p) p = new int(*o.p); else p = nullptr;
        ++constructions;
    }
    MyType(MyType&& o) noexcept {
        p = o.p;
        o.p = nullptr;
    }
    MyType& operator=(const MyType& o) {
        if (this == &o) return *this;
        delete p;
        p = o.p ? new int(*o.p) : nullptr;
        return *this;
    }
    MyType& operator=(MyType&& o) noexcept {
        if (this != &o) {
            delete p;
            p = o.p;
            o.p = nullptr;
        }
        return *this;
    }
    ~MyType() {
        if (p) {
            delete p;
            ++destructions;
            p = nullptr;
        }
    }

    bool operator==(const MyType& o) const noexcept {
        if (p == nullptr && o.p == nullptr) return true;
        if (p == nullptr || o.p == nullptr) return false;
        return *p == *o.p;
    }
};

int MyType::constructions = 0;
int MyType::destructions = 0;

struct ResetCounts {
    ResetCounts() { MyType::constructions = 0; MyType::destructions = 0; }
    ~ResetCounts() {}
};

TEST(HashTableGTest, DefaultConstructionAndProperties) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    EXPECT_EQ(ht.size(), 0u);
    EXPECT_TRUE(ht.empty());
    EXPECT_LT(0u, ht.max_size());
    EXPECT_EQ(ht.begin(), ht.end());
}

TEST(HashTableGTest, InsertFindAtAndBracket) {
    ResetCounts rc;
    HashTable<int, MyType> ht;

    auto r0 = ht.insert({0, MyType()});
    EXPECT_TRUE(r0.second);
    EXPECT_EQ(r0.first->first, 0);
    EXPECT_NE(r0.first, ht.end());

    ht[1] = MyType(7);
    EXPECT_NE(ht.find(1), ht.end());
    EXPECT_EQ(ht.find(1)->second, MyType(7));

    EXPECT_NO_THROW({ auto &v = ht.at(0); (void)v; });

    EXPECT_EQ(ht.size(), 2u);
}

TEST(HashTableGTest, EraseByKeyAndIterator) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    for (int i = 0; i < 6; ++i) ht.insert({i, MyType(i)});
    EXPECT_EQ(ht.size(), 6u);

    EXPECT_EQ(ht.erase(2), 1u);
    EXPECT_EQ(ht.find(2), ht.end());
    EXPECT_EQ(ht.size(), 5u);

    auto it = ht.find(3);
    ASSERT_NE(it, ht.end());
    auto next = ht.erase(it);
    EXPECT_TRUE(next == ht.end() || next->first != 3);
}

TEST(HashTableGTest, ClearAndRehashAndEmplace) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    for (int i = 0; i < 20; ++i) ht.emplace(i, MyType(i));
    EXPECT_EQ(ht.size(), 20u);

    for (int i = 0; i < 20; ++i) EXPECT_NE(ht.find(i), ht.end());

    ht.clear();
    EXPECT_EQ(ht.size(), 0u);
    EXPECT_TRUE(ht.empty());
}

TEST(HashTableGTest, CopyAndMoveAndSwapAndEquality) {
    ResetCounts rc;
    HashTable<int, MyType> a;
    a.insert({0, MyType(10)});
    a.insert({1, MyType(11)});

    HashTable b(a);
    EXPECT_EQ(a.size(), b.size());
    EXPECT_TRUE(a == b);

    b[2] = MyType(22);
    EXPECT_NE(a.size(), b.size());

    HashTable c(std::move(b));
    EXPECT_EQ(c.size(), 3u);
    EXPECT_TRUE(b.empty());

    HashTable<int, MyType> d;
    d.insert({100, MyType(100)});
    c.swap(d);
    EXPECT_NE(c.find(100), c.end());
    EXPECT_NE(d.find(0), d.end());
}

TEST(HashTableGTest, IteratorsPrePostIncrementDecrementAndConstIter) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    ht.insert({0, MyType(0)});
    ht.insert({1, MyType(1)});
    ht.insert({2, MyType(2)});

    auto it = ht.begin();
    ASSERT_NE(it, ht.end());
    auto it_post = it++;
    EXPECT_EQ(it_post->first, it->first - 1);

    ++it;
    EXPECT_TRUE(it == ht.end() || it->first == 2);

    auto it_end = ht.end();
    --it_end;
    EXPECT_NE(it_end, ht.end());

    HashTable<int, MyType>::const_iterator cit = it_end;
    EXPECT_EQ(cit->first, it_end->first);
}

TEST(HashTableGTest, InsertExistingAndEmplaceDuplicate) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    auto r1 = ht.insert({5, MyType(5)});
    EXPECT_TRUE(r1.second);
    auto r2 = ht.insert({5, MyType(50)});
    EXPECT_FALSE(r2.second);

    auto r3 = ht.emplace(5, MyType(500));
    EXPECT_FALSE(r3.second);
}

TEST(HashTableGTest, MaxSizeAndEmptyEraseBehavior) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    EXPECT_GT(ht.max_size(), 0u);

    EXPECT_EQ(ht.erase(9999), 0u);

    auto it = ht.end();
    auto res = ht.erase(it);
    EXPECT_EQ(res, ht.end());
}

struct ZeroHash {
    size_t operator()(const int&) const noexcept { return 0; }
};

TEST(HashTableExtra, CopyConstructFromMovedFromEmpty) {
    ResetCounts rc;
    HashTable<int, MyType> a;
    auto b = std::move(a);
    HashTable c(a);
    EXPECT_EQ(c.size(), 0u);
    EXPECT_EQ(c.begin(), c.end());
    EXPECT_EQ(c.find(123), c.end());
}

TEST(HashTableExtra, ClearOnMovedFromAndFindCapacityZero) {
    ResetCounts rc;
    HashTable<int, MyType> a;
    a.insert({1, MyType(1)});
    auto b = std::move(a);
    EXPECT_NO_THROW(a.clear());
    EXPECT_EQ(a.find(1), a.end());
}

TEST(HashTableExtra, AtThrowsWhenNotFoundBothConstAndNonConst) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    EXPECT_THROW(ht.at(999), std::out_of_range);
    const HashTable<int, MyType> cht;
    EXPECT_THROW(cht.at(123), std::out_of_range);
}

TEST(HashTableExtra, InsertRangeAndIteratorEdgeCases) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    std::vector<HashTable<int, MyType>::value_type> v;
    v.push_back({3, MyType(3)});
    v.push_back({4, MyType(4)});
    ht.insert(v.begin(), v.end());
    EXPECT_EQ(ht.size(), 2u);
    EXPECT_NE(ht.find(3), ht.end());
    EXPECT_NE(ht.find(4), ht.end());

    HashTable<int, MyType>::iterator def_it;
    auto def_it_copy = def_it;
    ++def_it;
    EXPECT_TRUE(def_it == def_it_copy);

    auto it = ht.begin();
    ASSERT_NE(it, ht.end());
    auto before_key = it->first;
    --it;
    EXPECT_EQ(it->first, before_key);

    auto itn = ht.begin();
    HashTable<int, MyType>::const_iterator cit = itn;
    EXPECT_TRUE(itn == cit);
    EXPECT_TRUE(cit == itn);
}

TEST(HashTableExtraCoverage, IteratorAssignmentOperatorWithConst) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    ht.insert({1, MyType(10)});
    ht.insert({2, MyType(20)});

    HashTable<int, MyType>::iterator it = ht.begin();
    HashTable<int, MyType>::const_iterator cit;
    cit = it;
    EXPECT_EQ(cit->first, it->first);

    HashTable<int, MyType>::const_iterator cit2 = ht.cbegin();
    HashTable<int, MyType>::const_iterator cit3;
    cit3 = cit2;
    EXPECT_EQ(cit3->first, cit2->first);
}

TEST(HashTableExtraCoverage, IteratorPostDecrement) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    for (int i = 0; i < 3; ++i) ht.insert({i, MyType(i)});

    auto it = ht.end();
    auto prev = it--;
    EXPECT_EQ(prev, ht.end());
    EXPECT_NE(it, ht.end());
}

TEST(HashTableExtraCoverage, AssignFromInitializerListOperator) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    ht = { {10, MyType(10)}, {20, MyType(20)} };
    EXPECT_EQ(ht.size(), 2u);
    EXPECT_NE(ht.find(10), ht.end());
    EXPECT_NE(ht.find(20), ht.end());
}

TEST(HashTableExtraCoverage, InsertFromInitializerList) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    ht.insert({ {1, MyType(1)}, {2, MyType(2)} });
    EXPECT_EQ(ht.size(), 2u);
    EXPECT_NE(ht.find(1), ht.end());
    EXPECT_NE(ht.find(2), ht.end());
}

TEST(HashTableExtraCoverage, OperatorBracketInsertionAndAccess) {
    ResetCounts rc;
    HashTable<int, MyType> ht;
    ht[5] = MyType(55);
    EXPECT_EQ(ht.size(), 1u);
    EXPECT_EQ(ht[5], MyType(55));

    ht[5] = MyType(555);
    EXPECT_EQ(ht[5], MyType(555));

    auto& val_ref = ht[10];
    EXPECT_EQ(val_ref, MyType());
    EXPECT_EQ(ht.size(), 2u);
}

TEST(HashTableExtraCoverage, ConstructorFromInitializerList) {
    ResetCounts rc;
    HashTable<int, MyType> ht({ {1, MyType(1)}, {2, MyType(2)} });
    EXPECT_EQ(ht.size(), 2u);
    EXPECT_NE(ht.find(1), ht.end());
    EXPECT_NE(ht.find(2), ht.end());
}

TEST(HashTableExtraCoverage, CopyAssignmentOperator) {
    ResetCounts rc;
    HashTable<int, MyType> a({ {1, MyType(1)}, {2, MyType(2)} });
    HashTable<int, MyType> b;
    b = a;
    EXPECT_EQ(b.size(), a.size());
    EXPECT_TRUE(b == a);

    b = b;
    EXPECT_EQ(b.size(), a.size());
    EXPECT_TRUE(b == a);
}

TEST(HashTableExtraCoverage, MoveAssignmentOperator) {
    ResetCounts rc;
    HashTable<int, MyType> a({ {1, MyType(1)}, {2, MyType(2)} });
    HashTable<int, MyType> b;
    b = std::move(a);
    EXPECT_EQ(b.size(), 2u);
    EXPECT_TRUE(a.empty());
}

TEST(HashTableExtraCoverage, OperatorBracketInsertsNewValueIfNotFound) {
	ResetCounts rc;
	HashTable<int, MyType> ht;

	EXPECT_TRUE(ht.empty());

	MyType& val = ht[42];
	EXPECT_EQ(*val.p, 42);
	EXPECT_EQ(ht.size(), 1u);

	val = MyType(100);
	EXPECT_EQ(ht[42], MyType(100));

	MyType& val2 = ht[42];
	EXPECT_EQ(val2, MyType(100));
	EXPECT_EQ(ht.size(), 1u);
}

TEST(HashTableExtraCoverage, OperatorBracketConstKeyRefOverload) {
	ResetCounts rc;
	HashTable<int, MyType> ht;

	const int key = 50;

	EXPECT_TRUE(ht.empty());
	MyType& inserted_val = ht[key];
	EXPECT_EQ(ht.size(), 1u);
	EXPECT_EQ(*inserted_val.p, 42);

	inserted_val = MyType(999);
	const int same_key = 50;
	MyType& found_val = ht[same_key];
	EXPECT_EQ(*found_val.p, 999);
	EXPECT_EQ(ht.size(), 1u);

	const int key2 = 51;
	ht[key2] = MyType(888);
	EXPECT_EQ(ht.size(), 2u);
	EXPECT_NE(ht.find(key2), ht.end());
	EXPECT_EQ(*ht.find(key2)->second.p, 888);
}
