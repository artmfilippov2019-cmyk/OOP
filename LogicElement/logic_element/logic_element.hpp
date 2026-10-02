/**
 * @file logic_element.hpp
 * @brief Заголовочный файл, содержащий определение класса LogicElement
 */
#ifndef LOGIC_ELEMENT_H
#define LOGIC_ELEMENT_H

#include "terminal.hpp"

/**
 * @brief Класс, представляющий логический элемент
 */
class LogicElement {
    Terminal *input_terminals{}; ///< Массив входных клемм
    Terminal *output_terminals{}; ///< Массив выходных клемм

    size_t count_inputs{}; ///< Количество входных клемм
    size_t count_outputs{}; ///< Количество выходных клемм
    size_t capacity_inputs{}; ///< Вместимость массива входных клемм
    size_t capacity_outputs{}; ///< Вместимость массива выходных клемм

    static void resize_terminals(Terminal*& terminals, size_t& capacity, size_t& count, size_t new_capacity);
    void resize_input(size_t new_capacity);
    void resize_output(size_t new_capacity);

public:
    /**
     * @brief Конструктор по умолчанию
     */
    LogicElement() = default;

    /**
     * @brief Конструктор с заданным количеством входных и выходных клемм
     * @param num_inputs Количество входных клемм
     * @param num_outputs Количество выходных клемм
     * @throw std::invalid_argument Если количество клемм отрицательное
     */
    LogicElement(int num_inputs, int num_outputs);

    /**
     * @brief Конструктор из массива клемм
     * @param terminals Массив клемм
     * @param count Количество клемм в массиве
     */
    explicit LogicElement(const Terminal* terminals, size_t count);

    /**
     * @brief Перемещающий конструктор
     * @param other Перемещаемый логический элемент
     */
    LogicElement(LogicElement &&other) noexcept;

    /**
     * @brief Копирующий конструктор
     * @param other Копируемый логический элемент
     */
    LogicElement(const LogicElement& other);

    /**
     * @brief Деструктор
     */
    ~LogicElement();

    /**
     * @brief Оператор доступа к клемме по индексу (не константный)
     * @param index Индекс клеммы
     * @return Ссылка на клемму
     * @throw std::out_of_range Если индекс вне допустимого диапазона
     */
    Terminal& operator [](size_t index);

    /**
     * @brief Оператор доступа к клемме по индексу (константный)
     * @param index Индекс клеммы
     * @return Копия клеммы
     * @throw std::out_of_range Если индекс вне допустимого диапазона
     */
    const Terminal& operator [](size_t index) const;

    /**
     * @brief Оператор присваивания перемещением
     * @param other Временный логический элемент
     * @return Ссылка на текущий объект
     */
    LogicElement& operator =(LogicElement&& other) noexcept;

    /**
     *@brief Соединение между собой выходной и входной клемм двух логических элементов (если имеется несколько клемм, то соединяются первые незанятые клеммы)
     * @param other Второй логический элемент
     * @throw std::runtime_error Если нет свободных клемм для соединения
     */
    void operator >>(const LogicElement& other) const;

    /**
     * @brief Оператор присваивания копированием
     * @param other Присваиваемый логический элемент
     * @return Ссылка на текущий объект
     */
    LogicElement& operator =(const LogicElement& other);

    /**
     * @brief Оператор ввода состояния логического элемента из потока
     * @param is Входной поток
     * @param e Изменяемый логический элемент
     * @return Ссылка на входной поток
     */
    friend std::istream& operator >>(std::istream& is, LogicElement& e);

    /**
     * @brief Оператор вывода состояния логического элемента в поток
     * @param os Выходной поток
     * @param e Выводимый логический элемент
     * @return Ссылка на выходной поток
     */
    friend std::ostream& operator <<(std::ostream& os, const LogicElement& e);

    /**
     * @brief Добавляет входную клемму к элементу
     * @param terminal Добавляемая входная клемма
     * @throw std::invalid_argument Если тип клеммы не INPUT
     */
    void add_input_terminal(const Terminal& terminal = Terminal(Terminal::INPUT));

    /**
     * @brief Добавляет выходную клемму к элементу
     * @param terminal Добавляемая выходная клемма
     * @throw std::invalid_argument Если тип клеммы не OUTPUT
     */
    void add_output_terminal(const Terminal& terminal = Terminal(Terminal::OUTPUT));

    /**
     * @brief Форматирование логического элемента в виде псевдографической строки с символами ASCII
     * @return Псевдографическая строка с символами ASCII
     */
    [[nodiscard]] std::string to_string() const;

    /**
     * @brief Возвращает количество входных клемм логического элемента
     * @return Количество входных клемм логического элемента
     */
    [[nodiscard]] size_t get_count_inputs() const;

    /**
     * @brief Возвращает количество выходных клемм логического элемента
     * @return Количество выходных клемм логического элемента
     */
    [[nodiscard]] size_t get_count_outputs() const;

    /**
     * @brief Возвращает вместимость массива входных клемм логического элемента
     * @return Вместимость массива входных клемм логического элемента
     */
    [[nodiscard]] size_t get_capacity_inputs() const;

    /**
     * Возвращает вместимость массива выходных клемм логического элемента
     * @return Вместимость массива выходных клемм логического элемента
     */
    [[nodiscard]] size_t get_capacity_outputs() const;
};

#endif