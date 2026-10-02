#ifndef LAB33_IPERSON_H
#define LAB33_IPERSON_H

#include <string>
#include <vector>

class IPerson {
public:
	virtual ~IPerson() = default;

	[[nodiscard]] virtual int get_id() const = 0;
	[[nodiscard]] virtual const std::string& get_name() const = 0;
	[[nodiscard]] virtual int get_age() const = 0;

	[[nodiscard]] virtual const std::vector<int>& get_friends() const = 0;
	[[nodiscard]] virtual const std::vector<int>& get_enemies() const = 0;

	virtual void add_friend(int person_id) = 0;
	virtual void add_enemy(int person_id) = 0;
	virtual void remove_friend(int person_id) = 0;
	virtual void remove_enemy(int person_id) = 0;
};

#endif