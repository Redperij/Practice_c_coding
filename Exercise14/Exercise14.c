#pragma warning(disable:4996) //disabling warning

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME 255
#define CHUNK_SIZE 64

typedef struct chunk {
	uint8_t data[CHUNK_SIZE];
	uint16_t size;
	uint16_t crc;
} CHUNK;

//CRC-calculation
uint16_t crc16(const uint8_t *data_p, unsigned int length);

void read_file(FILE *file, CHUNK **data_array, int *count);
//Gets the file from the user. (Accepts names with whitespaces)
FILE *get_file();
//Gets rid of the newline characters in the string.
void no_newline(char **string);

int main() {
	FILE *in_file;
	CHUNK *dff; //Data from file
	unsigned int count = 0; //Data chunks count.

	in_file = get_file();
	read_file(in_file, &dff, &count);

	for (int i = 0; i < count; i++) {
		printf("%d (%d byte):", i + 1, dff[i].size);
		for (int q = 0; q < dff[i].size; q++) {
			
			printf("%x ", dff[i].data[q]);
		}
		printf("\n");
	}
	
	return 0;
}

//CRC-calculation
uint16_t crc16(const uint8_t *data_p, unsigned int length) {
	uint8_t x;
	uint16_t crc = 0xFFFF;
	while (length--) {
		x = crc >> 8 ^ *data_p++;
		x ^= x >> 4;
		crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
	}
	return crc;
}

//EASY
void read_file(FILE *file, CHUNK **data_array, unsigned int *count) {
	*data_array = (CHUNK *)malloc(sizeof(CHUNK));
	int i = 0;
	*count = 1;

	if (*data_array == NULL) {
		return;
	}

	data_array[0][0].size = 0;

	while (!feof(file) && !ferror(file)) {
		if (i < CHUNK_SIZE && fread(&data_array[0][(*count) - 1].data[i], 1, 1, file) == 1) {
			i++;
			data_array[0][(*count) - 1].size++;
		}
		else if (i >= CHUNK_SIZE){
			CHUNK *temp_data_chunk;

			(*count)++;
			i = 0;
			temp_data_chunk = (CHUNK *)realloc(*data_array, (*count) * sizeof(CHUNK));
			if (temp_data_chunk == NULL) {
				return *data_array;
			}
			*data_array = temp_data_chunk;
			data_array[0][(*count) - 1].size = 0;
		}
		else if (i == 0) { //Since reading returned 0 on the last chunk and there was no more data -> right ammount of data was given in the file. 
			CHUNK *temp_data_chunk;

			(*count)--;
			temp_data_chunk = (CHUNK *)realloc(*data_array, (*count) * sizeof(CHUNK));
			if (temp_data_chunk == NULL) {
				return *data_array;
			}
			*data_array = temp_data_chunk;
		}
	}
}

//Gets the file from the user. (Accepts names with whitespaces)
FILE *get_file() {
	char *filename = (char *) malloc(MAX_FILENAME * sizeof(char));
	FILE *file = NULL;
	int length = 0;

	printf("Please, enter the name of the file: ");
	fgets(filename, MAX_FILENAME, stdin);
	no_newline(&filename);

	file = fopen(filename, "rb");
	while (file == NULL) { //Requesting user to give correct filename.
		printf("\nNo such file found, please enter the name again: ");
		fgets(filename, MAX_FILENAME, stdin);
		no_newline(&filename);
		file = fopen(filename, "rb");
	}
	return file;
}

//Gets rid of the newline characters in the string.
void no_newline(char **string) {
	for (unsigned int i = 0; i < strlen(*string); i++) {
		if (string[0][i] == '\n') {
			string[0][i] = '\0';
		}
	}
}