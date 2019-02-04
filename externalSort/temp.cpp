#include <fstream>
#include <iostream>
#include <string>
#include <limits.h>
#include <algorithm>
#include <stdlib.h>
#include <queue>
#include <time.h>
#include <vector>

using namespace std;

static int ELEMENTS_SIZE = 8;

void generateInput() {
    ofstream myFile ("input.bin", ios::out | ios::binary);
    long long n =42;
    myFile.write(reinterpret_cast<const char *>(&n), ELEMENTS_SIZE);

    static const long long arr[] = {20, 19, 18, 17, 16, 15, 14 ,13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
                                    0, 1, 2, 3, 4, 5, 6, 7 ,8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                    20, 19, 18, 17, 16, 15, 14 ,13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
                                    0, 1, 2, 3, 4, 5, 6, 7 ,8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                    5, 3, 4, 2, 1, 1, 1152921504606847001 ,12, 2, 45, 0, 9, 11, 3, 100, 1, 7, 77, 1,0,
                                    5, 3, 4, 2, 1, 1, 2 ,12, 2, 45, 0, 9, 11, 3, 100, 1, 7, 77, 1,0,
                                    5, 3, 1152921504606847001, 2, 1, 1, 2 ,12, 2, 45, 0, 9, 11, 3, 100, 1, 7, 77, 1,0};

//    auto* arr = new long long[n];
//
//    srand(time(NULL));
//    for (long long  i = 1280000; i >= 0 ; i--) {
//        arr[i] = rand() % 1280000;
//    }
//    random_shuffle(&arr[0], &arr[n]);

    for (int i = 0 ; i < n ; i++) {
        cout<< arr[i] << endl;
        myFile.write(reinterpret_cast<const char *>(&arr[i]), ELEMENTS_SIZE);
    }
    myFile.close();
}

void readOutput(string fileName) {
    cout << "-------------" << endl;
    std::ifstream output(fileName, ios::in | ios::binary);

    long long n;
    output.read(reinterpret_cast<char *>(&n), ELEMENTS_SIZE);
    cout << n << endl;

    for (int i = 0 ; i < n ; i++) {
        if (i % 1000 == 0) {
            cout << endl;
        }
        long long temp;
        output.read(reinterpret_cast<char *>(&temp), ELEMENTS_SIZE);
        cout << temp << " ";
    }

//    for (int i = 0 ; i < 10000 ; i++) {
//        if (i % 1000 == 0) {
//            cout << endl;
//        }
//        long long temp;
//        output.read(reinterpret_cast<char *>(&temp), sizeof(temp));
//        cout << temp << " ";
//    }
    cout << endl << "-------------" << endl;
}


long long readFileSize(string fileName) {
    std::ifstream input(fileName, ios::in | ios::binary);
    long long n;
    input.read(reinterpret_cast<char *>(&n), ELEMENTS_SIZE);
    input.close();
    return n;
}

void readBlock(long long blockSize, long long offset, long long* block, string fileName) {
    std::ifstream input(fileName, ios::in | ios::binary);
    input.seekg(ELEMENTS_SIZE + offset * ELEMENTS_SIZE, ios::beg);
    input.read(reinterpret_cast<char *>(block), blockSize * ELEMENTS_SIZE);
    input.close();
}

void writeBlock(long long* block, long long blockSize, long long offset, string fileName) {
    ofstream output(fileName, ios::out | ios::binary | ios::app);
    output.seekp(ELEMENTS_SIZE + offset * ELEMENTS_SIZE, ios::beg);
    output.write(reinterpret_cast<const char *>(block), blockSize * ELEMENTS_SIZE);
    output.close();
}

void writeFileSize(long long size, string fileName) {
    ofstream output(fileName, ios::out | ios::binary | ios::app);
    output.write(reinterpret_cast<const char *>(&size), ELEMENTS_SIZE);
    output.close();
}

int Compare(const void* a, const void* b) {
    if (*(long long*)a < *(long long*)b) {
        return -1;
    } else {
        return 1;
    }
}

void sortBlock(long long* block, long long blockSize, long long blockNumber) {
    qsort(block, blockSize, ELEMENTS_SIZE, Compare);
    string filename = "0" + to_string(blockNumber);
    writeFileSize(blockSize, filename);
    writeBlock(block, blockSize, 0, filename);
}

long long mergeFiles(int startFile, int filesCount, int iteration, int localIteration, int blockSize, long long n,
                     vector<long long> fileSizes) {
    auto ** blocksToMerge = new long long*[filesCount];
    //auto * fileSizes = new long long [filesCount];
    auto * filePointers = new long long [filesCount];
    auto * globalFilePointers = new long long [filesCount];
    long long allFilesSize = 0;
    // читаю по блоку их каждого файла
    // для каждого файла ввёл указатель для текущено считанного блока и глобальный указатель по всему файлу
    for (int  i = 0 ; i < filesCount ; i++) {
        string fileName = to_string(iteration) + to_string(startFile + i);
        long long fileSize = fileSizes[i + startFile];
        long long currentBlockSize = blockSize;
        if (fileSize < currentBlockSize) {
            currentBlockSize = fileSize;
        }
        auto * blockI = new long long[currentBlockSize];
        readBlock(currentBlockSize, 0, blockI, fileName);
        blocksToMerge[i] = blockI;
        filePointers[i] = 0;
        globalFilePointers[i] = 0;
        allFilesSize += fileSize;
    }

    auto* sortedBlock = new long long[blockSize];
    long long sortedBlockPointer = 0;
    long long sortedBlockGlobalPointer = 0;

    for (int i = 0 ; i < allFilesSize ; i++) {
        long long currentMinForAllFiles = 1152921504606847001; // 2^60 +1
        int blockWithMinElement = -1;
        for (int j = 0 ; j < filesCount ; j ++) {
            if (globalFilePointers[j] != fileSizes[j] &&
                blocksToMerge[j][filePointers[j]] < currentMinForAllFiles) {
                currentMinForAllFiles = blocksToMerge[j][filePointers[j]];
                blockWithMinElement = j;
            }
        }

        sortedBlock[sortedBlockPointer] = currentMinForAllFiles;
        sortedBlockPointer++;
        if (sortedBlockPointer == blockSize || (sortedBlockGlobalPointer + sortedBlockPointer) == allFilesSize) {
            if (allFilesSize == n) {
                if (sortedBlockGlobalPointer == 0) {
                    writeFileSize(allFilesSize, "output.bin");
                    writeBlock(sortedBlock, sortedBlockPointer, sortedBlockGlobalPointer, "output.bin");
                } else {
                    writeBlock(sortedBlock, sortedBlockPointer, sortedBlockGlobalPointer, "output.bin");
                }
            } else {
                string fileName = to_string(iteration + 1) + to_string(localIteration);
                if (sortedBlockGlobalPointer == 0) {
                    writeFileSize(allFilesSize, fileName);
                    writeBlock(sortedBlock, sortedBlockPointer, sortedBlockGlobalPointer, fileName);
                } else {
                    writeBlock(sortedBlock, sortedBlockPointer, sortedBlockGlobalPointer, fileName);
                }
            }
            sortedBlockGlobalPointer += sortedBlockPointer;
            sortedBlockPointer = 0;
        }

        filePointers[blockWithMinElement] += 1;
        globalFilePointers[blockWithMinElement] += 1;
        if (globalFilePointers[blockWithMinElement] != fileSizes[blockWithMinElement]  &&
            filePointers[blockWithMinElement] == blockSize) {
            string fileName = to_string(iteration) + to_string(startFile + blockWithMinElement);
            long long currentBlockSize = blockSize;
            if (fileSizes[blockWithMinElement] - globalFilePointers[blockWithMinElement] < currentBlockSize) {
                currentBlockSize = fileSizes[blockWithMinElement] - globalFilePointers[blockWithMinElement];
            }
//            readBlock(currentBlockSize, globalFilePointers[blockWithMinElement], blocksToMerge[blockWithMinElement], fileName);
//            filePointers[blockWithMinElement] = 0;

            delete[] blocksToMerge[blockWithMinElement];
            auto * block = new long long[currentBlockSize];
            readBlock(currentBlockSize, globalFilePointers[blockWithMinElement], block, fileName);
            blocksToMerge[blockWithMinElement] = block;
            filePointers[blockWithMinElement] = 0;
        }
    }

    //delete[] fileSizes;
    delete[] filePointers;
    delete[] globalFilePointers;
    for (int i = 0 ; i < filesCount ; i++) {
        delete[] blocksToMerge[i];
    }
    delete[] blocksToMerge;
    delete[] sortedBlock;
    return allFilesSize;
}

int main(int argc, char* argv[]) {
    remove("output.bin");
    remove("input.bin");
    generateInput();
//    clock_t tStart = clock();
    fstream input("input.bin", ios::in | ios::binary);
    long long n;
    input.read(reinterpret_cast<char *>(&n), ELEMENTS_SIZE);

    int blockSize = 5000;

    int blockNumber = 0;
    vector<long long> fileSizes;
    for (int i = 0 ; i < n ; i+= blockSize) {
        long long currentBlockSize = blockSize;
        if (n - i < blockSize) {
            currentBlockSize = n - i;
        }
        auto* block = new long long[currentBlockSize];
        readBlock(currentBlockSize, i, block, "input.bin");
        sortBlock(block, currentBlockSize, blockNumber);
        delete[] block;
        blockNumber++;
        fileSizes.push_back(currentBlockSize);
    }

    int k = 4;
    int iteration = 0;
    int segmentsCount = blockNumber;
    while (true) {
        long long processed = 0;
        int localIteration = 0;
        vector<long long> newFileSizes;
        while (processed != n) {
            int currentK = k;
            if (segmentsCount < currentK) {
                currentK = segmentsCount;
            }
            long long tempProcessed = mergeFiles(localIteration * k, currentK, iteration, localIteration, blockSize, n, fileSizes);
            processed += tempProcessed;
            newFileSizes.push_back(tempProcessed);
            if (processed == n && localIteration == 0) { // за одну итерация отсортировали всё => конец
//                readOutput("output.bin");
//                printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
                //Time taken: 1.34s
                //Time taken: 0.35s
                //cout << endl << iteration;
                return 0;
            }
            segmentsCount -= currentK;
            localIteration++;
        }
        fileSizes = newFileSizes;
        k = min(k, localIteration);
        segmentsCount = localIteration;
        iteration++;
    }
    //readOutput("input.bin");
}