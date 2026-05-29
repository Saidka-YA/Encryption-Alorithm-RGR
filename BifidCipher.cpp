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

// Шифрование
string encrypt(const string& text, const string& alphabet) {
    int size = static_cast<int>(ceil(sqrt(alphabet.length())));
    vector<vector<char>> grid(size, vector<char>(size, 0));
    
    int idx = 0;
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (idx < static_cast<int>(alphabet.length())) {
                grid[r][c] = alphabet[idx++];
            }
        }
    }

    vector<int> rows, cols;
    for (char c : text) {
        pair<int, int> coords = findCoords(grid, size, c);
        if (coords.first != -1) {
            rows.push_back(coords.first);
            cols.push_back(coords.second);
        } else {
            cerr << " Символ '" << c << "' пропущен (нет в алфавите)." << endl;
        }
    }

    if (rows.empty()) return "";

    vector<int> combined = rows;
    combined.insert(combined.end(), cols.begin(), cols.end());

    string result;
    for (size_t i = 0; i + 1 < combined.size(); i += 2) {
        int r = combined[i];
        int c = combined[i + 1];
        if (r < size && c < size) {
            result += grid[r][c];
        }
    }
    return result;
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
