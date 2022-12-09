#include <x86intrin.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <immintrin.h>
// #include <avx2intrin.h>

using namespace std::chrono;

void write_memory_loop(void* array, int64_t size) {
    int64_t* carray = (int64_t*) array;
    int64_t i;

    auto start = high_resolution_clock::now();

// #pragma GCC unroll 16384
#pragma omp paralell for
    for (i = 0; i < size / sizeof(int64_t); i++) {
        carray[i] = 6;
    }

    auto end = high_resolution_clock::now();

    double time = duration_cast< duration<double> >(end - start).count();
    double bw = (size >> 30) / time;
    printf("Naive write BW of 1GB: elapsed time %.3f s, calculated bandwidth %.3f GB/s\n", time, bw);
}

void write_memory_avx(void* array, size_t size) {
    __m256i* varray = (__m256i*) array;

    __m256i value = _mm256_set1_epi32(1);
    size_t i;

    auto start = high_resolution_clock::now();

#pragma omp paralell for
    for (i = 0; i < size / sizeof(__m256i); i++) {
        _mm256_stream_si256(&varray[i], value);
    }

    auto end = high_resolution_clock::now();

    double time = duration_cast< duration<double> >(end - start).count();
    double bw = (size >> 30) / time;
    printf("AVX write BW of 1GB: elapsed time %.3f s, calculated bandwidth %.3f GB/s\n", time, bw);
}

void read_memory_avx(void* array, size_t size) {
    __m128i* const varray = (__m128i*) array;
    // __m128i value = _mm256_set1_epi32(32);
    __m128i value = _mm_set1_epi32(32);
    size_t i;

    auto start = high_resolution_clock::now();
    for (i = 0; i < size / sizeof(__m128i); i++) {
        // value += _mm256_stream_load_si256(&varray[i]);;  // This will generate the vmovaps instruction.
        value += _mm_stream_load_si128(&varray[i]);;  // This will generate the vmovaps instruction.
    }
    auto end = high_resolution_clock::now();

    double time = duration_cast< duration<double> >(end - start).count();
    double bw = (size >> 20) / time;
    printf("AVX read BW of 1GB: elapsed time %.3f s, calculated bandwidth %.3f MB/s\n", time, bw);
}

void write_memory_rep_stosq(void* buffer, size_t size) {
    asm("cld\n"
        "rep stosq"
        : : "D" (buffer), "c" (size / 8), "a" (0) );
}

int main() {
    // printf("size_t %d", sizeof(size_t));
    size_t size = 4<<20;
    char* char_array = (char*) malloc(size);
    for (size_t i = 0; i < size; i++) {
        char_array[i] = i % 255;
    }

    __m256i* avx_array = new __m256i[(1 << 30) / 32];
    // std::this_thread::sleep_for(seconds(5));
    printf("Allocated array of %ld GB\n", (size >> 30));

    // uint64_t start = __rdtsc();
    // write_memory_loop(avx_array, size);
    read_memory_avx(avx_array, size);
    // write_memory_avx(char_array, size);

}