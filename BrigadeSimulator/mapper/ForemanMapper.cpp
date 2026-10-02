#include "ForemanMapper.hpp"
#include "model/entity/Foreman.hpp"
#include <typeinfo>

std::string ForemanMapper::get_type_name() const {
	return "foreman";
}

const std::type_info& ForemanMapper::get_type_index() const {
	return typeid(Foreman);
}

PersonDto ForemanMapper::to_dto(const IPerson* p) const {
	auto f = dynamic_cast<const Foreman*>(p);

	PersonDto person_dto;
	person_dto.dto["type"] = get_type_name();
	person_dto.dto["id"] = f->get_id();
	person_dto.dto["age"] = f->get_age();
	person_dto.dto["name"] = f->get_name();
	person_dto.dto["friends"] = f->get_friends();
	person_dto.dto["enemies"] = f->get_enemies();
	return person_dto;
}

IPerson* ForemanMapper::from_dto(const PersonDto& person_dto) const {
	auto dto = person_dto.dto;
	auto f = new Foreman(
		std::any_cast<int>(dto.at("id")),
		std::any_cast<int>(dto.at("age")),
		std::any_cast<std::string>(dto.at("name"))
	);

	for (int x : std::any_cast<std::vector<int>>(dto.at("friends"))) {
		f->add_friend(x);
	}
	for (int x : std::any_cast<std::vector<int>>(dto.at("enemies"))) {
		f->add_enemy(x);
	}

	return f;
}