#pragma warning(disable:4996) //disabling warning

#include<stdio.h>

#define MATRIX_DIMENSIONS 3
#define VECTOR_DIMENSIONS 3

//Multiplies vector by matrix
void multiply_vector_by_matrix(int* result_vector, const int vector_to_multiply[], const int multiplying_matrix[][MATRIX_DIMENSIONS]);

int main() {
	int matrix3x3[MATRIX_DIMENSIONS][MATRIX_DIMENSIONS]; //Matrix, user input
	int vector[VECTOR_DIMENSIONS]; //vector, user input
	int result_vector[VECTOR_DIMENSIONS] = { 0 }; //Result of muliplication
	
	printf("Please, enter the matrix (3x3) in the reading order:\n");
	//Reading matrix
	for (int i = 0; i < MATRIX_DIMENSIONS; i++) {
		for (int q = 0; q < MATRIX_DIMENSIONS; q++) {
			while (scanf("%d", &matrix3x3[i][q]) != 1) {
				printf("Incorrect input. Trying again from next line. Continue typing matrix from x = %d y = %d element\n", q, i);
				while (getchar() != '\n'); //getting rid of trash
			} 
		}
	}
	printf("Please, enter the vector (3x1) in the reading order:\n");
	//Reading vector
	for (int i = 0; i < VECTOR_DIMENSIONS; i++) {
		while (scanf("%d", &vector[i]) != 1) {
			printf("Incorrect input. Trying again from next line. Continue typing vector from %d element\n",i);
			while (getchar() != '\n'); //getting rid of trash
		}
	}

	//Printing given matrix
	printf("\nMatrix 3x3:\n");
	for (int i = 0; i < MATRIX_DIMENSIONS; i++) {
		for (int q = 0; q < MATRIX_DIMENSIONS; q++) {
			printf("%d ", matrix3x3[i][q]);
		}
		printf("\n");
	}

	//Printing given vector
	printf("\nVector 3x1:\n");
	for (int i = 0; i < VECTOR_DIMENSIONS; i++) {
		printf("%d\n", vector[i]);
	}

	//Multiplying vector by matrix
	multiply_vector_by_matrix(result_vector, vector, matrix3x3);

	//Printing result
	printf("\nRESULT:\n");
	for (int q = 0; q < VECTOR_DIMENSIONS; q++) {
		printf("%d\n", result_vector[q]);
	}
	return 0;
}

//Multiplies vector by matrix
void multiply_vector_by_matrix(int *result_vector, const int vector_to_multiply[], const int multiplying_matrix[][MATRIX_DIMENSIONS]) {
	if (MATRIX_DIMENSIONS != VECTOR_DIMENSIONS) { //Multiplication is impossible. Dimensions are not equal.
		return;
	}

	for (int i = 0; i < MATRIX_DIMENSIONS; i++) { //Matrix by vector multiplication
		for (int q = 0; q < MATRIX_DIMENSIONS; q++) {
			result_vector[i] += multiplying_matrix[i][q] * vector_to_multiply[q];
		}
	}
}