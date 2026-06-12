#pragma once
#include <string>

// Перечисление для режимов работы алгоритма ADFGX
enum class AdfgxMode {
    Encrypt = 1,
    Decrypt = 2
};

std::string encryptADFGX(const std::string& plaintext, const std::string& keyword);
std::string decryptADFGX(const std::string& ciphertext, const std::string& keyword);