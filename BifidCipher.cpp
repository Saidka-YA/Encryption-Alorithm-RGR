#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <clocale>
#include <limits>

using namespace std;

// Проверка на дубликаты в алфавите
bool hasDuplicates(const string& alphabet) {
    for (size_t i = 0; i < alphabet.length(); ++i) {
        if (alphabet.find(alphabet[i], i + 1) != string::npos) {
            return true;
        }
    }
    return false;
}

// Поиск координат символа в таблице Полибия
pair<int, int> findCoords(const vector<vector<char>>& grid, int size, char target) {
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (grid[r][c] == target) {
                return {r, c};
            }
        }
    }
    return {-1, -1};
}




int main() {
    
    string text;
    cout << "Введите текст: ";
    getline(cin, text);

    if (text.empty()) {
        cerr << " Ошибка: текст не может быть пустым." << endl;
        return 1;
    }

    string result;
    
    if (result.empty()) {
        cerr << " Не удалось обработать текст (проверьте алфавит и входные данные)." << endl;
        return 1;
    }

    cout << "\n Результат: " << result << endl;
    return 0;
}
