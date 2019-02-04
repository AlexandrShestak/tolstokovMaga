#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;


void generateInput() {
    ofstream myFile ("input.bin", ios::out | ios::binary);
    int n = 2;
    int m = 7;
    myFile.write(reinterpret_cast<const char *>(&n), sizeof(n));
    myFile.write(reinterpret_cast<const char *>(&m), sizeof(m));

    static const char arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9,1, 2, 3, 4, 5, 6, 7, 8, 9,1, 2, 3, 4, 5, 6, 7, 8, 9};
    //int arraySize = sizeof(arr)/sizeof(*arr);
    for (int  i = 0 ; i < n * m ; i++) {
        myFile.write(&arr[i], sizeof(arr[i]));
    }
    myFile.close();
}

void readOutput() {
    std::ifstream output("output.bin", ios::in | ios::binary);

    int n,m;
    output.read(reinterpret_cast<char *>(&n), sizeof(n));
    output.read(reinterpret_cast<char *>(&m), sizeof(m));
    cout << n << " " << m << " " << endl;

    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            char temp;
            output.read(&temp, sizeof(temp));
            cout << (int) temp << " ";
        }
        cout << endl;
    }
}

char* readBlock(int blockLinesCount, int blockColsCount, int heightOffset, int widthOffset, int originalMatrixRowsCount, char* block) {
    std::ifstream input( "input.bin", ios::in | ios::binary);

    if (originalMatrixRowsCount == blockColsCount) {
        input.seekg(8 + heightOffset*blockColsCount, ios::beg);
        input.read(block, blockLinesCount*blockColsCount);
    } else {
        for (int i = 0; i < blockLinesCount; i++) {
            input.seekg(8 + (heightOffset + i) * originalMatrixRowsCount + widthOffset, ios::beg);
            input.read(&block[i*blockColsCount], blockColsCount);
        }
    }
    input.close();

//    for (int i = 0 ; i < blockLinesCount ; i++) {
//        for (int j = 0 ; j < blockColsCount ; j ++) {
//            count << (int)block[i * blockColsCount + j];
//        }
//        count << endl;
//    }
//    count<< "------";
    return block;
}

void writeBlock(char* matrixBlock, int blockLinesCount, int blockColsCount, int heightOffset, int widthOffset,
                int originalMatrixLinesCount) {
    //std::ofstream output( "output.bin", ios::out | ios::binary | ios::app);
    std::ofstream output( "output.bin", ios::out | ios::binary | ios::in);

    if (originalMatrixLinesCount == blockLinesCount) {
        auto * transposeBlock = new char[blockLinesCount * blockColsCount];
        for (int i = 0 ; i < blockLinesCount ; i++) {
            for (int j = 0 ; j < blockColsCount ; j++) {
                transposeBlock[i + j * blockLinesCount] = matrixBlock[j + i * blockColsCount];
            }
        }
        output.seekp(8 + widthOffset * blockLinesCount, ios::beg);
        output.write(transposeBlock, blockLinesCount * blockColsCount);
        delete[] transposeBlock;
    } else {

        for (int i = 0; i < blockColsCount; i++) {
            auto *buf = new char[blockLinesCount];
            for (int j = 0; j < blockLinesCount; j++) {
                buf[j] = matrixBlock[i + j * blockColsCount];
            }
            output.seekp(8 + (widthOffset + i) * originalMatrixLinesCount + heightOffset, ios::beg);
            output.write(buf, blockLinesCount);
            delete[] buf;
        }
    }
    output.close();
}

int main(int argc, char* argv[]) {
//    remove("output.bin");
//    remove("input.bin");
//    generateInput();
    std::ifstream input( "input.bin", ios::in | ios::binary );
    int n, m;
    input.read(reinterpret_cast<char *>(&n), 4);
    input.read(reinterpret_cast<char *>(&m), 4);

    std::ofstream output("output.bin", ios::out | ios::binary | ios::app);
    output.write(reinterpret_cast<const char *>(&m), 4);
    output.write(reinterpret_cast<const char *>(&n), 4);
    output.close();

    const int ELEMENTS_FIT_IN_MEMORY = 80000;
    int blockWidth = 1000;

    if (m < blockWidth) {
        blockWidth = min(m, ELEMENTS_FIT_IN_MEMORY);
    }
    int blockHeight = ELEMENTS_FIT_IN_MEMORY / blockWidth;
    if (blockHeight == 0) {
        blockHeight = 1;
    }

    int processed = 0;
    int widthProcessed = 0;
    int blockLineIteration = 0;
    int blockRowIteration = 0;
    while (processed < n * m) {
        int currentBlockWidth = blockWidth;
        if (m - widthProcessed < blockWidth) {
            currentBlockWidth = m - widthProcessed;
        }
        int currentBlockHeight = blockHeight;
        if (n < (blockLineIteration + 1) * blockHeight) {
            currentBlockHeight = n - blockLineIteration * blockHeight;
        }

        auto *block = new char[currentBlockWidth * currentBlockHeight];
        int heightOffset = blockHeight * blockLineIteration;
        int widthOffset = widthProcessed;
        char* matrixBlock = readBlock(currentBlockHeight, currentBlockWidth,
                                      heightOffset, widthOffset, m, block);
        writeBlock(matrixBlock, currentBlockHeight, currentBlockWidth, heightOffset, widthOffset, n);

        processed += currentBlockHeight * currentBlockWidth;
        widthProcessed += blockWidth;
        blockRowIteration += 1;
        if (widthProcessed >= m) {
            blockLineIteration += 1;
            widthProcessed = 0;
            blockRowIteration = 0;
        }
        delete[] block;
    }

//    count << endl << "-------------------"<< endl;
//    readOutput();
}