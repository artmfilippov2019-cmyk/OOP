/**
 * @file terminal.hpp
 * @brief Заголовочный файл, содержащий определение класса Terminal
 */

#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>

/**
 * @brief Класс, представляющий клемму
 */
class Terminal {
public:
    /// @brief Тип клеммы: входная или выходная
    enum Type {INPUT=0, OUTPUT=1}; ///< Входная или выходная

    /// @brief Состояние сигнала на клемме
    enum Signal {ZERO=0, ONE=1, X=-1}; ///< 0, 1 или неопределенное состояние

private:
    Type type; ///< Тип клеммы
    size_t connections; ///< Количество соединений клеммы
    Signal signal; ///< Сигнал на клемме

    /**
     * @brief Проверяет допустимость количества соединений для данного типа клеммы
     * @param conn Проверяемое количество соединений
     * @return True если количество соединений допустимо, false в противном случае
     */
    [[nodiscard]] bool is_valid_connections(size_t conn) const;

    /**
     * @brief Проверяет корректность сигнала для данной клеммы
     * @param sig Проверяемый сигнал
     * @return True если сигнал допустим, false в противном случае
     */
    [[nodiscard]] bool is_valid_signal(Signal sig) const;

public:
    /**
     * @brief Конструктор по умолчанию
     */
    Terminal();

    /**
     * @brief Конструктор с указанием типа клеммы
     * @param t Тип создаваемой клеммы (INPUT или OUTPUT)
     */
    explicit Terminal(Type t);

    /**
     * Полный конструктор клеммы
     * @param t Тип клеммы
     * @param conn Количество соединений
     * @param sig Сигнал на клемме
     * @throw std::invalid_argument Если параметры некорректны
     */
    Terminal(Type t, size_t conn, Signal sig);

    /**
     * @brief Возвращает тип клеммы
     * @return Тип клеммы
     */
    [[nodiscard]] Type getType() const;

    /**
     * @brief Возвращает количество соединений клеммы
     * @return Количество соединений клеммы
     */
    [[nodiscard]] size_t getConnections() const;

    /**
     * @brief Возвращает сигнал на клемме
     * @return Сигнал на клемме
     */
    [[nodiscard]] Signal getSignal() const;

    /**
     * Устанавливает тип клеммы
     * @param t Новый тип клеммы
     */
    void setType(Type t);

    /**
     * @brief Устанавливает количество соединений
     * @param c Новое количество соединений
     * @throw std::invalid_argument Если количество соединений некорректно
     */
    void setConnections(size_t c);

    /**
     * @brief Устанавливает сигнал на клемме
     * @param s Новый сигнал
     * @throw std::invalid_argument Если сигнал некорректен для данной клеммы
     */
    void setSignal(Signal s);

    /**
     * @brief Префиксный инкремент - увеличивает количество соединений клеммы на 1
     * @return Ссылка на текущий объект
     * @throw std::invalid_argument Если достигнут максимум соединений
     */
    Terminal& operator ++();

    /**
     * @brief Префиксный декремент - уменьшает количество соединений клеммы на 1
     * @return Ссылка на текущий объект
     * @throw std::invalid_argument Если соединений нет
     */
    Terminal& operator --();

    /**
     * @brief Оператор соединения выходной клеммы с входной
     * @param input Входная клемма
     * @throw std::invalid_argument Если типы клемм некорректны или превышены лимиты соединений
     */
    void operator >>(Terminal& input);

    /**
     * @brief Разъединяет выходную и входную клеммы
     * @param output Выходная клемма
     * @param input Входная клемма
     * @throw std::invalid_argument Если типы клемм некорректны
     * @throw std::runtime_error Если клеммы не были соединены
     */
    static void disconnect(Terminal& output, Terminal& input);

    /**
     * @brief Оператор вывода состояния клеммы в поток
     * @param os Выходной поток
     * @param t Выводимая клемма
     * @return Ссылка на выходной поток
     *
     * Формат вывода: "Тип: [TYPE], Соединения: [CONNECTIONS], Сигнал: [SIGNAL]"
     */
    friend std::ostream& operator <<(std::ostream& os, const Terminal& t);

    /**
     * @brief Оператор ввода состояния клеммы из потока
     * @param is Входной поток
     * @param t Изменяемая клемма
     * @return Ссылка на входной поток
     *
     * Формат ввода: три целых числа (тип, соединения, сигнал)
     */
    friend std::istream& operator >>(std::istream& is, Terminal& t);
};

#endif