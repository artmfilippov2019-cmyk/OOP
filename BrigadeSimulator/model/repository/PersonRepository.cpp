#include "PersonRepository.hpp"
#include <stdexcept>

const HashTable<int, std::unique_ptr<IPerson>>& PersonRepository::get_storage() const noexcept {
	return storage;
}

void PersonRepository::add_person(IPerson *person) {
	if (person == nullptr) {
		throw std::invalid_argument("Error: null pointer");
	}
	int id = person->get_id();
	if (storage.find(id) != storage.end()) {
		throw std::runtime_error("Person with id " + std::to_string(id) + " already exists");
	}

	auto res = storage.insert(std::make_pair(id, std::unique_ptr<IPerson>(person)));
	if (!res.second) {
		throw std::runtime_error("Insert failed for id " + std::to_string(id));
	}
}

void PersonRepository::remove_person(int id) {
	auto erased = storage.erase(id);
	if (erased == 0) {
		throw std::runtime_error("Person with id " + std::to_string(id) + " not found");
	}
}

IPerson* PersonRepository::get_person(int id) const {
	auto it = storage.find(id);
	if (it == storage.end()) {
		throw std::runtime_error("Person with id " + std::to_string(id) + " not found");
	}
	return it->second.get();
}

std::vector<IPerson *> PersonRepository::get_all() const {
	std::vector<IPerson *> result;
	for (const auto &kv : storage) {
		result.push_back(kv.second.get());
	}
	return result;
}

bool PersonRepository::contains(int id) const {
	return storage.find(id) != storage.end();
}
