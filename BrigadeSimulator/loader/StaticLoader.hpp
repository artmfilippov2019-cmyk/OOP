#ifndef LAB3_STATICLOADER_H
#define LAB3_STATICLOADER_H

#include "Loader.hpp"

class StaticLoader: public Loader {
	SuperPersonMapper super_person_mapper;
	WorksiteMapper worksite_mapper;
	PersonRepository person_repository;
	WorksiteRepository worksite_repository;
	BrigadeView brigade_view;
	BrigadePresenter brigade_presenter;
	StateBrigadeService state_brigade_service;
	BrigadeService brigade_service;
	StrategyPromote strategy_promote;
public:
	StaticLoader();

	SuperPersonMapper& get_super_person_mapper() override;
	WorksiteMapper& get_worksite_mapper() override;
	PersonRepository& get_person_repository() override;
	WorksiteRepository& get_worksite_repository() override;
	BrigadeService& get_service() override;
	StateBrigadeService& get_state_service() override;
	BrigadeView& get_view() override;
	BrigadePresenter& get_presenter() override;
};


#endif