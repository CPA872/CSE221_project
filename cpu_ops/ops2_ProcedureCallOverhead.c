#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include <unistd.h>    // for sleep
#include <x86intrin.h> // for rdtsc
#include <stdlib.h>    // for system()
#include <numeric>     // for std calculation

const int NUM_TRIALS = 10; 
const int NUM_SAMPLES = 1000;

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
    // cout << "SD: ";
    // cout << stdev << endl;
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


void procedure0() {
}

void procedure1(int a) {
}

void procedure2(int a, int b) {
}

void procedure3(int a, int b, int c) {
}

void procedure4(int a, int b, int c, int d) {
}

void procedure5(int a, int b, int c, int d, int e) {
}

void procedure6(int a, int b, int c, int d, int e, int f) {
}

void procedure7(int a, int b, int c, int d, int e, int f, int g) {
}

int main() {
    nice(-20);

    int a = 10, b = 20, c = 30, d = 40;
    int e = 50, f = 60, g = 70;

    int i;
    int total0 = 0, total1 = 0, total2 = 0, total3 = 0;
    int total4 = 0, total5 = 0, total6 = 0, total7 = 0;
    
    vector<float> vector0;
    vector<float> vector1;
    vector<float> vector2;
    vector<float> vector3;
    vector<float> vector4;
    vector<float> vector5;
    vector<float> vector6;
    vector<float> vector7;

    __uint64_t start = 0, end = 0;
    
    for (int t = 0; t < NUM_TRIALS; t++) {
        i = 0;
        start = __rdtsc();
        do {
            procedure0();
            i++;
        } while (i < NUM_SAMPLES);
        end   = __rdtsc();
        total0 = end - start;

        double average = (double) total0 / NUM_SAMPLES;
        vector0.push_back(average);
    }

    for (int t = 0; t < NUM_TRIALS; t++) {
        i = 0;
        start = __rdtsc();
        do {
            procedure1(a);
            i++;
        } while (i < NUM_SAMPLES);
        end   = __rdtsc();
        total1 = end - start;

        double average = (double) total1 / NUM_SAMPLES;
        vector1.push_back(average);
    }


    for (int t = 0; t < NUM_TRIALS; t++) {
        i = 0;
        start = __rdtsc();
        do {
            procedure2(a, b);
            i++;
        } while (i < NUM_SAMPLES);
        end   = __rdtsc();
        total2 = end - start;

        double average = (double) total2 / NUM_SAMPLES;
        vector2.push_back(average);
    }


    for (int t = 0; t < NUM_TRIALS; t++) {
        i = 0;
        start = __rdtsc();
        do {
            procedure3(a, b, c);
            i++;
        } while (i < NUM_SAMPLES);
        end   = __rdtsc();
        total3 = end - start;

        double average = (double) total3 / NUM_SAMPLES;
        vector3.push_back(average);
    }

    for (int t = 0; t < NUM_TRIALS; t++) {
        i = 0;
        start = __rdtsc();
        do {
            procedure4(a, b, c, d);
            i++;
        } while (i < NUM_SAMPLES);
        end   = __rdtsc();
        total4 = end - start;

        double average = (double) total4 / NUM_SAMPLES;
        vector4.push_back(average);
    }

    for (int t = 0; t < NUM_TRIALS; t++) {
        i = 0;
        start = __rdtsc();
        do {
            procedure5(a, b, c, d, e);
            i++;
        } while (i < NUM_SAMPLES);
        end   = __rdtsc();
        total5 = end - start;

        double average = (double) total5 / NUM_SAMPLES;
        vector5.push_back(average);
    }

    for (int t = 0; t < NUM_TRIALS; t++) {
        i = 0;
        start = __rdtsc();
        do {
            procedure6(a, b, c, d, e, f);
            i++;
        } while (i < NUM_SAMPLES);
        end   = __rdtsc();
        total6 = end - start;

        double average = (double) total6 / NUM_SAMPLES;
        vector6.push_back(average);
    }

    for (int t = 0; t < NUM_TRIALS; t++) {
        i = 0;
        start = __rdtsc();
        do {
            procedure7(a, b, c, d, e, f, g);
            i++;
        } while (i < NUM_SAMPLES);
        end   = __rdtsc();
        total7 = end - start;

        double average = (double) total7 / NUM_SAMPLES;
        vector7.push_back(average);
    }


    printf("---- Question 2: Procedure call overhead with arguments ----\n");
    printf("0 argument\n");
    findMeanSD(vector0);
    findSD(vector0);
    
    printf("1 argument\n");
    findMeanSD(vector1);
    findSD(vector1);

    printf("2 argument\n");
    findMeanSD(vector2);
    findSD(vector2);

    printf("3 argument\n");
    findMeanSD(vector3);
    findSD(vector3);
    
    printf("4 argument\n");
    findMeanSD(vector4);
    findSD(vector4);
    
    printf("5 argument\n");
    findMeanSD(vector5);
    findSD(vector5);
    
    printf("6 argument\n");
    findMeanSD(vector6);
    findSD(vector6);

    printf("7 argument\n");
    findMeanSD(vector7);
    findSD(vector7);
    return 0;
}