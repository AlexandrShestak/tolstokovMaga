#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

int blockSizeConst = 1000;

void generateInput() {
    ofstream myFile ("input.bin", ios::out | ios::binary);
//    int a = 5;
//    uint8_t a1 = 4;
//    uint8_t a2 = 4;
//    uint8_t a3 = 4;
//    uint8_t a4 = 5;
//    uint8_t a5 = 4;
//
//    int b = 3;
//    uint8_t b1 = 1;
//    uint8_t b2 = 6;
//    uint8_t b3 = 1;

    int a = 1;
    uint8_t a1 = 5;

    int b = 1;
    uint8_t b1 = 5;



    myFile.write(reinterpret_cast<const char *>(&a), 4);
    myFile.write(reinterpret_cast<const char *>(&a1), 1);
//    myFile.write(reinterpret_cast<const char *>(&a2), 1);
//    myFile.write(reinterpret_cast<const char *>(&a3), 1);
//    myFile.write(reinterpret_cast<const char *>(&a4), 1);
//    myFile.write(reinterpret_cast<const char *>(&a5), 1);

    myFile.write(reinterpret_cast<const char *>(&b), 4);
    myFile.write(reinterpret_cast<const char *>(&b1), 1);
//    myFile.write(reinterpret_cast<const char *>(&b2), 1);
//    myFile.write(reinterpret_cast<const char *>(&b3), 1);


    myFile.close();
}

void readOutput1(int c) {
    std::ifstream output("output1.bin", ios::in | ios::binary);
    for (int i = 0 ; i < c ; i++) {
        char temp;
        output.read(&temp, sizeof(temp));
        cout << (int) temp << " ";
    }
    cout << endl;
    output.close();
}

void readOutput() {
    std::ifstream output("output.bin", ios::in | ios::binary);

    int n;
    output.read(reinterpret_cast<char *>(&n), sizeof(n));
    cout << n << endl;

    for (int i = 0 ; i < n ; i++) {
        char temp;
        output.read(&temp, sizeof(temp));
        cout << (int) temp << " ";
    }
    cout << endl;
}

int main(int argc, char* argv[]) {
//    generateInput();

    fstream input("input.bin", ios::in | ios::binary);
    int a;
    int b;
    int c = 0;
    input.read(reinterpret_cast<char *>(&a), 4);
    input.seekg(4 + a, ios::beg);
    input.read(reinterpret_cast<char *>(&b), 4);
//    cout << a << b;
    int tempA = a;
    int tempB = b;


    int blockSize = blockSizeConst;

    if (a < blockSize) {
        blockSize = a;
    }
    if  (b < blockSize) {
        blockSize = b;
    }

    ofstream output("output1.bin", ios::out | ios::binary | ios::app);


    int iterations = 0;
    int perenos = 0;
    int currentBlockSize = blockSize;
    while (tempA > 0 && tempB > 0) {
        currentBlockSize = min(min(tempA, tempB), blockSize);
        auto *block1 = new char[currentBlockSize];
        auto *block2 = new char[currentBlockSize];
        auto *block3 = new char[currentBlockSize];

        input.seekp(4 + a - blockSize * iterations - currentBlockSize, ios::beg);
        input.read(&block1[0], currentBlockSize);
        input.seekp(8 + a + b - blockSize * iterations - currentBlockSize, ios::beg);
        input.read(&block2[0], currentBlockSize);

        for (int i = currentBlockSize - 1 ; i >=0 ; i--) {
            int number1 = block1[i];
            int number2 = block2[i];
            int number3 = number1 + number2 + perenos;

            block3[currentBlockSize - 1 - i] = static_cast<char>(number3 % 10);
            if (number3 >= 10) {
                perenos = 1;
            } else {
                perenos = 0;
            }
        }

//        output.seekp(iterations * blockSize, ios::beg);
        output.write(reinterpret_cast<const char *>(block3), currentBlockSize);
        c += currentBlockSize;
        tempA -= currentBlockSize;
        tempB -= currentBlockSize;
        iterations++;

        delete[] block1;
        delete[] block2;
        delete[] block3;
    }

    if (tempA == 0 && tempB == 0 && perenos != 0) {
        c++;
        output.write(reinterpret_cast<const char *>(&perenos), 1);
    }

    currentBlockSize = blockSize;
    iterations = 0;
    while (tempA > 0) {
        if (tempA < currentBlockSize) {
            currentBlockSize = tempA;
        }
        auto *block1 = new char[currentBlockSize];
        auto *block3 = new char[currentBlockSize];
        input.seekp(4 + a - b - blockSize * iterations - currentBlockSize, ios::beg);
        input.read(&block1[0], currentBlockSize);

        for (int i = currentBlockSize - 1 ; i  >=0 ; i--) {
            int number1 = block1[i];
            int number3 = number1 + perenos;
            block3[currentBlockSize - 1 - i] = static_cast<char>(number3 % 10);
            if (number3 >= 10) {
                perenos = 1;
            } else {
                perenos = 0;
            }
        }

//        output.seekp(iterations * blockSize, ios::beg);
        output.write(reinterpret_cast<const char *>(block3), currentBlockSize);
        c += currentBlockSize;
        tempA -= currentBlockSize;
        iterations++;

        delete[] block1;
        delete[] block3;
    }

    while (tempB > 0) {
        if (tempB < currentBlockSize) {
            currentBlockSize = tempB;
        }
        auto *block2 = new char[currentBlockSize];
        auto *block3 = new char[currentBlockSize];
        input.seekp(8 + b - blockSize * iterations - currentBlockSize, ios::beg);
        input.read(&block2[0], currentBlockSize);

        for (int i = currentBlockSize - 1 ; i >=0 ; i--) {
            int number2 = block2[i];
            int number3 = number2 + perenos;
            block3[currentBlockSize - 1 - i] = static_cast<char>(number3 % 10);
            if (number3 >= 10) {
                perenos = 1;
            } else {
                perenos = 0;
            }
        }
        //output.seekp(iterations * blockSize, ios::beg);
        output.write(reinterpret_cast<const char *>(block3), currentBlockSize);
        c += currentBlockSize;
        tempB -= currentBlockSize;
        iterations++;

        delete[] block2;
        delete[] block3;
    }
    output.close();
    input.close();

    //todo deletme
    //readOutput1(c);


    fstream inputFinal("output1.bin", ios::in | ios::binary);
    ofstream finalOutput("output.bin", ios::out | ios::binary | ios::app);
    finalOutput.write(reinterpret_cast<const char *>(&c), 4);

    blockSize = blockSizeConst;
    if (c < blockSize) {
        blockSize = c;
    }

    iterations = 0;
    currentBlockSize = blockSize;
    int tempC = c;
    while (tempC > 0) {
        if (tempC < currentBlockSize) {
            currentBlockSize = tempC;
        }
        auto *finalBlock = new char[currentBlockSize];

        inputFinal.seekp(c - iterations *blockSize - currentBlockSize, ios::beg);
        inputFinal.read(&finalBlock[0], currentBlockSize);

        reverse(finalBlock, &finalBlock[currentBlockSize]);
        finalOutput.write(finalBlock, currentBlockSize);

        tempC -= currentBlockSize;
        iterations++;
        delete[] finalBlock;
    }
    inputFinal.close();
    finalOutput.close();


//   readOutput();
}