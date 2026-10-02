#ifndef LAB3_PERSONMAPPER_H
#define LAB3_PERSONMAPPER_H

#include <string>
#include "model/entity/Person.hpp"
#include "dto/PersonDto.hpp"

class PersonMapper {
public:
	virtual ~PersonMapper() = default;

	[[nodiscard]] virtual std::string get_type_name() const = 0;
	[[nodiscard]] virtual const std::type_info& get_type_index() const = 0;
	virtual PersonDto to_dto(const IPerson* p) const = 0;
	[[nodiscard]] virtual IPerson* from_dto(const PersonDto& dto) const = 0;
};

#endif