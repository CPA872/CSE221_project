/**
 * This file performs experiments for File System question 4.
 * Here, we measure the avg time it takes to read a block (bypassing file cache)
 * for a variable number of simultaenous processes.
 * 
 * To compile: g++ fs4_Contention.cpp -o fs4_Contention
 * Run script using: ./fs4_Contention
*/


#include <iostream>
#include <stdlib.h>
#include <fcntl.h> // open
#include <chrono> // time
#include <thread>
#include <unistd.h>
#include <vector>

#include <algorithm> // for tranform
#include <cmath> // for sqrt
#include <numeric> // for std calculation
#include <x86intrin.h> // for rdtsc


uint64_t rdtsc()
{
    return __rdtsc();
}

using namespace std;

uint64_t CLOCK_FREQ = 2592199936;
const int NUM_SAMPLES = 20;
const int NUM_PROCESSES = 40;
const int BLOCK_SIZE = 4096; // 4KB

// 131072 blocks of 4096 bytes in 0.5 GB
const int NUM_BLOCKS_IN_HALF_GB = 131072;
const int NUM_BLOCKS = 100000;


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


// Reads NUM_BLOCKS blocks in a file but returns number of blocks per second
float readBlocks(int pid) {
    void * buf = malloc(BLOCK_SIZE);
    
    // O_DIRECT ensures direct reading (no file cache)
    int file;
    
    if (pid == 0) {
        file = open("../example_files/0.5GBcopy.txt", O_DIRECT); 
    } else {
        // not a child process
        file = open("../example_files/0.5GB.txt", O_DIRECT); 
    }
        
    uint64_t tick = rdtsc();
    for (int unsigned num_blocks = 0; num_blocks < NUM_BLOCKS; num_blocks++) {
        lseek(file, BLOCK_SIZE, SEEK_CUR); // move read pointer
        read(file, buf, BLOCK_SIZE); 
    }
    float numTicks = rdtsc() - tick;

    free(buf);
    close(file);

    return convertCyclesToSeconds(numTicks) / NUM_BLOCKS; // Returns number of seconds per block
}


float createParallelReadingProcesses(int num_processes)
{
    float seconds;
    int pid = 1;

    for(unsigned int i = 0; i < num_processes; i++) 
    {
        pid = fork();
        if (pid == 0)
        {
            // Arbitrary delay
            for(unsigned int j = i; j < num_processes; j++) 
            {
                this_thread::sleep_for(0.5 * chrono::nanoseconds(1));
            }
            break;
        }
    }

    seconds = readBlocks(pid);

    if (pid == 0) {
        // cout << "killing child" << endl;
        exit(EXIT_SUCCESS);
    }

    return seconds;
}


int main(int argc, char *argv[])
{
    for (unsigned int num_processes = 0; num_processes <= NUM_PROCESSES; num_processes++) {
        // create up to NUM_PROCESSES number of parallel processes

        vector<float> sampleVec;
        for (unsigned int i = 0; i < NUM_SAMPLES; i++) {
            float seconds = createParallelReadingProcesses(num_processes);
            sampleVec.push_back(seconds); 
        }

        float mean = findMean(sampleVec);

        cout << num_processes << " ";
        cout << "Child Processes: "; 
        cout << mean << " secs/block" << endl;
    }
}