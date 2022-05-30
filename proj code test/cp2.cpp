#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <string.h>

using namespace std;

// int **getMatrix(int row, int col);

int matrixChainMultiplication(int *dims, const int n);

int **getK(int *dims, const int n);

int **MatrixChainOr0decr(int p[], const int n)
{

    int **m = new int *[n];
    int **k_mat = new int *[n];
    for (int i = 0; i < n; ++i)
    {
        m[i] = new int[n];
        k_mat[i] = new int[n];
    }

    // cost is zero when multiplying
    // one matrix.
    for (int i = 1; i < n; i++)
        m[i][i] = 0;

    // L is chain length.
    for (int L = 2; L < n; L++)
    {
        for (int i = 1; i < n - L + 1; i++)
        {
            int j = i + L - 1;
            m[i][j] = INT_MAX;
            for (int k = i; k <= j - 1; k++)
            {
                // q = cost/scalar multiplications
                int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j])
                {
                    m[i][j] = q;
                    k_mat[i][j] = k;
                }
            }
        }
    }

    return m;
}

int **MatrixChainOrder1(int size[], const int n)
{

    int **m = new int *[n];
    int **k_mat = new int *[n];
    for (int i = 0; i < n; ++i)
    {
        m[i] = new int[n];
        k_mat[i] = new int[n];
    }

    for (int i = 0; i > n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            m[i][j] = INT_MAX;
            k_mat[i][j] = -1;
        }
    }

    // cost is zero when multiplying
    // one matrix.
    for (int i = 0; i < n; i++)
        m[i][i] = 0;

    // L is chain length.
    for (int iter = -1; iter < n; ++iter)
    {
        cout << "a" << endl;

        for (int l = 0; l < n - iter - 1; ++l)
        {
            cout << "b" << endl;

            if (l != iter + l + 1)
                // m[l][iter + l + 1] = INT_MAX;

                for (int k = 0; k <= iter; ++k)
                {
                    cout << "iter: " << iter << " l: " << l << " k: " << k << endl;

                    cout << "[ " << l << "," << iter + l + 1 << " ] = ";

                    cout << "[" << l << "," << l + k << "]  +  "
                         << "[" << 1 + k << "," << iter + l + 1 << "]" << endl;

                    int curr = m[l][l + k] + m[l + k][iter + l + 1] + size[l] * size[l + k + 1] * size[iter + l + 1];

                    cout << " .. " << m[l][l + k] << " + " << m[l + k][iter + l + 1] << endl;

                    // if (curr < m[l][iter + l + 1])
                    //{
                    cout << "curr " << curr << endl;

                    m[l][iter + l + 1] = curr;
                    k_mat[l][iter + l + 1] = k;
                    //}

                    cout << "c" << endl;
                }

            printMatrix(m, n, n);
        }
    }

    cout << "here" << endl;

    return m;
}

void printMatrix(int **matrix, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

int main()
{
    int size[] = {2, 2, 4, 2, 5};
    int matrixCount = 4;

    int **k_mat = MatrixChainOrder1(size, 4);
    printMatrix(k_mat, 4, 4);

    // printMatrix(matrix, 5, 5);

    // Matrix chain multiplication
}

int matrixChainMultip00lication(int *dims, const int n)
{

    // c[i, j] = Minimum number of scalar multiplications (i.e., cost)
    // needed to compute matrix `M[i] M[i+1] … M[j] = M[i…j]`
    // The cost is zero when multiplying one matrix
    // int c[n + 1][n + 1];

    int **c = new int *[n + 1];
    for (int i = 0; i < n + 1; ++i)
    {
        c[i] = new int[n + 1];
    }

    for (int i = 1; i <= n; i++)
    {
        c[i][i] = 0;
    }

    for (int len = 2; len <= n; len++) // subsequence lengths
    {
        for (int i = 1; i <= n - len + 1; i++)
        {
            int j = i + len - 1;
            c[i][j] = INT_MAX;

            for (int k = i; j < n && k <= j - 1; k++)
            {
                int cost = c[i][k] + c[k + 1][j] + dims[i - 1] * dims[k] * dims[j];
                if (cost < c[i][j])
                {
                    c[i][j] = cost;
                }
            }
        }
    }

    printMatrix(c, n + 1, n + 1);

    return c[1][n - 1];
}

int matrixChainMultiplication(int *dims, const int n)
{

    // c[i, j] = Minimum number of scalar multiplications (i.e., cost)
    // needed to compute matrix `M[i] M[i+1] … M[j] = M[i…j]`
    // The cost is zero when multiplying one matrix
    // int c[n + 1][n + 1];

    int **c = new int *[n + 1];
    for (int i = 0; i < n + 1; ++i)
    {
        c[i] = new int[n + 1];
    }

    for (int i = 1; i <= n; i++)
    {
        c[i][i] = 0;
    }

    for (int len = 2; len <= n; len++) // subsequence lengths
    {
        for (int i = 1; i <= n - len + 1; i++)
        {
            int j = i + len - 1;
            c[i][j] = INT_MAX;

            for (int k = i; j < n && k <= j - 1; k++)
            {
                int cost = c[i][k] + c[k + 1][j] + dims[i - 1] * dims[k] * dims[j];
                if (cost < c[i][j])
                {
                    c[i][j] = cost;
                }
            }
        }
    }

    printMatrix(c, n + 1, n + 1);

    return c[1][n - 1];
}

int **getK(int *dims, const int n)
{

    int **costMatrix = new int *[n];
    int **kMatrix = new int *[n];

    for (int i = 0; i < n; ++i)
    {

        costMatrix[i] = new int[n];
        kMatrix[i] = new int[n];
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {

            if (i == j)
            {
                costMatrix[j][i] = 0;
            }

            costMatrix[i][j] = INT_MAX;
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            kMatrix[i][j] = -1;
        }
    }
    // for (int j = 0; j < i; ++j)
    for (int i = 1; i < n; ++i)
    {

        for (int j = 0; j < i; ++j)
        // for (int i = 1; i < n; ++i)
        {
            for (int k = 0; k < (i - j); ++k)
            {

                cout << "i: " << i << " j: " << j << " k: " << k << endl;

                cout << "[" << j << ',' << k << "] $$$  [" << k + 1 << ',' << i << "]" << endl;

                int current = costMatrix[j][k] + costMatrix[k + 1][i] + dims[j] * dims[k] * dims[i];
                if (current < costMatrix[j][i])
                {
                    costMatrix[j][i] = current;
                    kMatrix[j][i] = k;
                }
            }
        }
    }

    printMatrix(costMatrix, n, n);

    return kMatrix;
}