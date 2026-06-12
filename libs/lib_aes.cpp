/*
 * libs/lib_aes.cpp  —  AES-128 CFB
 */
#include "AES128.h"
#include "CFBmode.h"
#include "utilities.h"
#include "plugin_api.h"
#include "validation.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace aes;

static void run_file(bool do_encrypt) {
    const string keyPath = "aesKey.bin";

    string inputPath  = val_input_existing_file(
        do_encrypt ? "Путь к входному файлу" : "Путь к зашифрованному файлу");
    string outputPath = val_input_writable_file("Путь для сохранения результата");

    uint8_t key[16];
    uint8_t expandedKey[176];

    if (do_encrypt) {
        generateKey(key);
        if (!saveKey(keyPath, key)) {
            cerr << "  [!] Ошибка: не удалось сохранить ключ в " << keyPath << "\n";
            return;
        }
    } else {
        if (!loadKey(keyPath, key)) {
            cerr << "  [!] Ошибка: не удалось загрузить ключ из '" << keyPath << "'.\n"
                    "      Убедитесь, что файл существует и не повреждён.\n";
            return;
        }
    }
    expandRoundKey(key, expandedKey);

    vector<uint8_t> data;
    if (!readFile(inputPath, data)) {
        cerr << "  [!] Ошибка: не удалось прочитать файл '" << inputPath << "'.\n";
        return;
    }

    vector<uint8_t> result;
    if (do_encrypt) {
        Padding(data);
        encryptCFB(data, result, expandedKey);
    } else {
        decryptCFB(data, result, expandedKey);
        deletePadding(result);
    }

    if (!writeFile(outputPath, result)) {
        cerr << "  [!] Ошибка: не удалось записать файл '" << outputPath << "'.\n";
        return;
    }

    if (do_encrypt)
        cout << "  Готово. Ключ сохранён в " << keyPath << "\n";
    else
        cout << "  Файл расшифрован.\n";
}

static void run_text() {
    string text = val_input_nonempty("Введите текст для шифрования: ");

    const string keyPath = "aesKey.bin";
    uint8_t key[16];
    uint8_t expandedKey[176];
    generateKey(key);
    saveKey(keyPath, key);
    expandRoundKey(key, expandedKey);

    vector<uint8_t> data(text.begin(), text.end());
    Padding(data);

    vector<uint8_t> ciphertext;
    encryptCFB(data, ciphertext, expandedKey);
    cout << "\n  Зашифрованный текст (hex):\n  " << hexDisplay(ciphertext) << "\n";

    vector<uint8_t> plaintext;
    decryptCFB(ciphertext, plaintext, expandedKey);
    deletePadding(plaintext);
    cout << "\n  Расшифрованный текст:\n  "
         << string(plaintext.begin(), plaintext.end()) << "\n";
}

extern "C" void plugin_run() {
    cout << "\n=== AES-128 (CFB) ===\n";
    cout << "  1. Зашифровать файл\n";
    cout << "  2. Расшифровать файл\n";
    cout << "  3. Зашифровать текст (консоль)\n";
    cout << "  0. Назад\n";

    int choice = val_input_int("Выбор: ", 0, 3);
    switch (choice) {
        case 1: run_file(true);  break;
        case 2: run_file(false); break;
        case 3: run_text();      break;
        case 0: break;
    }
}
