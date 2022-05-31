#include <iostream>
#include <climits>
#include <mpi.h>
using namespace std;

int **matrixChainMultiplication(int ***matrix, int **costMatrix, int *matrixSizes, int numMatrix, int row, int col);
int **multiplyMatrix(int **matrix1, int **matrix2, int row1, int col1, int row2, int col2);

int **getK(int size[], const int n);
// Generates random matrix of various sizes
int ***generateRandomMatrix(int numMatrix);

int ***initialiseMatrix(int num, int size[])
{
    int ***matrix = new int **[num];
    for (int i = 0; i < num; i++)
    {
        matrix[i] = new int *[size[i]];
        for (int j = 0; j < size[i]; j++)
        {
            matrix[i][j] = new int[size[i]];
        }
    }
    return matrix;
}

void copyMatrix(int **matrixOrginal, int **matrixCopy, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            matrixCopy[i][j] = matrixOrginal[i][j];
        }
    }
}

int **generateMatrix(int row, int col);

void printMatrix(int **matrix, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    // random 3d matrix

    // int ***matrix = new int **[4];

    int numtasks,              /* number of tasks in partition */
        taskid,                /* a task identifier */
        numworkers,            /* number of worker tasks */
        source,                /* task id of message source */
        dest,                  /* task id of message destination */
        mtype,                 /* message type */
        rows,                  /* rows of matrix A sent to each worker */
        averow, extra, offset; /* used to determine rows sent to each worker */

    int size[] = {2, 2, 4, 2, 5};

    int **k = getK(size, 4);

    int ***matrix = new int **[4];

    matrix[0] = generateMatrix(2, 2);
    matrix[1] = generateMatrix(2, 4);
    matrix[2] = generateMatrix(4, 2);
    matrix[3] = generateMatrix(2, 5);

    cout << "----- matrix 1 -----" << endl;
    printMatrix(matrix[0], 2, 2);
    cout << "----- matrix 2 -----" << endl;
    printMatrix(matrix[1], 2, 4);
    cout << "----- matrix 3 -----" << endl;
    printMatrix(matrix[2], 4, 2);
    cout << "----- matrix 4 -----" << endl;
    printMatrix(matrix[3], 2, 5);
    cout << "----End----" << endl;

    int **costMatrix = new int *[4];
    costMatrix[0] = new int[4];
    costMatrix[1] = new int[4];
    costMatrix[2] = new int[4];
    costMatrix[3] = new int[4];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            costMatrix[i][j] = -1;

    costMatrix[0][1] = 0;
    costMatrix[0][2] = 0;
    costMatrix[0][3] = 2;

    costMatrix[1][2] = 1;
    costMatrix[1][3] = 2;

    costMatrix[2][3] = 2;

    printMatrix(k, 4, 4);

    k[0][3] = 2;

    cout
        << "Hello World! " << costMatrix[1][0] << endl;

    printMatrix(k, 4, 4);

    if (taskid == MASTER)
    {

        int **ans = matrixChainMultiplication(matrix, k, size, 4, -1, -1);
    }

    cout
        << "Hello World!" << endl;

    cout << "Final Result" << endl;
    // printMatrix(ans, 2, 5);

    return 0;
}

int **getK(int size[], const int n)
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

int **matrixChainMultiplication(int ***matrix, int **costMatrix, int *matrixSizes, int numMatrix, int row, int col)
{
    // cout << "In here" << endl;
    // cout << "Row: " << row << endl;

    if (row == -1 && col == -1)
    {

        int k = costMatrix[0][numMatrix - 1];
        cout << "*************************************** " << k << ", " << numMatrix << endl;

        int **m1 = matrixChainMultiplication(matrix, costMatrix, matrixSizes, numMatrix, 0, k);
        int **m2 = matrixChainMultiplication(matrix, costMatrix, matrixSizes, numMatrix, k + 1, numMatrix - 1);

        int row1 = matrixSizes[0];
        int col1 = matrixSizes[k];

        cout << "final K " << k << endl;

        // return nullptr;

        return (multiplyMatrix(m1, m2, matrixSizes[0], matrixSizes[k + 1], matrixSizes[k + 1], matrixSizes[numMatrix]));
    }

    if (row == col)
        return matrix[row];

    int k = costMatrix[row][col];

    cout << "row: " << row << ", col: " << col << ", k: " << k << endl;
    cout << "k: " << k << endl;

    while (k == -1)
    {
        cout << "row: " << row << endl;
        cout << "col: " << col << endl;
    }

    cout << "row: " << row << endl;
    cout << "col: " << col << endl;

    cout << costMatrix[row][col] << endl;
    ;

    int **m1 = matrixChainMultiplication(matrix, costMatrix, matrixSizes, numMatrix, row, k + row);
    int **m2 = matrixChainMultiplication(matrix, costMatrix, matrixSizes, numMatrix, k + row + 1, col);

    cout << "row: " << row << " col: " << col << endl;
    cout << "k: " << k << endl;

    printMatrix(m1, matrixSizes[row], matrixSizes[k]);

    return (multiplyMatrix(m1, m2, matrixSizes[row], matrixSizes[k + 1], matrixSizes[k + 1], matrixSizes[col + 1]));
}

int **multiplyMatrix(int **matrix1, int **matrix2, int row1, int col1, int row2, int col2)
{
    int **result = new int *[row1];
    try
    {
        /* code */

        if (col1 != row2)
        {
            cout << "Error: Matrix 1 and Matrix 2 cannot be multiplied" << endl;
            throw "Error: Matrix 1 and Matrix 2 cannot be multiplied. Dimensions Error";
        }

        cout << "Multiplying Matrix" << endl;
        printMatrix(matrix1, row1, col1);
        cout << "-------------------------" << endl;
        printMatrix(matrix2, row2, col2);

        for (int i = 0; i < row1; i++)
        {
            result[i] = new int[col2];
        }

        for (int i = 0; i < row1; i++)
        {
            for (int j = 0; j < col2; j++)
            {
                for (int k = 0; k < col1; k++)
                {
                    result[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }

        cout << " ------------------ Result " << endl;
        printMatrix(result, row1, col2);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return result;
}

// Generates random matrix of various sizes
int ***generateRandomMatrix(int numMatrix, int row, int col)
{
    int ***matrix = new int **[numMatrix];
    for (int i = 0; i < numMatrix; i++)
    {
        matrix[i] = new int *[row];
        for (int j = 0; j < row; j++)
        {
            matrix[i][j] = new int[col];
        }
    }

    for (int i = 0; i < numMatrix; i++)
    {
        for (int j = 0; j < row; j++)
        {
            for (int k = 0; k < col; k++)
            {
                matrix[i][j][k] = rand() % 10;
            }
        }
    }

    return matrix;
}

int **generateMatrix(int row, int col)
{
    int **matrix = new int *[row];
    for (int i = 0; i < row; i++)
    {
        matrix[i] = new int[col];
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }

    return matrix;
}