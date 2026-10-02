#include <gtest/gtest.h>
#include "model/repository/PersonRepository.hpp"
#include "model/repository/WorksiteRepository.hpp"
#include "model/entity/Person.hpp"
#include "model/entity/Worker.hpp"
#include "model/entity/Worksite.hpp"

TEST(PersonRepositoryTest, AddAndGetPerson) {
    PersonRepository repo;
    auto p = new Person(1, 25, "Timur");
    repo.add_person(p);

    EXPECT_TRUE(repo.contains(1));
    EXPECT_EQ(repo.get_person(1)->get_name(), "Timur");
}

TEST(PersonRepositoryTest, AddNullPointerThrows) {
    PersonRepository repo;
    EXPECT_THROW(repo.add_person(nullptr), std::invalid_argument);
}

TEST(PersonRepositoryTest, AddDuplicateIdThrows) {
    PersonRepository repo;
    repo.add_person(new Person(1, 20, "Misha"));

    auto* duplicate = new Person(1, 30, "Stepan");
    EXPECT_THROW(repo.add_person(duplicate), std::runtime_error);
    delete duplicate;
}

TEST(PersonRepositoryTest, RemovePerson) {
    PersonRepository repo;
    repo.add_person(new Person(2, 22, "Artem"));
    EXPECT_TRUE(repo.contains(2));

    repo.remove_person(2);
    EXPECT_FALSE(repo.contains(2));
}

TEST(PersonRepositoryTest, RemoveNonexistentPersonThrows) {
    PersonRepository repo;
    EXPECT_THROW(repo.remove_person(100), std::runtime_error);
}

TEST(PersonRepositoryTest, GetNonexistentPersonThrows) {
    PersonRepository repo;
    EXPECT_THROW((void)repo.get_person(50), std::runtime_error);
}

TEST(PersonRepositoryTest, GetAllPersons) {
    PersonRepository repo;
    repo.add_person(new Person(1, 20, "Timur"));
    repo.add_person(new Person(2, 30, "Ivan"));
    auto all = repo.get_all();
    EXPECT_EQ(all.size(), 2);
    EXPECT_TRUE((all[0]->get_id() == 1 || all[0]->get_id() == 2));
}

TEST(WorksiteRepositoryTest, AddAndGetWorksite) {
    WorksiteRepository repo;
    auto ws = new Worksite(10, 100.0);
    repo.add_worksite(ws);

    EXPECT_TRUE(repo.contains(10));
    EXPECT_EQ(repo.get_worksite(10)->get_work_volume(), 100.0);
}

TEST(WorksiteRepositoryTest, AddNullPointerThrows) {
    WorksiteRepository repo;
    EXPECT_THROW(repo.add_worksite(nullptr), std::invalid_argument);
}

TEST(WorksiteRepositoryTest, AddDuplicateIdThrows) {
    WorksiteRepository repo;
    repo.add_worksite(new Worksite(5, 50.0));

    auto* duplicate = new Worksite(5, 60.0);
    EXPECT_THROW(repo.add_worksite(duplicate), std::runtime_error);
    delete duplicate;
}

TEST(WorksiteRepositoryTest, RemoveWorksite) {
    WorksiteRepository repo;
    repo.add_worksite(new Worksite(7, 70.0));
    EXPECT_TRUE(repo.contains(7));

    repo.remove_worksite(7);
    EXPECT_FALSE(repo.contains(7));
}

TEST(WorksiteRepositoryTest, RemoveNonexistentWorksiteThrows) {
    WorksiteRepository repo;
    EXPECT_THROW(repo.remove_worksite(123), std::runtime_error);
}

TEST(WorksiteRepositoryTest, GetNonexistentWorksiteThrows) {
    WorksiteRepository repo;
    EXPECT_THROW((void)repo.get_worksite(999), std::runtime_error);
}

TEST(WorksiteRepositoryTest, GetAllWorksites) {
    WorksiteRepository repo;
    repo.add_worksite(new Worksite(1, 10.0));
    repo.add_worksite(new Worksite(2, 20.0));
    auto all = repo.get_all();
    EXPECT_EQ(all.size(), 2);
    EXPECT_TRUE((all[0]->get_id() == 1 || all[0]->get_id() == 2));
}