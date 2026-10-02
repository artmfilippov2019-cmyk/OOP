#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "terminal.hpp"
#include "logic_element.hpp"

TEST_CASE("Terminal class testing", "[terminal]") {
    SECTION("Terminal constructors") {
        SECTION("Default constructor") {
            Terminal t;
            REQUIRE(t.getType() == Terminal::INPUT);
            REQUIRE(t.getConnections() == 0);
            REQUIRE(t.getSignal() == Terminal::X);
        }
        SECTION("Constructor with type") {
            Terminal input(Terminal::INPUT);
            REQUIRE(input.getType() == Terminal::INPUT);
            REQUIRE(input.getConnections() == 0);
            REQUIRE(input.getSignal() == Terminal::X);

            Terminal output(Terminal::OUTPUT);
            REQUIRE(output.getType() == Terminal::OUTPUT);
            REQUIRE(output.getConnections() == 0);
            REQUIRE(output.getSignal() == Terminal::X);
        }
        SECTION("Valid constructor with type, connections, signal") {
            Terminal t(Terminal::OUTPUT, 2, Terminal::ONE);

            REQUIRE(t.getType() == Terminal::OUTPUT);
            REQUIRE(t.getConnections() == 2);
            REQUIRE(t.getSignal() == Terminal::ONE);
        }
        SECTION("Invalid constructor with type, connections. signal") {
            REQUIRE_THROWS_WITH(Terminal(Terminal::INPUT, 3, Terminal::ONE), "Ошибка: некорректное количество клемм");
            REQUIRE_THROWS_WITH(Terminal(Terminal::INPUT, 0, Terminal::ZERO), "Ошибка: некорректный сигнал");
        }
    }

    SECTION("Terminal setters") {
        SECTION("Type setter") {
            Terminal t;
            t.setType(Terminal::OUTPUT);
            REQUIRE(t.getType() == Terminal::OUTPUT);
        }
        SECTION("Correct Connections setter") {
            Terminal t;
            t.setConnections(1);
            REQUIRE(t.getConnections() == 1);
        }
        SECTION("incorrect connection setter") {
            Terminal t;
            REQUIRE_THROWS_WITH(t.setConnections(5), "Ошибка: некорректное количество соединений");
        }
        SECTION("Correct signal setter") {
            Terminal t;
            t.setConnections(1);
            t.setSignal(Terminal::ONE);
            REQUIRE(t.getSignal() == Terminal::ONE);
        }
        SECTION("Incorrect signal setter") {
            Terminal t;
            REQUIRE_THROWS_WITH(t.setSignal(Terminal::ONE), "Ошибка: некорректный сигнал");
        }
    }

    SECTION("Terminal operations") {
        SECTION("Increment operator ++") {
            Terminal output(Terminal::OUTPUT);

            ++output;
            REQUIRE(output.getConnections() == 1);
            ++output;
            REQUIRE(output.getConnections() == 2);
            ++output;
            REQUIRE(output.getConnections() == 3);

            REQUIRE_THROWS_WITH(++output, "Ошибка: клемма уже имеет максимальное число соединений");
        }
        SECTION("Decrement operator --") {
            Terminal input(Terminal::INPUT);
            input.setConnections(1);
            input.setSignal(Terminal::ONE);

            --input;
            REQUIRE(input.getConnections() == 0);
            REQUIRE(input.getSignal() == Terminal::X);

            REQUIRE_THROWS_WITH(--input, "Ошибка: клемма не может иметь отрицательное число соединений");
        }
    }

    SECTION("Terminal connection operations") {
        SECTION("Operator >> valid connection") {
            Terminal output(Terminal::OUTPUT);
            Terminal input(Terminal::INPUT);

            output.setSignal(Terminal::ONE);

            output >> input;
            REQUIRE(output.getConnections() == 1);
            REQUIRE(input.getConnections() == 1);
            REQUIRE(input.getSignal() == Terminal::ONE);
        }
        SECTION("Operator >> invalid output connections") {
            Terminal output(Terminal::OUTPUT, 3, Terminal::ONE);
            Terminal input(Terminal::INPUT);

            REQUIRE_THROWS_WITH(output >> input, "Ошибка: выходная клемма не может иметь больше 3 соединений");
        }
        SECTION("Operator >> invalid input connections") {
            Terminal output(Terminal::OUTPUT);
            Terminal input(Terminal::INPUT, 1, Terminal::ONE);

            REQUIRE_THROWS_WITH(output >> input, "Ошибка: входная клемма не может иметь больше 1 соединения");
        }
        SECTION("Operator >> invalid terminal types") {
            Terminal input1(Terminal::INPUT);
            Terminal input2(Terminal::INPUT);
            Terminal output1(Terminal::OUTPUT);
            Terminal output2(Terminal::OUTPUT);

            REQUIRE_THROWS_WITH(input1 >> input2, "Ошибка: первый операнд должен быть выходной клеммой");
            REQUIRE_THROWS_WITH(output1 >> output2, "Ошибка: второй операнд должен быть входной клеммой");
        }
    }

    SECTION("Disconnect operation") {
        SECTION("Valid disconnect") {
            Terminal output(Terminal::OUTPUT);
            Terminal input(Terminal::INPUT);

            output >> input;
            REQUIRE(output.getConnections() == 1);
            REQUIRE(input.getConnections() == 1);

            Terminal::disconnect(output, input);
            REQUIRE(output.getConnections() == 0);
            REQUIRE(input.getConnections() == 0);
            REQUIRE(input.getSignal() == Terminal::X);
        }
        SECTION("Disconnect not connected terminals") {
            Terminal output(Terminal::OUTPUT);
            Terminal input(Terminal::INPUT);

            REQUIRE_THROWS_WITH(Terminal::disconnect(output, input), "Ошибка: клеммы не соединены");
        }
        SECTION("Disconnect invalid terminal types") {
            Terminal input1(Terminal::INPUT);
            Terminal input2(Terminal::INPUT);
            Terminal output1(Terminal::OUTPUT);
            Terminal output2(Terminal::OUTPUT);

            REQUIRE_THROWS_WITH(Terminal::disconnect(input1, input2), "Ошибка: первый операнд должен быть выходной клеммой");
            REQUIRE_THROWS_WITH(Terminal::disconnect(output1, output2), "Ошибка: второй операнд должен быть входной клеммой");
        }
    }

    SECTION("Stream operations") {
        SECTION("Operator >> input") {
            Terminal t;
            std::istringstream iss("1 3 0");
            iss >> t;

            REQUIRE(t.getType() == Terminal::OUTPUT);
            REQUIRE(t.getConnections() == 3);
            REQUIRE(t.getSignal() == Terminal::ZERO);
        }
        SECTION("Operator >> input invalid type") {
            Terminal t;
            std::istringstream iss("5 0 0");
            iss >> t;

            REQUIRE(iss.fail());
            REQUIRE(t.getType() == Terminal::INPUT);
            REQUIRE(t.getConnections() == 0);
            REQUIRE(t.getSignal() == Terminal::X);
        }
        SECTION("Operator >> input invalid input connections") {
            Terminal t;
            std::istringstream iss("0 3 0");
            iss >> t;

            REQUIRE(iss.fail());
            REQUIRE(t.getType() == Terminal::INPUT);
            REQUIRE(t.getConnections() == 0);
            REQUIRE(t.getSignal() == Terminal::X);
        }
        SECTION("Operator >> input invalid output connections") {
            Terminal t;
            std::istringstream iss("1 4 0");
            iss >> t;

            REQUIRE(iss.fail());
            REQUIRE(t.getType() == Terminal::INPUT);
            REQUIRE(t.getConnections() == 0);
            REQUIRE(t.getSignal() == Terminal::X);
        }
        SECTION("Operator << output") {
            Terminal t(Terminal::OUTPUT);
            t.setConnections(2);
            t.setSignal(Terminal::ONE);
            std::ostringstream oss;
            oss << t;
            std::string result = oss.str();
            REQUIRE(result.find("OUTPUT") != std::string::npos);
            REQUIRE(result.find("Соединения: 2") != std::string::npos);
            REQUIRE(result.find("ONE(1)") != std::string::npos);

            t.setSignal(Terminal::X);
            oss << t;
            result = oss.str();
            REQUIRE(result.find("X(-1)") != std::string::npos);
        }
    }
}
TEST_CASE("LogicElement class testing", "[logic_element]") {
    SECTION("LogicElement constructors") {
        SECTION("Default constructor") {
            LogicElement e;
            REQUIRE(e.get_count_inputs() == 0);
            REQUIRE(e.get_count_outputs() == 0);
            REQUIRE(e.get_count_inputs() == 0);
            REQUIRE(e.get_count_outputs() == 0);
        }
        SECTION("Constructor with input/output counts") {
            LogicElement e(3, 2);
            REQUIRE(e.get_count_inputs() == 3);
            REQUIRE(e.get_count_outputs() == 2);
            REQUIRE(e.get_capacity_inputs() == 3);
            REQUIRE(e.get_capacity_outputs() == 2);

            for (size_t i = 0; i < 3; i++) {
                REQUIRE(e[i].getType() == Terminal::INPUT);
            }
            for (size_t i = 3; i < 5; i++) {
                REQUIRE(e[i].getType() == Terminal::OUTPUT);
            }
        }
        SECTION("Constructor with input/output counts with bad alloc") {
            REQUIRE_THROWS_AS(LogicElement(1, 100000000000), std::bad_alloc);
        }
        SECTION("Constructor with negative counts should throw") {
            REQUIRE_THROWS_AS(LogicElement(-1, 2), std::invalid_argument);
            REQUIRE_THROWS_AS(LogicElement(2, -1), std::invalid_argument);
        }
        SECTION("Constructor from vector of terminals") {
            Terminal terminals[4] = {
                Terminal(Terminal::INPUT),
                Terminal(Terminal::OUTPUT),
                Terminal(Terminal::INPUT),
                Terminal(Terminal::OUTPUT)
            };

            LogicElement e(terminals, 4);
            REQUIRE(e.get_count_inputs() == 2);
            REQUIRE(e.get_count_outputs() == 2);
        }
        SECTION("Copy constructor") {
            LogicElement original(2, 2);
            original[3].setSignal(Terminal::ONE);

            LogicElement copy(original);
            REQUIRE(copy.get_count_inputs() == 2);
            REQUIRE(copy.get_count_outputs() == 2);
            REQUIRE(copy[3].getSignal() == Terminal::ONE);
        }
        SECTION("Move constructor") {
            LogicElement original(2, 2);
            original[2].setSignal(Terminal::ONE);

            LogicElement moved(std::move(original));
            REQUIRE(moved.get_count_inputs() == 2);
            REQUIRE(moved.get_count_outputs() == 2);
            REQUIRE(moved[2].getSignal() == Terminal::ONE);

            REQUIRE(original.get_count_inputs() == 0);
            REQUIRE(original.get_count_outputs() == 0);
        }
    }

    SECTION("LogicElement assignment operators") {
        SECTION("Copy assignment operator") {
            LogicElement original(2, 1);
            original[2].setSignal(Terminal::ONE);

            LogicElement copy;
            copy = original;

            REQUIRE(copy.get_count_inputs() == 2);
            REQUIRE(copy.get_count_outputs() == 1);
            REQUIRE(copy[2].getSignal() == Terminal::ONE);

            copy[2].setSignal(Terminal::ZERO);
            REQUIRE(original[2].getSignal() == Terminal::ONE);
        }
        SECTION("Move assignment operator") {
            LogicElement original(1, 2);
            original[1].setSignal(Terminal::ONE);

            LogicElement moved;
            moved = std::move(original);

            REQUIRE(moved.get_count_inputs() == 1);
            REQUIRE(moved.get_count_outputs() == 2);
            REQUIRE(moved[1].getSignal() == Terminal::ONE);

            REQUIRE(original.get_count_inputs() == 0);
            REQUIRE(original.get_count_outputs() == 0);
        }
    }

    SECTION("LogicElement terminal operations") {
        SECTION("Add input terminal") {
            LogicElement e(1, 1);
            Terminal input(Terminal::INPUT);

            e.add_input_terminal(input);
            REQUIRE(e.get_count_inputs() == 2);
            REQUIRE(e[0].getType() == Terminal::INPUT);
            REQUIRE(e.get_capacity_inputs() == 2);
        }
        SECTION("Add output terminal") {
            LogicElement e(1, 1);
            Terminal output(Terminal::OUTPUT);

            e.add_output_terminal(output);
            REQUIRE(e.get_count_outputs() == 2);
            REQUIRE(e[1].getType() == Terminal::OUTPUT);
            REQUIRE(e.get_capacity_outputs() == 2);
        }
        SECTION("Add terminal with wrong type should throw") {
            LogicElement e;
            Terminal output(Terminal::OUTPUT);
            Terminal input(Terminal::INPUT);

            REQUIRE_THROWS_WITH(e.add_input_terminal(output), "Ошибка: клемма должна быть входной");
            REQUIRE_THROWS_WITH(e.add_output_terminal(input), "Ошибка: клемма должна быть выходной");
        }
    }

    SECTION("Terminal access operations") {
        SECTION("Operator[] with valid indices") {
            LogicElement e(3, 2);

            REQUIRE(e[0].getType() == Terminal::INPUT);
            REQUIRE(e[2].getType() == Terminal::INPUT);
            REQUIRE(e[3].getType() == Terminal::OUTPUT);
            REQUIRE(e[4].getType() == Terminal::OUTPUT);
        }
        SECTION("Operator[] with invalid indices should throw") {
            LogicElement e(2, 2);

            REQUIRE_THROWS_WITH(e[5], "Ошибка: индекс клеммы находится вне диапазона");
        }
        SECTION("Const operator[] access") {
            const LogicElement e(2, 2);

            REQUIRE(e[0].getType() == Terminal::INPUT);
            REQUIRE(e[3].getType() == Terminal::OUTPUT);
            REQUIRE_THROWS_AS(e[4], std::out_of_range);
        }
    }

    SECTION("LogicElement connection operations") {
        SECTION("Connect two LogicElements") {
            LogicElement e1(1, 1);
            LogicElement e2(1, 1);
            e1 >> e2;

            REQUIRE(e1[1].getConnections() == 1);
            REQUIRE(e2[0].getConnections() == 1);
        }

        SECTION("Connect LogicElements with no free terminals should throw") {
            LogicElement e1(0, 1);
            LogicElement e2(0, 1);

            REQUIRE_THROWS_WITH(e1 >> e2, "Ошибка: нет свободных клемм для соединения");
        }
    }

    SECTION("LogicElement stream operations") {
        SECTION("Output stream operator") {
            LogicElement e(1, 1);
            e[0].setConnections(1);
            e[0].setSignal(Terminal::ONE);
            e[1].setSignal(Terminal::ZERO);

            std::ostringstream oss;
            oss << e;

            std::string result = oss.str();
            REQUIRE(result.find("Входные клеммы (1)") != std::string::npos);
            REQUIRE(result.find("Выходные клеммы (1)") != std::string::npos);
        }

        SECTION("Input stream operator") {
            LogicElement e(1, 1);
            std::istringstream iss("1 1\n0 0 -1\n1 1 0\n");

            iss >> e;

            REQUIRE(e[0].getType() == Terminal::INPUT);
            REQUIRE(e[0].getConnections() == 0);
            REQUIRE(e[0].getSignal() == Terminal::X);

            REQUIRE(e[1].getType() == Terminal::OUTPUT);
            REQUIRE(e[1].getConnections() == 1);
            REQUIRE(e[1].getSignal() == Terminal::ZERO);
        }
        SECTION("Input stream operator with fail") {
            LogicElement e(1, 1);
            std::istringstream iss("1 1\n5 0 -1\n1 1 0\n");

            iss >> e;
            REQUIRE(iss.fail());
        }
        SECTION("Input stream operator with bad alloc") {
            LogicElement e(1, 1);
            std::istringstream iss("1 10000000000\n0 0 -1\n1 1 0\n");

            REQUIRE_THROWS_AS(iss >> e, std::bad_alloc);
        }
    }

    SECTION("LogicElement print operations") {
        SECTION("Print empty element") {
            LogicElement e;
            std::string result = e.to_string();

            REQUIRE(result == "┌────────────┐\n│            │\n└────────────┘\n");
        }
        SECTION("Print element with only input terminals") {
            LogicElement e(3, 0);
            e[0].setConnections(1);
            e[0].setSignal(Terminal::ZERO);
            e[1].setConnections(0);
            e[2].setConnections(1);
            e[2].setSignal(Terminal::X);

            std::string result = e.to_string();

            REQUIRE(result.find("┌--------------------┐") != std::string::npos);
            REQUIRE(result.find("└--------------------┘") != std::string::npos);
            REQUIRE(result.find("IN 0(1)") != std::string::npos);
            REQUIRE(result.find("IN X(0)") != std::string::npos);
            REQUIRE(result.find("IN X(1)") != std::string::npos);
        }
        SECTION("Print element with only output terminals") {
            LogicElement e(0, 2);
            e[0].setSignal(Terminal::ONE);
            e[0].setConnections(2);
            e[1].setSignal(Terminal::ZERO);
            e[1].setConnections(1);

            std::string result = e.to_string();

            REQUIRE(result.find("┌--------------------┐") != std::string::npos);
            REQUIRE(result.find("└--------------------┘") != std::string::npos);
            REQUIRE(result.find("1(2) OUT") != std::string::npos);
            REQUIRE(result.find("0(1) OUT") != std::string::npos);
        }
    }
}