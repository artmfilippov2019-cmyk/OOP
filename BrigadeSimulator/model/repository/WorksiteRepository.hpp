/**
 * @file WorksiteRepository.hpp
 * @brief Заголовочный файл, содержащий объявление класса WorksiteRepository
 */

#ifndef LAB3_WORKSITEREPOSITORY_H
#define LAB3_WORKSITEREPOSITORY_H

#include <vector>
#include <memory>
#include "model/entity/Worksite.hpp"
#include "HashTable/HashTable.hpp"

/**
 * @class WorksiteRepository
 * @brief Репозиторий для хранения и управления рабочими участками
 */
class WorksiteRepository {
    /// Хранилище рабочих участков: id -> уникальный указатель на Worksite
    HashTable<int, std::unique_ptr<Worksite>> storage;

public:
    /**
     * @brief Добавляет рабочий участок в репозиторий
     * @param ws Указатель на объект рабочего участка
     * @throws std::invalid_argument Если передан nullptr
     * @throws std::runtime_error Если участок с таким id уже существует
     */
    void add_worksite(Worksite* ws);

    /**
     * @brief Удаляет рабочий участок из репозитория
     * @param id Идентификатор рабочего участка
     * @throws std::runtime_error Если участок с данным id не найден
     */
    void remove_worksite(int id);

    /**
     * @brief Возвращает рабочий участок по идентификатору
     * @param id Идентификатор рабочего участка
     * @return Указатель на объект Worksite
     * @throws std::runtime_error Если участок с данным id не найден
     */
    [[nodiscard]] Worksite* get_worksite(int id) const;

    /**
     * @brief Возвращает все рабочие участки
     * @return Вектор указателей на объекты Worksite
     */
    [[nodiscard]] std::vector<Worksite*> get_all() const;

    /**
     * @brief Проверяет наличие рабочего участка с заданным идентификатором
     * @param id Идентификатор рабочего участка
     * @return True, если участок существует, иначе false
     */
    [[nodiscard]] bool contains(int id) const;
};

#endif
