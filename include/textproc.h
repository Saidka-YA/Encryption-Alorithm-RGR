#ifndef TEXTPROC_H
#define TEXTPROC_H

#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

// Генерация алфавита
vector<char32_t> genalphabet();

// UTF-8 утилиты
vector<char32_t> to_codes(const string& str);
string to_text(const vector<char32_t>& codes);

#endif
