#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


vector<int> compute_prefix_function(const string& s)
{
    int len = s.length();
    vector<int> p(len); // значения префикс-функции
    // индекс вектора соответствует номеру последнего символа аргумента
    p[0] = 0; // для префикса из нуля и одного символа функция равна нулю

    int k = 0;
    for (int i = 1; i < len; ++i) {
        while ((k > 0) && (s[k] != s[i]))
            k = p[k - 1];
        if (s[k] == s[i])
            ++k;
        p[i] = k;
    }
    return p;
}

int main(int argc, char* argv[]) {

    string line;
    ifstream inputFile ("input.txt");
    ofstream outputFile ("output.txt");

    int n;
    inputFile >> n;
    string str1, str2;
    inputFile >> str1 >> str2;

    string str3 = str2 + "0" + str1 + str1;
    vector<int> prefix_function = compute_prefix_function(str3);

    for (int i = n ; i < 3*n ; i++) {
        if (prefix_function[i] == n) {
            cout << i - 2 *n;
            outputFile << i -2*n;
            return 0;
        }
    }

    outputFile << "-1";
    return 0;
}