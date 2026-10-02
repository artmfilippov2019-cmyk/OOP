#ifndef LAB3_IBRIGADEPRESENTER_H
#define LAB3_IBRIGADEPRESENTER_H

#include <vector>
#include <string>
#include "dto/WorksiteDto.hpp"
#include "dto/PersonDto.hpp"
#include "TimingPoint.hpp"

class IBrigadePresenter {
public:
	virtual ~IBrigadePresenter() = default;

	virtual void on_add_person(const PersonDto& dto) const = 0;
	virtual void on_remove_person(int person_id) const = 0;

	[[nodiscard]] virtual PersonDto on_get_person(int id) const = 0;
	[[nodiscard]] virtual std::vector<PersonDto> on_get_all_persons() const = 0;

	virtual void on_change_worker_productivity(int worker_id, int new_value) const = 0;
	virtual void on_change_master_efficiency(int master_id, double new_value) const = 0;
	virtual void on_promote_worker_to_master(int worker_id, double new_efficiency) const = 0;

	virtual void on_add_friendship(int id1, int id2) const = 0;
	virtual void on_add_enmity(int id1, int id2) const = 0;
	virtual void on_remove_friendship(int id1, int id2) const = 0;
	virtual void on_remove_enmity(int id1, int id2) const = 0;

	[[nodiscard]] virtual int on_create_worksite(double volume) const = 0;
	virtual void on_change_worksite_volume(int site_id, double new_volume) const = 0;
	virtual void on_assign_person_to_site(int person_id, int site_id) const = 0;
	virtual void on_remove_person_from_site(int person_id, int site_id) const = 0;

	[[nodiscard]] virtual WorksiteDto on_get_work_site(int site_id) const = 0;
	[[nodiscard]] virtual std::vector<WorksiteDto> on_get_all_sites() const = 0;

	[[nodiscard]] virtual double on_calculate_site_output(int site_id) const = 0;
	virtual void on_simulate_work_day() const = 0;

	[[nodiscard]] virtual bool on_all_tasks_completed() const = 0;
	[[nodiscard]] virtual int on_get_most_problematic_site_id() const = 0;

	virtual void on_save(const std::string& filename) const = 0;
	virtual void on_load(const std::string& filename) const = 0;

	[[nodiscard]] virtual std::vector<TimingPoint> on_timing(const std::vector<int>& worksites_counts, int runs) const = 0;
};

#endif