#include <iostream>
#include <omp.h>
#include <limits.h>

using namespace std;

#define NUM_THREADS 12
#define array_size 9099999
#define to_search 2587493

int main()
{
    bool found = false;
    bool stopSignal = false;
    int complete = 0;

    // int to_search = 250;
    omp_set_num_threads(NUM_THREADS);

    long long *arr = new long long[array_size];
    //#pragma omp parallel for
    for (long long i = 0; i < array_size; i++)
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
            long long each = array_size / (num_threads - 1);
            long long start = each * (thread_num - 1);
            long long end = start + each;

            for (long long i = start; i < end && !stopSignal; i++)
            {

                if (arr[i] == to_search)
                {
                    found = true;
                    cout << "Found! by : " << thread_num << endl;
                    cout << "Found Val: " << arr[i] << endl;

                    break;
                }
            }

            if (stopSignal)
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