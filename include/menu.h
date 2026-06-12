#pragma once
#include <string>
#include <vector>
#include <cstdint>

// ---- Общие утилиты ввода ----
std::string input_string(const std::string& prompt);
int         input_int(const std::string& prompt, int min_val, int max_val);
std::string input_path(const std::string& prompt);
bool        input_yes_no(const std::string& prompt, bool default_yes = true);

// ---- Точки входа для каждой группы шифров ----
void menu_aes();    // AES-128 CFB
void menu_a51();    // A5/1
void menu_rsa();    // RSA
void menu_nesterov(); // Хилл + Вернам
void menu_rozkov(); // Бифид + ADFGX
