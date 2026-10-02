/**
 * @file Person.hpp
 * @brief Заголовочный файл, содержащий объявление класса Person
 */

#ifndef LAB3_PERSON_H
#define LAB3_PERSON_H

#include <vector>
#include <string>
#include <stdexcept>
#include "IPerson.hpp"

/**
 * @class Person
 * @brief Базовый класс, представляющий работника
 */
class Person : public IPerson {
    /// Уникальный идентификатор работника
    int id;

    /// Возраст работника
    int age;

    /// Имя работника
    std::string name;

    /// Идентификаторы друзей работника
    std::vector<int> friends;

    /// Идентификаторы врагов работника
    std::vector<int> enemies;

public:
    /**
     * @brief Конструктор работника
     * @param id Уникальный идентификатор
     * @param age Возраст работника
     * @param name Имя работника
     */
    Person(int id, int age, const std::string& name);

    /**
     * @brief Возвращает идентификатор работника
     * @return Уникальный идентификатор
     */
    [[nodiscard]] int get_id() const override;

    /**
     * @brief Возвращает имя работника
     * @return Имя работника
     */
    [[nodiscard]] const std::string& get_name() const override;

    /**
     * @brief Возвращает возраст работника
     * @return Возраст работника
     */
    [[nodiscard]] int get_age() const override;

    /**
     * @brief Возвращает список друзей работника
     * @return Константная ссылка на вектор идентификаторов друзей
     */
    [[nodiscard]] const std::vector<int>& get_friends() const override;

    /**
     * @brief Возвращает список врагов работника
     * @return Константная ссылка на вектор идентификаторов врагов
     */
    [[nodiscard]] const std::vector<int>& get_enemies() const override;

    /**
     * @brief Добавляет другого работника в список друзей
     * @param person_id Идентификатор другого работника
     * @throws std::invalid_argument если person_id совпадает с id текущего работника
     */
    void add_friend(int person_id) override;

    /**
     * @brief Добавляет другого работника в список врагов
     * @param person_id Идентификатор другого работника
     * @throws std::invalid_argument если person_id совпадает с id текущего работника
     */
    void add_enemy(int person_id) override;

    /**
     * @brief Удаляет работника из списка друзей
     * @param person_id Идентификатор работника
     */
    void remove_friend(int person_id) override;

    /**
     * @brief Удаляет работника из списка врагов
     * @param person_id Идентификатор работника
     */
    void remove_enemy(int person_id) override;
};

#endif
