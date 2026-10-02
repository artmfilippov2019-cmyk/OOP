#ifndef LAB3_BRIGADEPRESENTER_H
#define LAB3_BRIGADEPRESENTER_H

#include <vector>
#include <string>
#include "IBrigadePresenter.hpp"
#include "mapper/SuperPersonMapper.hpp"
#include "dto/WorksiteDto.hpp"
#include "model/service/StateBrigadeService.hpp"
#include "model/service/BrigadeService.hpp"
#include "view/BrigadeView.hpp"
#include "model/repository/PersonRepository.hpp"
#include "model/repository/WorksiteRepository.hpp"

class BrigadePresenter : public IBrigadePresenter {
    BrigadeService& service;
    IBrigadeView& view;
    IStateBrigadeService& state_service;
    SuperPersonMapper& person_mapper;
    PersonRepository& person_repo;
    WorksiteRepository& worksite_repo;

public:
    BrigadePresenter(
        BrigadeService& service,
        IBrigadeView& view,
        IStateBrigadeService& state_service,
        SuperPersonMapper& person_mapper,
        PersonRepository& person_repo,
        WorksiteRepository& worksite_repo
    );

    void on_add_person(const PersonDto& dto) const override;
    void on_remove_person(int person_id) const override;
    [[nodiscard]] PersonDto on_get_person(int id) const override;
    [[nodiscard]] std::vector<PersonDto> on_get_all_persons() const override;

    void on_change_worker_productivity(int worker_id, int new_value) const override;
    void on_change_master_efficiency(int master_id, double new_value) const override;
    void on_promote_worker_to_master(int worker_id, double new_efficiency) const override;

    void on_add_friendship(int id1, int id2) const override;
    void on_add_enmity(int id1, int id2) const override;
    void on_remove_friendship(int id1, int id2) const override;
    void on_remove_enmity(int id1, int id2) const override;

    [[nodiscard]] int on_create_worksite(double volume) const override;
    void on_change_worksite_volume(int site_id, double new_volume) const override;
    void on_assign_person_to_site(int person_id, int site_id) const override;
    void on_remove_person_from_site(int person_id, int site_id) const override;

    [[nodiscard]] WorksiteDto on_get_work_site(int site_id) const override;
    [[nodiscard]] std::vector<WorksiteDto> on_get_all_sites() const override;
    [[nodiscard]] double on_calculate_site_output(int site_id) const override;

    void on_simulate_work_day() const override;
    [[nodiscard]] bool on_all_tasks_completed() const override;
    [[nodiscard]] int on_get_most_problematic_site_id() const override;

    void on_save(const std::string& filename) const override;
    void on_load(const std::string& filename) const override;

	[[nodiscard]] std::vector<TimingPoint> on_timing(const std::vector<int>& worksites_counts, int runs) const override;
};

#endif