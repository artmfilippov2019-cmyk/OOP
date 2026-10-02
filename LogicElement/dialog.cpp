#include "dialog.hpp"
#include <limits>
#include <vector>

void print_menu_terminal() {
    std::cout << "\n=== ТЕСТИРОВАНИЕ КЛАССА TERMINAL ===\n";
    std::cout << "1. Установить тип клеммы\n";
    std::cout << "2. Установить количество соединений\n";
    std::cout << "3. Установить сигнал\n";
    std::cout << "4. Оператор ++\n";
    std::cout << "5. Оператор --\n";
    std::cout << "6. Соединить клеммы\n";
    std::cout << "7. Разъединить клеммы\n";
    std::cout << "8. Ввод клеммы из потока\n";
    std::cout << "9. Вывод всех клемм в поток\n";
    std::cout << "0. Выход\n";
    std::cout << "Выберите действие: ";
}

void print_menu_element() {
    std::cout << "\n=== ТЕСТИРОВАНИЕ КЛАССА LOGIC ELEMENT ===\n";
    std::cout << "1. Вывести все логические элементы\n";
    std::cout << "2. Изменить клемму в логическом элементе\n";
    std::cout << "3. Добавить входную клемму\n";
    std::cout << "4. Добавить выходную клемму\n";
    std::cout << "5. Соединить два логических элемента\n";
    std::cout << "6. Ввести логический элемент из потока\n";
    std::cout << "7. Графическое представление элемента\n";
    std::cout << "0. Выход\n";
    std::cout << "Выберите действие: ";
}

void dialog_input_terminal() {
    std::cout << "=== ВВОД ДАННЫХ КЛЕММЫ ===\n";
    std::cout << "Формат: тип соединения сигнал\n";
    std::cout << "Тип: 0=INPUT, 1=OUTPUT\n";
    std::cout << "Соединения: для INPUT(0-1), для OUTPUT(0-3)\n";
    std::cout << "Сигнал: 0=ZERO, 1=ONE, -1=X\n";
    std::cout << "Введите данные: ";
}

void dialog_input_element() {
    std::cout << "=== ВВОД ДАННЫХ ЛОГИЧЕСКОГО ЭЛЕМЕНТА ===\n";
    std::cout << "Формат: количество_входов количество_выходов\n";
    std::cout << "Затем для каждой клеммы в формате: тип соединения сигнал\n";
    std::cout << "Тип: 0=INPUT, 1=OUTPUT\n";
    std::cout << "Соединения: для INPUT(0-1), для OUTPUT(0-3)\n";
    std::cout << "Сигнал: 0=ZERO, 1=ONE, -1=X\n";
    std::cout << "Введите данные: ";
}

void process_error(std::istream& in) {
    if (in.bad()) {
        throw std::runtime_error("Критическая ошибка потока");
    }
    if (in.eof()) {
        throw std::runtime_error("EOF");
    }
    if (in.fail()) {
        std::cout << "Ошибка: введено не целое число. Введите целое число: ";
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int read_int(std::istream& in) {
    int res;
    in >> res;
    while(!in.good()) {
        process_error(in);
        in >> res;
    }
    return res;
}

Terminal::Type read_type(std::istream& in) {
    int res = read_int(in);
    while(res != Terminal::INPUT && res != Terminal::OUTPUT) {
        std::cout << "Ошибка: введен недопустимый тип клеммы. Введите 0 или 1: ";
        process_error(in);
        in >> res;
    }
    return static_cast<Terminal::Type>(res);
}

Terminal::Signal read_signal(std::istream& in) {
    int res = read_int(in);
    while(res != Terminal::ZERO && res != Terminal::ONE && res != Terminal::X) {
        std::cout << "Ошибка: введен недопустимый тип сигнала. Введите от -1 до 1: ";
        process_error(in);
        in >> res;
    }
    return static_cast<Terminal::Signal>(res);
}

int select_terminal(const std::vector<Terminal>& terminals, const std::string& message) {
    std::cout << "Доступные клеммы:\n";
    for (size_t i = 0; i < terminals.size(); i++) {
        std::cout << i << ": " << terminals[i];
    }

    std::cout << message << "(0-" << terminals.size() - 1 << "): ";
    int ind = read_int(std::cin);
    while (ind < 0 || ind >= static_cast<int>(terminals.size())) {
        std::cout << "Ошибка: неверный индекс. Введите от 0 до " << terminals.size() - 1 << ": ";
        ind = read_int(std::cin);
    }
    return ind;
}

void dialog_test_terminal(int &option, std::vector<Terminal>& terminals) {
    print_menu_terminal();
    option = read_int(std::cin);

    switch (option) {
        case 0: {
            std::cout << "Завершение работы\n";
            break;
        }
        case 1: {
            int ind = select_terminal(terminals, "Выберите номер клеммы ");
            std::cout << "Введите тип клеммы (0=INPUT , 1=OUTPUT): ";
            Terminal::Type type = read_type(std::cin);
            terminals[ind].setType(type);
            std::cout << terminals[ind];
            break;
        }
        case 2: {
            int ind = select_terminal(terminals, "Выберите номер клеммы ");
            std::cout << "Введите количество соединений (для INPUT(0-1), для OUTPUT(0-3)): ";
            int conn = read_int(std::cin);
            terminals[ind].setConnections(conn);
            std::cout << terminals[ind];
            break;
        }
        case 3: {
            int ind = select_terminal(terminals, "Выберите номер клеммы ");
            std::cout << "Введите тип сигнала (0=ZERO, 1=ONE, -1=X): ";
            Terminal::Signal signal = read_signal(std::cin);
            terminals[ind].setSignal(signal);
            std::cout << terminals[ind];
            break;
        }
        case 4: {
            int ind = select_terminal(terminals, "Выберите номер клеммы ");
            ++terminals[ind];
            std::cout << terminals[ind];
            break;
        }
        case 5: {
            int ind = select_terminal(terminals, "Выберите номер клеммы ");
            --terminals[ind];
            std::cout << terminals[ind];
            break;
        }
        case 6: {
            int ind1 = select_terminal(terminals, "Выберите номер выходной клеммы для соединения ");
            int ind2 = select_terminal(terminals, "Выберите номер входной клеммы для соединения ");
            terminals[ind1] >> terminals[ind2];
            std::cout << terminals[ind1];
            std::cout << terminals[ind2];
            break;
        }
        case 7: {
            int ind1 = select_terminal(terminals, "Выберите номер выходной клеммы для разъединения ");
            int ind2 = select_terminal(terminals, "Выберите номер входной клеммы для разъединения ");
            Terminal::disconnect(terminals[ind1], terminals[ind2]);
            std::cout << terminals[ind1];
            std::cout << terminals[ind2];
            break;
        }
        case 8: {
            int ind = select_terminal(terminals, "Выберите клемму для ввода из потока");
            dialog_input_terminal();
            std::cin >> terminals[ind];
            if (std::cin.fail()) {
                std::cout << "Ошибка: некорректные данные\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            std::cout << "Клемма " << ind << " обновлена из потока\n";
            std::cout << terminals[ind];
            break;
        }
        case 9: {
            for (const auto & terminal : terminals) {
                std::cout << terminal;
            }
            break;
        }
        default: {
            std::cout << "Неверный выбор\n";
            break;
        }
    }
}

void dialog_test_element(int &option, std::vector<LogicElement>& logic_elements) {
    print_menu_element();
    option = read_int(std::cin);

    switch (option) {
        case 0: {
            std::cout << "Завершение работы\n";
            break;
        }
        case 1: {
            for (size_t i = 0; i < logic_elements.size(); ++i) {
                std::cout << "=== Логический элемент #" << i + 1 << " ===\n";
                std::cout << logic_elements[i];
            }
            break;
        }
        case 2: {
            int ind_e = select_logic_element(logic_elements, "Выберите номер логического элемента: ");
            LogicElement& elem = logic_elements[ind_e];

            std::cout << "Выберите тип клеммы для изменения (0 - входная, 1 - выходная): ";
            int type_choice = read_int(std::cin);

            if (type_choice == 0 && elem.get_count_inputs() > 0) {
                std::cout << "Введите индекс входной клеммы (0-" << elem.get_count_inputs() - 1 << "): ";
                int ind_t = read_int(std::cin);
                if (ind_t < 0 || static_cast<size_t>(ind_t) >= elem.get_count_inputs()) {
                    std::cout << "Некорректный индекс клеммы!\n";
                    break;
                }

                Terminal& t = elem[ind_t];
                std::cout << "Введите количество соединений: ";
                size_t conn = read_int(std::cin);
                std::cout << "Введите сигнал (0=ZERO, 1=ONE, -1=X): ";
                Terminal::Signal sig_val = read_signal(std::cin);

                try {
                    t.setConnections(conn);
                    t.setSignal(sig_val);
                    std::cout << "Входная клемма изменена успешно!\n";
                } catch (const std::exception& ex) {
                    std::cout << ex.what() << "\n";
                }

            } else if (type_choice == 1 && elem.get_count_outputs() > 0) {
                std::cout << "Введите индекс выходной клеммы (0-" << elem.get_count_outputs() - 1 << "): ";
                int ind_t = read_int(std::cin);
                if (ind_t < 0 || static_cast<size_t>(ind_t) >= elem.get_count_outputs()) {
                    std::cout << "Некорректный индекс клеммы!\n";
                    break;
                }

                Terminal& t = elem[elem.get_count_inputs() + ind_t];
                std::cout << "Введите количество соединений: ";
                size_t conn = read_int(std::cin);
                std::cout << "Введите сигнал (0=ZERO, 1=ONE, -1=X): ";
                int sig_val = read_int(std::cin);

                try {
                    t.setConnections(conn);
                    t.setSignal(static_cast<Terminal::Signal>(sig_val));
                    std::cout << "Выходная клемма изменена успешно!\n";
                } catch (const std::exception& ex) {
                    std::cout << ex.what() << "\n";
                }
            } else {
                std::cout << "Некорректный выбор типа клеммы или клеммы отсутствуют!\n";
            }

            break;
        }
        case 3: {
            int ind = select_logic_element(logic_elements, "Выберите номер логического элемента: ");
            Terminal input_terminal(Terminal::INPUT);
            logic_elements[ind].add_input_terminal(input_terminal);
            std::cout << "Добавлена входная клемма\n";
            break;
        }
        case 4: {
            int ind = select_logic_element(logic_elements, "Выберите номер логического элемента: ");
            Terminal output_terminal(Terminal::OUTPUT);
            logic_elements[ind].add_output_terminal(output_terminal);
            std::cout << "Добавлена выходная клемма\n";
            break;
        }
        case 5: {
            int ind1 = select_logic_element(logic_elements, "Выберите первый элемент (источник): ");
            int ind2 = select_logic_element(logic_elements, "Выберите второй элемент (приемник): ");

            logic_elements[ind1] >> logic_elements[ind2];
            std::cout << "Элементы соединены\n";
            break;
        }
        case 6: {
            int ind = select_logic_element(logic_elements, "Выберите номер логического элемента: ");
            dialog_input_element();
            std::cin >> logic_elements[ind];
            if (std::cin.fail()) {
                std::cout << "Ошибка: некорректные данные\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            std::cout << "Элемент обновлен из потока\n";
            break;
        }
        case 7: {
            if (logic_elements.empty()) {
                std::cout << "Список логических элементов пуст\n";
                break;
            }
            int ind = select_logic_element(logic_elements, "Выберите номер логического элемента: ");
            std::cout << logic_elements[ind].to_string() << std::endl;
            break;
        }
        default: {
            std::cout << "Неверный выбор\n";
            break;
        }
    }
}

int select_logic_element(const std::vector<LogicElement>& elements, const std::string& message) {
    if (elements.empty()) {
        throw std::runtime_error("Ошибка: список логических элементов пуст");
    }

    std::cout << "Доступные логические элементы:\n";
    for (size_t i = 0; i < elements.size(); ++i) {
        std::cout << "Элемент " << i + 1 << " (входов: " << elements[i].get_count_inputs() << ", выходов: " << elements[i].get_count_outputs() << ")\n";
    }

    while (true) {
        std::cout << message;
        int index = read_int(std::cin);
        if (index - 1 >= 0 && index - 1 < static_cast<int>(elements.size())) {
            return index - 1;
        }
        std::cout << "Ошибка: неверный индекс элемента\n";
    }
}