public class MatrixOp {
	//Question 1 Serial Version
	public static float[][] squareMatrixMultiply(float[][] A, float[][] B, int row) {
		float[][] C = new float[row][row];
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < row; ++j) {
				C[i][j] = 0;
				for (int k = 0; k < row; ++k) {
					C[i][j] = C[i][j] + A[i][k] * B[k][j];
				}
			}
		}
		return C;
	}
	
	public static void printSquareMatrix(float[][] mat, int rowSize) {
		
		for (int i = 0; i < rowSize; ++i) {
			for (int j = 0; j < rowSize; ++j) {
				System.out.print(mat[i][j] + " " );
			}
			System.out.println("\n");
		}
	}
	// Question 3 Correctness checking Function
	/**
	 * @param mat1 Matrix 1 to be compared
	 * @param mat2 Matrix 2 to be compared
	 * @param row  Row Size of both matrix
	 * @param col  Column size of both matrix
	 * @return Returns true if both matrix are equal
	 */
	public static boolean compareMatrix(float[][] mat1, float[][] mat2, int row, int col) {
		
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				if (mat1[i][j] != mat2[i][j]) {
					return false;
				}
			}
		}
		return true;
	}
	
}
