/**
 * Please compile using:
 * g++ -pthread ops345_SysCallTaskCreationContextS.cpp -o ops345 
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include <unistd.h>    // for sleep
#include <x86intrin.h> // for rdtsc
#include <stdlib.h>    // for system()
#include <numeric>     // for std calculation
#include <pthread.h>   // for pthread
#include <sys/wait.h>  // for wait()

// #include "procedures.c"

uint64_t rdtsc()
{
    return __rdtsc();
}

using namespace std;

const int NUM_TRIALS = 10; 
const int NUM_SAMPLES = 1000;

uint64_t CLOCK_FREQ = 0;

// Helper functions

// returns mean
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
    // return mean / 10;
    // divides by 10 seconds, Hz
    return mean;
}
float findSD(vector<float> &v)
{
    // code from https://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    std::vector<double> diff(v.size());
    double mean = sum / v.size();
    std::transform(v.begin(), v.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / v.size());
    cout << "Group SD: ";
    cout << stdev << endl;
    cout << "Num Trials: ";
    cout << v.size() << endl;
    // divides by 10 seconds, Hz
    return stdev;
}


// Code to answer questions

// Returns mean cycle time
uint64_t findCycleTime()
{
    cout << "Finding the clock frequency. Beginning for-loop..." << endl;
    vector<float> vec;

    for (int i = 0; i < NUM_SAMPLES; i++)
    {
        uint64_t tick = rdtsc();
        sleep(10);
        float numTicks = rdtsc() - tick; // difference

        vec.push_back(numTicks);
        cout << "NumTicks: ";
        cout << numTicks << endl;
    }

    return findMeanSD(vec);
}

float convertCyclesToSeconds(uint64_t numTicks)
{
    // HZ = cycles/second (which we have in CLOCK_FREQ)
    // 1/Hz = seconds/cycle
    float inverseFreq = 1.0 / CLOCK_FREQ;

    // seconds/cycle * cycle = seconds
    return inverseFreq * numTicks;
}

void reading_time() {
    vector<float> vec;
    int i = 0;
    __uint64_t start, end;
    __uint64_t total = 0;
    for (int j = 0; j < NUM_TRIALS; j++) {
        for (i = 0; i < NUM_SAMPLES; i++) {
            start = __rdtsc();
            end = __rdtsc();
            total += end - start;
        }
        double average = (double) total / NUM_SAMPLES;
        vec.push_back(average);
    }

    findMeanSD(vec);
}

void for_overhead() {
    int i = 0;
    vector<float> vec;
    
    for (int t = 0; t < NUM_TRIALS; t++) {
        __uint64_t start = __rdtsc();
        for (i = 0; i < NUM_SAMPLES; i++);
        __uint64_t total = __rdtsc() - start;
        double average = (double) total / NUM_SAMPLES;
        vec.push_back(average);
    }

    findMeanSD(vec);
    findSD(vec);
}

void do_while_overhead() {
    int i = 0;
    vector<float> vec; 
    __uint64_t start = __rdtsc();
    
    for (int t = 0; t < NUM_TRIALS; t++) {
        do {
            i++;
        } while (i < NUM_SAMPLES);
        __uint64_t total = __rdtsc() - start;
        double average = (double) total / NUM_SAMPLES;
        vec.push_back(average);
    }

    findMeanSD(vec);
    findSD(vec);
}



void question3()
{    
    cout << endl;
    cout << "Running Question 3 ======================================" << endl;
    cout << "Outputting results for system call overhead (in seconds)..." << endl;

    vector<float> mean_vec;
    for (int i = 0; i < NUM_TRIALS; i++)
    {
        vector<float> vec;
        for (int i = 0; i < NUM_SAMPLES; i++)
        {
            uint64_t tick = rdtsc();            // tick before
            system("whoami > delete_me.txt");                   // minimal sys call?
            uint64_t numTicks = rdtsc() - tick; // difference
            float numSeconds = convertCyclesToSeconds(numTicks);
            // cout << "Seconds: ";
            // cout << numSeconds << endl;
            vec.push_back(numSeconds);
        }
        mean_vec.push_back(findMeanSD(vec));
    }

    // for (int i = 0; i < mean_vec.size(); i++) {
    //     cout << "Mean: ";
    //     cout << mean_vec.at(i) << endl;
    // }
    findSD(mean_vec);
}

// Code for pthread from https://www.geeksforgeeks.org/thread-functions-in-c-c/
void *question4Func(void *arg)
{
    // pthread_detach(pthread_self()); // detach the current thread from the calling thread
    uint64_t *iptr = (uint64_t *)malloc(sizeof(uint64_t));
    *iptr = rdtsc();
    // cout << "after detach in kernel thread" << endl;
    return iptr;
}

void question4()
{
    cout << endl;
    cout << "Running Question 4 ======================================" << endl;

    cout << "Outputting results for creating/running Processes (in seconds)..." << endl;
    vector<float> mean_vec;
    for (int i = 0; i < NUM_TRIALS; i++)
    {
        // Report the time to create and run both a process and a kernel thread
        vector<float> vec;
        for (int i = 0; i < NUM_SAMPLES; i++)
        {
            uint64_t tick = rdtsc();
            pid_t c_pid = fork(); // Create a process
            uint64_t numTicks = rdtsc() - tick;

            if (c_pid > 0)
            {
                // parent process
                wait(nullptr);
            }
            else
            {
                // child process
                exit(EXIT_SUCCESS);
            }

            float numSeconds = convertCyclesToSeconds(numTicks);
            // cout << "Seconds: ";
            // cout << numSeconds << endl;
            vec.push_back(numSeconds);
        }
        mean_vec.push_back(findMeanSD(vec));
    }
    findSD(mean_vec);

    cout << "Outputting results for creating/running Kernel Threads (in seconds)..." << endl;
    mean_vec.clear();
    for (int i = 0; i < NUM_TRIALS; i++)
    {
        vector<float> vec;
        for (int i = 0; i < NUM_SAMPLES; i++)
        {
            pthread_t ptid;
            uint64_t *endTick;

            uint64_t tick = rdtsc();

            // Creating a new thread
            pthread_create(&ptid, NULL, question4Func, NULL);

            // Waiting for the created thread to terminate
            pthread_join(ptid, (void **)&endTick);

            uint64_t numTicks = *endTick - tick;
            float numSeconds = convertCyclesToSeconds(numTicks);
            //cout << "Seconds: ";
            //cout << numSeconds << endl;
            vec.push_back(numSeconds);
        }
        mean_vec.push_back(findMeanSD(vec));
    }
    findSD(mean_vec);
}

pthread_cond_t cv;
pthread_mutex_t mp;
bool isThread2Done = false;

void *question5FuncThread1(void *arg)
{
    //printf("start thread 1\n");
    uint64_t *iptr = (uint64_t *)malloc(sizeof(uint64_t));
    int ret;

    pthread_mutex_lock(&mp);

    while (!isThread2Done)
    {
        // wait on condition variable
        ret = pthread_cond_wait(&cv, &mp);
        //printf("** thread 1 waiting, ret=%d\n", ret);
    }
    *iptr = rdtsc();

    pthread_mutex_unlock(&mp);
    // printf("end thread 1\n");
    return iptr;
}

// Code from: https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032r/index.html
void *question5FuncThread2(void *arg)
{
    //printf("start thread 2\n");
    int ret;

    pthread_mutex_lock(&mp);

    // wait on condition variable
    ret = pthread_cond_signal(&cv);

    uint64_t *iptr = (uint64_t *)malloc(sizeof(uint64_t));
    *iptr = rdtsc();

    isThread2Done = true;
    pthread_mutex_unlock(&mp);

    // TODO TRY MOVING RECORDING TIMESTAMP HERE WHEN USING SINGLE PROCESSOR

    //printf("end thread 2\n");
    return iptr;
}

void question5()
{
    // Try using taskset to restrict num of processor
    cout << endl;
    cout << "Running Question 5 ======================================" << endl;

    // Report the time to context switch from one process to another, and from one kernel thread to another

    // Code from http://www2.cs.uregina.ca/~hamilton/courses/330/notes/unix/pipes/pipes.html
    cout << "Outputting results for context switching Processes (in seconds)..." << endl;

    vector<float> mean_vec;
    for (int i = 0; i < NUM_TRIALS; i++)
    {
        vector<float> vec;
        for (int i = 0; i < NUM_SAMPLES; i++)
        {
            uint64_t tick = 0;
            uint64_t endTick = 0;

            int pid, pip[2];
            pipe(pip);

            pid = fork();
            if (pid == 0) // child : sends message to parent
            {
                // send characters in the string, including end-of-string
                endTick = rdtsc();
                write(pip[1], &endTick, sizeof(endTick));
                exit(1);
            }
            else // parent : receives message from child
            {
                // read from the pipe - force context switch
                tick = rdtsc();
                read(pip[0], &endTick, sizeof(endTick));
            }
            uint64_t numTicks = endTick - tick;
            if (endTick < tick) {
                cout << "END TICK LESS THAN" << endl;
                cout << tick << endl;
                cout << endTick << endl;
            }
            float numSeconds = convertCyclesToSeconds(numTicks);
            // cout << "Seconds: ";
            // cout << numSeconds << endl;
            vec.push_back(numSeconds);
        }
        mean_vec.push_back(findMeanSD(vec));
    }
    findSD(mean_vec);

    cout << "Outputting results for context switching Kernel Threads (in seconds)..." << endl;
    mean_vec.clear();
    for (int i = 0; i < NUM_TRIALS; i++)
    {
        vector<float> vec;
        for (int i = 0; i < NUM_SAMPLES; i++)
        {
            isThread2Done = false;

            pthread_t ptid1, ptid2;
            uint64_t *startTick;
            uint64_t *endTick;

            // Creating 2 new threads
            pthread_create(&ptid1, NULL, question5FuncThread1, NULL);
            pthread_create(&ptid2, NULL, question5FuncThread2, NULL);

            // Waiting for the created thread to terminate
            // context switch thread2 -> thread1
            pthread_join(ptid1, (void **)&endTick);
            pthread_join(ptid2, (void **)&startTick);

            uint64_t numTicks = *endTick - *startTick;
            // cout << *startTick << endl;
            // cout << *endTick << endl;
            // cout << numTicks << endl;
            float numSeconds = convertCyclesToSeconds(numTicks);
            // cout << "Seconds: ";
            // cout << numSeconds << endl;
            vec.push_back(numSeconds);
        }
        mean_vec.push_back(findMeanSD(vec));
    }
    findSD(mean_vec);
}

int main(int argc, char *argv[])
{
    nice(-20); // increase priority

    CLOCK_FREQ = 2592199936; // 2.6 GHZ

    question3();
    question4();
    question5();

    // Driver code here
    return 0;
}