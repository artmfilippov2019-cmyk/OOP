#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>
#include "model/entity/Person.hpp"
#include "model/entity/Worker.hpp"
#include "model/entity/Master.hpp"
#include "model/entity/Foreman.hpp"
#include "model/entity/Worksite.hpp"
#include "HashTable/HashTable.hpp"

TEST(PersonTest, BasicFieldsAndAccessors) {
    Person p(10, 25, "Ivan");
    EXPECT_EQ(p.get_id(), 10);
    EXPECT_EQ(p.get_age(), 25);
    EXPECT_EQ(p.get_name(), "Ivan");
    EXPECT_TRUE(p.get_friends().empty());
    EXPECT_TRUE(p.get_enemies().empty());
}

TEST(PersonTest, AddRemoveFriendsAndEnemies) {
    Person p1(1, 30, "A");
    Person p2(2, 28, "B");

    p1.add_friend(2);
    EXPECT_EQ(p1.get_friends().size(), 1);
    EXPECT_EQ(p1.get_friends()[0], 2);

    p1.add_friend(2);
    EXPECT_EQ(p1.get_friends().size(), 1);

    p1.remove_friend(2);
    EXPECT_TRUE(p1.get_friends().empty());

    p1.add_enemy(2);
    EXPECT_EQ(p1.get_enemies().size(), 1);
    EXPECT_EQ(p1.get_enemies()[0], 2);

    p1.add_enemy(2);
    EXPECT_EQ(p1.get_enemies().size(), 1);

    p1.remove_enemy(2);
    EXPECT_TRUE(p1.get_enemies().empty());
}

TEST(PersonTest, CannotAddYourselfAsFriendOrEnemy) {
    Person p(5, 40, "Self");
    EXPECT_THROW(p.add_friend(5), std::invalid_argument);
    EXPECT_THROW(p.add_enemy(5), std::invalid_argument);
}

TEST(WorkerTest, ConstructorRangeAndAccessor) {
    Worker w(11, 22, 5, "Worker");
    EXPECT_EQ(w.get_id(), 11);
    EXPECT_EQ(w.get_age(), 22);
    EXPECT_EQ(w.get_name(), "Worker");
    EXPECT_EQ(w.get_productivity(), 5);
}

TEST(WorkerTest, ConstructorThrowsOnInvalidProductivity) {
    EXPECT_THROW(Worker(1, 20, 0, "W"), std::invalid_argument);
    EXPECT_THROW(Worker(1, 20, 11, "W"), std::invalid_argument);
}

TEST(WorkerTest, SetProductivityValue) {
    Worker w(2, 20, 3, "W");
    w.set_productivity_value(10);
    EXPECT_EQ(w.get_productivity(), 10);
    EXPECT_THROW(w.set_productivity_value(0), std::invalid_argument);
    EXPECT_THROW(w.set_productivity_value(11), std::invalid_argument);
}

TEST(MasterTest, ConstructorAndEfficiency) {
    Master m(20, 45, 1.5, "Master");
    EXPECT_EQ(m.get_id(), 20);
    EXPECT_EQ(m.get_age(), 45);
    EXPECT_EQ(m.get_name(), "Master");
    EXPECT_DOUBLE_EQ(m.get_efficiency(), 1.5);
}

TEST(MasterTest, ConstructorThrowsOnInvalidEfficiency) {
    EXPECT_THROW(Master(1, 30, 1.0, "M"), std::invalid_argument);
    EXPECT_THROW(Master(1, 30, 0.5, "M"), std::invalid_argument);
    EXPECT_THROW(Master(1, 30, 5.1, "M"), std::invalid_argument);
}

TEST(MasterTest, SetEfficiency) {
    Master m(21, 35, 2.0, "M");
    m.set_efficiency(4.5);
    EXPECT_DOUBLE_EQ(m.get_efficiency(), 4.5);
    EXPECT_THROW(m.set_efficiency(1.0), std::invalid_argument);
    EXPECT_THROW(m.set_efficiency(6.0), std::invalid_argument);
}

TEST(ForemanTest, InheritsPerson) {
    Foreman f(30, 50, "Foreman");
    EXPECT_EQ(f.get_id(), 30);
    EXPECT_EQ(f.get_age(), 50);
    EXPECT_EQ(f.get_name(), "Foreman");
    Person *p = &f;
    EXPECT_NE(dynamic_cast<Foreman*>(p), nullptr);
}

TEST(WorksiteTest, ConstructorAndAccessors) {
    Worksite ws(100, 500.0, 200.0, {1,2});
    EXPECT_EQ(ws.get_id(), 100);
    EXPECT_DOUBLE_EQ(ws.get_work_volume(), 500.0);
    EXPECT_DOUBLE_EQ(ws.get_remaining_work(), 200.0);
    EXPECT_EQ(ws.get_assigned_ids().size(), 2);
}

TEST(WorksiteTest, ConstructorThrowsOnNegativeVolumes) {
    EXPECT_THROW(Worksite(1, -1.0), std::invalid_argument);
    EXPECT_THROW(Worksite(2, 10.0, -5.0, {}), std::invalid_argument);
}

TEST(WorksiteTest, SetWorkVolumeAdjustsRemaining) {
    Worksite ws(10, 100.0);
    ws.set_remaining_work(80.0);
    ws.set_work_volume(50.0);
    EXPECT_DOUBLE_EQ(ws.get_work_volume(), 50.0);
    EXPECT_DOUBLE_EQ(ws.get_remaining_work(), 50.0);
    EXPECT_THROW(ws.set_work_volume(-1.0), std::invalid_argument);
}

TEST(WorksiteTest, AddRemoveHasPerson) {
    Worksite ws(11, 100.0);
    EXPECT_FALSE(ws.has_person(7));
    ws.add_person(7);
    EXPECT_TRUE(ws.has_person(7));
    ws.add_person(7);
    EXPECT_TRUE(ws.has_person(7));
    ws.remove_person(7);
    EXPECT_FALSE(ws.has_person(7));
}

TEST(WorksiteTest, ApplyWorkAndSetRemaining) {
    Worksite ws(12, 100.0);
    ws.set_remaining_work(30.0);
    EXPECT_THROW(ws.apply_work(-1.0), std::invalid_argument);
    ws.apply_work(10.0);
    EXPECT_DOUBLE_EQ(ws.get_remaining_work(), 20.0);
    ws.apply_work(50.0);
    EXPECT_DOUBLE_EQ(ws.get_remaining_work(), 0.0);
    EXPECT_THROW(ws.set_remaining_work(-5.0), std::invalid_argument);
}

static void insert_persons(HashTable<int, std::unique_ptr<IPerson>>& table,
						   std::vector<std::unique_ptr<IPerson>>& persons) {
	for (auto& p : persons) {
		table.insert({p->get_id(), std::move(p)}); // переносим владение
	}
}

TEST(WorksiteTest, CalculateDailyOutputOnlyWorkersUniquePtr) {
	HashTable<int, std::unique_ptr<IPerson>> persons;

	std::vector<std::unique_ptr<IPerson>> people;
	people.push_back(std::make_unique<Worker>(1, 20, 3, "W1"));
	people.push_back(std::make_unique<Worker>(2, 22, 4, "W2"));

	Worksite ws(1, 100.0);
	ws.add_person(1);
	ws.add_person(2);

	insert_persons(persons, people);

	double out = ws.calculate_daily_output(persons);
	EXPECT_DOUBLE_EQ(out, 7.0);
}

TEST(WorksiteTest, CalculateDailyOutputWithMasterMultiplicationUniquePtr) {
	HashTable<int, std::unique_ptr<IPerson>> persons;
	std::vector<std::unique_ptr<IPerson>> people;

	people.push_back(std::make_unique<Worker>(1, 20, 3, "W1"));
	people.push_back(std::make_unique<Worker>(2, 22, 4, "W2"));
	people.push_back(std::make_unique<Master>(3, 40, 2.0, "M1"));

	Worksite ws(2, 100.0);
	ws.add_person(1);
	ws.add_person(2);
	ws.add_person(3);

	insert_persons(persons, people);

	double out = ws.calculate_daily_output(persons);
	EXPECT_DOUBLE_EQ(out, 14.0);
}

TEST(WorksiteTest, CalculateDailyOutputWithMultipleMastersUniquePtr) {
	HashTable<int, std::unique_ptr<IPerson>> persons;
	std::vector<std::unique_ptr<IPerson>> people;

	people.push_back(std::make_unique<Worker>(1, 20, 2, "W1"));
	people.push_back(std::make_unique<Master>(2, 40, 1.5, "M1"));
	people.push_back(std::make_unique<Master>(3, 45, 2.0, "M2"));

	Worksite ws(3, 100.0);
	ws.add_person(1);
	ws.add_person(2);
	ws.add_person(3);

	insert_persons(persons, people);

	double out = ws.calculate_daily_output(persons);
	EXPECT_DOUBLE_EQ(out, 6.0);
}

TEST(WorksiteTest, CalculateDailyOutputPenaltiesAndForemanUniquePtr) {
	HashTable<int, std::unique_ptr<IPerson>> persons;
	std::vector<std::unique_ptr<IPerson>> people;

	auto w1 = std::make_unique<Worker>(1, 25, 3, "W1");
	auto w2 = std::make_unique<Worker>(2, 26, 4, "W2");
	auto w3 = std::make_unique<Worker>(3, 27, 5, "W3");

	w1->add_enemy(2);
	w2->add_enemy(1);

	people.push_back(std::move(w1));
	people.push_back(std::move(w2));
	people.push_back(std::move(w3));

	Worksite ws(4, 200.0);
	ws.add_person(1);
	ws.add_person(2);
	ws.add_person(3);

	insert_persons(persons, people);

	double out = ws.calculate_daily_output(persons);
	EXPECT_DOUBLE_EQ(out, 3.0 + 4.0 + 5.0 - 1.0);

	auto f = std::make_unique<Foreman>(10, 50, "F");
	ws.add_person(10);
	persons.insert({10, std::move(f)});

	double out2 = ws.calculate_daily_output(persons);
	EXPECT_DOUBLE_EQ(out2, 3.0 + 4.0 + 5.0);
}

TEST(WorksiteTest, CalculateDailyOutputFriendTripletPenaltyUniquePtr) {
	HashTable<int, std::unique_ptr<IPerson>> persons;
	std::vector<std::unique_ptr<IPerson>> people;

	auto a = std::make_unique<Worker>(1, 20, 2, "A");
	auto b = std::make_unique<Worker>(2, 21, 2, "B");
	auto c = std::make_unique<Worker>(3, 22, 2, "C");

	a->add_friend(2); b->add_friend(1);
	a->add_friend(3); c->add_friend(1);
	b->add_friend(3); c->add_friend(2);

	people.push_back(std::move(a));
	people.push_back(std::move(b));
	people.push_back(std::move(c));

	Worksite ws(5, 100.0);
	ws.add_person(1);
	ws.add_person(2);
	ws.add_person(3);

	insert_persons(persons, people);

	double out = ws.calculate_daily_output(persons);
	EXPECT_DOUBLE_EQ(out, 5.0);
}

TEST(WorksiteTest, ConstructorThrowsOnNegativeWorkVolume) {
	EXPECT_THROW(Worksite ws1(1, -10.0), std::invalid_argument);
	EXPECT_THROW(Worksite ws2(2, -10.0, 5.0, {}), std::invalid_argument);
}