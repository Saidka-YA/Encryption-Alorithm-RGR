#ifndef UTILITIES_H
#define UTILITIES_H
#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <random>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>

namespace aes {

void Padding(std::vector<uint8_t>& data);
void deletePadding(std::vector<uint8_t>& data);
void generateKey(uint8_t key[16]);
void generateIV(uint8_t iv[16]);
bool saveKey(const std::string& path, uint8_t key[16]);
bool loadKey(const std::string& path, uint8_t key[16]);
bool readFile(const std::string& path, std::vector<uint8_t>& data);
bool writeFile(const std::string& path, const std::vector<uint8_t>& data);
std::string hexDisplay(const std::vector<uint8_t>& data);

} // namespace aes
#endif