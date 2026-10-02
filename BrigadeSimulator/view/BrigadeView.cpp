#include "BrigadeView.hpp"
#include "presenter/BrigadePresenter.hpp"
#include <iostream>
#include <any>
#include <map>
#include <string>
#include <limits>

BrigadeView::BrigadeView(IBrigadePresenter *p) : presenter(p) {}

void BrigadeView::set_presenter(IBrigadePresenter *p) {
    presenter = p;
}

void BrigadeView::show_menu() {
    std::cout << "\n--- BRIGADE ---\n"
              << "1. Add person\n"
              << "2. Promote worker to master\n"
              << "3. Add worksite\n"
              << "4. Assign worker to site\n"
              << "5. Show all\n"
              << "6. Simulate work day\n"
              << "7. Save state to file\n"
              << "8. Load state from file\n"
              << "9. Remove person\n"
              << "10. Get person\n"
              << "11. Change worker productivity\n"
              << "12. Change master efficiency\n"
              << "13. Add friendship\n"
              << "14. Add enmity\n"
              << "15. Remove friendship\n"
              << "16. Remove enmity\n"
              << "17. Change worksite volume\n"
              << "18. Remove worker from site\n"
              << "19. Get worksite\n"
              << "20. Get all worksites\n"
              << "21. Calculate site output\n"
              << "22. Check if all tasks complete\n"
              << "23. Get most problematic site\n"
	          << "24. Timing\n"
              << "0. Exit\n";
    std::cout << "> ";
}

void BrigadeView::run() {
    while (true) {
        show_menu();
        int cmd;
        std::cin >> cmd;

        switch (cmd) {
            case 1: menu_add_person(); break;
            case 2: menu_promote(); break;
            case 3: menu_add_worksite(); break;
            case 4: menu_assign(); break;
            case 5: menu_show_all(); break;
            case 6: menu_simulate(); break;
            case 7: menu_save(); break;
            case 8: menu_load(); break;
            case 9: menu_remove_person(); break;
            case 10: menu_get_person(); break;
            case 11: menu_change_worker_productivity(); break;
            case 12: menu_change_master_efficiency(); break;
            case 13: menu_add_friendship(); break;
            case 14: menu_add_enmity(); break;
            case 15: menu_remove_friendship(); break;
            case 16: menu_remove_enmity(); break;
            case 17: menu_change_worksite_volume(); break;
            case 18: menu_remove_person_from_site(); break;
            case 19: menu_get_worksite(); break;
            case 20: menu_get_all_worksites(); break;
            case 21: menu_calculate_site_output(); break;
            case 22: menu_check_all_completed(); break;
            case 23: menu_get_most_problematic(); break;
        	case 24: menu_timing(); break;

            case 0: return;
            default: std::cout << "Invalid command number\n";
        }
    }
}

void BrigadeView::menu_add_person() const {
	try {
		std::map<std::string, std::any> dto;
		int id, age;
		std::string name;
		int person_type;

		std::cout << "Enter id: ";
		while (!(std::cin >> id)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input. Enter a valid integer for id: ";
		}

		std::cout << "Enter name: ";
		std::cin.ignore();
		std::getline(std::cin, name);

		std::cout << "Enter age: ";
		while (!(std::cin >> age) || age <= 0) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input. Enter a positive integer for age: ";
		}

		std::cout << "Select person type:\n"
				  << "1. Worker (requires productivity 1-10)\n"
				  << "2. Master (requires efficiency 1.0-5.0)\n"
				  << "3. Foreman\n"
				  << "Your choice: ";

		while (true) {
			if (std::cin >> person_type) {
				if (person_type >= 1 && person_type <= 3) break;
				std::cout << "Invalid choice. Enter 1, 2 or 3: ";
			} else {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid input. Enter 1, 2 or 3: ";
			}
		}

		if (person_type == 3) {
			auto all_persons = presenter->on_get_all_persons();
			for (auto &p : all_persons) {
				try {
					if (std::any_cast<std::string>(p.dto.at("type")) == "foreman") {
						std::cout << "Error: Foreman already exists!\n";
						return;
					}
				} catch (...) {}
			}
		}

		dto["id"] = id;
		dto["name"] = name;
		dto["age"] = age;
		dto["friends"] = std::vector<int>();
		dto["enemies"] = std::vector<int>();

		switch (person_type) {
			case 1:
				dto["type"] = std::string("worker");
			{
				int productivity;
				std::cout << "Enter productivity [1..10]: ";
				while (true) {
					if (std::cin >> productivity) {
						if (productivity >= 1 && productivity <= 10) {
							dto["productivity"] = productivity;
							break;
						}
						std::cout << "Productivity must be between 1 and 10. Try again: ";
					} else {
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "Invalid input. Enter integer between 1-10: ";
					}
				}
			}
				break;

			case 2:
				dto["type"] = std::string("master");
			{
				double efficiency;
				std::cout << "Enter efficiency (1.0..5.0]: ";
				while (true) {
					if (std::cin >> efficiency) {
						if (efficiency > 1.0 && efficiency <= 5.0) {
							dto["efficiency"] = efficiency;
							break;
						}
						std::cout << "Efficiency must be between 1.0 and 5.0. Try again: ";
					} else {
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "Invalid input. Enter number between 1.0-5.0: ";
					}
				}
			}
				break;

			case 3:
				dto["type"] = std::string("foreman");
				break;
			default:
				break;
		}

		presenter->on_add_person(PersonDto(dto));
		std::cout << "Person added successfully!\n";
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_promote() const {
	try {
		int id;
		double new_value;

		std::cout << "Enter id of worker to promote to master: ";
		std::cin >> id;
		std::cout << "Enter efficiency of new master: ";
		std::cin >> new_value;

		presenter->on_promote_worker_to_master(id, new_value);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_add_worksite() const {
	try {
		double volume;
		std::cout << "Enter work volume: ";
		std::cin >> volume;

		int id = presenter->on_create_worksite(volume);
		std::cout << "Created worksite with id: " << id << "\n";
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_assign() const {
	try {
		int pid, wid;
		std::cout << "Person id: ";
		std::cin >> pid;
		std::cout << "Worksite id: ";
		std::cin >> wid;

		presenter->on_assign_person_to_site(pid, wid);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_show_all() const {
    std::cout << "\n--- PERSONS ---\n";

    for (auto& person_dto : presenter->on_get_all_persons()) {
        try {
            auto type = std::any_cast<std::string>(person_dto.dto.at("type"));
            int id = std::any_cast<int>(person_dto.dto.at("id"));
            auto name = std::any_cast<std::string>(person_dto.dto.at("name"));
            int age = std::any_cast<int>(person_dto.dto.at("age"));
            auto friends = std::any_cast<std::vector<int>>(person_dto.dto.at("friends"));
            auto enemies = std::any_cast<std::vector<int>>(person_dto.dto.at("enemies"));

            std::cout << "Id: " << id
                      << ", name: " << name
                      << ", age: " << age
                      << ", type: " << type;

            if (type == "worker") {
                std::cout << ", productivity: " << std::any_cast<int>(person_dto.dto.at("productivity"));
            }
            if (type == "master") {
                std::cout << ", efficiency: " << std::any_cast<double>(person_dto.dto.at("efficiency"));
            }

            std::cout << ", friends: ";
            for (size_t i = 0; i < friends.size(); ++i) {
                std::cout << friends[i];
                if (i + 1 < friends.size()) std::cout << ", ";
            }

            std::cout << ", enemies: ";
            for (size_t i = 0; i < enemies.size(); ++i) {
                std::cout << enemies[i];
                if (i + 1 < enemies.size()) std::cout << ", ";
            }

            std::cout << "\n";
        }
        catch (std::exception &e) {
            std::cout << "Error reading dto: " << e.what() << "\n";
        }
    }

    std::cout << "\n--- WORKSITES ---\n";
    for (auto& w : presenter->on_get_all_sites()) {
        try {
            std::cout << "Id: " << w.id
                      << ", total volume: " << w.work_volume
                      << ", remaining: " << w.remaining_work;

            std::cout << ", assigned workers: ";
            const auto& assigned = w.assigned_ids;
            for (size_t i = 0; i < assigned.size(); ++i) {
                std::cout << assigned[i];
                if (i + 1 < assigned.size()) std::cout << ", ";
            }
            std::cout << "\n";
        }
        catch (...) {
            std::cout << "Error reading worksite\n";
        }
    }
}

void BrigadeView::menu_get_person() const {
    int id;
    std::cout << "Enter person id: ";
    std::cin >> id;

    auto person_dto = presenter->on_get_person(id);

    try {
        std::cout << "Id: " << std::any_cast<int>(person_dto.dto.at("id"))
                  << ", name: " << std::any_cast<std::string>(person_dto.dto.at("name"))
                  << ", age: " << std::any_cast<int>(person_dto.dto.at("age"))
                  << ", type: " << std::any_cast<std::string>(person_dto.dto.at("type"));

        auto friends = std::any_cast<std::vector<int>>(person_dto.dto.at("friends"));
        auto enemies = std::any_cast<std::vector<int>>(person_dto.dto.at("enemies"));

        std::cout << ", friends: ";
        for (size_t i = 0; i < friends.size(); ++i) {
            std::cout << friends[i];
            if (i + 1 < friends.size()) std::cout << ", ";
        }

        std::cout << ", enemies: ";
        for (size_t i = 0; i < enemies.size(); ++i) {
            std::cout << enemies[i];
            if (i + 1 < enemies.size()) std::cout << ", ";
        }

        std::cout << "\n";
    }
    catch (...) {
        std::cout << "Error reading dto\n";
    }
}

void BrigadeView::menu_simulate() const {
	try {
		presenter->on_simulate_work_day();
		std::cout << "Work day simulation completed\n";
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_save() const {
	try {
		std::string file;
		std::cout << "Enter filename to save state: ";
		std::cin >> file;
		presenter->on_save(file);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_load() const {
	try {
		std::string file;
		std::cout << "Enter filename to load state: ";
		std::cin >> file;
		presenter->on_load(file);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_remove_person() const {
	try {
		int id;
		std::cout << "Enter person id: ";
		std::cin >> id;
		presenter->on_remove_person(id);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_change_worker_productivity() const {
	try {
		int id, value;
		std::cout << "Worker id: ";
		std::cin >> id;
		std::cout << "New productivity: ";
		std::cin >> value;
		presenter->on_change_worker_productivity(id, value);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_change_master_efficiency() const {
	try {
		int id;
		double value;
		std::cout << "Master id: ";
		std::cin >> id;
		std::cout << "New efficiency: ";
		std::cin >> value;
		presenter->on_change_master_efficiency(id, value);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_add_friendship() const {
	try {
		int a, b;
		std::cout << "Person A: ";
		std::cin >> a;
		std::cout << "Person B: ";
		std::cin >> b;
		presenter->on_add_friendship(a, b);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_add_enmity() const {
	try {
		int a, b;
		std::cout << "Person A: ";
		std::cin >> a;
		std::cout << "Person B: ";
		std::cin >> b;
		presenter->on_add_enmity(a, b);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_remove_friendship() const {
	try {
		int a, b;
		std::cout << "Person A: ";
		std::cin >> a;
		std::cout << "Person B: ";
		std::cin >> b;
		presenter->on_remove_friendship(a, b);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_remove_enmity() const {
	try {
		int a, b;
		std::cout << "Person A: ";
		std::cin >> a;
		std::cout << "Person B: ";
		std::cin >> b;
		presenter->on_remove_enmity(a, b);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_change_worksite_volume() const {
	try {
		int id;
		double vol;
		std::cout << "Worksite id: ";
		std::cin >> id;
		std::cout << "New volume: ";
		std::cin >> vol;
		presenter->on_change_worksite_volume(id, vol);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_remove_person_from_site() const {
	try {
		int pid, sid;
		std::cout << "Person id: ";
		std::cin >> pid;
		std::cout << "Worksite id: ";
		std::cin >> sid;
		presenter->on_remove_person_from_site(pid, sid);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_get_worksite() const {
	try {
		int id;
		std::cout << "Worksite id: ";
		std::cin >> id;

		auto w_dto = presenter->on_get_work_site(id);

		int wid = w_dto.id;
		double volume = w_dto.work_volume;
		double remaining = w_dto.remaining_work;
		auto assigned = w_dto.assigned_ids;

		std::cout << "Id: " << wid
				  << ", Volume: " << volume
				  << ", Remaining: " << remaining
				  << ", Assigned workers: ";

		for (size_t i = 0; i < assigned.size(); ++i) {
			std::cout << assigned[i];
			if (i + 1 < assigned.size()) std::cout << ", ";
		}

		std::cout << "\n";
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_get_all_worksites() const {
    for (auto& w : presenter->on_get_all_sites()) {
        std::cout << "Id: " << w.id
                  << ", Volume: " << w.work_volume
                  << ", Remaining: " << w.remaining_work << "\n";
    }
}

void BrigadeView::menu_calculate_site_output() const {
	try {
		int id;
		std::cout << "Site id: ";
		std::cin >> id;
		std::cout << "Output = " << presenter->on_calculate_site_output(id) << "\n";
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}

void BrigadeView::menu_check_all_completed() const {
    bool done = presenter->on_all_tasks_completed();
    std::cout << (done ? "All tasks completed\n" : "Tasks remain\n");
}

void BrigadeView::menu_get_most_problematic() const {
	int id = presenter->on_get_most_problematic_site_id();
	std::cout << "Most problematic site id: " << id << "\n";
}

void BrigadeView::menu_timing() const {
	int runs, sites_count_min, sites_count_max, sites_count_step;

	std::cout << "Worksites count range (min max step): ";
	std::cin >> sites_count_min >> sites_count_max >> sites_count_step;

	std::cout << "Runs per configuration: ";
	std::cin >> runs;

	std::vector<int> sites_counts;
	for (int s = sites_count_min; s <= sites_count_max; s += sites_count_step) {
		sites_counts.push_back(s);
	}

	try {
		auto results = presenter->on_timing(
			sites_counts,
			runs
		);

		std::cout << "\n--- TIMING RESULTS ---\n";
		std::cout << "People\tSites\tSingle-thread(ms)\tMulti-thread(ms)\tSpeedup\n";
		for (const auto& res : results) {
			std::cout << res.worksites_count << "\t"
					  << res.single_thread_ms << "\t\t"
					  << res.multi_thread_ms << "\t\t"
					  << res.speedup << "\n";
		}
	} catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << "\n";
	}
}
