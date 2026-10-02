#ifndef DIALOG_H
#define DIALOG_H

#include <iostream>
#include <vector>
#include "terminal/terminal.hpp"
#include "logic_element/logic_element.hpp"

void print_menu_terminal();
void dialog_input_terminal();
void dialog_input_element();
void dialog_test_terminal(int &option, std::vector<Terminal>& terminals);
void dialog_test_element(int &option, std::vector<LogicElement>& logic_elements);
void process_error(std::istream& in);
int read_int(std::istream& in);
std::string readline(std::istream& in);
Terminal::Type read_type(std::istream& in);
Terminal::Signal read_signal(std::istream& in);
int select_terminal(const std::vector<Terminal>& terminals, const std::string& message);
int select_logic_element(const std::vector<LogicElement>& elements, const std::string& message);

#endif