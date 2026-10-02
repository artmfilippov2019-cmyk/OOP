#include <gtest/gtest.h>
#include "model/service/BrigadeService.hpp"
#include "mapper/SuperPersonMapper.hpp"
#include "model/entity/Master.hpp"
#include "model/entity/Worker.hpp"
#include "model/repository/PersonRepository.hpp"
#include "model/repository/WorksiteRepository.hpp"
#include "model/service/IStrategyPromote.hpp"

class MockPromoteStrategy : public IStrategyPromote {
public:
	mutable bool called = false;

	void promote(PersonRepository* repo, int worker_id, double new_efficiency) const override {
		called = true;
		IPerson* p = repo->get_person(worker_id);

		auto* master = new Master(
			p->get_id(),
			p->get_age(),
			new_efficiency,
			p->get_name()
		);
		repo->remove_person(worker_id);
		repo->add_person(master);
	}
};

class BrigadeServiceTest : public testing::Test {
protected:
	PersonRepository person_repo;
	WorksiteRepository worksite_repo;
	MockPromoteStrategy promote_strategy;
	SuperPersonMapper mapper;

	BrigadeService* service = nullptr;

	void SetUp() override {
		service = new BrigadeService(
			&person_repo,
			&worksite_repo,
			&promote_strategy,
			&mapper
		);
	}

	void TearDown() override {
		delete service;
	}
};

TEST_F(BrigadeServiceTest, CreateWorksite) {
	int id = service->create_worksite(100.0);
	EXPECT_TRUE(worksite_repo.contains(id));
}

TEST_F(BrigadeServiceTest, ChangeWorksiteVolume) {
	int id = service->create_worksite(100.0);
	service->change_worksite_volume(id, 200.0);

	auto ws = worksite_repo.get_worksite(id);
	EXPECT_EQ(ws->get_work_volume(), 200.0);
}

TEST_F(BrigadeServiceTest, AssignPersonToWorksite) {
	auto* worker = new Worker(1, 30, 10, "Ivan");
	person_repo.add_person(worker);

	int ws_id = service->create_worksite(100.0);
	service->assign_person_to_site(1, ws_id);

	auto ws = worksite_repo.get_worksite(ws_id);
	EXPECT_TRUE(ws->has_person(1));
}

TEST_F(BrigadeServiceTest, RemovePersonFromWorksite) {
	auto* worker = new Worker(1, 30, 10, "Ivan");
	person_repo.add_person(worker);

	int ws_id = service->create_worksite(100.0);
	service->assign_person_to_site(1, ws_id);
	service->remove_person_from_site(1, ws_id);

	auto ws = worksite_repo.get_worksite(ws_id);
	EXPECT_FALSE(ws->has_person(1));
}

TEST_F(BrigadeServiceTest, CalculateSiteOutput) {
	auto* worker1 = new Worker(1, 30, 10, "A");
	auto* worker2 = new Worker(2, 30, 5, "B");

	person_repo.add_person(worker1);
	person_repo.add_person(worker2);

	int ws_id = service->create_worksite(15);
	service->assign_person_to_site(1, ws_id);
	service->assign_person_to_site(2, ws_id);

	double output = service->calculate_site_output(ws_id);
	EXPECT_GT(output, 0.0);
}

TEST_F(BrigadeServiceTest, AllTasksCompleted) {
	auto* worker = new Worker(1, 30, 5, "Ivan");
	person_repo.add_person(worker);

	int ws_id = service->create_worksite(5);
	service->assign_person_to_site(1, ws_id);

	service->simulate_work_day(worksite_repo.get_all().size());

	EXPECT_TRUE(service->all_tasks_completed());
}

TEST_F(BrigadeServiceTest, GetMostProblematicSite) {
	auto* worker = new Worker(1, 30, 5, "Ivan");
	person_repo.add_person(worker);

	int ws1 = service->create_worksite(50.0);
	int ws2 = service->create_worksite(200.0);

	service->assign_person_to_site(1, ws1);

	int problematic = service->get_most_problematic_site_id();
	EXPECT_EQ(problematic, ws2);
}

TEST_F(BrigadeServiceTest, ChangeWorkerProductivity) {
	auto* worker = new Worker(1, 30, 5, "Ivan");
	person_repo.add_person(worker);

	service->change_worker_productivity(1, 4);
	EXPECT_EQ(worker->get_productivity(), 4);
}

TEST_F(BrigadeServiceTest, ChangeMasterEfficiency) {
	auto* master = new Master(1, 30, 1.5, "Petr");
	person_repo.add_person(master);

	service->change_master_efficiency(1, 2.0);
	EXPECT_DOUBLE_EQ(master->get_efficiency(), 2.0);
}

TEST_F(BrigadeServiceTest, PromoteWorkerToMaster) {
	auto* worker = new Worker(1, 30, 10, "Ivan");
	person_repo.add_person(worker);

	service->promote_worker_to_master(1, 1.8);

	IPerson* p = person_repo.get_person(1);
	EXPECT_TRUE(dynamic_cast<IMaster*>(p) != nullptr);
	EXPECT_TRUE(promote_strategy.called);
}

TEST_F(BrigadeServiceTest, ChangeWorksiteVolumeThrowsIfNotFound) {
    EXPECT_THROW(service->change_worksite_volume(999, 100.0), std::runtime_error);
}

TEST_F(BrigadeServiceTest, AssignPersonToSiteThrowsIfPersonNotFound) {
    int ws_id = service->create_worksite(50.0);
    EXPECT_THROW(service->assign_person_to_site(999, ws_id), std::runtime_error);
}

TEST_F(BrigadeServiceTest, AssignPersonToSiteThrowsIfWorksiteNotFound) {
    auto* worker = new Worker(1, 30, 10, "Ivan");
    person_repo.add_person(worker);
    EXPECT_THROW(service->assign_person_to_site(1, 999), std::runtime_error);
}

TEST_F(BrigadeServiceTest, RemovePersonFromSiteThrowsIfWorksiteNotFound) {
    EXPECT_THROW(service->remove_person_from_site(1, 999), std::runtime_error);
}

TEST_F(BrigadeServiceTest, CalculateSiteOutputThrowsIfWorksiteNotFound) {
	[[maybe_unused]] auto result = [&]{ return service->calculate_site_output(999); };
	EXPECT_THROW(result(), std::runtime_error);
}

TEST_F(BrigadeServiceTest, GetMostProblematicSiteThrowsIfNoWorksites) {
	[[maybe_unused]] auto result = [&] { return service->get_most_problematic_site_id(); };
    EXPECT_THROW(result(), std::runtime_error);
}

TEST_F(BrigadeServiceTest, ChangeWorkerProductivityThrowsIfNotWorker) {
    auto* master = new Master(1, 30, 1.5, "Petr");
    person_repo.add_person(master);
    EXPECT_THROW(service->change_worker_productivity(1, 10), std::runtime_error);
}

TEST_F(BrigadeServiceTest, ChangeMasterEfficiencyThrowsIfNotMaster) {
    auto* worker = new Worker(1, 30, 10, "Ivan");
    person_repo.add_person(worker);
    EXPECT_THROW(service->change_master_efficiency(1, 2.0), std::runtime_error);
}

TEST_F(BrigadeServiceTest, SimulateWorkDayHandlesLessThanTwoPersons) {
    auto* worker = new Worker(1, 30, 10, "Ivan");
    person_repo.add_person(worker);

    int ws_id = service->create_worksite(50.0);
    service->assign_person_to_site(1, ws_id);

    EXPECT_NO_THROW(service->simulate_work_day(worksite_repo.get_all().size()));
    EXPECT_NO_THROW(service->simulate_work_day_single_thread(worksite_repo.get_all().size()));
}

TEST_F(BrigadeServiceTest, SetAndGetForemanId) {
    service->set_foreman_id(42);
    EXPECT_EQ(service->get_foreman_id(), 42);
}

TEST_F(BrigadeServiceTest, SimulateWorkDayMultiplePersonsAndSites) {
    for (int i = 1; i <= 5; ++i) {
        person_repo.add_person(new Worker(i, 25 + i, i * 2, "Worker" + std::to_string(i)));
    }

    int ws1 = service->create_worksite(50.0);
    int ws2 = service->create_worksite(100.0);

    for (int i = 1; i <= 5; ++i) {
        service->assign_person_to_site(i, (i % 2 == 0 ? ws1 : ws2));
    }

    EXPECT_NO_THROW(service->simulate_work_day(worksite_repo.get_all().size()));
}

TEST_F(BrigadeServiceTest, SimulateWorkDaySingleThreadEquivalence) {
    auto* w1 = new Worker(1, 30, 10, "A");
    auto* w2 = new Worker(2, 30, 5, "B");

    person_repo.add_person(w1);
    person_repo.add_person(w2);

    int ws = service->create_worksite(30);
    service->assign_person_to_site(1, ws);
    service->assign_person_to_site(2, ws);

    EXPECT_NO_THROW(service->simulate_work_day(worksite_repo.get_all().size()));
    EXPECT_NO_THROW(service->simulate_work_day_single_thread(worksite_repo.get_all().size()));
}

TEST_F(BrigadeServiceTest, SimulateWorkDayLessThanTwoPersons) {
    auto* w1 = new Worker(1, 30, 10, "Solo");
    person_repo.add_person(w1);

    int ws = service->create_worksite(50.0);
    service->assign_person_to_site(1, ws);

    EXPECT_NO_THROW(service->simulate_work_day(worksite_repo.get_all().size()));
    EXPECT_NO_THROW(service->simulate_work_day_single_thread(worksite_repo.get_all().size()));
}

TEST_F(BrigadeServiceTest, SimulateWorkDayEmptyWorksites) {
    EXPECT_NO_THROW(service->simulate_work_day(worksite_repo.get_all().size()));
    EXPECT_NO_THROW(service->simulate_work_day_single_thread(worksite_repo.get_all().size()));
}