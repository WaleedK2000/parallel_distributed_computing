#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <math.h>
#include <vector>
// fff
#define LAST_DIGIT 3
#define NUM_THREADS LAST_DIGIT + 9
#define TEN_DIGIT_LARGEST_NUMBER 999999999
#define TEN_DIGIT_SMALLEST_NUMBER 100000000

using namespace std;
using namespace std::chrono;

long long primes(long long number, int chunk_size)
{
    vector<long long> vector_ans;

    long long *A = new long long[number + 1];

    for (int p = 2; p < number; p++)
        A[p] = p;

    long long sqrt_n = sqrt(number);
// dynamic
//#pragma omp parallel for schedule(static, chunk_size) shared(A)
//#pragma omp parallel for schedule(dynamic, chunk_size) shared(A)
#pragma omp parallel for schedule(guided, chunk_size) shared(A)

    for (int p = 2; p < sqrt_n; ++p)
    {
        if (A[p] != 0)
        {
            int j = p * p;
            while (j <= number)
            {
                A[j] = 0;
                j += p;
            }
        }
    }

    for (long long i = 0; i < number; i++)
    {
        if (A[i] != 0)
            vector_ans.push_back(A[i]);
    }

    delete[] A;

    return vector_ans.back();
}

// Function to find largest ten digit prime number
/*long long largestTenDigitPrime(int chunk_size)
{

    long long largestPrime = 0;
    bool found = false;
    //#pragma omp parallel for schedule(static, chunk_size) shared(found, largestPrime)
    //#pragma omp parallel for schedule(dynamic, chunk_size) shared(found, largestPrime)
    //#pragma omp parallel for schedule(guided, chunk_size) shared(found, largestPrime)

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
*/
void call_largestTenDigitPrime(int chunk_size)
{
    auto start = high_resolution_clock::now();
    // primes
    //  long long largestPrime = largestTenDigitPrime(chunk_size);

    long long largestPrime = primes(TEN_DIGIT_LARGEST_NUMBER, chunk_size);

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

    // cout << largestTenDigitPrime(LAST_DIGIT + 0) << " Largest ten digit prime number" << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);

    cout << duration.count() << endl;

    // Begin of parallel region
}