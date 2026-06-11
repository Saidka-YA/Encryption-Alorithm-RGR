/*
 * libs/lib_rsa.cpp  —  RSA
 */
#include "RSAblock.h"
#include "RSAutilities.h"
#include "plugin_api.h"
#include "validation.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace rsa;

static void clear_keys(mpz_t n, mpz_t e, mpz_t d) {
    mpz_clear(n); mpz_clear(e); mpz_clear(d);
}

static void run_file(bool do_encrypt) {
    const string keyPath = "rsaKey.bin";

    string inputPath  = val_input_existing_file(
        do_encrypt ? "Путь к входному файлу" : "Путь к зашифрованному файлу");
    string outputPath = val_input_writable_file("Путь для сохранения результата");

    mpz_t n, e, d;
    mpz_init(n); mpz_init(e); mpz_init(d);

    if (do_encrypt) {
        cout << "  Генерация ключей (может занять несколько секунд)...\n";
        generateKeys(n, e, d);
        if (!saveKey(keyPath, n, e, d)) {
            cerr << "  [!] Ошибка: не удалось сохранить ключи в " << keyPath << "\n";
            clear_keys(n, e, d); return;
        }
    } else {
        if (!loadKey(keyPath, n, e, d)) {
            cerr << "  [!] Ошибка: не удалось загрузить ключи из '" << keyPath << "'.\n"
                    "      Убедитесь, что файл существует и не повреждён.\n";
            clear_keys(n, e, d); return;
        }
    }

    vector<uint8_t> data;
    if (!readFile(inputPath, data)) {
        cerr << "  [!] Ошибка: не удалось прочитать файл '" << inputPath << "'.\n";
        clear_keys(n, e, d); return;
    }

    vector<uint8_t> result;
    if (do_encrypt) {
        Padding(data);
        cout << "  Шифрование...\n";
        encryptData(data, result, n, e);
    } else {
        cout << "  Расшифрование...\n";
        decryptData(data, result, n, d);
        deletePadding(result);
    }

    if (!writeFile(outputPath, result)) {
        cerr << "  [!] Ошибка: не удалось записать файл '" << outputPath << "'.\n";
        clear_keys(n, e, d); return;
    }

    if (do_encrypt)
        cout << "  Готово. Ключ сохранён в " << keyPath << "\n";
    else
        cout << "  Файл расшифрован.\n";

    clear_keys(n, e, d);
}

static void run_text() {
    string text = val_input_nonempty("Введите текст для шифрования: ");

    const string keyPath = "rsaKey.bin";
    mpz_t n, e, d;
    mpz_init(n); mpz_init(e); mpz_init(d);

    cout << "  Генерация ключей (может занять несколько секунд)...\n";
    generateKeys(n, e, d);
    saveKey(keyPath, n, e, d);

    vector<uint8_t> data(text.begin(), text.end());
    Padding(data);

    cout << "  Шифрование...\n";
    vector<uint8_t> ciphertext;
    encryptData(data, ciphertext, n, e);
    cout << "\n  Зашифрованный текст (hex):\n  " << hexDisplay(ciphertext) << "\n";

    cout << "  Расшифрование...\n";
    vector<uint8_t> plaintext;
    decryptData(ciphertext, plaintext, n, d);
    deletePadding(plaintext);
    cout << "\n  Расшифрованный текст:\n  "
         << string(plaintext.begin(), plaintext.end()) << "\n";

    clear_keys(n, e, d);
}

extern "C" void plugin_run() {
    cout << "\n=== RSA ===\n";
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
