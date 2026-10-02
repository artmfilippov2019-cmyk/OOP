#ifndef LAB3_PERSONDTO_H
#define LAB3_PERSONDTO_H

#include <any>
#include <string>
#include <map>

struct PersonDto {
	std::map<std::string, std::any> dto;
};

#endif