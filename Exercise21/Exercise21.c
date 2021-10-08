#pragma warning(disable:4996) //disabling warning

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_FILENAME 255
#define EXISTING_CHARS 256 // 0 - 255
#define FREQUENT_N 10 //How much most frequent chars to find.

/*
* Gets the file from the user. (Accepts names with whitespaces)
*/
void get_filename(char **filename);
/*
* Clears out all '\n' from the string.
*/
void clear_newlines(char **string);
/*
* Checks if character is met in the char_array of 'size' size.
* Returns 1 if it is, 0 if it is not.
*/
int is_in_char_array(const uint8_t character, const uint8_t *char_array, const size_t size);

int main(void)
{	
	unsigned int count[EXISTING_CHARS] = { 0 }; //Holds number of times 'n' character is met ('n' is met 'count[n]' times) 
	uint8_t most_frequent[FREQUENT_N]; //Holds most frequently met characters from max to min.
	int n_freq = -1; //Counts number of most_frequent characters.
	unsigned int max = 0; //Used to find maximum value in the count[] array.
	uint8_t byte = 0; //Holds input byte.
	char *filename = NULL;
	FILE *file = NULL;
	uint8_t flag_found = 0; //Flag. Used to check if loop found a char.
	
	get_filename(&filename);
	file = fopen(filename, "rb");

	if (file != NULL) {
		//Read file byte by byte and increase count of corresponding to the byte place in the count array.
		while (!feof(file) && !ferror(file)) {
			fread(&byte, 1, 1, file);
			count[byte]++;
		}
		fclose(file); //Closing file before horrible loop.

		//Looping until there are no characters met or most_frequent array is filled.
		do {
			n_freq++; //Loop will break if haven't found character met at least once.
			max = 0; //reset max
			flag_found = 0; //reset flag

			//Looping through all characters.
			for (unsigned int i = 0; i < EXISTING_CHARS; i++) {
				//If it was met more than the previous saved one and it wasn't already taken -> take it as a most frequent.
				if (count[i] > max && !is_in_char_array(i, most_frequent, n_freq)) {
					max = count[i];
					most_frequent[n_freq] = i; //'i' is a character and a place in the count[] array.
					flag_found = 1; //Set flag.
				}
			}
		} while (max != 0 && n_freq != FREQUENT_N - 1);
		if(flag_found) n_freq++; //if found last character -> we have FREQUENT_N characters, so increase by one.

		//Printing the result.
		if (n_freq == FREQUENT_N) {
			printf("%d most frequent characters are:\n", FREQUENT_N);
		}
		else {
			printf("The only %d characters are:\n", n_freq);
		}
		for (unsigned int i = 0; i < n_freq; i++) {
			if (isprint(most_frequent[i])) {
				printf("%d (%c): %d times\n", most_frequent[i], most_frequent[i], count[most_frequent[i]]);
			}
			else {
				printf("%d: %d times\n", most_frequent[i], count[most_frequent[i]]);
			}
			
		}

		//Cleaning.
		free(filename);
		filename = NULL;
	}
	else {
		printf("File not found.\n");
	}

	return 0;
}

/*
* Gets the file from the user. (Accepts names with whitespaces)
*/
void get_filename(char **filename) {
	*filename = (char *)malloc(MAX_FILENAME * sizeof(char));
	FILE *file = NULL;
	int length = 0;

	printf("Please, enter the name of the file: ");
	fgets(*filename, MAX_FILENAME, stdin);
	clear_newlines(filename);

	if (strlen(*filename) == 0) { //Mistake with newline is possible. Just throwing in another fgets not to be confused by random error. (If everything is in order, size won't be 0)
		fgets(*filename, MAX_FILENAME, stdin);
		clear_newlines(filename);
	}

	file = fopen(*filename, "rb");
	while (file == NULL) { //Requesting user to give correct filename.
		printf("\nNo such file found. Enter the name again\nPlease, enter the name of the file: ");
		fgets(*filename, MAX_FILENAME, stdin);
		clear_newlines(filename);
		file = fopen(*filename, "rb");
	}
	rewind(file); //Returning to the start.
	fclose(file); //Closing the file.
}

/*
* Clears out all '\n' from the string.
*/
void clear_newlines(char **string) {
	if (*string == NULL) return; //String doesn't exist -> escape.

	int i = 0;
	int w = strlen(*string);
	for (i = 0; i < w; i++) {
		if (string[0][i] == '\n' || string[0][i] == '\r') {
			for (int q = i + 1; q <= w; q++) {
				string[0][q - 1] = string[0][q];
			}
			w--;
			i--;
		}
	}
}

/*
* Checks if character is met in the char_array of 'size' size.
* Returns 1 if it is, 0 if it is not.
*/
int is_in_char_array(const uint8_t character, const uint8_t *char_array, const size_t size) {
	for (unsigned int i = 0; i < size; i++) {
		if (character == char_array[i]) {
			return 1;
		}
	}
	return 0;
}