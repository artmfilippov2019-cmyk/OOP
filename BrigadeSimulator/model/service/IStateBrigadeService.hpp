#ifndef LAB33_ISTATEBRIGADESERVICE_H
#define LAB33_ISTATEBRIGADESERVICE_H

#include <string>

class IStateBrigadeService {
public:
	virtual ~IStateBrigadeService() = default;

	void virtual save_to_file(const std::string &filename) = 0;
	void virtual load_from_file(const std::string &filename) = 0;
};

#endif