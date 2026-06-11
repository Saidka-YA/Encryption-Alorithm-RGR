/*
 * libs/lib_a51.cpp  —  A5/1
 */
#include "A51EncryptDecrypt.h"
#include "A51Cipher.h"
#include "A51utilities.h"
#include "plugin_api.h"
#include "validation.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace a51;

static void run_file(bool do_encrypt) {
    const string keyPath = "a51Key.bin";

    string inputPath  = val_input_existing_file(
        do_encrypt ? "Путь к входному файлу" : "Путь к зашифрованному файлу");
    string outputPath = val_input_writable_file("Путь для сохранения результата");

    uint8_t key[8];
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

    vector<uint8_t> data;
    if (!readFile(inputPath, data)) {
        cerr << "  [!] Ошибка: не удалось прочитать файл '" << inputPath << "'.\n";
        return;
    }

    vector<uint8_t> result;
    if (do_encrypt) {
        Padding(data);
        encryptA51(data, result, key);
    } else {
        decryptA51(data, result, key);
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

    const string keyPath = "a51Key.bin";
    uint8_t key[8];
    generateKey(key);
    saveKey(keyPath, key);

    vector<uint8_t> data(text.begin(), text.end());
    Padding(data);

    vector<uint8_t> ciphertext;
    encryptA51(data, ciphertext, key);
    cout << "\n  Зашифрованный текст (hex):\n  " << hexDisplay(ciphertext) << "\n";

    vector<uint8_t> plaintext;
    decryptA51(ciphertext, plaintext, key);
    deletePadding(plaintext);
    cout << "\n  Расшифрованный текст:\n  "
         << string(plaintext.begin(), plaintext.end()) << "\n";
}

extern "C" void plugin_run() {
    cout << "\n=== A5/1 ===\n";
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
