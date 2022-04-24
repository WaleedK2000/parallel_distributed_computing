#include <iostream>

using namespace std;

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

    cout << countOccuranceofStringParallel("ATTTGCGCAGACCTAAGCA", "GCA");

    return 0;
}