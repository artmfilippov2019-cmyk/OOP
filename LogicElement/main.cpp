#include "dialog.hpp"
#include <sstream>
#include "logic_element.hpp"

int main() {
    int option = 0, choice_class = 0;
    std::vector terminals {
        Terminal(),
        Terminal(Terminal::OUTPUT),
        Terminal(Terminal::INPUT, 1, Terminal::ZERO)
    };
    std::vector logic_elements {
        LogicElement(1, 1),
        LogicElement(2, 2)
    };

    std::cout << "Выберите номер класса для тестирования (Terminal - 1, LogicElement - 2): ";
    choice_class = read_int(std::cin);

    do {
        try {
            if (choice_class == 1) {
                dialog_test_terminal(option, terminals);
            }
            else if (choice_class == 2) {
                dialog_test_element(option, logic_elements);
            }
        }
        catch (const std::exception& ex) {
            std::cout << ex.what() << std::endl;
        }
    }
    while (option != 0);

    return 0;
}
