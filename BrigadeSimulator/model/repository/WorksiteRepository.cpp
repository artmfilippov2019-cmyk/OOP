#include "WorksiteRepository.hpp"
#include <stdexcept>
#include <algorithm>

void WorksiteRepository::add_worksite(Worksite *ws) {
	if (ws == nullptr) {
		throw std::invalid_argument("Error: null pointer");
	}
	auto id = ws->get_id();
	if (storage.find(id) != storage.end()) {
		throw std::runtime_error("Worksite with id " + std::to_string(ws->get_id()) + " already exists");
	}

	auto res = storage.insert(std::make_pair(id, std::unique_ptr<Worksite>(ws)));
	if (!res.second) {
		throw std::runtime_error("Insert failed for id " + std::to_string(id));
	}
}

void WorksiteRepository::remove_worksite(int id) {
	auto erased = storage.erase(id);
	if (erased == 0) {
		throw std::runtime_error("Worksite with id " + std::to_string(id) + " not found");
	}
}

Worksite* WorksiteRepository::get_worksite(int id) const {
	auto it = storage.find(id);
	if (it == storage.end()) {
		throw std::runtime_error("Worksite with id " + std::to_string(id) + " not found");
	}
	return it->second.get();
}

std::vector<Worksite *> WorksiteRepository::get_all() const {
	std::vector<Worksite *> result;
	for (const auto& kv : storage) {
		result.push_back(kv.second.get());
	}
	return result;
}

bool WorksiteRepository::contains(int id) const {
	return storage.find(id) != storage.end();
}
