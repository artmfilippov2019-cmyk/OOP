#include "StaticLoader.hpp"
#include "mapper/ForemanMapper.hpp"
#include "mapper/MasterMapper.hpp"
#include "mapper/WorkerMapper.hpp"

StaticLoader::StaticLoader() : strategy_promote(&super_person_mapper), brigade_service(&person_repository, &worksite_repository, &strategy_promote, &super_person_mapper),
	state_brigade_service(&person_repository, &worksite_repository, &super_person_mapper, &worksite_mapper),
	brigade_presenter(brigade_service, brigade_view, state_brigade_service, super_person_mapper, person_repository, worksite_repository),
	brigade_view(&brigade_presenter) {
	super_person_mapper.add_mapper(new WorkerMapper());
	super_person_mapper.add_mapper(new MasterMapper());
	super_person_mapper.add_mapper(new ForemanMapper());
}

PersonRepository& StaticLoader::get_person_repository() {
	return person_repository;
}

BrigadePresenter& StaticLoader::get_presenter() {
	return brigade_presenter;
}

BrigadeService& StaticLoader::get_service() {
	return brigade_service;
}

StateBrigadeService& StaticLoader::get_state_service() {
	return state_brigade_service;
}

WorksiteMapper& StaticLoader::get_worksite_mapper() {
	return worksite_mapper;
}

BrigadeView& StaticLoader::get_view() {
	return brigade_view;
}

SuperPersonMapper &StaticLoader::get_super_person_mapper() {
	return super_person_mapper;
}

WorksiteRepository &StaticLoader::get_worksite_repository() {
	return worksite_repository;
}
