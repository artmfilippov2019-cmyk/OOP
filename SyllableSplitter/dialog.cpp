#include "dialog.hpp"
#include "prog.hpp"
#include <iostream>
#include <optional>

std::optional<std::string> readline(std::istream& in) {
    std::cout << "Введите слово (или напишите 'exit' для завершения): ";

    std::string line;
    if (!std::getline(in, line)) {
        if (in.eof()) {
            throw std::runtime_error("EOF");
        }
        if (in.bad()) {
            throw std::runtime_error("Критическая ошибка потока");
        }
    }
    if (line == "exit") {
        return std::nullopt;
    }
    if (line.empty()) {
        throw std::invalid_argument("Ошибка: пустая строка");
    }

    return line;
}