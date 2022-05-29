
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Cyclic implements Runnable{
	private final float[][] A;
	private final float[][] B;
	private final float[][] C;
	
	
	private final int rowSize;
	private final int left;
	private final int right;
	
	public Cyclic(float[][] a, float[][] b, float[][] c, int rowSize, int left, int right) {
		A = a;
		B = b;
		C = c;
		this.rowSize = rowSize;
		this.left = left;
		this.right = right;
	}
	
	public static void multiplyCyclic(float[][] a, float[][] b, float[][] c, int rowSize) throws InterruptedException {
		System.out.println("Enter Number of Threads");
		Scanner scan = new Scanner(System.in);
		int thread_count = scan.nextInt();
		List<Thread> thread_list = new ArrayList<>();
		
		int eachBlock = (rowSize / thread_count) / 4;
		
		for (int i = 0; i < rowSize; i += eachBlock) {
			BlockWiseThread block = new BlockWiseThread(a, b, c, i, i + eachBlock, rowSize);
			Thread thread = new Thread(block);
			thread.start();
			thread_list.add(thread);
			
			if (thread_list.size() % thread_count == 0) {
				for (Thread thread_c : thread_list) {
					thread.join();
				}
			}
			
			
		}
		
		//Wait for all threads to complete execution
		for (Thread thread_c : thread_list) {
			thread_c.join();
		}
	}
	
	@Override
	public void run() {
		for (int i = left; i < right; ++i) {
			for (int j = 0; j < rowSize; ++j) {
				C[i][j] = 0;
				for (int k = 0; k < rowSize; ++k) {
					C[i][j] += A[i][k] + B[k][j];
				}
			}
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		
		int size = 2048;
		
		float[][] A = MatrixGen.generateRandomSquareMatrix(size);
		float[][] B = MatrixGen.generateRandomSquareMatrix(size);
		float[][] C = new float[size][size];
		
		System.out.println("Printing A");
		
		System.out.println("Printing B");
		
		System.out.println("Start now");
		long startTime = System.nanoTime();
		multiplyCyclic(A, B, C, size);
		long endTime = System.nanoTime();
		long duration = endTime - startTime;
		duration /= 1000000;
		System.out.println("END Now." + " Time Taken: " + duration);
		
		System.out.println("Printing Result");
		
		System.out.println("Printing using traditional methord!");
		startTime = System.nanoTime();
		float[][] other_C = MatrixOp.squareMatrixMultiply(A, B, size);
		endTime = System.nanoTime();
		long other_duration = endTime - startTime;
		other_duration /= 1000000;
		System.out.println("END Now." + " Time Taken: " + other_duration);
		
		System.out.println("Compare both results");
		if (MatrixOp.compareMatrix(C, other_C, size, size)) {
			System.out.println("Equal");
		} else {
			System.out.println(" NOT Equal");
			MatrixOp.printSquareMatrix(other_C, size);
			System.out.println("-----------");
			MatrixOp.printSquareMatrix(C, size);
			
			System.out.println("----------- A -----------------");
			MatrixOp.printSquareMatrix(A, size);
			
			System.out.println("----------- B -----------------");
			MatrixOp.printSquareMatrix(B, size);
			
		}
		
	}
	
}
