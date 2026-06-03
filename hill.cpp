#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

using Matrix = vector<vector<int>>;

// Генерация алфавита для шифра
vector<char32_t> genalphabet() {
    vector<char32_t> alphabet;
    for (char32_t c = U'A'; c <= U'Z'; ++c) alphabet.push_back(c);
    for (char32_t c = U'a'; c <= U'z'; ++c) alphabet.push_back(c);
    for (char32_t c = U'А'; c <= U'Я'; ++c) alphabet.push_back(c);
    for (char32_t c = U'а'; c <= U'я'; ++c) alphabet.push_back(c);
    for (char32_t c = U'0'; c <= U'9'; ++c) alphabet.push_back(c);
    const char32_t punct[] = {
        U' ', U'!', U'@', U'#', U'$', U'%', U'^', U'&', U'*',
        U'(', U')', U'-', U'_', U'=', U'+', U'\\', U'|', U'/',
        U'"', U':', U';', U'.', U',', U'?'
    };
    for (char32_t c : punct) alphabet.push_back(c);

    return alphabet;
}

// делаем mod отдельно, т.к. могут быть отрицательные числа
int mod(int a, int m) {
    return ((a % m) + m) % m;
}

// Расширенный алгоритм Евклида
int gcd_euclidext(int a, int b, int& u) {
    if (b == 0) {
        u = 1;
        return a;
    }
    int u_prev = 1, u_curr = 0;
    int r1 = a, r2 = b;

    while (r2 != 0) {
        int q = r1 / r2;
        int temp = r2;
        r2 = r1 - r2 * q;
        r1 = temp;

        temp = u_curr;
        u_curr = u_prev - q * u_curr;
        u_prev = temp;
    }

    u = u_prev;
    return r1;
}

// Обратный элемент
int modInverse(int a, int m) {
    int u = 0;
    int gcd = gcd_euclidext(mod(a, m), m, u); // mod на случай отрицательного a
    if (gcd != 1) {
        cerr << "Обратный элемент не существует!\n";
        return 0;
    }
    return mod(u, m);
}

//перевод символов в числа и обратно
int charToIndex(char32_t c, const vector<char32_t>& alphabet) {
    for (int i = 0; i < (int)alphabet.size(); i++) {
        if (alphabet[i] == c) return i;
    }
    cerr << "Ошибка: Символ не найден в алфавите!\n";
    return -1;
}

char32_t indexToChar(int i, const vector<char32_t>& alphabet) {
    if (i < 0 || i >= (int)alphabet.size()) {
        cerr << "Ошибка: Индекс вне диапазона!\n";
        return U'?';
    }
    return alphabet[i];
}

// умножение матриц по модулю
Matrix matMul(const Matrix& A, const Matrix& B, int m) {
    int n = A.size();
    Matrix C(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k) C[i][j] = mod(C[i][j] + A[i][k] * B[k][j], m);      
    return C;
}

// определитель матрицы
int matDet(const Matrix& A, int m) {
    int n = A.size();
    Matrix M = A;
    int det = 1;

    for (int col = 0; col < n; ++col) {
        // Ищем ненулевой элемент
        int nenull = -1;
        for (int row = col; row < n; ++row) {
            if (mod(M[row][col], m) != 0) {
                nenull = row;
                break;
            }
        }

        // Если ненулевого элемента нет то определитель равен 0
        if (nenull == -1) return 0;

        // Меняем строки местами
        if (nenull != col) {
            swap(M[nenull], M[col]);
            det = mod(-det, m);
        }

        det = mod(det * M[col][col], m);

        // Обнуляем элементы ниже диагонали
        int nenullInv = modInverse(M[col][col], m);
        for (int row = col + 1; row < n; ++row) {
            int factor = mod(M[row][col] * nenullInv, m);
            for (int k = col; k < n; ++k) M[row][k] = mod(M[row][k] - factor * M[col][k], m);
        }
    }
    return det;
}

// Обратная матрица
Matrix invMatrix(const Matrix& A, int m) {
    int n = A.size();
    
    // расширяем матрицу
    Matrix M(n, vector<int>(2 * n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) M[i][j] = mod(A[i][j], m);
        M[i][i + n] = 1; // ед. матрица справа
    }

    for (int col = 0; col < n; ++col) {
        // Ищем ненулевой элемент
        int nenull = -1;
        for (int row = col; row < n; ++row) {
            if (mod(M[row][col], m) != 0) {
                nenull = row;
                break;
            }
        }

        if (nenull == -1) {
            cerr << "Матрица необратима!\n";
            return {};
        }

        // Меняем строки местами
        if (nenull != col) swap(M[nenull], M[col]);

        // делаем единичную матрицу слева
        int nenullInv = modInverse(M[col][col], m);
        for (int j = 0; j < 2 * n; ++j) M[col][j] = mod(M[col][j] * nenullInv, m);
        
        for (int row = 0; row < n; ++row) {
            if (row == col) continue;
            int fac = M[row][col];
            for (int j = 0; j < 2 * n; j++) M[row][j] = mod(M[row][j] - fac * M[col][j], m);
        }
    }

    // получаем нужную матрицу из правой части
    Matrix res(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) res[i][j] = M[i][j + n];
    }
    return res;
}



int main() {
    setlocale(LC_ALL, "Rus");
    
    return 0;
}