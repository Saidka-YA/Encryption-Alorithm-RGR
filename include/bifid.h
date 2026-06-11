#pragma once
#include <string>

// Перечисление для режимов работы алгоритма Бифида
enum class BifidMode {
    Encrypt = 1,
    Decrypt = 2
};

std::string encryptBifid(const std::string& plaintext);
std::string decryptBifid(const std::string& ciphertext);