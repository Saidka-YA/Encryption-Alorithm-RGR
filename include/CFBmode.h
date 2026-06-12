#ifndef CFBMODE_H
#define CFBMODE_H
#pragma once

#include <random>
#include <cstdint>
#include <cstring>

void encryptCFB(const std::vector<uint8_t>& plaintext, std::vector<uint8_t>& ciphertext, 
                uint8_t expandedKey[176]);
void decryptCFB(const std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& plaintext, 
                uint8_t expandedKey[176]);

#endif