#include <iostream>
#include <omp.h>
#include <limits.h>

using namespace std;

#define NUM_THREADS 12
#define array_size 909999
#define to_search 25874

int countOccuranceofString(string str, string subStr)
{

    int count = 0;
    int len = subStr.length();
    int i = 0;

    for (i = 0; i < str.length(); ++i)
    {
        if (str.substr(i, len) == subStr)
            count++;
    }

    return count;
}

int countOccuranceofStringParallel(string str, string subStr)
{

    int count = 0;
    int len = subStr.length();
    int i = 0;
#pragma omp parallel for reduction(+ \
                                   : count)
    for (i = 0; i < str.length(); ++i)
    {
        if (str.substr(i, len) == subStr)
            count++;
    }

    return count;
}

int main()
{
    bool found = false;
    bool stopSignal = false;
    int complete = 0;

    // int to_search = 250;
    omp_set_num_threads(NUM_THREADS);

    int arr[array_size];
    //#pragma omp parallel for
    for (int i = 0; i < array_size; i++)
        arr[i] = i;

#pragma omp parallel shared(found, complete, stopSignal)
    {

        int thread_num = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        if (thread_num == 0)
        {
            // Master thread

            while (!found && (complete != (num_threads - 1)))
                continue;

            stopSignal = true;
        }
        else
        {
            int each = array_size / (num_threads - 1);
            int start = each * (thread_num - 1);
            int end = start + each;

            for (int i = start; i < end && !stopSignal; i++)
            {

                if (arr[i] == to_search)
                {
                    found = true;
                    cout << "Found! by : " << thread_num << endl;
                    break;
                }
            }

            if (!stopSignal)
            {
                cout << thread_num << " Recived Stop Signal" << endl;
            }
            else
            {
                cout << thread_num << " Completed" << endl;
            }

            complete++;
        }
    }
    return 0;
}