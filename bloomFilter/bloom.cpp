#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

// https://habrahabr.ru/post/112069/
class BloomFilter {
private:
    std::vector<bool> bits;
    unsigned int* seeds;
    int bitsCount;
    int hashesCount;

public:
    BloomFilter(int bitsCountVar, int hashesCountVar);

    ~BloomFilter() {
//        delete[] bits;
        delete[] seeds;
    }

    void add(char* text) {
        for (int i = 0 ; i < hashesCount ; i++) {
            int hash = hashFunction(text, seeds[i]);
            hash = (hash  % bitsCount + bitsCount) % bitsCount;
            bits[hash] = true;
//            bits[hash/32] = 1 << (hash % 32);
        }
    }

    bool contains(char* text) {
        for (int i = 0 ; i < hashesCount ; i++) {
            int hash = hashFunction(text, seeds[i]);
            hash = (hash  % bitsCount + bitsCount) % bitsCount;
            //if (!(bits[hash / 32] & 1 << (hash % 32))) {
            if (!bits[hash]) {
                return false;
            }
        }
        return true;
    }

    unsigned int hashFunction(char *str, unsigned int seed) {
        unsigned int result = 1;
        for (int i = 0 ; i < strlen(str) ; i++) {
            result = (seed * result + (unsigned int)str[i]) % 2147483647;
        }
        return result;
    }
};

BloomFilter::BloomFilter(int bitsCountVar, int hashesCountVar):
        bits(bitsCountVar, false)
{

        hashesCount = hashesCountVar;
        bitsCount = bitsCountVar;
        //bits = new int[bitsCountVar/32 +1];
        srand(time(NULL));

        seeds = new unsigned int[hashesCount];
//        default_random_engine generator;
//        uniform_int_distribution<unsigned int> distribution(1, 4294967295);
//
//        for (int i = 0 ; i < hashesCount ; i++) {
//            seeds[i] = distribution(generator);
//            //seeds[i] = rand();
//            cout << seeds[i] << endl;
//        }
        seeds[0] = 282475249;
        seeds[1] = 2617694918;
        seeds[2] = 1457850878;
        seeds[3] = 3262921811;
        seeds[4] = 1137522503;
        seeds[5] = 2291026258;
        seeds[6] = 3622316815;

}

int main(int argc, char* argv[]) {
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    int N;
    cin >> N;

    //BloomFilter bloomFilter = BloomFilter(5751035, 7);
    BloomFilter bloomFilter = BloomFilter(6000000, 7);
    for (int i = 0 ; i < N ; i++) {
        char* tempStr = new char[30];
        int operation;
        cin >> operation;
        cin >> tempStr;
        if (operation == 0) {
            cout << bloomFilter.contains(tempStr);
        } else {
            bloomFilter.add(tempStr);
        }
        delete[] tempStr;
    }
    cin.close();
    cout.close();

    return 0;
}