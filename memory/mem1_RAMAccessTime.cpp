#include <cstring>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>
#include <x86intrin.h> // for rdtsc
#include <unistd.h>

#define MEGA 1024 * 1024
#define KILO 1024
#define NUM_LOADS 1000000
#define NUM_TRAILS 5
#define ARRAY_MB  1024

/* 
 * size: size of the desired access region
 *       therefore the number of ints is size / 4
 * stride_size: size of stride in bytes
 *       therefore the stride in num of ints is stride_size / 4
 */
double report_time(uint64_t size, uint64_t stride_size) {
    // length of access region in number of integers
    int length = size / sizeof(int);
    int stride = stride_size / sizeof(int);
    // std::cout << stride << "=stride size\n";

    int* array = (int*) malloc(length * sizeof(int));
    memset(array, 0, length * sizeof(int));

    uint64_t start, end;
    int index = 0;
    // fill the int array with indices to access next
    for (int i = 0; i < length; i++) {
        index = (i + stride) % length;
        array[i] = index;
        // sanity check that we don't produce weird index
        if (index < 0 || index > length) {
            std::cout << "!!!\n";
        }
    }

    int register value = 0;

    start = __rdtsc();
    for (int i = 0; i < NUM_LOADS; i++) {
        value = array[value];
        // std::cout << value << "\n";
    }
    end = __rdtsc();

    double average = (end - start) / (double) NUM_LOADS;
    free(array);

    // printf("Average cycles %.2f\n", average);
    return average;
}

int main() {
    // log2 sizes of arrays to be tested
    nice(-20);

    std::cout << "RAM Access Time for different sizes and strides\n";
    std::cout << "Note: Results are in cycles\n";
    std::cout << "\nStrides: 4, 8, 16, 32, 64, 128, 256 Bytes\n";
    
    
    int sizes[16]  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 16, 32, 64, 128, 256, 512};
    int strides[7] = {4, 8, 16, 32, 64, 128, 256};
    int stride;
    
    for (int i = 0; i < 16; i++) {
        std::cout << sizes[i] << "KB, ";

        for (int s = 0; s < 7; s++) {
            double average = 0.0;
            stride = strides[s];
            // std::cout << "   stride " << stride << " Bytes ";
            for (int t = 0; t < NUM_TRAILS; t++)
                average += report_time(sizes[i] << 10, stride);
            average /= NUM_TRAILS;
            printf("%.2f, ", average);
        }
        std::cout << "\n";
    }

    for (int i = 0; i < 16; i++) {
        // int size = 512;
        // int size = 4;
        int size = sizes[i];
        double average = 0.0;
        std::cout << size << "MB, ";
        for (int s = 0; s < 7; s++) {
            double average = 0.0;
            stride = strides[s];
            // std::cout << "   stride " << stride << " Bytes ";
            for (int t = 0; t < NUM_TRAILS; t++)
                average += report_time(size << 20, stride);
            average /= NUM_TRAILS;
            printf("%.2f, ", average);
        }
        std::cout << "\n";
    }

    {
        double average = 0.0;
        std::cout << "1 GB, ";
        for (int s = 0; s < 7; s++) {
            double average = 0.0;
            stride = strides[s];
            // std::cout << "   stride " << stride << " Bytes ";
            for (int t = 0; t < NUM_TRAILS; t++)
                average += report_time(1 << 30, stride);
            average /= NUM_TRAILS;
            printf("%.2f, ", average);
        }
        std::cout << "\n";
    }

    std::cout << "Done!\n";
    return 0;
}

