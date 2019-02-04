#include <fstream>
#include <iostream>
#include <algorithm>
#include <stack>
#include <cstdlib>
#include <ctime>


using namespace std;

void generateInput() {
    ofstream myFile ("input.bin", ios::out | ios::binary);
    int n = 100;
    int m = 11;
    myFile.write(reinterpret_cast<const char *>(&n), sizeof(n));
    myFile.write(reinterpret_cast<const char *>(&m), sizeof(m));

    static const int arr1[] = {2,1, 2,3, 2,11, 10,11, 11,12, 11,15, 4,12, 12,13, 8,14, 7,14, 5,6 };

    //srand(time(NULL));
    for (int  i = 0 ; i < m * 2 ; i++) {

        int val = rand() % n +1;
        myFile.write(reinterpret_cast<const char *>(&arr1[i]), sizeof(arr1[i]));
        //myFile.write(reinterpret_cast<const char *>(&val), sizeof(val));
    }

    myFile.close();
}


void readOutput() {
    std::ifstream output("output.bin", ios::in | ios::binary);
    int answer;
    output.read(reinterpret_cast<char *>(&answer), sizeof(answer));
    cout << answer;
}

class DSU {
public:
    DSU(int n) {
        srand(time(NULL));
        parent = new int[n+1];
//        size = new int[n+1];
        for (int i = 1 ; i <= n ; i++) {
            parent[i] = i;
//            size[i] = 1;
        }
        count = n;
    }

    ~DSU() {
        delete[] parent;
    }

    int find_set(int v) {
        stack<int> parentsStack;
        while (true) {
            parentsStack.push(v);
            if (v == parent[v]) {
                while (!parentsStack.empty()) {
                    int temp = parentsStack.top();
                    parentsStack.pop();
                    parent[temp] = v;
                }
                return v;
            }
            v = parent[v];
        }
//        if (v == parent[v])
//            return v;
//        return parent[v] = find_set(parent[v]);
    }

    void union_sets(int v1, int v2) {
        v1 = find_set(v1);
        v2 = find_set(v2);
        if (v1 != v2) {
            if (rand() % 2) {
                swap(v1, v2);
            }
            parent[v1] = v2;
            count--;

//            if (size[v1] > size[v2])
//                swap(v1, v2);
//            parent[v1] = v2;
//            size[v2] += size[v1];
////            parent[v1] = v2;
//            count--;
        }
    }

    int getCount() {
        return count;
    }

private:
//    int* size;
    int* parent;
    int count;
};

int main(int argc, char* argv[]) {
    //generateInput();

    fstream input("input.bin", ios::in | ios::binary);
    int n, m;

    input.read(reinterpret_cast<char *>(&n), 4);
    input.read(reinterpret_cast<char *>(&m), 4);
    int blockSize = 10000;
    if (2 * m  < blockSize) {
        blockSize = 2 * m;
    }

    DSU* dsu = new DSU(n);

    int currentBlockSize = blockSize;
    for (int i = 0 ; i < 2*m ; i+= blockSize) {
        if (2*m - i < currentBlockSize) {
            currentBlockSize = 2*m - i;
        }
        auto *block = new int[currentBlockSize];
        input.read(reinterpret_cast<char *>(&block[0]), currentBlockSize * 4);
        for (int j = 0 ; j < currentBlockSize ; j += 2) {
            int v1 = block[j];
            int v2 = block[j + 1];
            if (v1 != v2) {
                dsu->union_sets(v1, v2);
            }
        }
        delete[] block;
    }
    input.close();

    int count = dsu->getCount();
    ofstream output("output.bin", ios::out | ios::binary | ios::app);
    output.write(reinterpret_cast<const char *>(&count), 4);
    output.close();

    delete dsu;
    return 0;
}