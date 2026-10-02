#ifndef LAB3_STRATEGYPROMOTE_H
#define LAB3_STRATEGYPROMOTE_H

#include "IStrategyPromote.hpp"
#include "mapper/SuperPersonMapper.hpp"
#include "model/repository/PersonRepository.hpp"

class StrategyPromote : public IStrategyPromote {
	SuperPersonMapper *super_person_mapper;
public:
	explicit StrategyPromote(SuperPersonMapper *super_person_mapper);

	void promote(PersonRepository* person_repo, int worker_id, double new_efficiency) const override;
};

#endif