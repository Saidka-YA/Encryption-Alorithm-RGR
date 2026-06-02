#include <iostream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <utility>
#include <clocale>
#include <fstream>

using namespace std;

#ifdef _WIN32
    #include <windows.h>
#endif

using namespace std;

// Символы координат для осей матрицы ADFGX
const char COORD_LABELS[] = {'A', 'D', 'F', 'G', 'X'};

// Конвертация сырого байта в Hex-строку (например, 255 -> "FF")
string byteToHex(unsigned char byte) {
    stringstream ss;
    ss << uppercase << hex << setfill('0') << setw(2) << static_cast<int>(byte);
    return ss.str();
}

// Конвертация Hex-пары обратно в байт (например, "FF" -> 255)
unsigned char hexToByte(const string& hex) {
    if (hex.length() != 2) {
        throw invalid_argument("hex-строка должна иметь длину 2");
    }
    
    auto hexVal = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        return -1;
    };
    
    int high = hexVal(hex[0]);
    int low = hexVal(hex[1]);
    
    if (high == -1 || low == -1) {
        throw invalid_argument(string("некорректная hex-пара: ") + hex);
    }
    
    return static_cast<unsigned char>((high << 4) | low);
}

int main() {
    
}
