/**
 * This file performs experiments for File System question 1.
 * Here, we measure the avg time it takes to read a byte for files of various sizes.
 * 
 * To compile: g++ fs1_FileCache.cpp -o fs1_FileCache
 * Run script using: ./fs1_FileCache
*/

#include <stdio.h> // using this library for file i/o
#include <iostream> 
#include <vector>
#include <string.h>

#include <algorithm> // for tranform
#include <cmath> // for sqrt
#include <numeric> // for std calculation
#include <x86intrin.h> // for rdtsc


uint64_t rdtsc()
{
    return __rdtsc();
}

using namespace std;

const int NUM_SAMPLES = 20;
uint64_t CLOCK_FREQ = 2592199936;


float findMean(vector<float> &v)
{
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();
    return mean;
}
float convertCyclesToSeconds(uint64_t numTicks)
{
    float inverseFreq = 1.0 / CLOCK_FREQ;
    return inverseFreq * numTicks;
}


float getAvgBytesPerSecond(const char* filepath, long filebytes) {
    FILE *fp;
    vector<float> sampleVec;

    for (unsigned int i = 0; i < NUM_SAMPLES; i++) {

        fp = fopen(filepath,"r");

        if (fp == NULL) {
            cout << "file not found: ";
            cout << filepath << endl;
            return 0.0;
        }

        unsigned char ch;
        uint64_t tick = rdtsc();
        for (unsigned int fileindex = 0; fileindex < filebytes; fileindex++) {
            ch = fgetc(fp);
        }
        float numTicks = rdtsc() - tick;
        fclose(fp);

        // calculate avg number of sec / bytes
        sampleVec.push_back( convertCyclesToSeconds(numTicks) / filebytes ); 
    }
    
    float mean = findMean(sampleVec);
    return mean;
} 


int main () {

    vector<const char*> filenames = {"../example_files/0_125GB.txt", "../example_files/0_25GB.txt", 
                                     "../example_files/0_5GB.txt", "../example_files/1GB.txt",
                                     "../example_files/2GB.txt"};
    vector<long> filebytes = {134217728, 268435456, 536870912, 1073741824, 2147483648 };

    for (unsigned int i = 0; i < filenames.size(); i++) {
        cout << filenames[i];
        cout << ": ";

        cout << getAvgBytesPerSecond(filenames[i], filebytes[i]) << " secs/byte" << endl;
    }

    return(0);
}