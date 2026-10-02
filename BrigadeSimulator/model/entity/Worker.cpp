#include "Worker.hpp"
#include <stdexcept>

Worker::Worker(int id, int age, int productivity_value, const std::string& name)
	: Person(id, age, name), productivity_value(productivity_value) {
	if (productivity_value < 1 || productivity_value > 10) {
		throw std::invalid_argument("Worker productivity must be in the range [1..10]");
	}
}

void Worker::set_productivity_value(int value) {
	if (value < 1 || value > 10) {
		throw std::invalid_argument("Worker productivity must be in the range [1..10]");
	}
	productivity_value = value;
}

int Worker::get_productivity() const { return productivity_value; }
