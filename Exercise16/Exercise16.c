#pragma warning(disable:4996) //disabling warning

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME 255

//Gets the file from the user. (Accepts names with whitespaces)
void get_filename(char **filename);
//Gets rid of the newline characters in the string. Terminates string on the first newline.
void no_newline(char **string);
//Reads whole file into the data_arr in binary format.
uint32_t read_file(FILE *file, uint8_t **data_arr);
//Writes array to the specified file.
void write_to_file(const FILE *file, const uint8_t **data_arr, const uint32_t size);
/* 
 * Gets offset from the user.
 * Returns offset.
*/
unsigned int get_offset(const uint32_t size);
/*
 * Gets nymber of bytes to remove from the user.
 * Returns number of bytes to remove.
*/
unsigned int get_n_bytes_to_remove(const uint32_t size, const unsigned int offset);
//Removes n_bytes_to_remove after the offset in the array.
void remove_bytes(uint8_t **data_arr, uint32_t *size, const unsigned int offset, const unsigned int n_bytes_to_remove);
//Removes element from the array.
void rm_array_element(uint8_t **data_arr, unsigned int pos, unsigned int *size);

int main() {
	FILE *file = NULL; //File to work with.
	char *filename = NULL; //String with name of the file.
	uint8_t *data_arr = NULL; //Array with data in binary format.
	uint32_t size; //Size of the array.

	get_filename(&filename);

	//Reading the file.
	file = fopen(filename, "rb");
	size = read_file(file, &data_arr);
	fclose(file);

	if (size > 0) {
		unsigned int offset = 0; //offset from the start of the file.
		unsigned int n_bytes_to_remove = 0; //Number of bytes to remove.
		//Asking user to enter those:
		printf("Size of the file is: %d bytes\nPlease, enter offset from the beginning and number of bytes to remove.\n", size);
		offset = get_offset(size);
		n_bytes_to_remove = get_n_bytes_to_remove(size, offset);

		//Removing specified bytes.
		remove_bytes(&data_arr, &size, offset, n_bytes_to_remove);

		//Writing new contents to the file.
		file = fopen(filename, "wb");
		write_to_file(file, &data_arr, size);
		fclose(file);
	}
	else {
		printf("Error. Was unable to process the file or it is empty.\n");
	}

	//Emptying memory.
	if (data_arr != NULL) {
		free(data_arr);
	}

	return 0;
}

//Reads whole file into the data_arr in binary format.
uint32_t read_file(FILE *file, uint8_t **data_arr) {
	uint32_t size = 0; //Size of the array.
	//*data_arr must be null pointer.
	if (*data_arr != NULL) {
		free(*data_arr);
		*data_arr = NULL;
	}

	*data_arr = malloc(sizeof(uint8_t));

	//Check for file pointer and memory allocation for data array.
	if (file == NULL || *data_arr == NULL) {
		printf("uint32_t read_file(FILE *file, uint8_t **data_arr) :\n");
		if (file == NULL) printf("Unable to process the file.\n");
		if (*data_arr == NULL) printf("Memory allocation error.\n");
		return size;
	}

	size++; //Esimating, that there is data in the file.

	//We are reading data until it haven't reached the end of file.
	while (!feof(file) && !ferror(file)) {
		//Reading byte by byte.
		if (fread(&(data_arr[0][size - 1]), 1, 1, file) == 1) {
			uint8_t *temp_arr = realloc(*data_arr, (size + 1) * sizeof(uint8_t)); //Reallocating space for another element.
			if (temp_arr == NULL) { //If reallocation fails -> return what was read.
				printf("uint32_t read_file(FILE *file, uint8_t **data_arr) :\n");
				printf("Memory reallocation error.\n");
				return size;
			}
			size++; //Reallocation succeeded -> estimating to get another byte.
			(*data_arr) = temp_arr; //Getting correct pointer.
		}
		else { //Got no data -> decrease size by one to match array contents.
			size--;
		}
	}

	return size;
}

//Writes array to the specified file.
void write_to_file(const FILE *file, const uint8_t **data_arr, const uint32_t size) {
	for (int i = 0; i < size; i++) {
		fprintf(file, "%c", data_arr[0][i]);
	}
	printf("File contents successfully overwritten.\n");
}

//Gets the filename from the user. (Accepts names with whitespaces)
void get_filename(char **filename) {
	*filename = (char *)malloc(MAX_FILENAME * sizeof(char));
	FILE *file = NULL;
	int length = 0;

	printf("Please, enter the name of the file: ");
	fgets(*filename, MAX_FILENAME, stdin);
	no_newline(filename);

	if (strlen(*filename) == 0) { //Mistake with newline is possible. Just throwing in another fgets not to be confused by random error. (If everything is in order, size won't be 0)
		fgets(*filename, MAX_FILENAME, stdin);
		no_newline(filename);
	}

	file = fopen(*filename, "rb");
	while (file == NULL) { //Requesting user to give correct filename.
		printf("\nNo such file found. Enter the name again\nPlease, enter the name of the file: ");
		fgets(*filename, MAX_FILENAME, stdin);
		no_newline(filename);
		file = fopen(*filename, "rb");
	}
	rewind(file); //Returning to the start.
	fclose(file); //Closing the file.
}

//Gets rid of the newline characters in the string. Terminates string on the first newline character.
void no_newline(char **string) {
	for (unsigned int i = 0; i < strlen(*string); i++) {
		if (string[0][i] == '\n') {
			string[0][i] = '\0';
		}
	}
}
/*
 * Gets offset from the user.
 * Returns offset.
*/
unsigned int get_offset(const uint32_t size) {
	int offset = 0;

	printf("Offset: ");
	do {
		while (scanf("%d", &offset) != 1) {
			while (getchar() != '\n'); //getting rid of trash
			printf("Incorrect input. Enter a non-negative number\nOffset: ");
		}
		if (offset < 0) { //offset is positive or 0
			printf("Incorrect input. Enter a non-negative number\nOffset: ");
		}
		else if (offset >= size) { //offset is less than size
			printf("Incorrect input. Offset cannot be bigger or equal to the file size.\nOffset: ");
		}
	} while (offset < 0 || offset >= size);

	return offset;
}
/*
 * Gets nymber of bytes to remove from the user.
 * Returns number of bytes to remove.
*/
unsigned int get_n_bytes_to_remove(const uint32_t size, const unsigned int offset) {
	int n_bytes_to_remove = 0;

	printf("Number of bytes to remove: ");
	do {
		while (scanf("%d", &n_bytes_to_remove) != 1) {
			while (getchar() != '\n'); //getting rid of trash
			printf("Incorrect input. Enter a non-negative number\nNumber of bytes to remove: ");
		}
		if (n_bytes_to_remove < 0) { //n_bytes_to_remove is positive or 0
			printf("Incorrect input. Enter a non-negative number\nNumber of bytes to remove: ");
		}
		else if (offset + n_bytes_to_remove > size) { //(n_bytes_to_remove + offset) is less than size
			printf("Incorrect input. You cannot remove bytes past the end of the file.\nMaximum bytes to remove: %d\nNumber of bytes to remove: ", size - offset);
		}
	} while (n_bytes_to_remove < 0 || offset + n_bytes_to_remove > size);

	return n_bytes_to_remove;
}

//Removes n_bytes_to_remove after the offset in the array.
void remove_bytes(uint8_t **data_arr, uint32_t *size, const unsigned int offset, const unsigned int n_bytes_to_remove) {
	unsigned int i = offset; //Start iterations from the offset. (It marks first element to remove in the array)
	unsigned int removing = n_bytes_to_remove; //Number of bytes to remove.
	uint8_t *temp_arr = NULL; //Temporary array for memory reallocation.

	//Nothing to remove -> escape unnecessary checks.
	if (n_bytes_to_remove == 0) {
		return;
	}

	//Removing elements one by one if still left bytes to remove and haven't exceeded size. (Check for size should be redundant. It is here to be safe )
	while (i < (*size) && removing != 0) {
		rm_array_element(data_arr, i, size); //Removing element.
		removing--;
	}

	//Reallocating memory for the array if possible. (Shrinking it)
	temp_arr = realloc(*data_arr, (*size) * sizeof(uint8_t));
	if (temp_arr != NULL) {
		*data_arr = temp_arr; //Getting correct pointer.
	}
	else if ((*size) == 0) { //User decided to remove all contents from the file.
		*data_arr = NULL;
	}
}

//Removes element from the array.
void rm_array_element(uint8_t **data_arr, unsigned int pos, unsigned int *size) {
	for (int i = pos + 1; i < (*size); i++) {
		data_arr[0][i - 1] = data_arr[0][i];
	}
	(*size)--;
}