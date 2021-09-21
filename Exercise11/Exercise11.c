#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

#define INPUT_FILENAME "in.txt"
#define OUTPUT_FILENAME "in.txt"
#define N_OF_NUMBERS 3

//Reads and returns array of numbers from the received file.
//Warns the user if the formatting was incorrect.
float *read_numbers(FILE *input_file, int count);

//Sorts numbers in ascending order inside the numbers array.
void sort_numbers(float *numbers, int count);

//Writes array of numbers into the given file.
void write_numbers(FILE *output_file, float *numbers, int count);

int main() {
	FILE *file = fopen(INPUT_FILENAME, "r");
	float *numbers = NULL; //Pointer to the array with numbers

	numbers = read_numbers(file, N_OF_NUMBERS);
	fclose(file);

	if (numbers == NULL) { //Impossible to continue. Better to exit right away.
		return 1;
	}

	sort_numbers(numbers, N_OF_NUMBERS);

	file = fopen(OUTPUT_FILENAME, "w");
	write_numbers(file, numbers, N_OF_NUMBERS);
	fclose(file);
	free(numbers);

	return 0;
}

//Reads and returns array of numbers from the received file.
//Warns the user if the formatting was incorrect.
float *read_numbers(FILE *input_file, int count) {
	bool bad_formatting = false; //Flag to check if the data formatting was incorrect
	float *numbers = (float *) malloc(sizeof(float) * count); //Pointer to the array with numbers
	float number = 0; //Number to read.

	if (input_file == NULL) { //No file
		printf("No file found.\n");
		return NULL;
	}

	if (numbers == NULL) { //No memory
		printf("Was unable to allocate memory for array of numbers.\n");
		return NULL;
	}

	for (int i = 0; i < count; i++) {
		while (fscanf(input_file, "%f", &number) != 1) {
			bad_formatting = true;
			if (feof(input_file)) { //If encountered end of file -> Do not continue
				break;
			}
			else { //Skip char, it is still possible to get more values
				fgetc(input_file);
			}
		}
		numbers[i] = number;
		number = 0; //'Zeroing' the value in order to get 0 if the input will be incorrect
	}
	if (bad_formatting) printf("Wrong number count in the file.\nShould be %d.\n", count);

	return numbers;
}

//Sorts numbers in ascending order inside the numbers array.
void sort_numbers(float *numbers, int count) {
	float *temp = (float *) malloc(sizeof(float)); //Temporary variable to change places of array elements.

	if (temp == NULL) { //No memory
		printf("Was unable to allocate memory for temporary float.\n");
		return;
	}

	if (numbers == NULL) { // Additional check for NULL pointer, just in case.
		printf("Received NULL pointer as an array of numbers.\n");
		return;
	}

	for (int i = 1; i < count; i++){ //Simple array sorting, moving the 'heaviest' element to the end and looping again, exluding last element.
		for(int q = 0; q < count - i; q++) {
			if (numbers[q] > numbers[q + 1]) {
				*temp = numbers[q];
				numbers[q] = numbers[q + 1];
				numbers[q + 1] = *temp;
			}
		}
	}
	free(temp);
}

//Writes array of numbers into the given file.
void write_numbers(FILE *output_file, float *numbers, int count) {
	if(output_file == NULL) { //No file
		printf("No output file given.\n");
		return;
	}

	if (numbers != NULL) { // additional check for NULL pointer, just in case.
		for (int i = 0; i < count; i++) {
			fprintf(output_file, "%.2f\n", numbers[i]);
		}
	}
}