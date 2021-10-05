#pragma warning(disable:4996) //disabling warning

/*
You need to implement two functions: binary_reader and digit_counter

Binary reader takes a pointer to unsigned int as parameter and returns boolean value.
True means that a binary number was successfully read.
Call read_line to read strings that are to be converted to unsigned integers.
Read_line returns true when a line was successfully read and false when the end of input is reached.

Each valid input line contains a binary number that starts with prefix "0b".
There may be white space at beginning of the line before prefix.
When a valid prefix is found binary reader reads binary digits until it reads a character that is not a '0' or '1'.
Hint: when a valid digit is found shift the current value to left and and set the new value to least significant bit.

Digit counter returns the minimum number of hexadecimal digits that are needed to print the number given as parameter.
Hint: you need find the leftmost group of 4 bits that have value greater than 0.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/* call this function to read input */
bool read_line(char *line, size_t size);

/* you need to implement the following two functions */

bool binary_reader(unsigned int *pu);

int digit_counter(unsigned int nr);

FILE *file;

int main(int arcg, char **argv)
{
	unsigned int number = 0;

	file = fopen("in.txt", "r");

	while (binary_reader(&number)) {
		printf("%11u: %08X, %d\n", number, number, digit_counter(number));
	};

	fclose(file);

	return 0;

}

/* call this function to read input */
bool read_line(char *line, size_t size) {
	char *return_value;
	return_value = fgets(line, size, file);
	if (return_value != NULL) {
		return true;
	}
	return false;
}


bool binary_reader(unsigned int *pu){
	char *line = malloc(65 * sizeof(char)); //Line with read characters.
	char *char_ptr = NULL; //Pointer to char.
	int count = 0; //Counts characters in line.
	bool nostop = true; //Stop flag.
	*pu = 0;

	if (line == NULL) { //malloc() failed -> escape
		return false;
	}

	//Reading line.
	nostop = read_line(line, 64);

	//char_ptr = strstr(line, prefix); //Getting pointer to the prefix.
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] == 'b') {
			char_ptr = &line[i];
			break;
		}
	}

	//No prefix found -> return.
	if (char_ptr == NULL) {
		return false;
	}

	char_ptr++; //Move pointer by 2 characters forward. It is start of the binary number.
	while (*char_ptr == '0' || *char_ptr == '1') {
		if (*char_ptr == '0') {
			(*pu) <<= 1; //Shift one bit left.
		}
		if (*char_ptr == '1') {
			(*pu) <<= 1; //Shift one bit left...
			(*pu) += 1; //and add one.
		}
		char_ptr++; //Move one char forward.
	}

	free(line);
	line = NULL;

	return true;
}


int digit_counter(unsigned int nr){
	unsigned int count = 1;
	nr = nr >> 4;
	while (nr) {
		count++;
		nr = nr >> 4;
	}
	return count;
}
