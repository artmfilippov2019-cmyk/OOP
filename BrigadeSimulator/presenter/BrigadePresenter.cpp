#include "BrigadePresenter.hpp"
#include <chrono>
#include <random>
#include "model/service/BrigadeService.hpp"
#include "model/service/StateBrigadeService.hpp"
#include "mapper/SuperPersonMapper.hpp"
#include "mapper/WorksiteMapper.hpp"
#include "dto/WorksiteDto.hpp"

BrigadePresenter::BrigadePresenter(BrigadeService& service, IBrigadeView& view, IStateBrigadeService& state_service,
	SuperPersonMapper& person_mapper, PersonRepository &person_repo, WorksiteRepository &worksite_repo)
: service(service), view(view), state_service(state_service), person_mapper(person_mapper), person_repo(person_repo),
worksite_repo(worksite_repo) {}

void BrigadePresenter::on_add_person(const PersonDto& person_dto) const {
    IPerson* p = person_mapper.from_dto(person_dto);
	person_repo.add_person(p);
}

void BrigadePresenter::on_remove_person(int person_id) const {
	person_repo.remove_person(person_id);
}

PersonDto BrigadePresenter::on_get_person(int id) const {
    IPerson* p = person_repo.get_person(id);
    return person_mapper.to_dto(p);
}

std::vector<PersonDto> BrigadePresenter::on_get_all_persons() const {
    std::vector<PersonDto> result;
    for (IPerson* p : person_repo.get_all())
        result.push_back(person_mapper.to_dto(p));
    return result;
}

void BrigadePresenter::on_change_worker_productivity(int worker_id, int new_value) const {
    service.change_worker_productivity(worker_id, new_value);
}

void BrigadePresenter::on_change_master_efficiency(int master_id, double new_value) const {
    service.change_master_efficiency(master_id, new_value);
}

void BrigadePresenter::on_promote_worker_to_master(int worker_id, double new_efficiency) const {
    service.promote_worker_to_master(worker_id, new_efficiency);
}

void BrigadePresenter::on_add_friendship(int id1, int id2) const {
	IPerson* p1 = person_repo.get_person(id1);
	IPerson* p2 = person_repo.get_person(id2);
	p1->add_friend(id2);
	p2->add_friend(id1);
}

void BrigadePresenter::on_add_enmity(int id1, int id2) const {
	IPerson* p1 = person_repo.get_person(id1);
	IPerson* p2 = person_repo.get_person(id2);
	p1->add_enemy(id2);
	p2->add_enemy(id1);
}

void BrigadePresenter::on_remove_friendship(int id1, int id2) const {
	IPerson* p1 = person_repo.get_person(id1);
	IPerson* p2 = person_repo.get_person(id2);
	p1->remove_friend(id2);
	p2->remove_friend(id1);
}

void BrigadePresenter::on_remove_enmity(int id1, int id2) const {
	IPerson* p1 = person_repo.get_person(id1);
	IPerson* p2 = person_repo.get_person(id2);
	p1->remove_enemy(id2);
	p2->remove_enemy(id1);
}

int BrigadePresenter::on_create_worksite(double volume) const {
    return service.create_worksite(volume);
}

void BrigadePresenter::on_change_worksite_volume(int site_id, double new_volume) const {
    service.change_worksite_volume(site_id, new_volume);
}

void BrigadePresenter::on_assign_person_to_site(int person_id, int site_id) const {
    service.assign_person_to_site(person_id, site_id);
}

void BrigadePresenter::on_remove_person_from_site(int person_id, int site_id) const {
    service.remove_person_from_site(person_id, site_id);
}

WorksiteDto BrigadePresenter::on_get_work_site(int site_id) const {
    Worksite* ws = worksite_repo.get_worksite(site_id);
    return WorksiteMapper::to_dto(ws);
}

std::vector<WorksiteDto> BrigadePresenter::on_get_all_sites() const {
    std::vector<WorksiteDto> result;
    for (Worksite* ws : worksite_repo.get_all())
        result.push_back(WorksiteMapper::to_dto(ws));
    return result;
}

double BrigadePresenter::on_calculate_site_output(int site_id) const {
    return service.calculate_site_output(site_id);
}

void BrigadePresenter::on_simulate_work_day() const {
    service.simulate_work_day(worksite_repo.get_all().size());
}

bool BrigadePresenter::on_all_tasks_completed() const {
    return service.all_tasks_completed();
}

int BrigadePresenter::on_get_most_problematic_site_id() const {
    return service.get_most_problematic_site_id();
}

void BrigadePresenter::on_save(const std::string& filename) const {
    state_service.save_to_file(filename);
}

void BrigadePresenter::on_load(const std::string& filename) const {
    state_service.load_from_file(filename);
}

std::vector<TimingPoint> BrigadePresenter::on_timing(const std::vector<int>& worksites_counts, int runs) const {
	using clock = std::chrono::high_resolution_clock;
	std::vector<TimingPoint> results;

	for (int w_count : worksites_counts) {
		auto measure = [&](bool multithreaded) {
			double total_ms = 0.0;

			for (int r = 0; r < runs; ++r) {
				auto start = clock::now();
				if (multithreaded)
					service.simulate_work_day(w_count);
				else
					service.simulate_work_day_single_thread(w_count);
				auto end = clock::now();

				total_ms += std::chrono::duration<double, std::milli>(end - start).count();
			}

			return total_ms / runs;
		};

		double st = measure(false);
		double mt = measure(true);
		results.push_back({w_count, st, mt, st / mt});
	}

	return results;
}