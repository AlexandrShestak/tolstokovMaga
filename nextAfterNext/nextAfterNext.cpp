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

static int ELEMENTS_SIZE = 4;
static int N = 10;

void generateInput() {
    ofstream myFile ("input.bin", ios::out | ios::binary);
    myFile.write(reinterpret_cast<const char *>(&N), ELEMENTS_SIZE);

    static const int arr[] = {4, 5, 5, 6, 1, 2, 3, 4, 2, 3, 7, 8, 9, 10, 6, 7, 10, 1, 8, 9};
    for (int i = 0 ; i < N * 2 ; i++) {
        //count<< arr[i] << endl;
        myFile.write(reinterpret_cast<const char *>(&arr[i]), ELEMENTS_SIZE);
    }
    myFile.close();
}

void readOutput(string fileName, int elementsInLineCount) {
    cout << "-------------" << endl;
    std::ifstream output(fileName, ios::in | ios::binary);

    int n;
    output.read(reinterpret_cast<char *>(&n), ELEMENTS_SIZE);
//    count << n << endl;

    for (int i = 0 ; i < N * elementsInLineCount; i++) {
        int temp;
        output.read(reinterpret_cast<char *>(&temp), ELEMENTS_SIZE);
        cout << temp << " ";
        if (i % elementsInLineCount == elementsInLineCount - 1) {
            cout << endl;
        }
    }
    cout << endl << "-------------" << endl;
}


void readOutput1(string fileName, int elementsInLineCount) {
    cout << "-------------" << endl;
    std::ifstream output(fileName, ios::in | ios::binary);

    for (int i = 0 ; i < N * elementsInLineCount; i++) {
        int temp;
        output.read(reinterpret_cast<char *>(&temp), ELEMENTS_SIZE);
        cout << temp << " ";
        if (i % elementsInLineCount == elementsInLineCount - 1) {
            cout << endl;
        }
    }
    cout << endl << "-------------" << endl;
}

long long readFileSize(string fileName) {
    std::ifstream input(fileName, ios::in | ios::binary);
    long long n;
    input.read(reinterpret_cast<char *>(&n), ELEMENTS_SIZE);
    input.close();
    return n;
}

void readBlock(int blockSize, int offset, int* block, string fileName) {
    std::ifstream input(fileName, ios::in | ios::binary);
    input.seekg(ELEMENTS_SIZE + offset * ELEMENTS_SIZE, ios::beg);
    input.read(reinterpret_cast<char *>(block), blockSize * ELEMENTS_SIZE);
    input.close();
}

void writeBlock(int* block, int blockSize, int offset, string fileName) {
//                bool writeFileSize, long long fileSize) {
    ofstream output(fileName, ios::out | ios::binary | ios::app);
//    if (writeFileSize) {
//        output.write(reinterpret_cast<const char *>(&fileSize), sizeof(fileSize));
//    }
    output.seekp(ELEMENTS_SIZE + offset * ELEMENTS_SIZE, ios::beg);
    output.write(reinterpret_cast<const char *>(block), blockSize * ELEMENTS_SIZE);
    output.close();

//    readOutput(fileName);
}

void writeFileSize(int size, string fileName) {
    ofstream output(fileName, ios::out | ios::binary | ios::app);
    output.write(reinterpret_cast<const char *>(&size), ELEMENTS_SIZE);
    output.close();
}

int Compare1(const void* a, const void* b) {
    if (*(int*)a < *(int*)b) {
        return -1;
    } else {
        return 1;
    }
}

int Compare2(const void* a, const void* b) {
    if (*((int*)(a+ELEMENTS_SIZE)) < *((int*)(b+ELEMENTS_SIZE))) {
        return -1;
    } else {
        return 1;
    }
}

void sortBlock1(int* block, int blockSize, int blockNumber, string prefix) {
    qsort(block, static_cast<size_t>(blockSize/2), ELEMENTS_SIZE * 2, Compare1);
    string filename = prefix + "0" + to_string(blockNumber);
    writeFileSize(blockSize, filename);
    writeBlock(block, blockSize, 0, filename);
}

void sortBlock2(int* block, int blockSize, int blockNumber, string prefix) {
    qsort(block, static_cast<size_t>(blockSize/2), ELEMENTS_SIZE * 2, Compare2);
    string filename = prefix + "0" + to_string(blockNumber);
    writeFileSize(blockSize, filename);
    writeBlock(block, blockSize, 0, filename);
}

void sortBlock3(int* block, int blockSize, int blockNumber, string prefix) {
    qsort(block, static_cast<size_t>(blockSize/3), ELEMENTS_SIZE * 3, Compare1);
    string filename = prefix + "0" + to_string(blockNumber);
    writeFileSize(blockSize, filename);
    writeBlock(block, blockSize, 0, filename);
}


int mergeFiles(int startFile, int filesCount, int iteration, int localIteration, int blockSize, int n,
                     vector<int> fileSizes, string prefix, int startPointer) {
    auto ** blocksToMerge = new int*[filesCount];
    auto * filePointers = new int[filesCount];
    auto * globalFilePointers = new int [filesCount];
    int allFilesSize = 0;
    // читаю по блоку их каждого файла
    // для каждого файла ввёл указатель для текущено считанного блока и глобальный указатель по всему файлу
    for (int  i = 0 ; i < filesCount ; i++) {
        string fileName = prefix + to_string(iteration) + to_string(startFile + i);

        int fileSize = fileSizes[i + startFile];
        int currentBlockSize = blockSize;
        if (fileSize < currentBlockSize) {
            currentBlockSize = fileSize;
        }
        auto * blockI = new int[currentBlockSize];
        readBlock(currentBlockSize, 0, blockI, fileName);
        blocksToMerge[i] = blockI;
        filePointers[i] = startPointer;
        globalFilePointers[i] = 0;
        allFilesSize += fileSize;
    }

    auto* sortedBlock = new int[blockSize];
    int sortedBlockPointer = 0;
    int sortedBlockGlobalPointer = 0;

    for (int i = 0 ; i < allFilesSize ; i+=2) {
        int currentMinForAllFiles = 2147483647; // INT MAX
        //int currentMinForAllFiles = 1073741825; // 2^30 +1
        int blockWithMinElement = -1;
        for (int j = 0 ; j < filesCount ; j ++) {
            if (globalFilePointers[j] != fileSizes[j + startFile] &&
                blocksToMerge[j][filePointers[j]] < currentMinForAllFiles) {
                currentMinForAllFiles = blocksToMerge[j][filePointers[j]];
                blockWithMinElement = j;
            }
        }

        if (blockWithMinElement == -1) {
            throw 20;
        }

        if (startPointer == 1) {
            sortedBlock[sortedBlockPointer+1] = currentMinForAllFiles;
            sortedBlock[sortedBlockPointer] = blocksToMerge[blockWithMinElement][filePointers[blockWithMinElement]-1];
        } else {
            sortedBlock[sortedBlockPointer] = currentMinForAllFiles;
            sortedBlock[sortedBlockPointer+1] = blocksToMerge[blockWithMinElement][filePointers[blockWithMinElement]+1];
        }
        sortedBlockPointer += 2;
        if (sortedBlockPointer == blockSize || (sortedBlockGlobalPointer + sortedBlockPointer) == allFilesSize) {
            if (allFilesSize == n * 2) {
                if (sortedBlockGlobalPointer == 0) {
                    writeFileSize(allFilesSize, prefix + "output.bin");
                    writeBlock(sortedBlock, sortedBlockPointer, sortedBlockGlobalPointer, prefix + "output.bin");
                } else {
                    writeBlock(sortedBlock, sortedBlockPointer, sortedBlockGlobalPointer,  prefix + "output.bin");
                }
            } else {
                string fileName =  prefix + to_string(iteration + 1) + to_string(localIteration);
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

        filePointers[blockWithMinElement] += 2;
        globalFilePointers[blockWithMinElement] += 2;
        if (globalFilePointers[blockWithMinElement] != fileSizes[blockWithMinElement + startFile]  &&
            filePointers[blockWithMinElement] == blockSize + startPointer) {

            string fileName = prefix + to_string(iteration) + to_string(startFile + blockWithMinElement);
            int currentBlockSize = blockSize;
            if (fileSizes[blockWithMinElement + startFile] - globalFilePointers[blockWithMinElement] < currentBlockSize) {
                currentBlockSize = fileSizes[blockWithMinElement + startFile] - globalFilePointers[blockWithMinElement];
            }
//            readBlock(currentBlockSize, globalFilePointers[blockWithMinElement], blocksToMerge[blockWithMinElement], fileName);
//            filePointers[blockWithMinElement] = startPointer;
            delete[] blocksToMerge[blockWithMinElement];
            auto * block = new int[currentBlockSize];
            readBlock(currentBlockSize, globalFilePointers[blockWithMinElement], block, fileName);
            blocksToMerge[blockWithMinElement] = block;
            filePointers[blockWithMinElement] = startPointer;
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


int mergeFiles2(int startFile, int filesCount, int iteration, int localIteration, int blockSize, int n,
               vector<int> fileSizes, string prefix) {
    auto ** blocksToMerge = new int*[filesCount];
    auto * filePointers = new int[filesCount];
    auto * globalFilePointers = new int [filesCount];
    int allFilesSize = 0;
    // читаю по блоку их каждого файла
    // для каждого файла ввёл указатель для текущено считанного блока и глобальный указатель по всему файлу
    for (int  i = 0 ; i < filesCount ; i++) {
        string fileName = prefix + to_string(iteration) + to_string(startFile + i);

        int fileSize = fileSizes[i + startFile];
        int currentBlockSize = blockSize;
        if (fileSize < currentBlockSize) {
            currentBlockSize = fileSize;
        }
        auto * blockI = new int[currentBlockSize];
        readBlock(currentBlockSize, 0, blockI, fileName);
        blocksToMerge[i] = blockI;
        filePointers[i] = 0;
        globalFilePointers[i] = 0;
        allFilesSize += fileSize;
    }

    auto* sortedBlock = new int[blockSize];
    int sortedBlockPointer = 0;
    int sortedBlockGlobalPointer = 0;

    for (int i = 0 ; i < allFilesSize ; i+=3) {
        int currentMinForAllFiles = 2147483647; // INT MAX
        //int currentMinForAllFiles = 1073741825; // 2^30 +1
        int blockWithMinElement = -1;
        for (int j = 0 ; j < filesCount ; j ++) {
            if (globalFilePointers[j] != fileSizes[j + startFile] &&
                blocksToMerge[j][filePointers[j]] < currentMinForAllFiles) {
                currentMinForAllFiles = blocksToMerge[j][filePointers[j]];
                blockWithMinElement = j;
            }
        }

        if (blockWithMinElement == -1) {
            throw 20;
        }
        sortedBlock[sortedBlockPointer] = currentMinForAllFiles;
        sortedBlock[sortedBlockPointer+1] = blocksToMerge[blockWithMinElement][filePointers[blockWithMinElement]+1];
        sortedBlock[sortedBlockPointer+2] = blocksToMerge[blockWithMinElement][filePointers[blockWithMinElement]+2];

        sortedBlockPointer += 3;
        if (sortedBlockPointer == blockSize || (sortedBlockGlobalPointer + sortedBlockPointer) == allFilesSize) {
            if (allFilesSize == n * 3) {
                if (sortedBlockGlobalPointer == 0) {
                    //writeFileSize(allFilesSize, "output.bin");
                    writeBlock(sortedBlock, sortedBlockPointer, sortedBlockGlobalPointer, "output.bin");
                } else {
                    writeBlock(sortedBlock, sortedBlockPointer, sortedBlockGlobalPointer,  "output.bin");
                }
            } else {
                string fileName =  prefix + to_string(iteration + 1) + to_string(localIteration);
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

        filePointers[blockWithMinElement] += 3;
        globalFilePointers[blockWithMinElement] += 3;
        if (globalFilePointers[blockWithMinElement] != fileSizes[blockWithMinElement + startFile] &&
            filePointers[blockWithMinElement] == blockSize) {

            string fileName = prefix + to_string(iteration) + to_string(startFile + blockWithMinElement);
            int currentBlockSize = blockSize;
            if (fileSizes[blockWithMinElement + startFile] - globalFilePointers[blockWithMinElement] < currentBlockSize) {
                currentBlockSize = fileSizes[blockWithMinElement + startFile] - globalFilePointers[blockWithMinElement];
            }
            delete[] blocksToMerge[blockWithMinElement];
            auto * block = new int[currentBlockSize];
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
//    remove("output.bin");
//    remove("input.bin");
//    remove("merge_output.bin");
//    generateInput();

    fstream input("input.bin", ios::in | ios::binary);
    int n;
    input.read(reinterpret_cast<char *>(&n), ELEMENTS_SIZE);

    int blockSize = 4000;

    int blockNumber = 0;
    vector<int> fileSizes;
    for (int i = 0 ; i < 2 * n ; i+= blockSize) {
        int currentBlockSize = blockSize;
        if (2*n - i < blockSize) {
            currentBlockSize = 2*n - i;
        }
        auto* block = new int[currentBlockSize];
        readBlock(currentBlockSize, i, block, "input.bin");
        sortBlock1(block, currentBlockSize, blockNumber, string("1_"));
        sortBlock2(block, currentBlockSize, blockNumber, string("2_"));
        delete[] block;
        blockNumber++;
        fileSizes.push_back(currentBlockSize);
    }
    vector<int> initFileSizes = fileSizes;

    int k = 2;
    int iteration = 0;
    int segmentsCount = blockNumber;
    while (true) {
        int processed = 0;
        int localIteration = 0;
        vector<int> newFileSizes;
        while (processed != 2*n) {
            int currentK = k;
            if (segmentsCount < currentK) {
                currentK = segmentsCount;
            }
            int tempProcessed = mergeFiles(localIteration * k, currentK, iteration, localIteration,
                                                 blockSize, n, fileSizes, string("1_"), 0);
            processed += tempProcessed;
            newFileSizes.push_back(tempProcessed);
            if (processed == n * 2 && localIteration == 0) { // за одну итерация отсортировали всё => конец
                //readOutput("1_output.bin", 2);
                goto firstSortEnded;
            }
            segmentsCount -= currentK;
            localIteration++;
        }
        fileSizes = newFileSizes;
        k = min(k, localIteration);
        segmentsCount = localIteration;
        iteration++;
    }
    firstSortEnded:

    k = 2;
    iteration = 0;
    segmentsCount = blockNumber;
    fileSizes = initFileSizes;
    while (true) {
        int processed = 0;
        int localIteration = 0;
        vector<int> newFileSizes;
        while (processed != 2*n) {
            int currentK = k;
            if (segmentsCount < currentK) {
                currentK = segmentsCount;
            }
            int tempProcessed = mergeFiles(localIteration * k, currentK, iteration, localIteration,
                                                 blockSize, n, fileSizes, string("2_"), 1);
            processed += tempProcessed;
            newFileSizes.push_back(tempProcessed);
            if (processed == n * 2 && localIteration == 0) { // за одну итерация отсортировали всё => конец
                //readOutput("2_output.bin", 2);
                goto letsMergeIt;
            }
            segmentsCount -= currentK;
            localIteration++;
        }
        fileSizes = newFileSizes;
        k = min(k, localIteration);
        segmentsCount = localIteration;
        iteration++;
    }

    letsMergeIt:

    blockNumber = 0;
    int processed = 0;
    writeFileSize(n, "merge_output.bin");
    for (int i = 0 ; i < 2 * n ; i+= blockSize) {
        int currentBlockSize = blockSize;
        if (2*n - i < blockSize) {
            currentBlockSize = 2*n - i;
        }
        auto* block1 = new int[currentBlockSize];
        auto* block2 = new int[currentBlockSize];
        auto* block3 = new int[(currentBlockSize/2)*3];
        readBlock(currentBlockSize, i, block1, "1_output.bin");
        readBlock(currentBlockSize, i, block2, "2_output.bin");

        for (int j = 0 ; j < currentBlockSize / 2 ; j++) {
            block3[j*3 ] = block2[j*2];
            block3[j*3 + 1] = block2[j*2 + 1];
            block3[j*3 + 2] = block1[j*2 + 1];
        }

        writeBlock(block3, (currentBlockSize/2)*3, processed, string("merge_output.bin"));

        delete[] block1;
        delete[] block2;
        delete[] block3;
        processed += (currentBlockSize/2)*3;
        blockNumber++;
    }

//    readOutput("merge_output.bin", 3);

//----------------

    blockSize = blockSize / 2 *3;
    vector<int> finalFileSizes;
    blockNumber = 0;
    for (int i = 0 ; i < 3 * n ; i+= blockSize) {
        int currentBlockSize = blockSize;
        if (3*n - i < blockSize) {
            currentBlockSize = 3*n - i;
        }
        auto* block = new int[currentBlockSize];
        readBlock(currentBlockSize, i, block, "merge_output.bin");
        sortBlock3(block, currentBlockSize, blockNumber, string("3_"));
        delete[] block;
        blockNumber++;
        finalFileSizes.push_back(currentBlockSize);
    }

    k = 2;
    iteration = 0;
    segmentsCount = blockNumber;
    while (true) {
        int processed = 0;
        int localIteration = 0;
        vector<int> newFileSizes;
        while (processed != 3*n) {
            int currentK = k;
            if (segmentsCount < currentK) {
                currentK = segmentsCount;
            }
            int tempProcessed = mergeFiles2(localIteration * k, currentK, iteration, localIteration,
                                           blockSize, n, finalFileSizes, string("3_"));
            processed += tempProcessed;
            newFileSizes.push_back(tempProcessed);
            if (processed == n * 3 && localIteration == 0) { // за одну итерация отсортировали всё => конец
                //readOutput1("output.bin", 3);
                return 0;
            }
            segmentsCount -= currentK;
            localIteration++;
        }
        finalFileSizes = newFileSizes;
        k = min(k, localIteration);
        segmentsCount = localIteration;
        iteration++;
    }
}