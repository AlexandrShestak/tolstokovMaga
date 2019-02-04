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
    ifstream inputFile ("input.txt");
    ofstream outputFile ("output.txt");

    int n;
    string str;

    inputFile >> n;
    inputFile >> str;

    int position = 0;
    vector<int> prefixFunction = compute_prefix_function(str);
    string answer = "";
    for(int& value: prefixFunction) {
        if (value * 2 >= position + 1) {
            int startLength = position + 1 - value;
            if ((position + 1) % startLength == 0) {
                int nextPrefixPosition = position;
                while (true) {
                    //value -= startLength;
                    if (value == startLength) {
                        //cout << position+1 << " " << (position + 1) / startLength << endl;
                        answer +=  std::to_string(position+1) + " " + std::to_string((position + 1) / startLength) + "\n";
                        //outputFile << position+1 << " " << (position + 1) / startLength << endl;
                        break;
                    } else if (prefixFunction[position-startLength] % startLength != 0 || value < 0) {
                        break;
                    }
                    nextPrefixPosition -= startLength;
                    value = prefixFunction[nextPrefixPosition];
                }
            }
        }
        position++;
    }
    outputFile << answer;
}