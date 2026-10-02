#ifndef LAB33_STATEBRIGADESERVICE_H
#define LAB33_STATEBRIGADESERVICE_H

#include <string>
#include "IStateBrigadeService.hpp"
#include "mapper/SuperPersonMapper.hpp"
#include "mapper/WorksiteMapper.hpp"
#include "model/repository/PersonRepository.hpp"
#include "model/repository/WorksiteRepository.hpp"

class StateBrigadeService : public IStateBrigadeService {
	PersonRepository *person_repo;
	WorksiteRepository *worksite_repo;
	SuperPersonMapper *super_person_mapper;
	WorksiteMapper *worksite_mapper;
public:
	StateBrigadeService(PersonRepository *person_repo,
						WorksiteRepository *worksite_repo,
						SuperPersonMapper *super_person_mapper,
						WorksiteMapper *worksite_mapper)
	: person_repo(person_repo), worksite_repo(worksite_repo), super_person_mapper(super_person_mapper),
	worksite_mapper(worksite_mapper) {}

	void save_to_file(const std::string &filename) override;
	void load_from_file(const std::string &filename) override;
};


#endif