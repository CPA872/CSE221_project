#include <stdio.h>
#include <x86intrin.h>
#include <stdlib.h>
#include <vector>

// #include "milestone.cpp"

#define ITERATIONS 1000




void reading_time() {
    // vector<float> vec;
    int i = 0;
    __uint64_t start, end;
    __uint64_t total = 0;
    __uint64_t elapsed;
    for (i = 0; i < ITERATIONS; i++) {
        start = __rdtsc();
        total += __rdtsc() - start;
        // vec.push_back(elapsed);
    }

    double average = (double) total / ITERATIONS;
    // findMeanSD(vec);

    printf("rdtsc() overhead: %.2lf cycles\n", average);
}

void for_overhead() {
    int i = 0;

    __uint64_t start = __rdtsc();
    for (i = 0; i < ITERATIONS; i++);
    __uint64_t total = __rdtsc() - start;
    double average = (double) total / ITERATIONS;

    printf("average for loop overhead: %.2lf cycles\n", average);
}

void do_while_overhead() {
    int i = 0;

    __uint64_t start = __rdtsc();
    do {
        i++;
    } while (i < ITERATIONS);
    __uint64_t total = __rdtsc() - start;
    double average = (double) total / ITERATIONS;
    
    printf("average do-while loop overhead: %.2lf cycles\n", average);
}

int main() {
    reading_time();
    for_overhead();
    do_while_overhead();
    return 0;
}