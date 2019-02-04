#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

int blockSizeConst = 1000;
int N = 3;

void generateInput() {
    ofstream myFile ("input.bin", ios::out | ios::binary);
    int m1 = 4;
    myFile.write(reinterpret_cast<const char *>(&N), sizeof(N));
    myFile.write(reinterpret_cast<const char *>(&m1), sizeof(m1));

    static const char arr1[] = {1, 2, 3, 4, 5, 6, 7, 8,9, 1,1,1 };
    for (int  i = 0 ; i < N * m1 ; i++) {
        myFile.write(&arr1[i], sizeof(arr1[i]));
    }


    static const char arr2[] = {1, 2, 3, 4 };
    for (int  i = 0 ; i < m1 ; i++) {
        myFile.write(&arr2[i], sizeof(arr2[i]));
    }
    myFile.close();
}


void readOutput() {
    std::ifstream output("output.bin", ios::in | ios::binary);

    for (int i = 0 ; i < N ; i++) {
        char temp;
        output.read(&temp, sizeof(temp));
        cout << (int) temp << " ";
    }
    cout << endl;
}

int main(int argc, char* argv[]) {
//    generateInput();

    fstream input("input.bin", ios::in | ios::binary);
    ofstream output("output.bin", ios::out | ios::binary | ios::app);
    int n, m;

    input.read(reinterpret_cast<char *>(&n), 4);
    input.read(reinterpret_cast<char *>(&m), 4);

    int blockSize = 100000;
    int rowsToReadCount = 1;
    if (m < blockSize) {
        blockSize = m;

        rowsToReadCount = 100000 / m;

        rowsToReadCount = min(n, rowsToReadCount);

    }
//    int blockHeight = ELEMENTS_FIT_IN_MEMORY / blockSize;

    int rowsToReadTempCount = rowsToReadCount;
    for (int i = 0 ; i < n ; i += rowsToReadCount) {


        int currentBlockWith = blockSize;
        int ci = 0;
        for (int j = 0; j < m; j += blockSize) {

            if (m - j < currentBlockWith) {
                currentBlockWith = m - j;
            }
            if (n-i < rowsToReadTempCount) {
                rowsToReadTempCount = n-i;
            }

            auto *block1 = new char[currentBlockWith * rowsToReadTempCount];
            auto *block2 = new char[currentBlockWith];

            input.seekp(8 + i * m + j, ios::beg);
            input.read(&block1[0], currentBlockWith * rowsToReadTempCount);

            input.seekp(8 + n * m + j, ios::beg);
            input.read(&block2[0], currentBlockWith);

            if (rowsToReadCount == 1) {
                for (int k = 0; k < currentBlockWith; k++) {
                    ci += ((int) block1[k]) * ((int) block2[k]) % 256;
                }
                ci = ci % 256;
            } else {
                for (int y = 0; y < rowsToReadTempCount; y++) {
                    int citemp = 0;
                    for (int k = 0; k < currentBlockWith; k++) {
                        citemp += ((int) block1[k + y*m]) * ((int) block2[k]) % 256;
                    }
                    citemp = citemp % 256;
                    output.write(reinterpret_cast<const char *>(&citemp), 1);
                }

            }
            delete[] block1;
            delete[] block2;
        }
        if (rowsToReadCount == 1) {
            output.write(reinterpret_cast<const char *>(&ci), 1);
        }


    }
    output.close();
//    readOutput();
}