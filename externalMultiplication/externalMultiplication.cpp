#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

void generateInput() {
    ofstream myFile ("input.bin", ios::out | ios::binary);
    int n1 = 5;
    int m1 = 5;
    myFile.write(reinterpret_cast<const char *>(&n1), sizeof(n1));
    myFile.write(reinterpret_cast<const char *>(&m1), sizeof(m1));

    static const char arr1[] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5,1, 2, 3, 4, 5,1, 2, 3, 4, 5,1, 2, 3, 4, 5  };
    for (int  i = 0 ; i < n1 * m1 ; i++) {
        myFile.write(&arr1[i], sizeof(arr1[i]));
    }

//    int n2 = 3;
//    int m2 = 3;
    myFile.write(reinterpret_cast<const char *>(&n1), sizeof(n1));
    myFile.write(reinterpret_cast<const char *>(&m1), sizeof(m1));

    static const char arr2[] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5,1, 2, 3, 4, 5,1, 2, 3, 4, 5,1, 2, 3, 4, 5  };
    for (int  i = 0 ; i < n1 * m1 ; i++) {
        myFile.write(&arr2[i], sizeof(arr2[i]));
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

char* readBlock(int blockLinesCount, int blockColsCount, int heightOffset, int widthOffset, int originalMatrixRowsCount, char* block,
                int basisOffset, bool output) {
    string fileName;
    if (output) {
        fileName = "output.bin";
    } else {
        fileName = "input.bin";
    }
    std::ifstream input(fileName, ios::in | ios::binary);

    if (originalMatrixRowsCount == blockColsCount) {
        input.seekg(8 + basisOffset + heightOffset*blockColsCount, ios::beg);
        input.read(block, blockLinesCount*blockColsCount);
    } else {
        for (int i = 0; i < blockLinesCount; i++) {
            input.seekg(8 + basisOffset + (heightOffset + i) * originalMatrixRowsCount + widthOffset, ios::beg);
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
//    count<< "------" << endl;
    return block;
}

void writeBlock(char* matrixBlock, int blockLinesCount, int blockColsCount, int heightOffset, int widthOffset,
                int originalMatrixLinesCount) {
    //std::ofstream output( "output.bin", ios::out | ios::binary | ios::app);
    std::ofstream output( "output.bin", ios::out | ios::binary | ios::in);

    if (originalMatrixLinesCount == blockLinesCount) {
//        auto * transposeBlock = new char[blockLinesCount * blockColsCount];
//        for (int i = 0 ; i < blockLinesCount ; i++) {
//            for (int j = 0 ; j < blockColsCount ; j++) {
//                transposeBlock[i + j * blockLinesCount] = matrixBlock[j + i * blockColsCount];
//            }
//        }
        output.seekp(widthOffset * blockLinesCount, ios::beg);
        output.write(matrixBlock, blockLinesCount * blockColsCount);
//        delete[] transposeBlock;
    } else {

        for (int i = 0; i < blockLinesCount; i++) {
            output.seekp((heightOffset + i) * originalMatrixLinesCount + widthOffset, ios::beg);
            output.write(&matrixBlock[i * blockColsCount], blockColsCount);
        }
    }
    output.close();
}


int main(int argc, char* argv[]) {
//    remove("output.bin");
//    remove("input.bin");
//    generateInput();
    std::ifstream input( "input.bin", ios::in | ios::binary );
    int n;
    input.read(reinterpret_cast<char *>(&n), 4);
    input.close();


    std::ofstream output("output.bin", ios::out | ios::binary | ios::app);
//    output.write(reinterpret_cast<const char *>(&n), 4);
//    output.write(reinterpret_cast<const char *>(&n), 4);
    output.close();

    int blockSize = 100;

    for (int i = 0 ; i < n ; i += blockSize) {
        for (int j = 0 ; j < n ; j += blockSize) {
            int currentBlockHeight = blockSize;
            int currentBlockWidth = blockSize;

            if (n - i < blockSize) {
                currentBlockHeight = n - i;
            }
            if (n - j < blockSize) {
                currentBlockWidth = n - j;
            }

            // initialize z with zeros
            char* zBlock = new char[currentBlockHeight*currentBlockWidth];
            for (int q = 0 ; q < currentBlockHeight*currentBlockWidth ; q++) {
                zBlock[q] = 0;
            }
            writeBlock(zBlock, currentBlockHeight, currentBlockWidth, i, j, n);
//            delete [] zBlock;

            for (int k = 0 ; k < n ; k += blockSize) {
                int kSise = blockSize;
                if (n - k < blockSize) {
                    kSise = n - k;
                }

                char* xBlock = new char[currentBlockHeight * kSise];
                readBlock(currentBlockHeight, kSise, i, k, n, xBlock, 0, false);
                int additionalOffsetForY = 8 + n * n;
                char* yBlock = new char[kSise * currentBlockWidth];
                int yBlockWidth = currentBlockHeight;
                if (n - j  < blockSize) {
                    yBlockWidth = n - j;
                }
                readBlock(kSise, currentBlockWidth, k, j, n, yBlock,
                          additionalOffsetForY, false);


                for (int p = 0 ; p < currentBlockHeight ; p++) {
                    for (int l = 0 ; l < currentBlockWidth ; l++) {
                        for (int h = 0 ; h < kSise ; h++) {
                            zBlock[p * currentBlockWidth + l] +=
                                    xBlock[p * kSise + h] * yBlock[h * currentBlockWidth + l];
                        }
                    }
                }
                delete[] xBlock;
                delete[] yBlock;
            }
            writeBlock(zBlock, currentBlockHeight, currentBlockWidth, i, j, n);
            delete [] zBlock;

//            count << endl << "-------------------"<< endl;
//            readOutput();
        }
    }

//    count << endl << "-------------------"<< endl;
//    readOutput();
}