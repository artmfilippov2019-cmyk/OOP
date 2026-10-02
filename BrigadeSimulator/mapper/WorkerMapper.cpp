#include "WorkerMapper.hpp"
#include <stdexcept>
#include <typeinfo>

std::string WorkerMapper::get_type_name() const {
	return "worker";
}

const std::type_info& WorkerMapper::get_type_index() const {
	return typeid(Worker);
}

PersonDto WorkerMapper::to_dto(const IPerson* p) const {
	auto w = dynamic_cast<const IWorker*>(p);

	PersonDto person_dto;
	person_dto.dto["type"] = get_type_name();
	person_dto.dto["id"] = p->get_id();
	person_dto.dto["age"] = p->get_age();
	person_dto.dto["name"] = p->get_name();
	person_dto.dto["productivity"] = w->get_productivity();
	person_dto.dto["friends"] = p->get_friends();
	person_dto.dto["enemies"] = p->get_enemies();

	return person_dto;
}

IPerson* WorkerMapper::from_dto(const PersonDto& person_dto) const {
	auto dto = person_dto.dto;
	auto w = new Worker(
		std::any_cast<int>(dto.at("id")),
		std::any_cast<int>(dto.at("age")),
		std::any_cast<int>(dto.at("productivity")),
		std::any_cast<std::string>(dto.at("name"))
	);

	for (int f : std::any_cast<std::vector<int>>(dto.at("friends"))) {
		w->add_friend(f);
	}
	for (int e : std::any_cast<std::vector<int>>(dto.at("enemies"))) {
		w->add_enemy(e);
	}

	return w;
}