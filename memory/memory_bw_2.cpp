#include "emmintrin.h"
#include "immintrin.h"
#include <chrono>
#include <vector>
#include <iostream>
#include <cmath> // for sqrt, pow
#include <numeric> // for std calculation
#include <algorithm> // for tranform
#include <string.h> // memset

const float SIZE = 4<<20;
const float GB = pow(10, 9);
const int NUM_TRIALS = 10;

using namespace std;

float findMeanSD(vector<float> &v)
{
    // code from https://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();
    std::vector<double> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / v.size());
    cout << "Mean: ";
    cout << mean << endl;
    cout << "SD: ";
    cout << stdev << endl;
    return mean;
}

void measureRead(__m256i* arr)
{
    vector<float> meanReadVec;
    for (int t = 0; t < NUM_TRIALS; t++)
    {
        // __m256i * arr = (__m256i *) malloc(SIZE);
        // memset(arr, 0, SIZE/sizeof(__m256i));
        __m256i sum = _mm256_set1_epi32(0);
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < SIZE / sizeof(__m256i); i++)
        {
            _mm256_add_epi16(sum, arr[i]);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;   
        meanReadVec.push_back(SIZE/GB/diff.count());   
        free(arr);  
    }
    findMeanSD(meanReadVec);

}

__m256i* measureWrite()
{
    vector<float> meanWriteVec;
    __m256i* ret;
    for (int t = 0; t < NUM_TRIALS; t++)
    {
        __m256i * arr = new __m256i[(int) SIZE / sizeof(__m256i)];
        memset(arr, 0, SIZE/sizeof(__m256i));
        __m256i val = _mm256_set1_epi32(1);
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < SIZE / sizeof(__m256i); i++)
        {
            _mm256_stream_si256((__m256i*) &arr[i], val);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;  
        meanWriteVec.push_back(SIZE/GB/diff.count());   
        if (t == NUM_TRIALS - 1) {
            break;
        }
        free(arr);
    }
    findMeanSD(meanWriteVec);
    return ret;
}

int main(int argc, char *argv[])
{
    cout << "mean write bandwidth (GB/s)";
    __m256i* vec_array = measureWrite();
    cout << "mean read bandwidth (GB/s)";
    measureRead(vec_array);
}