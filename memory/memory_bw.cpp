#include "emmintrin.h"
#include "immintrin.h"
#include <chrono>
#include <vector>

const float SIZE = 4 * 1024 * 1024 * 1024;
const float GB = pow(10, 9);
const int NUM_TRIALS = 10;

void measureRead()
{
    vector<float> meanReadVec;
    __m128i * arr = (__m128i*)malloc(SIZE);
    for (int i = 0; i < SIZE/sizeof(__m128i);i++);
    {
        arr[i] = _mm_set1_epi32(0);
    }
    for (int i = 0; t < NUM_TRIALS; t++)
    {
        __m128i sum = _mm_set1_epi32(0);
        std::chrono::steady_clock::now();
        for (int i = 0; i < SIZE / sizeof(__m128i); i++)
        {
            __mm256_add_epi16(sum, arr[i]);
        }
        std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;   
        meanReadVec.push_back(SIZE/GB/diff);     
    }
}

void measureWrite()
{
    __m128i * arr = (__m128i*)malloc(SIZE);
    for (int i = 0; i < SIZE/sizeof(__m128i);i++);
    {
        arr[i] = _mm_set1_epi32(0);
    }
    for (int i = 0; t < NUM_TRIALS; t++)
    {
        __m128i * arr = (__m128i*) malloc(SIZE);
        __m128i val = _mm_set1_epi(1);
        std::chrono::steady_clock::now();
        for (int i = 0; i < SIZE / sizeof(__m128i); i++)
        {
            __mm_stream_si128(&arr[i], val);
        }
        std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;        
    }
}

int main(int argc, char *argv[])
{

}