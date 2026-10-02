#ifndef LAB3_WORKSITEDTO_H
#define LAB3_WORKSITEDTO_H

#include <vector>

struct WorksiteDto {
	int id = -1;
	double work_volume = 0;
	double remaining_work = 0;
	std::vector<int> assigned_ids;
};

#endif