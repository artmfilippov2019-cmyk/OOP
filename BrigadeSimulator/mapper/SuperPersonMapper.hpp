#ifndef LAB3_SUPERPERSONMAPPER_H
#define LAB3_SUPERPERSONMAPPER_H

#include "PersonMapper.hpp"

class SuperPersonMapper {
	std::vector<PersonMapper*> mappers;
public:
	~SuperPersonMapper();

	void add_mapper(PersonMapper* m);
	PersonDto to_dto(const IPerson* p) const;
	[[nodiscard]] IPerson* from_dto(const PersonDto& dto) const;
};

#endif