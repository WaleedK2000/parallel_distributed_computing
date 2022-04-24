#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
// fff
#define LAST_DIGIT 3
#define NUM_THREADS LastDigit + 9
#define TEN_DIGIT_LARGEST_NUMBER 999999999
#define TEN_DIGIT_SMALLEST_NUMBER 1000000000

int main(int argc, char *argv[])
{
    // Calculate execution time
    auto start = high_resolution_clock::now();

    cout << largestTenDigitPrime(LAST_DIGIT) << "Largest ten digit prime number" << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << duration.count() << endl;

    // Begin of parallel region
}