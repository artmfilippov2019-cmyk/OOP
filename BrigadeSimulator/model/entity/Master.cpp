#include "Master.hpp"
#include <stdexcept>

Master::Master(int id, int age, double efficiency, const std::string &name)
	: Person(id, age, name), efficiency(efficiency) {
	if (efficiency <= 1 || efficiency > 5) {
		throw std::invalid_argument("Master efficiency must be in the range (1..5]");
	}
}

void Master::set_efficiency(double value){
	if (value <= 1 || value > 5) {
		throw std::invalid_argument("Master efficiency must be in the range (1..5]");
	}
	efficiency = value;
}

double Master::get_efficiency() const { return efficiency; }