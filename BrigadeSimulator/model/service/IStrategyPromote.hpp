/**
* @file IStrategyPromote.hpp
 * @brief Заголовочный файл, содержащий объявление интерфейса стратегии повышения рабочего
 */

#ifndef LAB3_ISTRATEGYPROMOTE_H
#define LAB3_ISTRATEGYPROMOTE_H

#include "model/repository/PersonRepository.hpp"

/**
 * @class IStrategyPromote
 * @brief Интерфейс стратегии повышения рабочего до мастера
 */
class IStrategyPromote {
public:
	/**
	 * @brief Виртуальный деструктор
	 */
	virtual ~IStrategyPromote() = default;

	/**
	 * @brief Повышает рабочего до мастера
	 * @param person_repo Репозиторий, содержащий работников
	 * @param worker_id Идентификатор рабочего, которого нужно повысить
	 * @param new_efficiency Новый мультипликативный вклад мастера
	 * @throws std::runtime_error Если переданный человек не является рабочим
	 */
	virtual void promote(PersonRepository *person_repo, int worker_id, double new_efficiency) const = 0;
};

#endif
