#ifndef LAB3_MASTERMAPPER_H
#define LAB3_MASTERMAPPER_H

#include <string>
#include "PersonMapper.hpp"
#include "dto/PersonDto.hpp"

class MasterMapper : public PersonMapper {
public:
	[[nodiscard]] std::string get_type_name() const override;
	[[nodiscard]] const std::type_info& get_type_index() const override;
	PersonDto to_dto(const IPerson* p) const override;
	[[nodiscard]] IPerson* from_dto(const PersonDto& person_dto) const override;
};

#endif