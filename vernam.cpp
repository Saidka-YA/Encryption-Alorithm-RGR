#include "vernam.h"

vector<int> vernamKeyFromText(const string& text, const vector<char32_t>& alphabet) {
    int m = to_codes(text).size();
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(0, (int)alphabet.size() - 1);
    vector<int> key;
    for (size_t i = 0; i < (size_t)m; ++i) key.push_back(dist(rng));
    return key;
}

string vernamEncrypt(const string& text, const vector<int>& key, const vector<char32_t>& alphabet);
string vernamDecrypt(const string& text, const vector<int>& key, const vector<char32_t>& alphabet);