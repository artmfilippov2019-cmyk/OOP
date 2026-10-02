#include "prog.hpp"
#include "dialog.hpp"
#include <iostream>

int main() {
    while (true) {
        try {
            auto word_opt = readline(std::cin);
            if (!word_opt.has_value()) break;
            const std::string& word = word_opt.value();
            std::string res1 = sep_by_syllables(word);
            const char *res2 = sep_by_syllables(word.c_str());

            std::cout << "Результат (std::string): " << res1 << std::endl;
            std::cout << "Результат (const char *): " << res2 << '\n' << std::endl;
            delete[] res2;
        }
        catch (const std::invalid_argument& ex) {
            std::cout << ex.what() << std::endl;
        }
        catch (const std::exception& ex) {
            std::cout << ex.what() << std::endl;
            return 1;
        }
    }

    std::cout << "Завершение работы" << std::endl;
    return 0;
}