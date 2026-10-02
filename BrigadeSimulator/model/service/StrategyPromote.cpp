#include "StrategyPromote.hpp"
#include "dto/PersonDto.hpp"
#include "model/entity/Master.hpp"
#include "model/entity/Worker.hpp"

StrategyPromote::StrategyPromote(SuperPersonMapper *super_person_mapper) : super_person_mapper(super_person_mapper) {}

void StrategyPromote::promote(PersonRepository *person_repo, int worker_id, double new_efficiency) const {
	IPerson *person = person_repo->get_person(worker_id);
	if (typeid(Worker) == typeid(*person)) {
		PersonDto master_dto;
		master_dto.dto["type"] = std::string("master");
		master_dto.dto["id"] = person->get_id();
		master_dto.dto["name"] = person->get_name();
		master_dto.dto["age"] = person->get_age();
		master_dto.dto["friends"] = person->get_friends();
		master_dto.dto["enemies"] = person->get_enemies();
		master_dto.dto["efficiency"] = new_efficiency;
		person_repo->remove_person(worker_id);
		person_repo->add_person(super_person_mapper->from_dto(master_dto));
	}
	else {
		throw std::runtime_error("Person is not a worker");
	}
}
