#ifndef RSAUTILITIES_H
#define RSAUTILITIES_H
#pragma once 
#include <gmp.h>
#include <vector>
#include <string>
#include <cstdint>
#include <random>
#include <sstream>
#include <fstream>
#include <iomanip>

namespace rsa {

void generatePrime(mpz_t prime, int bits);
void Padding(std::vector<uint8_t>& data);
void deletePadding(std::vector<uint8_t>& data);
bool saveKey(const std::string& path, const mpz_t n, const mpz_t e, const mpz_t d);
bool loadKey(const std::string& path, mpz_t n, mpz_t e, mpz_t d);
bool readFile(const std::string& path, std::vector<uint8_t>& data);
bool writeFile(const std::string& path, const std::vector<uint8_t>& data);
std::string hexDisplay(const std::vector<uint8_t>& data);



} // namespace rsa
#endif