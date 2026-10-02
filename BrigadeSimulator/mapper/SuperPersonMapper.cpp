#include "SuperPersonMapper.hpp"
#include <stdexcept>
#include "model/entity/Foreman.hpp"

SuperPersonMapper::~SuperPersonMapper() {
	for (auto m : mappers) {
		delete m;
	}
}

void SuperPersonMapper::add_mapper(PersonMapper* m) {
	mappers.push_back(m);
}

PersonDto SuperPersonMapper::to_dto(const IPerson* p) const {
	for (auto m: mappers) {
		const std::type_info& index = m->get_type_index();
		std::string name = m->get_type_name();

		if (typeid(*p) == index) {
			PersonDto dto = m->to_dto(p);
			return dto;
		}
	}

	throw std::runtime_error("No such mapper");
}

IPerson* SuperPersonMapper::from_dto(const PersonDto& person_dto) const {
	auto dto = person_dto.dto;
	auto type = std::any_cast<std::string>(dto.at("type"));

	for (auto* m : mappers) {
		if (m->get_type_name() == type)
			return m->from_dto(person_dto);
	}

	throw std::runtime_error("Mapper not found");
}