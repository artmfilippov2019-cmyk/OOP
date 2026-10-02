/**
 * @file Worksite.hpp
 * @brief Заголовочный файл, содержащий объявление класса Worksite
 */

#ifndef LAB3_WORKSITE_H
#define LAB3_WORKSITE_H

#include <memory>
#include <vector>

#include "HashTable/HashTable.hpp"
#include "model/entity/Person.hpp"

/**
 * @class Worksite
 * @brief Класс, представляющий рабочий участок
 */
class Worksite {
    /// Уникальный идентификатор рабочего участка
    int id;

    /// Общий объём работ на участке
    double work_volume;

    /// Оставшийся объём работ
    double remaining_work;

    /// Идентификаторы работников, назначенных на участок
    std::vector<int> assigned_ids;

public:
    /**
     * @brief Конструктор рабочего участка
     * @param id Идентификатор участка
     * @param work_volume Общий объём работ
     * @param remaining_work Оставшийся объём работ
     * @param assigned_ids Список идентификаторов назначенных работников
     * @throws std::invalid_argument Если объёмы работ отрицательны
     */
    Worksite(
        int id,
        double work_volume,
        double remaining_work,
        const std::vector<int>& assigned_ids
    );

    /**
     * @brief Конструктор рабочего участка
     * @param id Идентификатор участка
     * @param work_volume Общий объём работ
     * @throws std::invalid_argument Если объём работ отрицателен
     */
    Worksite(int id, double work_volume);

    /**
     * @brief Возвращает идентификатор рабочего участка
     * @return Идентификатор участка
     */
    [[nodiscard]] int get_id() const;

    /**
     * @brief Возвращает оставшийся объём работ
     * @return Оставшийся объём работ
     */
    [[nodiscard]] double get_remaining_work() const;

    /**
     * @brief Возвращает общий объём работ
     * @return Общий объём работ
     */
    [[nodiscard]] double get_work_volume() const;

    /**
     * @brief Возвращает список назначенных работников
     * @return Константная ссылка на вектор идентификаторов работников
     */
    [[nodiscard]] const std::vector<int>& get_assigned_ids() const;

    /**
     * @brief Устанавливает новый общий объём работ
     * @param volume Новый объём работ
     * @throws std::invalid_argument если volume отрицателен
     */
    void set_work_volume(double volume);

    /**
     * @brief Устанавливает оставшийся объём работ
     * @param volume Новый оставшийся объём работ
     * @throws std::invalid_argument если volume отрицателен
     */
    void set_remaining_work(double volume);

    /**
     * @brief Назначает работника на участок
     * @param person_id Идентификатор работника
     */
    void add_person(int person_id);

    /**
     * @brief Удаляет работника с участка
     * @param person_id Идентификатор работника
     */
    void remove_person(int person_id);

    /**
     * @brief Проверяет, назначен ли работник на участок
     * @param person_id Идентификатор работника
     * @return True, если работник назначен, иначе false
     */
    [[nodiscard]] bool has_person(int person_id) const;

    /**
     * @brief Вычисляет дневную производительность участка
     * @param persons Хеш-таблица работников (id → Person*)
     * @return Объём работы, выполненной за день
     */
    [[nodiscard]] double calculate_daily_output(const HashTable<int, std::unique_ptr<IPerson>>& persons) const;

    /**
     * @brief Применяет выполненную работу к участку
     * @param amount Объём выполненной работы
     * @throws std::invalid_argument Если amount отрицателен
     */
    void apply_work(double amount);
};

#endif
