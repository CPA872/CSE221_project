// reference: https://stackoverflow.com/questions/8107739/main-memory-bandwidth-measurement

#include <stdio.h>
#include <iostream>
#include <vector>
#include <numeric> // for std calculation
#include <functional>
#include <x86intrin.h> // for rdtsc
#include <algorithm> // for tranform
#include <cmath> // for sqrt
#include <unistd.h>
#include "immintrin.h"

uint64_t rdtsc()
{
    return __rdtsc();
}

using namespace std;

const int NUM_SAMPLES = 1000;
const int NUM_TRIALS = 10;
const uint64_t CLOCK_FREQ = 2592199936;
const float SIZE = 8 * 1024 * 1024; // 8MB size array
const int ARRAY_LEN = SIZE / sizeof(int);
float CYCLES = 2.6 * 1024 * 1024;
const float GB = pow(10, 9);

float findMean(vector<float> &v)
{
    // code from https://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();
    std::vector<double> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));
    cout << "Mean: ";
    cout << mean << endl;
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

float convertCyclesToSeconds(uint64_t numTicks)
{
    // HZ = cycles/second (which we have in CLOCK_FREQ)
    // 1/Hz = seconds/cycle
    float inverseFreq = 1.0 / CLOCK_FREQ;

    // seconds/cycle * cycle = seconds
    return inverseFreq * numTicks;
}

void measureRead()
{
    vector<float> readVec;
    vector<float> meanReadVec;
    for (int t = 0; t < NUM_TRIALS; t++)
    {
        for (int s = 0; s < NUM_SAMPLES; s++)
        {
            // int * arr = new int[(int)SIZE];
            int * arr = (int *) malloc((int)SIZE);
            int sum = 0;
            for (int i = 0; i < (int)SIZE/sizeof(int); i++)
            {
                arr[i] = 0;
            }

            uint64_t tick = rdtsc();
            for (int i = 0; i < (int)SIZE/sizeof(int); i += 40)
            {
                sum += arr[i];
                sum += arr[i + 1];
                sum += arr[i + 2];
                sum += arr[i + 3];
                sum += arr[i + 4];
                sum += arr[i + 5];
                sum += arr[i + 6];
                sum += arr[i + 7];
                sum += arr[i + 8];
                sum += arr[i + 9];
                sum += arr[i + 10];
                sum += arr[i + 11];
                sum += arr[i + 12];
                sum += arr[i + 13];
                sum += arr[i + 14];
                sum += arr[i + 15];
                sum += arr[i + 16];
                sum += arr[i + 17];
                sum += arr[i + 18];
                sum += arr[i + 19];
                sum += arr[i + 20];
                sum += arr[i + 21];
                sum += arr[i + 22];
                sum += arr[i + 23];
                sum += arr[i + 24];
                sum += arr[i + 25];
                sum += arr[i + 26];
                sum += arr[i + 27];
                sum += arr[i + 28];
                sum += arr[i + 29];
                sum += arr[i + 30];
                sum += arr[i + 31];
                sum += arr[i + 32];
                sum += arr[i + 33];
                sum += arr[i + 34];
                sum += arr[i + 35];
                sum += arr[i + 36];
                sum += arr[i + 37];
                sum += arr[i + 38];
                sum += arr[i + 39];
                sum += arr[i + 40];
            }
            float numTicks = rdtsc() - tick;
            float numSeconds = convertCyclesToSeconds(numTicks);
            readVec.push_back(SIZE/GB/numSeconds);
            free(arr);
        }

        cout << "readVec ";
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
            int * arr = new int[(int)SIZE];

            // int * arr = (int *) malloc((int)SIZE); //new int[size];
            // write
            uint64_t tick = rdtsc();
            for (int i = 0; i < (int)SIZE/sizeof(int); i += 40)
            {
                arr[i] = 0;
                arr[i + 1] = 0;
                arr[i + 2] = 0;
                arr[i + 3] = 0;
                arr[i + 4] = 0;
                arr[i + 5] = 0;
                arr[i + 6] = 0;
                arr[i + 7] = 0;
                arr[i + 8] = 0;
                arr[i + 9] = 0;
                arr[i + 10] = 0;
                arr[i + 11] = 0;
                arr[i + 12] = 0;
                arr[i + 13] = 0;
                arr[i + 14] = 0;
                arr[i + 15] = 0;
                arr[i + 16] = 0;
                arr[i + 17] = 0;
                arr[i + 18] = 0;
                arr[i + 19] = 0;
                arr[i + 20] = 0;
                arr[i + 21] = 0;
                arr[i + 22] = 0;
                arr[i + 23] = 0;
                arr[i + 24] = 0;
                arr[i + 25] = 0;
                arr[i + 26] = 0;
                arr[i + 27] = 0;
                arr[i + 28] = 0;
                arr[i + 29] = 0;
                arr[i + 30] = 0;
                arr[i + 31] = 0;
                arr[i + 32] = 0;
                arr[i + 33] = 0;
                arr[i + 34] = 0;
                arr[i + 35] = 0;
                arr[i + 36] = 0;
                arr[i + 37] = 0;
                arr[i + 38] = 0;
                arr[i + 39] = 0;
                arr[i + 40] = 0;
            }
            float numTicks = rdtsc() - tick; // cycles
            float numSeconds = convertCyclesToSeconds(numTicks);
            // writeVec.push_back(SIZE/1024/1024/numSeconds); // MB/s
            writeVec.push_back(SIZE/GB/numSeconds);

            free(arr);
        }

        cout << "writeVec ";
        float mean = findMean(writeVec);
        meanWriteVec.push_back(mean);

        writeVec.clear();
    }
    findSD(meanWriteVec);
}

int main(int argc, char *argv[])
{
    // measureRead();
    measureWrite();
}