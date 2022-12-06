#include <iostream>
#include <stdlib.h>
#include <fcntl.h> // open
#include <unistd.h> // read, close
#include <vector>
#include <numeric> // for std calculation
#include <algorithm> // for tranform
#include <malloc.h>
#include <x86intrin.h> // for rdtsc

uint64_t rdtsc()
{
    return __rdtsc();
}

using namespace std;

const int BLOCK_SIZE = 4096; // 4KB
const int NUM_SAMPLES = 10000;
uint64_t CLOCK_FREQ = 2592199936;

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

float convertCyclesToSeconds(uint64_t numTicks)
{
    // HZ = cycles/second (which we have in CLOCK_FREQ)
    // 1/Hz = seconds/cycle
    float inverseFreq = 1.0 / CLOCK_FREQ;

    // seconds/cycle * cycle = seconds
    return inverseFreq * numTicks;
}

void fileSequentialReadTime(const char * filename)
{
    vector<float> vec;
    int file = open(filename, O_DIRECT|O_SYNC);
    void * buf = malloc(BLOCK_SIZE);
    for (int i = 0; i < NUM_SAMPLES; i++)
    {
        lseek(file, BLOCK_SIZE, SEEK_CUR);
        uint64_t tick = rdtsc();
        read(file, buf, BLOCK_SIZE);
        float numTicks = rdtsc() - tick;
        vec.push_back(convertCyclesToSeconds(numTicks));
    }
    free(buf);

    close(file);
    findMean(vec);
}

void fileRandomReadTime(const char * filename, long filebyte)
{
    vector<float> vec;
    int file = open(filename, O_DIRECT|O_SYNC);
    void * buf = malloc(BLOCK_SIZE);
    for (int i = 0; i < NUM_SAMPLES; i++)
    {
        int randNum = rand() % (filebyte/BLOCK_SIZE);
        uint64_t tick = rdtsc();
        lseek(file, BLOCK_SIZE * randNum, SEEK_SET);
        read(file, buf, BLOCK_SIZE);
        float numTicks = rdtsc() - tick;
        vec.push_back(convertCyclesToSeconds(numTicks));
    }
    free(buf);

    close(file);
    findMean(vec);
}

int main(int argc, char *argv[])
{
    vector<const char*> filenames = {"remote/0_125GB.txt", "remote/0_25GB.txt", 
                                     "remote/0_5GB.txt", "remote/1GB.txt",
                                     "remote/2GB.txt", "remote/8GB.txt"};
    vector<long> filebytes = {134217728, 268435456, 536870912, 1073741824, 2147483648, 8589934592 };
    for (int i = 0; i < filenames.size(); i++)
    {
        cout << "remote sequential read time for " << filenames[i] << endl;
        fileSequentialReadTime(filenames[i]);
    }

    cout << "===========================================" << endl;

    for (int i = 0; i < filenames.size(); i++)
    {
        cout << "remote random read time for " << filenames[i] << endl;
        fileRandomReadTime(filenames[i], filebytes[i]);
    }
}