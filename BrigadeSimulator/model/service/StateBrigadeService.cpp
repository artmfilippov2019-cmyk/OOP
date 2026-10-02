#include "StateBrigadeService.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <any>
#include "dto/PersonDto.hpp"
#include <yaml-cpp/yaml.h>

void StateBrigadeService::save_to_file(const std::string &filename) {
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("Error opening file for writing: " + filename);
	}

	YAML::Emitter emitter;
	emitter << YAML::FloatPrecision(4);
	emitter << YAML::DoublePrecision(4);
	emitter << YAML::BeginMap;

	auto persons = person_repo->get_all();
	emitter << YAML::Key << "persons" << YAML::Value << YAML::BeginSeq;
	for (const auto &person : persons) {
		PersonDto p_dto = super_person_mapper->to_dto(person);

		emitter << YAML::BeginMap;
		for (const auto &[key, value] : p_dto.dto) {
			try {
				if (value.type() == typeid(int)) {
					emitter << YAML::Key << key << YAML::Value << std::any_cast<int>(value);
				}
				else if (value.type() == typeid(double)) {
					emitter << YAML::Key << key << YAML::Value << std::any_cast<double>(value);
				}
				else if (value.type() == typeid(std::string)) {
					emitter << YAML::Key << key << YAML::Value << std::any_cast<std::string>(value);
				}
				else if (value.type() == typeid(std::vector<int>)) {
					emitter << YAML::Key << key << YAML::Value << YAML::BeginSeq;
					for (int id : std::any_cast<const std::vector<int>&>(value)) {
						emitter << id;
					}
					emitter << YAML::EndSeq;
				}
				else {
					throw std::runtime_error("Unknown type in PersonDto for key: " + key);
				}
			}
			catch (const std::bad_any_cast &e) {
				throw std::runtime_error("Error casting value for key '" + key + "': " + e.what());
			}
		}
		emitter << YAML::EndMap;
	}
	emitter << YAML::EndSeq;

	auto worksites = worksite_repo->get_all();
	emitter << YAML::Key << "worksites" << YAML::Value << YAML::BeginSeq;
	for (const auto &ws : worksites) {
		WorksiteDto w_dto = WorksiteMapper::to_dto(ws);

		emitter << YAML::BeginMap;
		emitter << YAML::Key << "id" << YAML::Value << w_dto.id;
		emitter << YAML::Key << "work_volume" << YAML::Value << w_dto.work_volume;
		emitter << YAML::Key << "remaining_work" << YAML::Value << w_dto.remaining_work;

		emitter << YAML::Key << "assigned_ids" << YAML::Value << YAML::BeginSeq;
		for (int pid : w_dto.assigned_ids) {
			emitter << pid;
		}
		emitter << YAML::EndSeq;

		emitter << YAML::EndMap;
	}
	emitter << YAML::EndSeq;

	emitter << YAML::EndMap;

	out << emitter.c_str();
	out.close();
}

void StateBrigadeService::load_from_file(const std::string &filename) {
    YAML::Node config;
    try {
        config = YAML::LoadFile(filename);
    }
	catch (const std::exception &e) {
        throw std::runtime_error(std::string("Error loading YAML file: ") + e.what());
    }

    for (auto person : person_repo->get_all()) person_repo->remove_person(person->get_id());
    for (auto ws : worksite_repo->get_all()) worksite_repo->remove_worksite(ws->get_id());

    if (config["persons"] && config["persons"].IsSequence()) {
        for (const auto &node : config["persons"]) {
            PersonDto dto;

            for (const auto &it : node) {
                const auto key = it.first.as<std::string>();
                const YAML::Node &value = it.second;

                try {
                	if (value.IsScalar()) {
                        try {
	                        dto.dto[key] = value.as<int>();
                        }
                        catch (const std::exception &) {
                            try {
	                            dto.dto[key] = value.as<double>();
                            }
                            catch (const std::exception &) {
                                dto.dto[key] = value.as<std::string>();
                            }
                        }
                    }
                	else if (value.IsSequence()) {
                        std::vector<int> ids;
                        for (const auto &n : value) ids.push_back(n.as<int>());
                        dto.dto[key] = ids;
                    }
                	else {
                        throw std::runtime_error("Unknown field type in YAML for key: " + key);
                    }
                }
            	catch (const std::exception &e) {
                    throw std::runtime_error("Error parsing key '" + key + "': " + e.what());
                }
            }

            IPerson* person = super_person_mapper->from_dto(dto);
            person_repo->add_person(person);
        }
    }

    if (config["worksites"] && config["worksites"].IsSequence()) {
        for (const auto &wnode : config["worksites"]) {
            WorksiteDto w_dto;

            for (const auto &it : wnode) {
                const auto key = it.first.as<std::string>();
                const YAML::Node &value = it.second;

                try {
                	if (value.IsScalar()) {
                        if (key == "id") w_dto.id = value.as<int>();
                        else if (key == "work_volume") w_dto.work_volume = value.as<int>();
                        else if (key == "remaining_work") w_dto.remaining_work = value.as<int>();
                        else throw std::runtime_error("Unknown scalar key in Worksite: " + key);
                    }
                	else if (value.IsSequence() && key == "assigned_ids") {
                        for (const auto &n : value) w_dto.assigned_ids.push_back(n.as<int>());
                    }
                	else {
                        throw std::runtime_error("Unknown field type in YAML for Worksite: " + key);
                    }
                }
            	catch (const std::exception &e) {
                    throw std::runtime_error("Error parsing worksite key '" + key + "': " + e.what());
                }
            }

            Worksite* ws = WorksiteMapper::from_dto(w_dto);
            worksite_repo->add_worksite(ws);
        }
    }
}