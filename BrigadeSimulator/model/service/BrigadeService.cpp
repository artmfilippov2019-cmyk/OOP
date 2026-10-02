#include "BrigadeService.hpp"
#include <algorithm>
#include <mutex>
#include <optional>
#include "model/entity/Master.hpp"
#include "model/entity/Worker.hpp"
#include "model/entity/Worksite.hpp"
#include <stdexcept>
#include <random>
#include <thread>
#include "PendingResult.hpp"

BrigadeService::BrigadeService(PersonRepository *p_repo, WorksiteRepository *w_repo, IStrategyPromote* strategy_promote,
SuperPersonMapper *super_person_mapper):
person_repo(p_repo), worksite_repo(w_repo), strategy_promote(strategy_promote),
super_person_mapper(super_person_mapper), foreman_id(-1), next_worksite_id(1) {}

int BrigadeService::create_worksite(double volume) {
    auto *worksite = new Worksite(next_worksite_id, volume, volume, {});
    worksite_repo->add_worksite(worksite);
    return next_worksite_id++;
}

void BrigadeService::change_worksite_volume(int site_id, double new_volume) const {
    if (!worksite_repo->contains(site_id)) {
        throw std::runtime_error("Worksite not found");
    }

    auto worksite = worksite_repo->get_worksite(site_id);
    worksite->set_work_volume(new_volume);
}

void BrigadeService::assign_person_to_site(int person_id, int site_id) const {
    if (!person_repo->contains(person_id)) {
        throw std::runtime_error("Person not found");
    }
    if (!worksite_repo->contains(site_id)) {
        throw std::runtime_error("Worksite not found");
    }

    auto worksite = worksite_repo->get_worksite(site_id);
    auto all_worksites = worksite_repo->get_all();
    for (auto ws : all_worksites) {
        if (ws->has_person(person_id)) {
            ws->remove_person(person_id);
        }
    }
    worksite->add_person(person_id);
}

void BrigadeService::remove_person_from_site(int person_id, int site_id) const {
    if (!worksite_repo->contains(site_id)) {
        throw std::runtime_error("Worksite not found");
    }
    auto worksite = worksite_repo->get_worksite(site_id);
    worksite->remove_person(person_id);
}

double BrigadeService::calculate_site_output(int site_id) const {
    if (!worksite_repo->contains(site_id)) {
        throw std::runtime_error("Worksite not found");
    }
    Worksite* worksite = worksite_repo->get_worksite(site_id);
    return worksite->calculate_daily_output(person_repo->get_storage());
}

bool BrigadeService::all_tasks_completed() const {
    auto worksites = worksite_repo->get_all();
    return std::ranges::all_of(worksites.begin(), worksites.end(),
        [](auto worksite) { return worksite->get_remaining_work() <= 0; });
}

int BrigadeService::get_most_problematic_site_id() const {
    auto worksites = worksite_repo->get_all();
    if (worksites.empty()) {
        throw std::runtime_error("No worksites available");
    }

    int most_problematic_id = worksites[0]->get_id();
    double current_output = calculate_site_output(worksites[0]->get_id());
    double max_remaining_days = current_output > 0 ? worksites[0]->get_remaining_work() / current_output : 999999;

    for (size_t i = 1; i < worksites.size(); ++i) {
        double output = calculate_site_output(worksites[i]->get_id());
        double remaining_days = output > 0 ? worksites[i]->get_remaining_work() / output : 999999;
        if (remaining_days > max_remaining_days) {
            max_remaining_days = remaining_days;
            most_problematic_id = worksites[i]->get_id();
        }
    }

    return most_problematic_id;
}

void BrigadeService::simulate_work_day(size_t w_count) const {
    auto all_worksites = worksite_repo->get_all();
    if (all_worksites.empty()) return;

    w_count = std::min(w_count, all_worksites.size());
    std::vector worksites(all_worksites.begin(),all_worksites.begin() + static_cast<std::vector<Worksite*>::difference_type>(w_count));

    std::unordered_map<int, IPerson*> person_map;
    {
        auto all_persons = person_repo->get_all();
        person_map.reserve(all_persons.size());
        for (auto* p : all_persons) {
            if (p) person_map.emplace(p->get_id(), p);
        }
    }
    const auto& persons_storage = person_repo->get_storage();

    std::vector<PendingResult> pending_results(worksites.size());

    std::mutex queue_mutex;
    size_t next_idx = 0;

    unsigned int thread_count = std::thread::hardware_concurrency();
    thread_count = std::min<unsigned int>(thread_count, worksites.size());

    std::random_device rd;
    size_t base_seed = rd();

    auto worker = [&](unsigned int thread_id) {
        std::mt19937 gen(base_seed + thread_id);

        while (true) {
            size_t my_idx = 0;

            {
                std::lock_guard lock(queue_mutex);

                if (next_idx >= worksites.size()) {
                    break;
                }
                my_idx = next_idx;
                next_idx++;
            }
            Worksite* ws = worksites[my_idx];
            PendingResult result;
            result.ws = ws;

            result.output = ws->calculate_daily_output(persons_storage);

            const auto& ids = ws->get_assigned_ids();
            if (ids.size() >= 2) {
                std::uniform_int_distribution<size_t> dist(0, ids.size() - 1);
                auto pick_pair = [&]() -> std::pair<int, int> {
                    size_t a = dist(gen);
                    size_t b = dist(gen);
                    while (a == b) b = dist(gen);
                    return {ids[a], ids[b]};
                };
                result.new_friends = pick_pair();
                result.new_enemies = pick_pair();
            }

            pending_results[my_idx] = std::move(result);
        }
    };

    std::vector<std::thread> threads;
    threads.reserve(thread_count);
    for (unsigned int i = 0; i < thread_count; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto apply_relationship = [&](const std::optional<std::pair<int, int>>& ids, bool is_friend) {
        if (!ids.has_value()) return;
        auto it1 = person_map.find(ids->first);
        auto it2 = person_map.find(ids->second);
        if (it1 != person_map.end() && it2 != person_map.end()) {
            IPerson* p1 = it1->second;
            IPerson* p2 = it2->second;
            if (is_friend) {
                p1->add_friend(p2->get_id());
                p2->add_friend(p1->get_id());
            } else {
                p1->add_enemy(p2->get_id());
                p2->add_enemy(p1->get_id());
            }
        }
    };

    for (const auto& res : pending_results) {
        if (!res.ws) continue;
        res.ws->apply_work(res.output);
        apply_relationship(res.new_friends, true);
        apply_relationship(res.new_enemies, false);
    }
}

void BrigadeService::simulate_work_day_single_thread(size_t w_count) const {
	auto all_worksites = worksite_repo->get_all();
	if (all_worksites.empty()) return;

	w_count = std::min(w_count, all_worksites.size());
	auto it_end = all_worksites.begin() + static_cast<std::vector<Worksite*>::difference_type>(w_count);
	std::vector worksites(all_worksites.begin(), it_end);

	std::unordered_map<int, IPerson*> person_map;
	for (auto* p : person_repo->get_all()) {
		if (p) person_map[p->get_id()] = p;
	}

	std::mt19937 gen(std::random_device{}());

	for (Worksite* ws : worksites) {
		double output = calculate_site_output(ws->get_id());
		ws->apply_work(output);

		const auto& ids = ws->get_assigned_ids();
		if (ids.size() >= 2) {
			std::uniform_int_distribution<size_t> dist(0, ids.size() - 1);

			auto pick = [&] {
				size_t a = dist(gen), b = dist(gen);
				while (a == b) b = dist(gen);
				return std::pair{ids[a], ids[b]};
			};

			auto [f1, f2] = pick();
			if (auto* p1 = person_map[f1]; p1) {
				if (auto* p2 = person_map[f2]; p2) {
					p1->add_friend(f2);
					p2->add_friend(f1);
				}
			}

			auto [e1, e2] = pick();
			if (auto* p1 = person_map[e1]; p1) {
				if (auto* p2 = person_map[e2]; p2) {
					p1->add_enemy(e2);
					p2->add_enemy(e1);
				}
			}
		}
	}
}

void BrigadeService::change_worker_productivity(int worker_id, int new_value) const {
    IPerson* person = person_repo->get_person(worker_id);
    if (auto worker = dynamic_cast<IWorker *>(person)) {
        worker->set_productivity_value(new_value);
    } else {
        throw std::runtime_error("Person is not a worker");
    }
}

void BrigadeService::change_master_efficiency(int master_id, double new_value) const {
    IPerson* person = person_repo->get_person(master_id);
    if (auto master = dynamic_cast<IMaster *>(person)) {
        master->set_efficiency(new_value);
    } else {
        throw std::runtime_error("Person is not a master");
    }
}

void BrigadeService::promote_worker_to_master(int worker_id, double new_efficiency) const {
	strategy_promote->promote(person_repo, worker_id, new_efficiency);
}

void BrigadeService::set_foreman_id(int id) {
    this->foreman_id = id;
}

int BrigadeService::get_foreman_id() const {
    return foreman_id;
}
