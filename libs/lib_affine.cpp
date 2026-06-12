/*
 * libs/lib_affine.cpp
 * Компилируется в libaffine.so
 * Аффинный шифр: encrypt(x) = (a*x + b) mod 256
 *
 * Использует crypto_interface.h: get_algorithm_info / encrypt / decrypt
 * Обёрнут в plugin_run() для единого интерфейса плагинов.
 */
#include "plugin_api.h"
#include "validation.h"
#include "crypto_interface.h"
#include "file_processor.h"
#include "key_generator.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;

// ── Внутренние символы из affine_cipher.cpp (включаем .cpp напрямую) ─────
// affine_cipher.cpp компилируется вместе с этим файлом через Makefile —
// объявляем extern C символы которые он экспортирует.
extern "C" {
    const AlgorithmInfo* get_algorithm_info();
    size_t               get_output_size(size_t input_size, int operation_type);
    int                  encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output);
    int                  decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output);
}

// ── Вспомогательные функции ───────────────────────────────────────────────

static string bytes_to_hex(const vector<uint8_t>& data) {
    ostringstream oss;
    for (size_t i = 0; i < data.size(); ++i) {
        if (i > 0 && i % 16 == 0) oss << "\n";
        oss << hex << setw(2) << setfill('0') << (int)data[i] << " ";
    }
    return oss.str();
}

// Проверка кода возврата шифра с понятным сообщением
static bool check_crypto_result(int rc, const string& op) {
    if (rc == 0) return true;
    cerr << "  [!] Ошибка " << op << ": ";
    switch (rc) {
        case 1: cerr << "ключ слишком короткий.\n"; break;
        case 2: cerr << "параметр 'a' чётный — шифр необратим.\n"; break;
        default: cerr << "код ошибки " << rc << ".\n"; break;
    }
    return false;
}

// ── Сохранение / загрузка ключа ───────────────────────────────────────────

static bool save_affine_key(const string& path, const vector<uint8_t>& key) {
    ofstream f(path, ios::binary);
    if (!f) {
        cerr << "  [!] Ошибка: не удалось сохранить ключ в '"
             << path << "'.\n";
        return false;
    }
    f.write(reinterpret_cast<const char*>(key.data()), key.size());
    return true;
}

static vector<uint8_t> load_affine_key(const string& path) {
    try {
        vector<uint8_t> key = FileProcessor::read_file(path);
        if (key.size() < 2) {
            cerr << "  [!] Ошибка: файл ключа '" << path
                 << "' повреждён (меньше 2 байт).\n";
            return {};
        }
        if (key[0] % 2 == 0) {
            cerr << "  [!] Ошибка: файл ключа повреждён "
                    "(параметр 'a' = " << (int)key[0] << " — чётный).\n";
            return {};
        }
        return key;
    } catch (const exception& e) {
        cerr << "  [!] Ошибка загрузки ключа: " << e.what() << "\n";
        return {};
    }
}

// ── Режим: файл ───────────────────────────────────────────────────────────

static void run_file(bool do_encrypt) {
    const string keyPath = "affineKey.bin";

    string inputPath  = val_input_existing_file(
        do_encrypt ? "Путь к входному файлу"
                   : "Путь к зашифрованному файлу");

    string outputPath = val_input_writable_file(
        "Путь для сохранения результата");

    vector<uint8_t> key;
    if (do_encrypt) {
        cout << "  Способ задания ключа:\n"
                "    1. Ввести вручную\n"
                "    2. Сгенерировать случайный\n";
        int km = val_input_int("  Выбор: ", 1, 2);

        if (km == 1) {
            key = val_input_affine_key();
        } else {
            // Генерируем, проверяем что a — нечётное
            do { key = KeyGenerator::generate_key(2); } while (key[0] % 2 == 0);
            cout << "  Сгенерирован ключ: a=" << (int)key[0]
                 << " b=" << (int)key[1] << "\n";
        }

        string kp = val_input_optional_file("Путь для сохранения ключа");
        if (!kp.empty()) {
            if (save_affine_key(kp, key))
                cout << "  Ключ сохранён в '" << kp << "'\n";
        }
    } else {
        key = load_affine_key(keyPath);
        if (key.empty()) {
            cerr << "  Стандартный файл ключа '" << keyPath
                 << "' не найден или повреждён.\n"
                    "  Введите путь к файлу ключа вручную:\n";
            string kp = val_input_existing_file("Путь к файлу ключа");
            key = load_affine_key(kp);
            if (key.empty()) return;
        }
    }

    vector<uint8_t> data;
    try {
        data = FileProcessor::read_file(inputPath);
    } catch (const exception& e) {
        cerr << "  [!] Ошибка чтения: " << e.what() << "\n";
        return;
    }

    vector<uint8_t> result(data.size());
    ConstBuffer kb = { key.data(), key.size() };
    ConstBuffer ib = { data.data(), data.size() };
    MutBuffer   ob = { result.data(), result.size() };

    int rc = do_encrypt ? encrypt(kb, ib, &ob) : decrypt(kb, ib, &ob);
    if (!check_crypto_result(rc, do_encrypt ? "шифрования" : "расшифрования"))
        return;

    try {
        FileProcessor::write_file(outputPath, result);
    } catch (const exception& e) {
        cerr << "  [!] Ошибка записи: " << e.what() << "\n";
        return;
    }

    if (do_encrypt) {
        // Сохраняем ключ рядом с бинарником автоматически
        save_affine_key(keyPath, key);
        cout << "  Готово. Ключ сохранён в " << keyPath << "\n";
    } else {
        cout << "  Файл расшифрован.\n";
    }
}

// ── Режим: текст с консоли ────────────────────────────────────────────────

static void run_text() {
    string text = val_input_nonempty("Введите текст для шифрования: ");

    cout << "  Способ задания ключа:\n"
            "    1. Ввести вручную\n"
            "    2. Сгенерировать случайный\n";
    int km = val_input_int("  Выбор: ", 1, 2);

    vector<uint8_t> key;
    if (km == 1) {
        key = val_input_affine_key();
    } else {
        do { key = KeyGenerator::generate_key(2); } while (key[0] % 2 == 0);
        cout << "  Сгенерирован ключ: a=" << (int)key[0]
             << " b=" << (int)key[1] << "\n";
    }

    vector<uint8_t> data(text.begin(), text.end());
    vector<uint8_t> cipher(data.size());
    vector<uint8_t> plain(data.size());

    ConstBuffer kb  = { key.data(),  key.size()  };
    ConstBuffer ib  = { data.data(), data.size() };
    MutBuffer   cb  = { cipher.data(), cipher.size() };
    MutBuffer   pb  = { plain.data(),  plain.size()  };

    if (!check_crypto_result(encrypt(kb, ib, &cb), "шифрования")) return;
    cout << "\n  Зашифрованный текст (hex):\n  " << bytes_to_hex(cipher) << "\n";

    ConstBuffer cib = { cipher.data(), cipher.size() };
    if (!check_crypto_result(decrypt(kb, cib, &pb), "расшифрования")) return;
    cout << "\n  Расшифрованный текст:\n  "
         << string(plain.begin(), plain.end()) << "\n";
}

// ── Точка входа плагина ───────────────────────────────────────────────────

extern "C" void plugin_run() {
    const AlgorithmInfo* info = get_algorithm_info();
    cout << "\n=== Аффинный шифр (" << info->algorithm_name << ") ===\n";
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
