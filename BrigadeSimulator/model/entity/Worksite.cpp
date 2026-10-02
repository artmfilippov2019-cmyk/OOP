#include "Worksite.hpp"
#include "Person.hpp"
#include <algorithm>
#include <memory>
#include <stdexcept>
#include "Foreman.hpp"
#include "Master.hpp"
#include "Worker.hpp"

Worksite::Worksite(int id, double work_volume, double remaining_work, const std::vector<int> &assigned_ids)
    : id(id), work_volume(work_volume), remaining_work(remaining_work), assigned_ids(assigned_ids) {
    if (work_volume < 0) {
        throw std::invalid_argument("Объём работ не может быть отрицательным");
    }
    if (remaining_work < 0) {
        throw std::invalid_argument("Оставшийся объём работ не может быть отрицательным");
    }
}

Worksite::Worksite(int id, double work_volume) : id(id), work_volume(work_volume), remaining_work(0) {
	if (work_volume < 0) {
		throw std::invalid_argument("Объём работ не может быть отрицательным");
	}
}

int Worksite::get_id() const { return id; }

double Worksite::get_remaining_work() const { return remaining_work; }

double Worksite::get_work_volume() const { return work_volume; }

const std::vector<int>& Worksite::get_assigned_ids() const { return assigned_ids; }

void Worksite::set_work_volume(double volume) {
    if (volume < 0) {
        throw std::invalid_argument("Объём работ не может быть отрицательным");
    }
    work_volume = volume;
    if (remaining_work > work_volume) {
        remaining_work = work_volume;
    }
}

void Worksite::add_person(int person_id) {
    if (has_person(person_id)) {
        return;
    }
    assigned_ids.push_back(person_id);
}

void Worksite::remove_person(int person_id) {
    auto it = std::ranges::find(assigned_ids, person_id);
    if (it != assigned_ids.end()) {
        assigned_ids.erase(it);
    }
}

bool Worksite::has_person(int person_id) const {
    return std::ranges::find(assigned_ids, person_id) != assigned_ids.end();
}

double Worksite::calculate_daily_output(const HashTable<int, std::unique_ptr<IPerson>>& persons) const {
    double total = 0.0;

    std::vector<IPerson *> present;
    for (int pid : assigned_ids) {
    	auto it = persons.find(pid);
        if (it != persons.end()) {
            IPerson* p = persons.at(pid).get();
            present.push_back(p);
        }
    }

    for (IPerson* p : present) {
        if (auto w = dynamic_cast<IWorker*>(p)) {
            total += static_cast<double>(w->get_productivity());
        }
    }

    for (IPerson* p : present) {
        if (auto m = dynamic_cast<IMaster*>(p)) {
            total *= m->get_efficiency();
        }
    }

    bool has_foreman = false;
    for (IPerson* p : present) {
        if (dynamic_cast<Foreman*>(p)) {
            has_foreman = true;
            break;
        }
    }

    if (!has_foreman) {
        int n = static_cast<int>(present.size());
        double penalty = 0.0;

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                const auto &en_i = present[i]->get_enemies();
                const auto &en_j = present[j]->get_enemies();
                bool mutual_enemy = (std::ranges::find(en_i, present[j]->get_id()) != en_i.end()
                                  && std::ranges::find(en_j, present[i]->get_id()) != en_j.end());
                if (mutual_enemy) {
                    penalty += 1.0;
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    const auto &fi = present[i]->get_friends();
                    const auto &fj = present[j]->get_friends();
                    const auto &fk = present[k]->get_friends();
                    bool ij = (std::ranges::find(fi, present[j]->get_id()) != fi.end()
                            && std::ranges::find(fj, present[i]->get_id()) != fj.end());
                    bool ik = (std::ranges::find(fi, present[k]->get_id()) != fi.end()
                            && std::ranges::find(fk, present[i]->get_id()) != fk.end());
                    bool jk = (std::ranges::find(fj, present[k]->get_id()) != fj.end()
                            && std::ranges::find(fk, present[j]->get_id()) != fk.end());
                    if (ij && ik && jk) {
                        penalty += 1.0;
                    }
                }
            }
        }

        total -= penalty;
    }

    if (total < 0.0) total = 0.0;
    return total;
}

void Worksite::apply_work(const double amount) {
    if (amount < 0) {
        throw std::invalid_argument("Объём работ не может быть отрицательным");
    }
    remaining_work -= amount;
    if (remaining_work < 0) {
        remaining_work = 0;
    }
}

void Worksite::set_remaining_work(double volume) {
	if (volume < 0) {
		throw std::invalid_argument("Оставшийся объём работ не может быть отрицательным");
	}
	remaining_work = volume;
}