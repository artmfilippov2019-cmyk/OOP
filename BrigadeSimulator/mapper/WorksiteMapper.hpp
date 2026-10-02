#ifndef LAB3_WORKSITEMAPPER_H
#define LAB3_WORKSITEMAPPER_H

#include "model/entity/Worksite.hpp"
#include "dto/WorksiteDto.hpp"

class WorksiteMapper {
public:
	static WorksiteDto to_dto(const Worksite* w);
	static Worksite* from_dto(const WorksiteDto& dto);
};

#endif