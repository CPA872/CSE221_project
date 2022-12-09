#include "emmintrin.h"
#include "immintrin.h"
#include <chrono>
#include <vector>
#include <iostream>
#include <cmath> // for sqrt, pow
#include <numeric> // for std calculation
#include <algorithm> // for tranform
#include <string.h> // memset

const uint64_t SIZE = 8 * 1024 * 1024;
const float GB = pow(10, 9);
const int NUM_TRIALS = 10;
const int NUM_SAMPLES = 1000;

using namespace std;

float findMean(vector<float> &v)
{
    // code from https://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();
    std::vector<double> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));
    cout << "Mean: ";
    cout << mean;
    cout << " GB/s" << endl;
    return mean;
}

float findSD(std::vector<float> &v)
{
    // code from https://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-std::vector-of-samples-in-c-using-boos
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    std::vector<double> diff(v.size());
    double mean = sum / v.size();
    std::transform(v.begin(), v.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / v.size());
    std::cout << "Group SD: ";
    std::cout << stdev << std::endl;
    std::cout << "Num Trials: ";
    std::cout << v.size() << std::endl;
    // divides by 10 seconds, Hz
    return stdev;
}

void measureRead()
{
    vector<float> readVec;
    vector<float> meanReadVec;
    for (int t = 0; t < NUM_TRIALS; t++)
    {
        for (int s = 0; s < NUM_SAMPLES; s++)
        {
            __m128i * arr = (__m128i *) malloc(SIZE);
            memset(arr, 0, SIZE/sizeof(__m128i));
            __m128i sum = _mm_set1_epi32(0);
            auto start = std::chrono::steady_clock::now();
            for (int i = 0; i < SIZE / sizeof(__m128i); i++)
            {
                _mm_add_epi16(sum, arr[i]);
            }
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> diff = end - start;   
            readVec.push_back(SIZE/GB/diff.count());   
            free(arr);
        }
        cout << "Read Bandwidth ";
        float mean = findMean(readVec);
        meanReadVec.push_back(mean);

        readVec.clear();

    }
    findSD(meanReadVec);
}

void measureWrite()
{
    vector<float> writeVec;
    vector<float> meanWriteVec;
    for (int t = 0; t < NUM_TRIALS; t++)
    {
        for (int s = 0; s < NUM_SAMPLES; s++)
        {
            __m128i * arr = (__m128i *) malloc(SIZE);
            memset(arr, 0, SIZE/sizeof(__m128i));
            __m128i val = _mm_set1_epi32(1);
            auto start = std::chrono::steady_clock::now();
            for (int i = 0; i < SIZE / sizeof(__m128i); i++)
            {
                _mm_stream_si128((__m128i*) &arr[i], val);
            }
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> diff = end - start;  
            writeVec.push_back(SIZE/GB/diff.count());   
            free(arr);
        }
        cout << "Write Bandwidth ";
        float mean = findMean(writeVec);
        meanWriteVec.push_back(mean);

        writeVec.clear();
 
    }
    findSD(meanWriteVec);
}

int main(int argc, char *argv[])
{
    cout << "mean read bandwidth (GB/s)" << endl;
    measureRead();
    cout << "mean write bandwidth (GB/s)" << endl;
    measureWrite();
}