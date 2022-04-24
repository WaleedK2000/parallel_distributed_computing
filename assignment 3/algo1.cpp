#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
// fff
#define LAST_DIGIT 3
#define NUM_THREADS LAST_DIGIT + 9
#define TEN_DIGIT_LARGEST_NUMBER 9999999999
#define TEN_DIGIT_SMALLEST_NUMBER 1000000000

using namespace std;
using namespace std::chrono;

bool isPrime(long long number)
{
    bool isPrime = true;

    for (long long divisor = 2; divisor <= number / 2; divisor++)
    {
        if (number % divisor == 0)
        {
            isPrime = false;
            break;
        }
    }
    return isPrime;
}

// Function to find largest ten digit prime number
long long largestTenDigitPrime(int chunk_size)
{
    long long largestPrime = 0;
    long long currentPrime = 0;
    bool found = false;
//#pragma omp parallel for schedule(static, chunk_size) shared(found, largestPrime) reduction(max: currentPrime)
#pragma omp parallel for schedule(dynamic, chunk_size) shared(found, largestPrime) reduction(max \
                                                                                             : currentPrime)
    //#pragma omp parallel for schedule(guided, chunk_size) shared(found, largestPrime) reduction(max: currentPrime)

    for (long long i = TEN_DIGIT_LARGEST_NUMBER; i > TEN_DIGIT_SMALLEST_NUMBER; --i)
    {

        if (found)
            continue;
        else if (isPrime(i))
        {
            currentPrime = i;
            found = true;
        }
    }

    return currentPrime;
}

int main(int argc, char *argv[])
{
    omp_set_num_threads(NUM_THREADS);
    // Calculate execution time
    auto start = high_resolution_clock::now();

    cout << largestTenDigitPrime(LAST_DIGIT + 4) << " Largest ten digit prime number" << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);

    cout << duration.count() << endl;

    // Begin of parallel region
}