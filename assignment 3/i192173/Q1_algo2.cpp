#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <math.h>
// fff
#define LAST_DIGIT 3
#define NUM_THREADS LAST_DIGIT + 9
#define TEN_DIGIT_LARGEST_NUMBER 999999999
#define TEN_DIGIT_SMALLEST_NUMBER 100000000

using namespace std;
using namespace std::chrono;

bool primes(long long number)
{

    bool *a = new bool[number + 1];
    a[1] = false;

    for (long long i = 2; i <= number; i++)
        a[i] = true;

    long long sqrt_n = sqrt(number);

    for (long long i = 2; i < sqrt_n; ++i)
    {

        if (!a[number])
        {
            continue;
        }

        if (a[i])
        {

            // set all multiples of i upto n to false
            for (long long j = 1; i * j <= number; j++)
            {

                a[(i * j)] = false;
            }
        }
    }

    bool val = a[number];
    delete[] a;

    return val;
}

// Function to find largest ten digit prime number
long long largestTenDigitPrime(int chunk_size)
{
    long long largestPrime = 0;
    bool found = false;
    long long currentPrime = 0;
//#pragma omp parallel for schedule(static, chunk_size) shared(found, largestPrime) reduction(max: currentPrime)
//#pragma omp parallel for schedule(dynamic, chunk_size) shared(found, largestPrime) reduction(max: currentPrime)
#pragma omp parallel for schedule(guided, chunk_size) shared(found, largestPrime) reduction(max \
                                                                                            : currentPrime)

    for (long long i = TEN_DIGIT_LARGEST_NUMBER; i > TEN_DIGIT_SMALLEST_NUMBER; i--)
    {
        // cout << "ok";

        if (i % 2 == 0)
            continue;

        if (found)
            continue;
        else if (primes(i))
        {
#pragma omp critical
            {

                if (i > largestPrime)
                {
                    found = true;
                    largestPrime = i;

                    cout << "Largest prime found is " << largestPrime << endl;
                }
            }
        }
    }

    return largestPrime;
}

void call_largestTenDigitPrime(int chunk_size)
{
    auto start = high_resolution_clock::now();

    long long largestPrime = largestTenDigitPrime(chunk_size);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(stop - start);
    cout << "Largest prime found is " << largestPrime << endl;
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
    cout << "for chunk size: " << chunk_size << endl;
    cout << " ============================================================================================" << endl;
    cout << endl;
    cout << endl;
}

int main(int argc, char *argv[])
{
    // 9999999967

    // cout << primes(999999997);

    // return 0;
    // bool val = new bool;

    // cout << "Nice: " << val;

    for (int i = 0; i < 5; ++i)
    {
        call_largestTenDigitPrime(LAST_DIGIT + i);
    }

    return 0;
    omp_set_num_threads(NUM_THREADS);
    // Calculate execution time
    auto start = high_resolution_clock::now();

    cout << largestTenDigitPrime(LAST_DIGIT + 0) << " Largest ten digit prime number" << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);

    cout << duration.count() << endl;

    // Begin of parallel region
}