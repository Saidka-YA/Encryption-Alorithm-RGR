#ifndef VERNAM_H
#define VERNAM_H
 
#include <iostream>
#include <vector>
#include <random>
#include "textproc.h"
#include "filef.h"
 
using namespace std;
 
// Генерация случайного ключа по длине текста
vector<int> vernamKeyFromText(const string& text, const vector<char32_t>& alphabet);
 
// Шифрование / дешифрование
string vernamEncrypt(const string& text, const vector<int>& key, const vector<char32_t>& alphabet);
string vernamDecrypt(const string& text, const vector<int>& key, const vector<char32_t>& alphabet);
 
#endif
