#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <stdlib.h>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {
    std::map<int,int> kMax;

    //ifstream cin("input.txt");

    int k;
    int kTemp = 0;
    cin >> k;

    int a;
    cin >> a;

    while (a != -1) {
        if (a == 0) {
            std::map<int, int>::iterator it = kMax.begin();
            string result;
            while (it != kMax.end()) {
                //result += std::to_string(it->first);
                cout << it->first << " ";
                it++;
                //result += " ";

            }
            if (kMax.size() < k -1) {
                for (int i = 1 ; i < kMax.size()-k+i ; i++) {
                    //result += "1 ";
                    cout << "1 ";
                }
            }
            cout << endl;
        } else {
            if (!kMax[a]) {
                kMax[a] = 1;
                kTemp++;
            } else {
                kMax[a] += 1;
            }
            if (kTemp == k) {
                std::map<int, int>::iterator it = kMax.begin();

                while (it != kMax.end()) {
                    kMax[it->first] -= 1;
                    if (kMax[it->first] == 0) {
                        it = kMax.erase(it);
                        kTemp--;
                    } else {
                        it++;
                    }
                }
            }
        }
        cin >> a;
    }

    return 0;
}