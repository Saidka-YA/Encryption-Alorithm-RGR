#include "crypto_interface.h"
#include <cstring>

extern "C" {

const AlgorithmInfo* get_algorithm_info() {
    static const AlgorithmInfo info = { "playfair", 16 };
    return &info;
}

size_t get_output_size(size_t input_size, int operation_type) {
    return input_size;
}

int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size < 16) return 1;
    
    for (size_t i = 0; i < input.size; i += 2) {
        output->data[i] = input.data[i] ^ key.data[i % key.size];
        if (i + 1 < input.size)
            output->data[i + 1] = input.data[i + 1] ^ key.data[(i + 1) % key.size];
    }
    return 0;
}

int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    return encrypt(key, input, output);
}

}