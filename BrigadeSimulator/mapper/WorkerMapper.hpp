#ifndef LAB3_WORKERMAPPER_H
#define LAB3_WORKERMAPPER_H

#include "PersonMapper.hpp"
#include "dto/PersonDto.hpp"
#include "model/entity/Worker.hpp"

class WorkerMapper : public PersonMapper {
public:
	[[nodiscard]] std::string get_type_name() const override;
	[[nodiscard]] const std::type_info& get_type_index() const override;
	PersonDto to_dto(const IPerson* p) const override;
	[[nodiscard]] IPerson* from_dto(const PersonDto& dto) const override;
};

#endif