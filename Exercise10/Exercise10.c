#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

#define INPUT_FILENAME "in.txt"

//Counts average of 'n' numbers from given file
int countAverageFromFile(FILE* input_file, float* average, const int n, bool* bad_formatting);

int main() {
	FILE *input_file = fopen(INPUT_FILENAME, "r"); //Input file
	int n = 0; //Ammount of numbers in the file
	int read_numbers_count = 0; //Ammount of numbers actually read from file
	float average = 0; //Average of numbers from file

	bool bad_formatting = false; //Checks if format of the file was corrupted. Used only to notify the user.

	if (input_file == NULL) {
		printf("Unable to open file '%s'\n", INPUT_FILENAME);
	}
	else {
		while (fscanf(input_file, "%d", &n) != 1) { //Reading first number indicating the ammount of numbers in the file
			bad_formatting = true;
			fgetc(input_file);
		}

		if (n <= 0) { //There is no point to continue, if there is no positive number in the file
			printf("Text formatting is flawed. First number must be greater than 0.\n");
		}
		else {
			read_numbers_count = countAverageFromFile(input_file, &average, n, &bad_formatting);

			fclose(input_file); //closing file

			//Printing the result
			if (bad_formatting) {
				printf("File formatting is flawed. There should be no text.\nResult can be incorrect!\n\n");
			}
			if (read_numbers_count != n && read_numbers_count == 0) {
				printf("Error! Tried to read %d numbers, %d numbers were read.\n", n, read_numbers_count);
			}
			else if (read_numbers_count != n) {
				printf("Error! Tried to read %d numbers, %d numbers were read.\n", n, read_numbers_count);
				printf("Average of %d numbers is: %.2f\n", read_numbers_count, average);
			}
			else {
				printf("Average of %d numbers is: %.2f\n", read_numbers_count, average);
			}
		}
	}


	return 0;
}

//Counts average of 'n' numbers from file
//Returns number count or 0.
int countAverageFromFile(FILE *input_file, float *average, const int n, bool *bad_formatting) {
	int actual_numbers = 0; //Counts successfully read numbers
	float input; //Gets input from file.

	bool number_exists = true; //Used to check if the last number exists in the file

	if (input_file == NULL) {
		printf("File '%s' is not opened.\n", INPUT_FILENAME);
	}
	//Behaviour:
	//Tries to read number from file. If that fails -> Tries to skip one character and repeats the process.
	else {
		while (actual_numbers < n && !feof(input_file)) {
			number_exists = true; //Assume there is a number
			while (fscanf(input_file, "%f", &input) != 1 && number_exists) { //Trying to read number
				if (fgetc(input_file) == EOF) { //If encountered end of file -> There is no number ahead
					number_exists = false;
				}
				*bad_formatting = true;
			}
			if (number_exists) { //This will be handled only if the assumption was correct
				*average += input;
				actual_numbers++;
			}
		}
		*average = *average / actual_numbers; //Counting average
		return actual_numbers;
	}
	return 0; //If no opened file was provided -> return 0
}