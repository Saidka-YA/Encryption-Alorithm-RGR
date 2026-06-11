/*
libs/lib_bifid_adfgx.cpp  —  Бифид + ADFGX
*/
#include "bifid.h"
#include "adfgx.h"
#include "hex_utils.h"
#include "console_utils.h"
#include "plugin_api.h"
#include "validation.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

using namespace std;

// Перечисление вариантов главного меню плагина
enum class PluginAction {
    Exit = 0,
    BifidEncrypt = 1,
    BifidDecrypt = 2,
    AdfgxEncrypt = 3,
    AdfgxDecrypt = 4
};

// Чтение текстовых данных из выбранного источника
static string read_text(bool from_file) {
    if (from_file) {
        string path = val_input_existing_file("  Путь к входному файлу");
        ifstream f(path, ios::binary);
        if (!f) throw runtime_error("Не удалось открыть файл: " + path);
        stringstream buf;
        buf << f.rdbuf();
        string text = buf.str();
        if (text.empty()) throw invalid_argument("Файл пустой");
        return text;
    } else {
        return val_input_nonempty("  Введите текст: ");
    }
}

// Запись результирующих данных в файл
static void write_result(const string & result, const string & outfile) {
    ofstream f(outfile, ios::binary);
    if (!f) throw runtime_error("Не удалось создать файл:  " + outfile);
    f << result;
    cout << "  Результат сохранён в '" << outfile << "'\n";
}

// Главная управляющая функция плагина
extern "C" void plugin_run() {
    cout << "\n===  Бифид / ADFGX ===\n";
    cout << "  1. Бифид — зашифровать\n";
    cout << "  2. Бифид — расшифровать\n";
    cout << "  3. ADFGX — зашифровать\n";
    cout << "  4. ADFGX — расшифровать\n";
    cout << "  0. Назад\n";
    
    int choiceInt = val_input_int("Выбор:  ", 0, 4);
    PluginAction action = static_cast<PluginAction>(choiceInt);
    
    if (action == PluginAction::Exit) return;

    try {
        int src = val_input_int("  Источник данных (1 — файл, 2 — консоль):  ", 1, 2);
        string text = read_text(src == 1);

        string keyword;
        if (action == PluginAction::AdfgxEncrypt || action == PluginAction::AdfgxDecrypt) {
            keyword = val_input_keyword("  Ключевое слово ");
        }

        string outfile = val_input_writable_file("  Файл для сохранения результата ");
        string result;
        
        // Обработка действий пользователя с помощью enum class
        switch (action) {
            case PluginAction::BifidEncrypt: 
                result = encryptBifid(text);          
                break;
            case PluginAction::BifidDecrypt: 
                result = decryptBifid(text);          
                break;
            case PluginAction::AdfgxEncrypt: 
                result = encryptADFGX(text, keyword); 
                break;
            case PluginAction::AdfgxDecrypt: 
                result = decryptADFGX(text, keyword); 
                break;
            default:
                break;
        }

        write_result(result, outfile);
    } catch (const exception & e) {
        cout << "  Ошибка: " << e.what() << "\n";
    }
}