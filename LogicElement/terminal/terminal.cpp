#include "terminal.hpp"

#include <stdexcept>

bool Terminal::is_valid_connections(size_t conn) const {
    if (type == INPUT) {
        return conn <= 1;
    }
    return conn <= 3;
}

bool Terminal::is_valid_signal(Signal sig) const {
    if (connections == 0 && type == INPUT) {
        return sig == X;
    }
    return true;
}

Terminal::Terminal(): type(INPUT), connections(0), signal(X) {}

Terminal::Terminal(Type t): type(t), connections(0), signal(X) {}

Terminal::Terminal(Type t, size_t conn, Signal sig): type(t), connections(conn), signal(sig) {
    if (!is_valid_connections(conn)) {
        throw std::invalid_argument("Ошибка: некорректное количество клемм");
    }
    if (!is_valid_signal(sig)) {
        throw std::invalid_argument("Ошибка: некорректный сигнал");
    }
}

Terminal::Type Terminal::getType() const {return type; }

size_t Terminal::getConnections() const {return connections; }

Terminal::Signal Terminal::getSignal() const {return signal; }

void Terminal::setType(Type t) {type = t; }

void Terminal::setConnections(size_t c) {
    if (!is_valid_connections(c)) {
        throw std::invalid_argument("Ошибка: некорректное количество соединений");
    }
    connections = c;
    if (type == INPUT && connections == 0) {
        signal = X;
    }
}

void Terminal::setSignal(Signal s) {
    if (!is_valid_signal(s)) {
        throw std::invalid_argument("Ошибка: некорректный сигнал");
    }
    signal = s;
}

Terminal& Terminal::operator++() {
    if (!is_valid_connections(connections + 1)) {
        throw std::invalid_argument("Ошибка: клемма уже имеет максимальное число соединений");
    }
    connections++;
    return *this;
}

Terminal& Terminal::operator--() {
    if (connections <= 0) {
        throw std::invalid_argument("Ошибка: клемма не может иметь отрицательное число соединений");
    }
    connections--;
    if (type == INPUT && connections == 0) {
        signal = X;
    }
    return *this;
}

void Terminal::operator >>(Terminal& input) {
    if (input.getType() != INPUT) {
        throw std::invalid_argument("Ошибка: второй операнд должен быть входной клеммой");
    }
    if (this->getType() != OUTPUT) {
        throw std::invalid_argument("Ошибка: первый операнд должен быть выходной клеммой");
    }
    if (input.getConnections() >= 1) {
        throw std::invalid_argument("Ошибка: входная клемма не может иметь больше 1 соединения");
    }
    if (this->getConnections() >= 3) {
        throw std::invalid_argument("Ошибка: выходная клемма не может иметь больше 3 соединений");
    }
    this->connections++;
    ++input;
    input.setSignal(this->getSignal());
}

std::ostream& operator <<(std::ostream& os, const Terminal& t) {
    os << "Тип: " << (t.getType() == Terminal::INPUT ? "INPUT" : "OUTPUT");
    os << ", Соединения: " << t.getConnections();
    os << ", Сигнал: ";
    switch(t.getSignal()) {
        case Terminal::ZERO: os << "ZERO(0)"; break;
        case Terminal::ONE: os << "ONE(1)"; break;
        case Terminal::X: os << "X(-1)"; break;
    }
    os << std::endl;
    return os;
}

std::istream& operator >>(std::istream& is, Terminal& t) {
    int type, connections, signal;
    is >> type >> connections >> signal;
    if (is.fail()) return is;

    if (type > 1 || signal > 2) {
        is.setstate(std::ios::failbit);
        return is;
    }

    auto new_type = static_cast<Terminal::Type>(type);
    auto new_signal = static_cast<Terminal::Signal>(signal);

    if (new_type == Terminal::INPUT && connections > 1) {
        is.setstate(std::ios::failbit);
        return is;
    }
    if (new_type == Terminal::OUTPUT && connections > 3) {
        is.setstate(std::ios::failbit);
        return is;
    }

    t.setType(new_type);
    t.setConnections(connections);
    t.setSignal(new_signal);

    return is;
}

void Terminal::disconnect(Terminal& output, Terminal& input) {
    if (input.getType() != INPUT) {
        throw std::invalid_argument("Ошибка: второй операнд должен быть входной клеммой");
    }
    if (output.getType() != OUTPUT) {
        throw std::invalid_argument("Ошибка: первый операнд должен быть выходной клеммой");
    }
    if (output.getConnections() <= 0 || input.getConnections() <= 0 || input.getSignal() != output.getSignal()) {
        throw std::runtime_error("Ошибка: клеммы не соединены");
    }
    --input;
    --output;
    if (input.getConnections() == 0) {
        input.setSignal(X);
    }
}