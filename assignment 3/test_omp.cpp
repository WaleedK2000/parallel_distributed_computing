#include <iostream>
#include <omp.h>
#include <limits.h>

using namespace std;

int main(int argc, char *argv[])
{

    int i, a[10];
#pragma omp parallel num_threads(2)
    {

#pragma omp for schedule(dynamic, 1)
        for (i = 0; i < 10; i++)
            a[i] = omp_get_thread_num();
    }

    for (i = 0; i < 10; i++)
        cout << a[i] << "\n";
}