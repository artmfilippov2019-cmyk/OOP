/**
 * @file PersonRepository.hpp
 * @brief Заголовочный файл, содержащий объявление класса PersonRepository
 */

#ifndef LAB3_PERSONREPOSITORY_H
#define LAB3_PERSONREPOSITORY_H

#include <memory>
#include <vector>
#include "model/entity/Person.hpp"
#include "HashTable/HashTable.hpp"

/**
 * @class PersonRepository
 * @brief Репозиторий для хранения и управления объектами Person
 */
class PersonRepository {
    /// Хранилище работников: id -> уникальный указатель на Person
    HashTable<int, std::unique_ptr<IPerson>> storage;

public:
    /**
     * @brief Добавляет работника в репозиторий
     * @param person Указатель на объект работника
     * @throws std::invalid_argument Если передан nullptr
     * @throws std::runtime_error Если работник с таким id уже существует
     */
    void add_person(IPerson* person);

    /**
     * @brief Удаляет работника из репозитория
     * @param id Идентификатор работника
     * @throws std::runtime_error Если работник с данным id не найден
     */
    void remove_person(int id);

    /**
     * @brief Возвращает указатель на работника по идентификатору
     * @param id Идентификатор работника
     * @return Указатель на объект Person
     * @throws std::runtime_error Если работник с данным id не найден
     */
    [[nodiscard]] IPerson* get_person(int id) const;

    /**
     * @brief Возвращает всех работников в репозитории
     * @return Вектор указателей на объекты Person
     */
    [[nodiscard]] std::vector<IPerson*> get_all() const;

    /**
     * @brief Проверяет наличие работника с заданным идентификатором
     * @param id Идентификатор работника
     * @return True, если работник существует, иначе false
     */
    [[nodiscard]] bool contains(int id) const;

	[[nodiscard]] const HashTable<int, std::unique_ptr<IPerson>>& get_storage() const noexcept;
};

#endif
