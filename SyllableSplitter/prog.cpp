#include "prog.hpp"
#include <ranges>
#include <algorithm>
#include <vector>
#include <cstring>

bool is_russian_char(const std::string& str, size_t pos) {
    unsigned char b1 = str[pos];
    unsigned char b2 = str[pos + 1];

    if (b1 == 0xD0 && b2 >= 0x90 && b2 <= 0xBF) return true;
    if (b1 == 0xD1 && b2 >= 0x80 && b2 <= 0x8F) return true;
    return false;
}

bool is_vowel(const std::string& str, size_t pos) {
    const std::vector<std::string> vowels = {
        "А", "Е", "Ё", "И", "О", "У", "Ы", "Э", "Ю", "Я",
        "а", "е", "ё", "и", "о", "у", "ы", "э", "ю", "я"
    };

    std::string sym = str.substr(pos, 2);
    return std::ranges::find(vowels, sym) != std::end(vowels);;
}

const char *sep_by_syllables(const char *word) {
    size_t len = strlen(word);

    auto even_indexes = std::views::iota(0ul, len) | std::views::filter([](size_t i) { return i % 2 == 0; });
    if (std::ranges::any_of(even_indexes, [&word](size_t i) { return !is_russian_char(word, i); })) {
        throw std::invalid_argument("Ошибка: обнаружен нерусский символ");
    }
    size_t count_vowels = std::ranges::count_if(even_indexes, [&word](size_t i) { return is_vowel(word, i); });

    auto result = new char[len * 2];
    size_t count_cur_vowels = 0, index_res = 0;

    for (size_t i: even_indexes) {
        result[index_res++] = word[i];
        result[index_res++] = word[i + 1];
        if (is_vowel(word, i) && i < len - 1 && count_cur_vowels != count_vowels - 1) {
            count_cur_vowels++;
            result[index_res++] = '-';
        }
    }

    result[index_res] = '\0';
    return result;
}

std::string sep_by_syllables(const std::string& word) {
    const char *temp = sep_by_syllables(word.c_str());
    std::string result = temp;
    delete[] temp;
    return result;
}