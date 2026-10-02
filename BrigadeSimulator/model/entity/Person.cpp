#include "Person.hpp"
#include <algorithm>
#include <stdexcept>

Person::Person(int id, int age, const std::string &name)
	: id(id), age(age), name(name) {}

int Person::get_id() const { return id; }

const std::string& Person::get_name() const { return name; }

int Person::get_age() const { return age; }

const std::vector<int>& Person::get_friends() const { return friends; }

const std::vector<int>& Person::get_enemies() const { return enemies; }

void Person::add_friend(int person_id) {
	if (person_id == id) {
		throw std::invalid_argument("You cannot add yourself as a friend");
	}
	if (std::ranges::find(friends, person_id) != friends.end()) {
		return;
	}
	friends.push_back(person_id);
}

void Person::add_enemy(int person_id) {
	if (person_id == id) {
		throw std::invalid_argument("You cannot add yourself as an enemy");
	}
	if (std::ranges::find(enemies, person_id) != enemies.end()) {
		return;
	}
	enemies.push_back(person_id);
}

void Person::remove_friend(int person_id) {
	auto it = std::ranges::find(friends, person_id);
	if (it != friends.end()) {
		friends.erase(it);
	}
}

void Person::remove_enemy(int person_id) {
	auto it = std::ranges::find(enemies, person_id);
	if (it != enemies.end()) {
		enemies.erase(it);
	}
}
