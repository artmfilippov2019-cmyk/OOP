#ifndef LAB3_PENDINGRESULT_H
#define LAB3_PENDINGRESULT_H

#include "model/entity/Worksite.hpp"
#include <utility>
#include <optional>

struct PendingResult {
	Worksite* ws = nullptr;
	double output = 0.0;
	std::optional<std::pair<int, int>> new_friends;
	std::optional<std::pair<int, int>> new_enemies;
};

#endif