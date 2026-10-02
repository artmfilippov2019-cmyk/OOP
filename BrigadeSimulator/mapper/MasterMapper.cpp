#include "MasterMapper.hpp"
#include <stdexcept>
#include "model/entity/Master.hpp"
#include <typeinfo>

std::string MasterMapper::get_type_name() const {
	return "master";
}

const std::type_info& MasterMapper::get_type_index() const {
	return typeid(Master);
}

PersonDto MasterMapper::to_dto(const IPerson* p) const {
	auto m = dynamic_cast<const IMaster*>(p);

	PersonDto person_dto;
	person_dto.dto["type"] = get_type_name();
	person_dto.dto["id"] = p->get_id();
	person_dto.dto["age"] = p->get_age();
	person_dto.dto["name"] = p->get_name();
	person_dto.dto["efficiency"] = m->get_efficiency();
	person_dto.dto["friends"] = p->get_friends();
	person_dto.dto["enemies"] = p->get_enemies();

	return person_dto;
}

IPerson* MasterMapper::from_dto(const PersonDto& person_dto) const {
	auto dto = person_dto.dto;

	auto m = new Master(
		std::any_cast<int>(dto.at("id")),
		std::any_cast<int>(dto.at("age")),
		std::any_cast<double>(dto.at("efficiency")),
		std::any_cast<std::string>(dto.at("name"))
	);

	for (int f : std::any_cast<std::vector<int>>(dto.at("friends"))) {
		m->add_friend(f);
	}
	for (int e : std::any_cast<std::vector<int>>(dto.at("enemies"))) {
		m->add_enemy(e);
	}

	return m;
}