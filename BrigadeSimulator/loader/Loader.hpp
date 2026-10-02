#ifndef LAB3_LOADER_H
#define LAB3_LOADER_H

#include "mapper/SuperPersonMapper.hpp"
#include "mapper/WorksiteMapper.hpp"
#include "model/repository/PersonRepository.hpp"
#include "model/service/BrigadeService.hpp"
#include "model/service/StateBrigadeService.hpp"
#include "view/BrigadeView.hpp"
#include "presenter/BrigadePresenter.hpp"
#include "model/service/StrategyPromote.hpp"

class Loader {
public:
	virtual ~Loader() = default;

	virtual SuperPersonMapper& get_super_person_mapper() = 0;
	virtual WorksiteMapper& get_worksite_mapper() = 0;
	virtual PersonRepository& get_person_repository() = 0;
	virtual WorksiteRepository& get_worksite_repository() = 0;
	virtual BrigadeService& get_service() = 0;
	virtual StateBrigadeService& get_state_service() = 0;
	virtual BrigadeView& get_view() = 0;
	virtual BrigadePresenter& get_presenter() = 0;
};

#endif