#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <vector>
#include <string>
#include <cstdint>

class FileProcessor {
public:
    static std::vector<uint8_t> read_file(const std::string& path);
    static void write_file(const std::string& path, const std::vector<uint8_t>& data);
};

#endif