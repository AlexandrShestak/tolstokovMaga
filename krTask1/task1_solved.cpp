#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

typedef bitset<8> BYTE;

void generateInput() {
    ofstream myFile ("input.bin", ios::out | ios::binary);
    int a = 1;
    uint8_t a1 = 4;
    int b = 2;
    uint8_t b1 = 1;
    uint8_t b2 = 6;
    myFile.write(reinterpret_cast<const char *>(&a), 4);
    myFile.write(reinterpret_cast<const char *>(&a1), 1);
    myFile.write(reinterpret_cast<const char *>(&b), 4);
    myFile.write(reinterpret_cast<const char *>(&b1), 1);
    myFile.write(reinterpret_cast<const char *>(&b1), 1);


    myFile.close();
}

void readOutput() {
    std::ifstream output("output.bin", ios::in | ios::binary);

    int n,m;
    output.read(reinterpret_cast<char *>(&n), sizeof(n));
    output.read(reinterpret_cast<char *>(&m), sizeof(m));
    cout << n << " " << m << " " << endl;

//    for (int i = 0 ; i < n ; i++) {
//        for (int j = 0 ; j < m ; j++) {
//            char temp;
//            output.read(&temp, sizeof(temp));
//            cout << (int) temp << " ";
//        }
//        cout << endl;
//    }
}

int main(int argc, char* argv[]) {
    //generateInput();

    fstream input("input.bin", ios::in | ios::binary);
    int a;
    int b;
    input.read(reinterpret_cast<char *>(&a), 4);
    input.seekg(4, ios::beg);
    input.read(reinterpret_cast<char *>(&b), 4);
    input.close();


    int c = a + b;
    ofstream output("output.bin", ios::out | ios::binary | ios::app);
    output.write(reinterpret_cast<const char *>(&c), 4);
    output.close();

    readOutput();
}