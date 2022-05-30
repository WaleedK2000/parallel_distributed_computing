#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <string.h>

using namespace std;

// int **getMatrix(int row, int col);

int **MatrixChainMultiplication111(int p[], int n);

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

int **MatrixChainOrder1(int size[], const int n);
/*
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
*/
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

int **MatrixChainOrder1(int size[], const int n)
{

    int **m = new int *[n];
    int **k_mat = new int *[n];
    for (int i = 0; i < n; ++i)
    {
        m[i] = new int[n];
        k_mat[i] = new int[n];
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            m[i][j] = INT_MAX;
            k_mat[i][j] = -1;
        }
    }

    for (int i = 0; i < n; i++)
    {

        m[i][i] = 0;
        k_mat[i][i] = 0;
    }

    for (int iter = 0; iter < n; ++iter)
    {
        cout << "a" << endl;

        for (int l = 0; l < n - iter - 1; ++l)
        {
            cout << "b" << endl;

            if (l != iter + l + 1)
                //   m[l][iter + l + 1] = INT_MAX;

                for (int k = 0; k <= iter; ++k)
                {
                    int lc = l + k;
                    int rr = l + k + 1;
                    int rc = iter + l + 1;

                    int curr = m[l][lc] + m[rr][rc] + size[l] * size[rr] * size[rr + 1];

                    if (curr < m[l][rc])
                    {

                        m[l][iter + l + 1] = curr;
                        k_mat[l][iter + l + 1] = k;
                    }
                }

            printMatrix(m, n, n);
        }
    }

    return k_mat;
}

int **MatrixChainMultiplication111(int p[], int n)
{
    // int m[n][n];

    int **m = new int *[n];

    for (int i = 0; i < n; i++)
    {
        m[i] = new int[n];
    }

    int i, j, k, L, q;

    for (i = 1; i < n; i++)
        m[i][i] = 0; // number of multiplications are 0(zero) when there is only one matrix

    // Here L is chain length. It varies from length 2 to length n.
    for (L = 2; L < n; L++)
    {
        for (i = 1; i < n - L + 1; i++)
        {
            j = i + L - 1;
            m[i][j] = INT_MAX; // assigning to maximum value

            for (k = i; k <= j - 1; k++)
            {
                q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j])
                {
                    m[i][j] = q; // if number of multiplications found less that number will be updated.
                }
            }
        }
    }

    return m; // returning the final answer which is M[1][n]
}