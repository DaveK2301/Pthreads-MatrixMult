/*
 * TCSS 422 Operating System Sp 14
 * Homework 1 multi-threaded matrix multiplication
 * Pre-supplied template extended - 4 threads hard coded
 * 
 * @author Dave Kaplan 
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define R 10
#define S 15
#define T 18

/* An R-by-S matrix. */
int A[R][S];
/* An S-by-T matrix. */
int B[S][T];
/* An R-by-T matrix. */
int C[R][T];

int multiply();
int cross_product(const int row_A, const int col_B);
void print_ABC();

typedef struct {
	int upper_left_r;
	int upper_left_c;
	int lower_right_r;
	int lower_right_c;
} mat_mult_arg;

void* mat_mult_wrapper(const mat_mult_arg* arg);

int main(int argc, char* argv[]) {
	int i, j;
	
	setvbuf(stdout, NULL, _IONBF, 0);
	srand(time(NULL));

	for (i = 0; i < R; i++)
		for (j = 0; j < S; j++)
			A[i][j] = rand() % 21 - 10;
	for (i = 0; i < S; i++)
		for (j = 0; j < T; j++)
			B[i][j] = rand() % 21 - 10;

	return multiply();
}

/* Make C = A * B. */
int multiply() {
	int i;
	pthread_t threads[4];
	mat_mult_arg mm_args[4];
	
	//divide the matrix into 4 quadrants
	//works for 4 threads only
    
    //args for upper left quadrant
	mm_args[0].upper_left_r = 0;
	mm_args[0].upper_left_c = 0;
	mm_args[0].lower_right_r = (R - 1) / 2;
	mm_args[0].lower_right_c = (T - 1) / 2;
	
    //args for upper right quadrant
	mm_args[1].upper_left_r = 0;
	mm_args[1].upper_left_c = (T - 1) / 2 + 1;
	mm_args[1].lower_right_r = (R - 1) / 2; 
	mm_args[1].lower_right_c = T - 1;
	
	//args for lower left quadrant
    mm_args[2].upper_left_r = (R - 1) / 2 + 1;
	mm_args[2].upper_left_c = 0;
	mm_args[2].lower_right_r = R - 1; 
	mm_args[2].lower_right_c = (T - 1) / 2;
	
    //args for lower right quadrant
	mm_args[3].upper_left_r = (R - 1) / 2 + 1;
	mm_args[3].upper_left_c = (T - 1) / 2 + 1;
	mm_args[3].lower_right_r = R - 1; 
	mm_args[3].lower_right_c = T - 1;
	
	for(i = 0; i < 4; i++) {
			if (pthread_create(&threads[i], NULL, mat_mult_wrapper, &mm_args[i]) != 0) {
			printf("ERROR: Unable to create new thread.\n");
			exit(EXIT_FAILURE);
		}
    }
    for(i = 0; i < 4; i++)
		pthread_join(threads[i], NULL);
    print_ABC();
	return EXIT_SUCCESS;
}
/*
 * Computes a portion of the array C, the result of the matrix
 * multiplication of A X B.
 * 
 * @param upper_left_r the row index of the upper left
 * corner of the section to compute.
 * 
 * @param upper_left_c the column index of the upper left
 * corner of the section to compute.
 * 
 * @param lower_right_r the row index of the lower right
 * corner of the section to compute.
 * 
 * @param lower_right_c the column index of the lower right
 * corner of the section to compute.
 */
int mat_multiply(const int upper_left_r, const int upper_left_c, 
		const int lower_right_r, const int lower_right_c) {
    int r;
	int c;
    printf("Computing section [%d, %d] - [%d, %d]\n",
                upper_left_r, upper_left_c,
                lower_right_r, lower_right_c);

	for (r = upper_left_r; r <= lower_right_r; r++) {
		for (c = upper_left_c; c <= lower_right_c; c++) {
			//for the (r,c)'th entry of C compute the cross product
			//of [the r'th row of A] X [the c'th column of B]
            C[r][c] = cross_product(r, c);
		}
	
	}		
	return EXIT_SUCCESS;
}
/*
 * Returns the cross product of a row in matrix A 
 * and a column in matrix B
 */
int cross_product(const int row_A, const int col_B) {
	int i;
    int sum = 0;
	for (i = 0; i < S; i++) {
        sum += A[row_A][i] * B[i][col_B];
	}
    //printf("Computed cell: [%d, %d] = %d\n", row_A, col_B, sum);
	return sum;
}

void* mat_mult_wrapper(const mat_mult_arg* arg) {
	mat_multiply(arg->upper_left_r, arg->upper_left_c,
            arg->lower_right_r, arg->lower_right_c);
	return NULL;
}

void print_ABC(){
    int i;
    int j;
    printf("Matrix A:\n");
    for (i = 0; i < R; i++) {
        for (j = 0; j < S; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("----------------------------------\n");
    printf("Matrix B:\n");
    for (i = 0; i < S; i++) {
        for (j = 0; j < T; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }
    printf("----------------------------------\n");
        printf("Matrix C:\n");
    for (i = 0; i < R; i++) {
        for (j = 0; j < T; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    printf("----------------------------------\n");
}
