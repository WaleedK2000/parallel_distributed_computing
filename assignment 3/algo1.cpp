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

using namespace std;
using namespace std::chrono;

bool isPrime_static(long long number, int chunk_size)
{
    bool isPrime = true;

// Test if number is prime
#pragma omp parallel for schedule(static, chunk_size) shared(isPrime)
    for (long long divisor = 2; divisor <= number / 2; divisor++)
    {

        if (!isPrime)
            continue;

        if (number % divisor == 0)
        {
            isPrime = false;
            cout << "Number " << number << " is not prime" << endl;
        }
    }
    return isPrime;
}

bool isPrime_dynamic(long long number, int chunk_size)
{
    bool isPrime = true;

// Test if number is prime
#pragma omp parallel for schedule(dynamic, chunk_size)
    for (long long divisor = 2; divisor <= number / 2; divisor++)
    {
        if (!isPrime)
            continue;
        if (number % divisor == 0)
        {
            isPrime = false;
        }
    }
    return isPrime;
}

bool isPrime_guided(long long number, int chunk_size)
{
    bool isPrime = true;

// Test if number is prime
#pragma omp parallel for schedule(guided, chunk_size)
    for (long long divisor = 2; (divisor <= number / 2); divisor++)
    {
        if (!isPrime)
            continue;
        if (number % divisor == 0)
        {
            isPrime = false;
            cout << "Not prime: " << number << endl;
        }
    }
    return isPrime;
}

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
    bool found = false;

#pragma omp parallel for schedule(guided, chunk_size)
    for (long long i = TEN_DIGIT_LARGEST_NUMBER; i > TEN_DIGIT_SMALLEST_NUMBER; --i)
    {
        /* code */
        if (found)
            continue;

        if (isPrime_guided(i, 3))
        {
            largestPrime = i;
            found = true;
        }
    }

    return largestPrime;
}

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