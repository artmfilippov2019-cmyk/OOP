#ifndef PROG_H
#define PROG_H

#include <string>

bool is_russian_char(const std::string& str, size_t pos);
const char *sep_by_syllables(const char *word);
bool is_vowel(const std::string& str, size_t pos);
std::string sep_by_syllables(const std::string& word);

#endif