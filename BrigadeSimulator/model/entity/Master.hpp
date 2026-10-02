/**
 * @file Master.hpp
 * @brief Заголовочный файл, содержащий объявление класса Master
 */

#ifndef LAB3_MASTER_H
#define LAB3_MASTER_H

#include "IMaster.hpp"
#include "Person.hpp"

/**
 * @class Master
 * @brief Класс, представляющий мастера
 */
class Master : public Person, public IMaster {
	/// Мультипликативный вклад мастера (диапазон: (1..5])
	double efficiency;

public:
	/**
	 * @brief Конструктор мастера
	 * @param id Уникальный идентификатор мастера
	 * @param age Возраст мастера
	 * @param efficiency Мультипликативный вклад мастера (должна в диапазоне (1..5])
	 * @param name Имя мастера
	 * @throws std::invalid_argument Если эффективность выходит за допустимый диапазон
	 */
	Master(int id, int age, double efficiency, const std::string& name);

	/**
	 * @brief Устанавливает мультипликативный вклад мастера
	 * @param value Новое значение мультипликативного вклада
	 * @throws std::invalid_argument Если значение вне диапазона (1..5]
	 */
	void set_efficiency(double value) override;

	/**
	 * @brief Возвращает мультипликативный вклад мастера
	 * @return Мультипликативный вклад мастера
	 */
	[[nodiscard]] double get_efficiency() const override;
};

#endif
