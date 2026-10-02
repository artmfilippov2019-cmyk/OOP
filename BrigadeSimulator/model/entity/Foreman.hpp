/**
 * @file Foreman.hpp
 * @brief Заголовочный файл, содержащий объявление класса Foreman
 */

#ifndef LAB3_FOREMAN_H
#define LAB3_FOREMAN_H

#include "Person.hpp"

/**
 * @class Foreman
 * @brief Класс, представляющий бригадира
 */
class Foreman : public Person {
public:
	/**
	 * @brief Конструктор бригадира
	 * @param id Уникальный идентификатор работника в системе
	 * @param age Возраст бригадира
	 * @param name Имя бригадира
	 */
	Foreman(int id, int age, const std::string& name);
};

#endif
