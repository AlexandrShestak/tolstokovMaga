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

vector<int> compute_z_function(const string& s) {
    int n = (int) s.length();
    vector<int> z (n);
    for (int i=1, l=0, r=0; i<n; ++i) {
        if (i <= r)
            z[i] = min (r-i+1, z[i-l]);
        while (i+z[i] < n && s[z[i]] == s[i+z[i]])
            ++z[i];
        if (i+z[i]-1 > r)
            l = i,  r = i+z[i]-1;
    }
    return z;
}



bool is_prefix_function_correct(vector<int> p) {
    for (int i = 0 ; i < p.size() ; i++) {
        if (i==0 && p[i]>1) {
            return false;
        }
        if (p[i]>p[i-1]+1) {
            return false;
        }
    }
    return true;
}

vector<int> buildZFunctionFromPrefix(vector<int> p) {
    if (is_prefix_function_correct(p)) {
        string s = "a";
        int charactersUsed = 1;
        for (int i = 0 ; i < p.size() ; i++) {
            if (p[i] == 0) {
                s += 'a' + charactersUsed;
                charactersUsed++;
            } else {
                s += s[p[i] - 1];
            }
        }
        return compute_z_function(s);
    } else {
        return vector<int>();
    }
}

vector<int> buildPrefixFunctionFromZ(vector<int> z) {
    string s = "a";
    int prefixLength = 0; // длина префикса, который мы записываем
    int j; // позиция символа в строке, который будем записывать
    int charactersUsed = 1; // индекс нового символа
    for (int i = 0 ; i < z.size() ; i++) {
        if (z[i] == 0 && prefixLength == 0) {
            s += 'a' + charactersUsed;
            charactersUsed++;
        }
        if (z[i] > prefixLength) {
            prefixLength = z[i];
            j = 0;
        }
        if (prefixLength > 0) {
            s += s[j];
            j++;
            prefixLength--;
        }
    }

    vector<int> newZFucntion = compute_z_function(s);
    for (int k = 1 ; k < newZFucntion.size() ; k++) {
        if (newZFucntion[k] != z[k-1]) {
            return vector<int>();
        }
    }

    return compute_prefix_function(s);
}

int main(int argc, char* argv[]) {
    ifstream inputFile ("input.txt");
    ofstream outputFile ("output.txt");

    int t;

    inputFile >> t;
    for (int i = 0 ; i < t ; i++) {
        int n;
        vector<int> function;
        inputFile >> n;
        for (int j = 0 ; j < n - 1 ; j++) {
            int temp;
            inputFile >> temp;
            function.push_back(temp);
        }


        vector<int> prefixFunction = buildPrefixFunctionFromZ(function);
        if (prefixFunction.size()) {
            for (int l = 1 ; l < prefixFunction.size() ; l++) {
                if (l == prefixFunction.size()-1) {
                    outputFile << prefixFunction[l];
                } else {
                    outputFile << prefixFunction[l] << " ";
                }
            }
            outputFile << endl;
        } else {
            outputFile << "-1" << endl;
        }
        vector<int> zFunction = buildZFunctionFromPrefix(function);
        if (zFunction.size()) {
            for (int l = 1 ; l < zFunction.size() ; l++) {
                if (l == prefixFunction.size()-1) {
                    outputFile << zFunction[l];
                } else {
                    outputFile << zFunction[l] << " ";
                }
            }
            outputFile << endl;
        } else {
            outputFile << "-1" << endl;
        }
    }
}