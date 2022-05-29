//Generate Random R size matrix

public class MatrixGen {
	public static float[][] generateRandomSquareMatrix(int r) {
		float[][] matrix = new float[r][r];

		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < r; ++j) {
				matrix[i][j] = 2 ;//(float) (Math.random() * 10);
			}
			
		}
		
		return matrix;
	}
}
