#include "logic_element.hpp"
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <optional>

void LogicElement::resize_terminals(Terminal*& terminals, size_t& capacity, size_t& count, size_t new_capacity) {
    if (new_capacity == capacity) return;

    Terminal* new_terminals = new_capacity > 0 ? new Terminal[new_capacity] : nullptr;
    size_t elements_to_copy = std::min(count, new_capacity);

    if (terminals && new_terminals) {
        std::copy_n(terminals, elements_to_copy, new_terminals);
    }

    delete[] terminals;
    terminals = new_terminals;
    capacity = new_capacity;
    count = elements_to_copy;
}

void LogicElement::resize_input(size_t new_capacity) {
    resize_terminals(input_terminals, capacity_inputs, count_inputs, new_capacity);
}

void LogicElement::resize_output(size_t new_capacity) {
    resize_terminals(output_terminals, capacity_outputs, count_outputs, new_capacity);
}

LogicElement::LogicElement(int num_inputs, int num_outputs) {
    if (num_inputs < 0 || num_outputs < 0) {
        throw std::invalid_argument("Ошибка: число клемм не может быть отрицательным");
    }

    try {
        capacity_inputs = num_inputs;
        input_terminals = new Terminal[capacity_inputs];
        capacity_outputs = num_outputs;
        output_terminals = new Terminal[capacity_outputs];
    }
    catch (...) {
        delete[] input_terminals;
        delete[] output_terminals;
        throw;
    }

    std::fill_n(input_terminals, num_inputs, Terminal(Terminal::INPUT));
    count_inputs = num_inputs;
    std::fill_n(output_terminals, num_outputs, Terminal(Terminal::OUTPUT));
    count_outputs = num_outputs;
}

LogicElement::LogicElement(const Terminal *terminals, size_t count) {
    size_t num_inputs = 0, num_outputs = 0;
    for (size_t i = 0; i < count; i++) {
        if (terminals[i].getType() == Terminal::INPUT) {
            num_inputs++;
        } else {
            num_outputs++;
        }
    }

    try {
        if (num_inputs > 0) {
            capacity_inputs = num_inputs;
            input_terminals = new Terminal[capacity_inputs];
        }
        if (num_outputs > 0) {
            capacity_outputs = num_outputs;
            output_terminals = new Terminal[capacity_outputs];
        }
    }
    catch (const std::bad_alloc&) {
        delete[] input_terminals;
        delete[] output_terminals;
        throw;
    }

    size_t input_ind = 0;
    size_t output_ind = 0;
    for (size_t i = 0; i < count; i++) {
        if (terminals[i].getType() == Terminal::INPUT) {
            if (input_terminals != nullptr && input_ind < capacity_inputs) {
                input_terminals[input_ind++] = terminals[i];
                count_inputs++;
            }
        }
        else {
            if (output_terminals != nullptr && output_ind < capacity_outputs) {
                output_terminals[output_ind++] = terminals[i];
                count_outputs++;
            }
        }
    }
}

LogicElement::LogicElement(LogicElement &&other) noexcept : input_terminals(other.input_terminals),
output_terminals(other.output_terminals), count_inputs(other.count_inputs), count_outputs(other.count_outputs),
capacity_inputs(other.capacity_inputs), capacity_outputs(other.capacity_outputs) {
    other.input_terminals = nullptr;
    other.output_terminals = nullptr;
    other.count_inputs = 0;
    other.count_outputs = 0;
    other.capacity_inputs = 0;
    other.capacity_outputs = 0;
}

LogicElement::LogicElement(const LogicElement& other) : count_inputs(other.count_inputs),
count_outputs(other.count_outputs), capacity_inputs(other.capacity_inputs), capacity_outputs(other.capacity_outputs) {
    if (capacity_inputs > 0) {
        input_terminals = new Terminal[capacity_inputs];
    }
    if (capacity_outputs > 0) {
        output_terminals = new Terminal[capacity_outputs];
    }

    if (input_terminals) {
        std::copy_n(other.input_terminals, count_inputs, input_terminals);
    }
    if (output_terminals) {
        std::copy_n(other.output_terminals, count_outputs, output_terminals);
    }
}

LogicElement::~LogicElement() {
    delete[] input_terminals;
    delete[] output_terminals;
}

Terminal& LogicElement::operator [](size_t index) {
    if (index < count_inputs) {
        return input_terminals[index];
    }
    if (index - count_inputs < count_outputs) {
        return output_terminals[index - count_inputs];
    }
    throw std::out_of_range("Ошибка: индекс клеммы находится вне диапазона");
}

const Terminal &LogicElement::operator[](size_t index) const {
    if (index < count_inputs) {
        return input_terminals[index];
    }
    if (index - count_inputs < count_outputs) {
        return output_terminals[index - count_inputs];
    }
    throw std::out_of_range("Ошибка: индекс клеммы находится вне диапазона");
}

void LogicElement::operator >>(const LogicElement &other) const {
    std::optional<size_t> ind_terminal_out;
    std::optional<size_t> ind_terminal_input;

    for (size_t i = 0; i < this->count_outputs; ++i) {
        if (other.output_terminals[i].getConnections() < 3) {
            ind_terminal_out = i;
            break;
        }
    }
    for (size_t i = 0; i < other.count_inputs; ++i) {
        if (other.input_terminals[i].getConnections() == 0) {
            ind_terminal_input = i;
            break;
        }
    }

    if (ind_terminal_out && ind_terminal_input) {
        this->output_terminals[*ind_terminal_out] >> other.input_terminals[*ind_terminal_input];
    }
    else {
        throw std::runtime_error("Ошибка: нет свободных клемм для соединения");
    }
}

LogicElement& LogicElement::operator =(const LogicElement& other) {
    return *this = LogicElement{other};
}

LogicElement& LogicElement::operator=(LogicElement&& other) noexcept {
    if (this == &other) return *this;
    delete[] input_terminals;
    delete[] output_terminals;

    input_terminals = other.input_terminals;
    output_terminals = other.output_terminals;
    count_inputs = other.count_inputs;
    count_outputs = other.count_outputs;
    capacity_inputs = other.capacity_inputs;
    capacity_outputs = other.capacity_outputs;

    other.input_terminals = nullptr;
    other.output_terminals = nullptr;
    other.count_inputs = 0;
    other.count_outputs = 0;
    other.capacity_inputs = 0;
    other.capacity_outputs = 0;
    return *this;
}

std::istream& operator>>(std::istream& is, LogicElement& e) {
    size_t size_inputs, size_outputs;
    is >> size_inputs >> size_outputs;
    if (!is) return is;

    Terminal* new_inputs = nullptr;
    Terminal* new_outputs = nullptr;

    try {
        if (size_inputs > 0) {
            new_inputs = new Terminal[size_inputs];
            for (size_t i = 0; i < size_inputs && is; ++i) {
                is >> new_inputs[i];
                if (!is) {
                    delete[] new_inputs;
                    return is;
                }
            }
        }
        if (size_outputs > 0) {
            new_outputs = new Terminal[size_outputs];
            for (size_t i = 0; i < size_outputs && is; ++i) {
                is >> new_outputs[i];
                if (!is) {
                    delete[] new_inputs;
                    delete[] new_outputs;
                    return is;
                }
            }
        }

        delete[] e.input_terminals;
        delete[] e.output_terminals;
        e.input_terminals = new_inputs;
        e.output_terminals = new_outputs;
        e.count_inputs = size_inputs;
        e.count_outputs = size_outputs;
    }
    catch (const std::bad_alloc&) {
        delete[] new_inputs;
        delete[] new_outputs;
        throw;
    }

    return is;
}

std::ostream& operator <<(std::ostream& os, const LogicElement& e) {
    os << "Входные клеммы (" << e.count_inputs << "):\n";
    for (size_t i = 0; i < e.count_inputs; i++) {
        os << i + 1 << ": " << e.input_terminals[i];
    }
    os << "Выходные клеммы (" << e.count_outputs << "):\n";
    for (size_t i = 0; i < e.count_outputs; i++) {
        os << i + e.count_inputs + 1 << ": " << e.output_terminals[i];
    }
    return os;
}

void LogicElement::add_input_terminal(const Terminal& terminal) {
    if (terminal.getType() != Terminal::INPUT) {
        throw std::invalid_argument("Ошибка: клемма должна быть входной");
    }

    if (count_inputs >= capacity_inputs) {
        resize_input(capacity_inputs == 0 ? 1 : capacity_inputs * 2);
    }
    input_terminals[count_inputs++] = terminal;
}

void LogicElement::add_output_terminal(const Terminal& terminal) {
    if (terminal.getType() != Terminal::OUTPUT) {
        throw std::invalid_argument("Ошибка: клемма должна быть выходной");
    }

    if (count_outputs >= capacity_outputs) {
        resize_output(capacity_outputs == 0 ? 1 : capacity_outputs * 2);
    }
    output_terminals[count_outputs++] = terminal;
}

std::string LogicElement::to_string() const {
    std::ostringstream oss;

    size_t max_terminals = std::max(count_inputs, count_outputs);
    if (max_terminals == 0) {
        return "┌────────────┐\n│            │\n└────────────┘\n";
    }

    constexpr int inner_width = 20;
    const std::string top_bottom(inner_width, '-');

    oss << "┌" << top_bottom << "┐\n";

    for (size_t i = 0; i < max_terminals; ++i) {
        char in_sig  = ' ';
        size_t in_conn = 0;
        if (i < count_inputs) {
            in_sig = (input_terminals[i].getSignal() == Terminal::ZERO ? '0' :
                      input_terminals[i].getSignal() == Terminal::ONE  ? '1' : 'X');
            in_conn = input_terminals[i].getConnections();
        }

        char out_sig = ' ';
        size_t out_conn = 0;
        if (i < count_outputs) {
            out_sig = (output_terminals[i].getSignal() == Terminal::ZERO ? '0' :
                       output_terminals[i].getSignal() == Terminal::ONE  ? '1' : 'X');
            out_conn = output_terminals[i].getConnections();
        }

        if (in_sig != ' ') oss << "│ IN " << in_sig << "(" << in_conn << ") │ ";
        else oss << "│         │ ";
        if (out_sig != ' ') oss << out_sig << "(" << out_conn << ") OUT │\n";
        else oss << "         │\n";
    }

    oss << "└" << top_bottom << "┘";

    return oss.str();
}

size_t LogicElement::get_count_inputs() const {return count_inputs; }

size_t LogicElement::get_count_outputs() const {return count_outputs; }

size_t LogicElement::get_capacity_inputs() const {return capacity_inputs; }

size_t LogicElement::get_capacity_outputs() const {return capacity_outputs; }


