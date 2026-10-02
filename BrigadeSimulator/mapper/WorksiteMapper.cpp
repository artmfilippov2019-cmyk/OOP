#include "WorksiteMapper.hpp"
#include "../model/entity/Worksite.hpp"
#include "../dto/WorksiteDto.hpp"

WorksiteDto WorksiteMapper::to_dto(const Worksite* w) {
	WorksiteDto dto;

	dto.id = w->get_id();
	dto.work_volume = w->get_work_volume();
	dto.remaining_work = w->get_remaining_work();
	dto.assigned_ids = w->get_assigned_ids();

	return dto;
}

Worksite* WorksiteMapper::from_dto(const WorksiteDto& dto) {
	auto w = new Worksite(dto.id, dto.work_volume);

	w->set_remaining_work(dto.remaining_work);
	for (int id : dto.assigned_ids) {
		w->add_person(id);
	}

	return w;
}