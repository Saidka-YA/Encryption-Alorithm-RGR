#include "textproc.h"

// Генерация алфавита для шифра
vector<char32_t> genalphabet() {
    vector<char32_t> alphabet;

    // Управляющие символы \t, \n, \r
    for (char32_t c : {(char32_t)0x09, (char32_t)0x0A, (char32_t)0x0D})
        alphabet.push_back(c);

    // Вся ASCII
    for (char32_t c = 0x20; c <= 0x7E; ++c)
        alphabet.push_back(c);

    // тире, кавычки, многоточие, неразрывный дефиз и прочее
    for (char32_t c = 0x2000; c <= 0x206F; ++c)
        alphabet.push_back(c);
        
    // дополнительные символы «», неразрывный пробел и прочее
    for (char32_t c = 0xA0; c <= 0xFF; ++c)
        alphabet.push_back(c);

    // Вся кириллица
    for (char32_t c = 0x400; c <= 0x4FF; ++c)
        alphabet.push_back(c);

    return alphabet; // Итого: 3 + 95 + 256 = 354 символа
}

// UTF-8 строка в вектор кодпоинтов
vector<char32_t> to_codes(const string& str) {
    vector<char32_t> codes;
    for (size_t i = 0; i < str.length(); ) {
        unsigned char c = str[i];
        if (c <= 0x7F) {
            codes.push_back((char32_t)c);
            i += 1;
        } else if ((c & 0xE0) == 0xC0 && i + 1 < str.length()) {
            codes.push_back((char32_t)(((str[i] & 0x1F) << 6) | (str[i+1] & 0x3F)));
            i += 2;
        } else if ((c & 0xF0) == 0xE0 && i + 2 < str.length()) {
            codes.push_back((char32_t)(((str[i] & 0x0F) << 12) | ((str[i+1] & 0x3F) << 6) | (str[i+2] & 0x3F)));
            i += 3;
        } else if ((c & 0xF8) == 0xF0 && i + 3 < str.length()) {
            codes.push_back((char32_t)(((str[i] & 0x07) << 18) | ((str[i+1] & 0x3F) << 12) | ((str[i+2] & 0x3F) << 6) | (str[i+3] & 0x3F)));
            i += 4;
        } else {
            i += 1;
        }
    }
    return codes;
}

// Вектор кодпоинтов в строку
string to_text(const vector<char32_t>& codes) {
    string result;
    for (char32_t cp : codes) {
        if (cp < 0x80) {
            result += (char)cp;
        } else if (cp < 0x800) {
            result += (char)(0xC0 | (cp >> 6));
            result += (char)(0x80 | (cp & 0x3F));
        } else if (cp < 0x10000) {
            result += (char)(0xE0 | (cp >> 12));
            result += (char)(0x80 | ((cp >> 6) & 0x3F));
            result += (char)(0x80 | (cp & 0x3F));
        } else {
            result += (char)(0xF0 | (cp >> 18));
            result += (char)(0x80 | ((cp >> 12) & 0x3F));
            result += (char)(0x80 | ((cp >> 6) & 0x3F));
            result += (char)(0x80 | (cp & 0x3F));
        }
    }
    return result;
}
