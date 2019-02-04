#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;

using llong = long long;

void generate_input(llong N, llong * matrix);
void read_and_print(char *filename);

void read_block(llong *block, llong offset, llong block_size, char *filename);
void write_block(llong *block, llong offset, llong block_size, char *filename);
void print_sequence(llong * seq, llong size);


void external_merge_sort(llong start, llong end);
void external_merge(llong start, llong half, llong end);

const llong OFFSET = 8;
const llong BASE_BLOCK_SIZE = 1000;
llong BLOCK_SIZE;

char temp_file[] = "temp.bin";


int main() {

    remove("input.bin");
    remove("output.bin");
    remove("temp.bin");
    llong N0 = 1000000;
    llong * matrix = new long long[N0];
    generate_input(N0, matrix);

    //read_and_print("input.bin");


    llong N;
    ifstream input("input.bin", std::ios::binary | std::ios::in);
    input.read(reinterpret_cast<char*>(&N), sizeof(N));
    input.close();

    ofstream output("output.bin", std::ios::binary | std::ios::out);
    output.write(reinterpret_cast<char*>(&N), sizeof(N));
    output.close();

    ofstream output_temp(temp_file, std::ios::binary | std::ios::out);
    output_temp.write(reinterpret_cast<char*>(&N), sizeof(N));
    output_temp.close();

    BLOCK_SIZE = std::min(BASE_BLOCK_SIZE, N);

    external_merge_sort(0, N);

//    read_and_print("output.bin");


    return 0;
}

void read_block(llong *block, llong offset, llong block_size, char *filename) {
    ifstream input(filename, std::ios::binary | std::ios::in);
    input.seekg(OFFSET + offset * sizeof(llong));
    input.read(reinterpret_cast<char*>(block), block_size * sizeof(llong));
    input.close();
}

void write_block(llong *block, llong offset, llong block_size, char *filename) {
    ofstream output(filename, std::ios::binary | std::ios::out | std::ios::in);
    output.seekp(OFFSET + offset * sizeof(llong), std::ios::beg);
    output.write(reinterpret_cast<char*>(block), block_size * sizeof(llong));
    output.close();
}

// end is +1 to last element index
void external_merge_sort(llong start, llong end) {

    if(end - start <= BLOCK_SIZE){
        llong block_size = end - start;
        llong * block = new long long[block_size];
        read_block(block, start, block_size, "input.bin");

        std::sort(block, block + block_size);

//        for(int q = 0; q < block_size; q++){
//            cout << block[q] << " ";
//        }
//        cout << endl;

        write_block(block, start, block_size, "output.bin");

        delete [] block;
        //cout << endl << "start: " << start << ", end: " << end << ", length: " << end - start << endl << endl;

        return;
    }

    int half = (start + end) / 2;


    external_merge_sort(start, half);
    external_merge_sort(half, end);

    external_merge(start, half, end);
    //cout << endl << "start: " << start << ", end: " << end << ", length: " << end - start << endl << endl;


}

void external_merge(llong start, llong half, llong end){
    llong left_ind = start;
    llong right_ind = half;
    llong merged_ind = start;
    llong left_block_ind = BLOCK_SIZE, right_block_ind = BLOCK_SIZE;
    llong merged_block_ind = 0;
    llong merged_write_offset = start;

    llong * left_block = new long long [BLOCK_SIZE];
    llong * right_block = new long long [BLOCK_SIZE];
    llong * merged = new long long [BLOCK_SIZE];

    while(left_ind < half || right_ind < end){

        // not enough data, take from file
        if(left_block_ind == BLOCK_SIZE && left_ind < half){
            read_block(left_block, left_ind, BLOCK_SIZE, "output.bin");
//            cout << "left block: " << endl;
//            print_sequence(left_block, BLOCK_SIZE);
            left_block_ind = 0;
        }
        if(right_block_ind == BLOCK_SIZE && right_ind < end){
            read_block(right_block, right_ind, BLOCK_SIZE, "output.bin");
//            cout << "right block: " << endl;
//            print_sequence(right_block, BLOCK_SIZE);
            right_block_ind = 0;
        }

        // merge
        if(left_ind < half && right_ind < end){
            llong a_left = left_block[left_block_ind];
            llong a_right = right_block[right_block_ind];
            if(a_left < a_right){
                merged[merged_block_ind] = a_left;
                left_block_ind++;
                left_ind++;
            }
            else {
                merged[merged_block_ind] = a_right;
                right_block_ind++;
                right_ind++;
            }
        } else if(left_ind < half) {
            merged[merged_block_ind] = left_block[left_block_ind];
            left_block_ind++;
            left_ind++;
        } else if(right_ind < end){
            merged[merged_block_ind] = right_block[right_block_ind];
            right_block_ind++;
            right_ind++;
        }
        merged_ind++;
        merged_block_ind++;


//        cout << "merge step: " << endl;
//        print_sequence(merged, BLOCK_SIZE);
//
//        cout << "merged_block_ind: " << merged_block_ind << ", merged_ind: "
//             << merged_ind << ", merged_write_offset: " << merged_write_offset << endl;

        // flush data if necessary
        if(merged_block_ind == BLOCK_SIZE || merged_ind == end){
//            cout << "merged_ind at flush: " << merged_ind << endl;
            llong merged_block_size = std::min(BLOCK_SIZE, end - merged_write_offset);
            write_block(merged, merged_write_offset, merged_block_size, temp_file);
            merged_block_ind = 0;
            merged_write_offset += merged_block_size;
//            cout << "partly merged: " << endl;
//            read_and_print(temp_file);
        }
    }

    for(llong k = start; k < end; k += BLOCK_SIZE){
        llong block_size = std::min(BLOCK_SIZE, end - k);
        llong * copy_from_temp = new long long [block_size];
        read_block(copy_from_temp, k, block_size, temp_file);
        write_block(copy_from_temp, k, block_size, "output.bin");
        delete [] copy_from_temp;
    }


//    cout << "partly merged output.bin: " << endl;
//    read_and_print("output.bin");

    delete [] left_block;
    delete [] right_block;
    delete [] merged;
}

std::vector<std::string> create_file_names(llong K){
    auto file_names = std::vector<std::string>(K);
    for(llong i = 0; i < K; i++){
        std::stringstream name;
        name << "temp" << i << ".bin";
        file_names[i] = std::string(name.str());
    }

    return file_names;
}


void generate_input(llong N, llong * matrix){
    std::srand(unsigned(std::time(0)));
    const llong MODULE = 10;

    for(llong i = 0; i < N; i++){
        matrix[i] = /*std::rand() % MODULE*/N - i;
    }

    ofstream output("input.bin", std::ios::binary | std::ios::out);
    output.write(reinterpret_cast<char*>(&N), sizeof(N));
    output.write(reinterpret_cast<char*>(matrix), N*sizeof(llong));
    output.close();
}

void read_and_print(char *filename) {
    llong N;
    ifstream input(filename, std::ios::binary | std::ios::in);
    input.read(reinterpret_cast<char*>(&N), sizeof(N));
    cout << N << endl;

    for(llong i = 0; i < N; i++){
        llong elem;
        input.read(reinterpret_cast<char*>(&elem), sizeof(elem));
        cout << elem << " ";
    }
    cout << endl;

    input.close();
}

void print_sequence(llong * seq, llong size){
    for(llong i = 0; i < size; i++){
        cout << seq[i] << " ";
    }
    cout << endl;
}