#ifndef RSABLOCK_H
#define RSABLOCK_H
#pragma once
#include <gmp.h>
#include <vector>
#include <cstdint>

void modPow(mpz_t result, const mpz_t base, const mpz_t exp, const mpz_t mod);
void gcd(mpz_t result, const mpz_t a, const mpz_t b);
void modInverse(mpz_t result, const mpz_t a, const mpz_t m);
void generateKeys(mpz_t n, mpz_t e, mpz_t d);
void encryptBlock(const uint8_t* input, int blockSize, mpz_t c, 
                  const mpz_t n, const mpz_t e);
void decryptBlock(mpz_t message, const mpz_t c, const mpz_t n, const mpz_t d);
void encryptData(const std::vector<uint8_t>& plaintext, std::vector<uint8_t>& ciphertext, 
                 const mpz_t n, const mpz_t e);
void decryptData(const std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& plaintext, 
                 const mpz_t n, const mpz_t d);

#endif