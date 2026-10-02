/**
 * @file BrigadeService.hpp
 * @brief Заголовочный файл, содержащий объявление класса BrigadeService
 */

#ifndef LAB3_BRIGADESERVICE_H
#define LAB3_BRIGADESERVICE_H

#include "IStrategyPromote.hpp"
#include "mapper/SuperPersonMapper.hpp"
#include "model/repository/PersonRepository.hpp"
#include "model/repository/WorksiteRepository.hpp"

/**
 * @class BrigadeService
 * @brief Сервис бизнес-логики бригады
 */
class BrigadeService {
    /// Репозиторий работников
    PersonRepository* person_repo;

    /// Репозиторий рабочих участков
    WorksiteRepository* worksite_repo;

	/// Стратегия повышения рабочего до мастера
	IStrategyPromote* strategy_promote;

	/// Полиморфный маппер
	SuperPersonMapper* super_person_mapper;

    /// Идентификатор бригадира
    int foreman_id;

    /// Идентификатор следующего создаваемого рабочего участка
    int next_worksite_id;
public:
    /**
     * @brief Конструктор сервиса
     * @param p_repo Указатель на репозиторий работников
     * @param w_repo Указатель на репозиторий рабочих участков
     * @param strategy_promote Указатель на стратегию повышения рабочего до мастера
     * @param super_person_mapper Указатель на полиморфный маппер
     */
    BrigadeService(PersonRepository* p_repo, WorksiteRepository* w_repo, IStrategyPromote* strategy_promote,
    	SuperPersonMapper* super_person_mapper);

    /**
     * @brief Создаёт новый рабочий участок
     * @param volume Общий объём работ
     * @return Идентификатор созданного рабочего участка
     */
    int create_worksite(double volume);

    /**
     * @brief Изменяет объём работ на рабочем участке
     * @param site_id Идентификатор участка
     * @param new_volume Новый объём работ
     */
    void change_worksite_volume(int site_id, double new_volume) const;

    /**
     * @brief Назначает работника на рабочий участок
     * @param person_id Идентификатор работника
     * @param site_id Идентификатор рабочего участка
     */
    void assign_person_to_site(int person_id, int site_id) const;

    /**
     * @brief Удаляет работника с рабочего участка
     * @param person_id Идентификатор работника
     * @param site_id Идентификатор рабочего участка
     */
    void remove_person_from_site(int person_id, int site_id) const;

    /**
     * @brief Вычисляет дневную производительность рабочего участка
     * @param site_id Идентификатор рабочего участка
     * @return Объём выполненной работы за день
     */
    [[nodiscard]] double calculate_site_output(int site_id) const;

    /**
     * @brief Проверяет, завершены ли все работы на всех участках
     * @return True, если все работы выполнены, иначе false
     */
    [[nodiscard]] bool all_tasks_completed() const;

    /**
     * @brief Возвращает идентификатор самого проблемного рабочего участка
     * @return Идентификатор рабочего участка
     */
    [[nodiscard]] int get_most_problematic_site_id() const;

    /**
     * @brief Симулирует один рабочий день
     */
    void simulate_work_day(size_t w_count) const;

    /**
     * @brief Изменяет аддитивный вклад рабочего
     * @param worker_id Идентификатор рабочего
     * @param new_value Новое значение аддитивного вклада
     */
    void change_worker_productivity(int worker_id, int new_value) const;

    /**
     * @brief Изменяет  мультипликативный вклад мастера
     * @param master_id Идентификатор мастера
     * @param new_value Новое значение  мультипликативного вклада
     */
    void change_master_efficiency(int master_id, double new_value) const;

    /**
     * @brief Повышает рабочего до мастера
     * @param worker_id Идентификатор рабочего
     * @param new_efficiency Мультипликативный вклад нового мастера
     */
    void promote_worker_to_master(int worker_id, double new_efficiency) const;

    /**
     * @brief Устанавливает идентификатор бригадира
     * @param id Идентификатор бригадира
     */
    void set_foreman_id(int id);

    /**
     * @brief Возвращает идентификатор бригадира
     * @return Идентификатор бригадира
     */
    [[nodiscard]] int get_foreman_id() const;

	void simulate_work_day_single_thread(size_t w_count) const;
};

#endif
