#include <iostream>
#include <climits>
#include <mpi.h>
using namespace std;

int **matrixChainMultiplication(int ***matrix, int **costMatrix, int *matrixSizes, int numMatrix, int row, int col, int slaveTaskCount);

/*
 *    This function convers 2D array to 1D array
 * @param matrix 2d matrix
 * @param row row size of matrix
 * @param col column size of matrix
 * @return 1d matrix
 */

int *convert2Dto1D(int **matrix, int row, int col);

/*
 *    This function convers 1D array to 2D array
 * @param matrix 2d matrix
 * @param row row size of matrix
 * @param col column size of matrix
 * @return 1d matrix
 */

int **convert1Dto2D(int *matrix, int row, int col);
/*
 *   This function generates random array of r rows and c columns
 * @param r row size of matrix
 * @param c column size of matrix
 * @return 2d matrix
 */

int **generate2DMatrix(int r, int c);

/*
 * This function Multiplies two matrices. Uses MPI_Send()
 * @param matrix1 first matrix
 * @param matrix2 second matrix
 * @param row1 row size of matrix1
 * @param col1 column size of matrix1
 * @param row2 row size of matrix2
 * @param col2 column size of matrix2
 * @param slaveTaskCount number of slaves
 * @return int** matrix result
 */
int **multiplyMatrix(int **matrix1, int **matrix2, int row1, int col1, int row2, int col2, int slaveTaskCount);

// This function calculates K which is used to determine best order of multiplication
// @param size array of sizes of matrices
// @param n number of matrix
// @return int** K
int **getK(int size[], const int n);
// Generates random matrix of various sizes
int ***generateRandomMatrix(int numMatrix);

// Generate Assortment of matrixes of various sizes used in initialization in this code

int ***initialiseMatrix(int num, int size[])
{
    int ***matrix = new int **[num];
    for (int i = 0; i < num; i++)
    {
        matrix[i] = new int *[size[i]];
        for (int j = 0; j < size[i]; j++)
        {
            matrix[i][j] = new int[size[i + 1]];
        }
    }
    return matrix;
}

// Generates Matrix of row and col
/*
 * @param row row size of matrix
 * @param col column size of matrix
 * @return 2d matrix
 */
int **generateMatrix(int row, int col);

// Prints matrix of row and col

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

int main(int argc, char **argv)
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

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    numworkers = numtasks - 1;

    int numMatrix = 4;

    int size[] = {2, 2, 4, 2, 5};

    int ***matrix = initialiseMatrix(numMatrix, size);

    matrix[0] = generateMatrix(2, 2);
    matrix[1] = generateMatrix(2, 4);
    matrix[2] = generateMatrix(4, 2);
    matrix[3] = generateMatrix(2, 5);

    int MASTER = 0;

    if (taskid == MASTER)
    {
        // inside master
        int numMatrix = 4;
        int **k = getK(size, numMatrix);
        MPI_Send(&numMatrix, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        int **ans = matrixChainMultiplication(matrix, k, size, 4, -1, -1, numworkers);
    }
    else
    {
        // inside slaves
        int numMatrix;
        MPI_Status status;
        MPI_Recv(&numMatrix, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);

        int rowA, colA;
        int rowB, colB;
        int *matrixA;
        int *matrixB;
        // int status;
        source = 0;

        // int **ans = matrixChainMultiplication(matrix, k, size, numMatrix, -1, -1);
        // Recives Matrix num then each slave runs below loop once for each matrix count
        while (numMatrix - 1 > 0)
        {
            numMatrix--;
            // for (int i = 1; i <= slaveTaskCount; ++i)
            //{

            MPI_Recv(&rowA, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&colA, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);

            MPI_Recv(&rowB, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&colB, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);

            MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);

            matrixA = new int[rows * colA];
            matrixB = new int[rowB * colB];

            MPI_Recv(matrixA, rows * colA, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(matrixB, rowB * colB, MPI_INT, source, 1, MPI_COMM_WORLD, &status);

            int **mat_a = convert1Dto2D(matrixA, rows, colA);
            int **mat_b = convert1Dto2D(matrixB, rowB, colB);

            // matrix multiplication of mat_a and mat_b

            int **mat_c = new int *[rows];
            for (int i = 0; i < rows; i++)
            {
                mat_c[i] = new int[colB];
            }

            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < colB; j++)
                {
                    mat_c[i][j] = 0;

                    for (int k = 0; k < colA; k++)
                    {
                        mat_c[i][j] += mat_a[i][k] * mat_b[k][j];
                    }
                }
            }

            int *arr_c = convert2Dto1D(mat_c, rows, colB);

            MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
            MPI_Send(arr_c, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        }
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

int **matrixChainMultiplication(int ***matrix, int **costMatrix, int *matrixSizes, int numMatrix, int row, int col, int slaveTaskCount)
{
    // cout << "In here" << endl;
    // cout << "Row: " << row << endl;

    if (row == -1 && col == -1)
    {

        int k = costMatrix[0][numMatrix - 1];
        cout << "*************************************** " << k << ", " << numMatrix << endl;

        int **m1 = matrixChainMultiplication(matrix, costMatrix, matrixSizes, numMatrix, 0, k, slaveTaskCount);
        int **m2 = matrixChainMultiplication(matrix, costMatrix, matrixSizes, numMatrix, k + 1, numMatrix - 1, slaveTaskCount);

        int row1 = matrixSizes[0];
        int col1 = matrixSizes[k];

        cout << "final K " << k << endl;

        // return nullptr;

        return (multiplyMatrix(m1, m2, matrixSizes[0], matrixSizes[k + 1], matrixSizes[k + 1], matrixSizes[numMatrix], slaveTaskCount));
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

    int **m1 = matrixChainMultiplication(matrix, costMatrix, matrixSizes, numMatrix, row, k + row, slaveTaskCount);
    int **m2 = matrixChainMultiplication(matrix, costMatrix, matrixSizes, numMatrix, k + row + 1, col, slaveTaskCount);

    cout << "row: " << row << " col: " << col << endl;
    cout << "k: " << k << endl;

    printMatrix(m1, matrixSizes[row], matrixSizes[k]);

    return (multiplyMatrix(m1, m2, matrixSizes[row], matrixSizes[k + 1], matrixSizes[k + 1], matrixSizes[col + 1], slaveTaskCount));
}

int **multiplyMatrix(int **matrix1, int **matrix2, int row1, int col1, int row2, int col2, int slaveTaskCount)
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

        int rows = row1 / slaveTaskCount;
        int offset = 0;

        int *matArr1 = convert2Dto1D(matrix1, row1, col1);
        int *matArr2 = convert2Dto1D(matrix2, row2, col2);

        for (int dest = 1; dest <= slaveTaskCount; ++dest)
        {

            cout << "hehehehehehehehhehehehehehehe  8559" << endl;

            MPI_Send(&row1, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&col1, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);

            MPI_Send(&row2, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&col2, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);

            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);

            MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);

            MPI_Send(&matArr1[offset], rows * col1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(matArr2, row2 * col2, MPI_INT, dest, 1, MPI_COMM_WORLD);

            cout << "hehehehehehehehhehehehehehehe  8558" << endl;

            offset = offset + rows * col1;
        }

        int *resultArr = new int[row1 * col2];

        for (int dest = 1; dest <= slaveTaskCount; ++dest)
        {
            int offset, rows;
            int *arr_min;

            MPI_Recv(&offset, 1, MPI_INT, dest, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&rows, 1, MPI_INT, dest, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Recv(&resultArr[offset], rows * col2, MPI_INT, dest, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // MPI_Recv(resultArr, row1 * col2, MPI_INT, dest, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        int **result2D = convert1Dto2D(resultArr, row1, col2);
        cout << "im out" << endl;
        return result2D;

        /*

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
        printMatrix(result, row1, col2); */
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

int **generate2DMatrix(int r, int c)
{
    int **matrix = new int *[r];
    for (int i = 0; i < r; i++)
    {
        matrix[i] = new int[c];
    }

    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }

    return matrix;
}

// Convert 2d matrix to 1 d matrix
int *convert2Dto1D(int **matrix, int row, int col)
{
    int *result = new int[row * col];
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            result[i * col + j] = matrix[i][j];
        }
    }
    return result;
}

int **convert1Dto2D(int *matrix, int row, int col)
{
    int **result = new int *[row];
    for (int i = 0; i < row; i++)
    {
        result[i] = new int[col];
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            result[i][j] = matrix[i * col + j];
        }
    }
    return result;
}