/**
 * @file Worker.hpp
 * @brief Заголовочный файл, содержащий объявление класса Worker
 */

#ifndef LAB3_WORKER_H
#define LAB3_WORKER_H

#include "Person.hpp"
#include "IWorker.hpp"
#include <stdexcept>

/**
 * @class Worker
 * @brief Класс, представляющий рабочего
 */
class Worker : public Person, public IWorker {
	/// Аддитивный вклад рабочего (диапазон [1..10])
	int productivity_value;

public:
	/**
	 * @brief Конструктор рабочего
	 * @param id Уникальный идентификатор работника
	 * @param age Возраст работника
	 * @param productivity_value Начальное значение аддитивного вклада [1..10]
	 * @param name Имя работника
	 * @throws std::invalid_argument Если productivity_value не входит в диапазон [1..10]
	 */
	Worker(int id, int age, int productivity_value, const std::string& name);

	/**
	 * @brief Устанавливает новое значение аддитивного вклада
	 * @param value Новое значение аддитивного вклада
	 * @throws std::invalid_argument Если value не входит в диапазон [1..10]
	 */
	void set_productivity_value(int value) override;

	/**
	 * @brief Возвращает аддитивный вклад рабочего
	 * @return Значение производительности в диапазоне [1..10]
	 */
	[[nodiscard]] int get_productivity() const override;
};

#endif
