#pragma once
#include <string>
#include <utility>

// Перевод байта в Hex-строку
std::string byteToHex(unsigned char byte);

// Перевод Hex-строки в байт
unsigned char hexToByte(const std::string& hex);

// Получение координат в сетке 4x4 для Hex-символа
std::pair<int, int> hexDigitToCoords(char hexDigit);

// Получение Hex-символа из координат сетки 4x4
char coordsToHexDigit(int row, int col);
