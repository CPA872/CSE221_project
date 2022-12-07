#include <stdio.h>
#include <iostream>
#include <vector>
#include <numeric> // for std calculation
#include <functional>
#include <x86intrin.h> // for rdtsc
#include <algorithm> // for tranform
#include <cmath> // for sqrt
#include <unistd.h>
#include <fcntl.h> // open

#include <sys/stat.h>
#include <sys/mman.h>

uint64_t rdtsc()
{
    return __rdtsc();
}

using namespace std;

const int NUM_SAMPLES = 1000;
const int NUM_TRIALS = 10;
const uint64_t CLOCK_FREQ = 2592199936;
const int PAGE_SIZE = sysconf(_SC_PAGE_SIZE);

float findMean(vector<float> &v)
{
    // code from https://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();
    std::vector<double> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));
    cout << "Mean: ";
    cout << mean;
    cout << " s" << endl;
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
    return stdev;
}

float convertCyclesToSeconds(uint64_t numTicks)
{
    float inverseFreq = 1.0 / CLOCK_FREQ;
    return inverseFreq * numTicks;
}

void pageFaultExperiments() {

    const char *filepath = "../example_files/8GB.txt";

    int fd = open(filepath, O_RDONLY);

    struct stat fdStat = {0};
    // get file size
    if (fstat(fd, &fdStat) == -1)
    {
        cout << "Error calling fstat on the file at: ";
        cout << filepath << endl;
        return;
    }

    unsigned index = 0;
    char *map = (char*) mmap(0, fdStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    vector<float> trialVec;
    for (int i = 0; i < NUM_TRIALS; i++) {

        vector<float> sampleVec;
        char c = 'b';

        for (int j = 0; j < NUM_SAMPLES; j++) {

            uint64_t tick = rdtsc();
            c = map[index * PAGE_SIZE * 200]; // Read a single byte
            float num_ticks = rdtsc() - tick;

            index += 1;

            sampleVec.push_back(convertCyclesToSeconds(num_ticks));
        }

        trialVec.push_back(findMean(sampleVec));
    }

    findSD(trialVec);

    munmap(map, fdStat.st_size);
    close(fd);
}


void singleByteReadExperiments() {

    vector<float> trialVec;

    for (int i = 0; i < NUM_TRIALS; i++) {

        vector<float> sampleVec;

        for (int j = 0; j < NUM_SAMPLES; j++) {

            char* byteToRead = (char*) malloc(sizeof(char));
            *byteToRead = 'b';
            char byteToWrite = 'a';
            uint64_t tick = rdtsc();
            byteToWrite = *byteToRead;
            float num_ticks = rdtsc() - tick;

            sampleVec.push_back(convertCyclesToSeconds(num_ticks));
        }

        trialVec.push_back(findMean(sampleVec));
    }

    findSD(trialVec);
}

int main(int argc, char *argv[]) {
    cout << "Results for Page Fault experiments (in seconds)..." << endl;
    pageFaultExperiments();

    cout << "\nResults for Main Memory experiments (in seconds)..." << endl;
    singleByteReadExperiments();

    cout << "\nPage Size was: ";
    cout << PAGE_SIZE << endl;
}
