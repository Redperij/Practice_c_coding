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
//Reads file with data writing it by bytes into the data_array. One chunk of data equals chunk_size bytes. Counts number of chunks with data.
void read_file(FILE *file, CHUNK **data_array, unsigned int *count, const unsigned int chunk_size);
//Gets the file from the user. (Accepts names with whitespaces)
FILE *get_file();
//Gets rid of the newline characters in the string.
void no_newline(char **string);

int main() {
	FILE *in_file;
	CHUNK *dff; //Data from file
	unsigned int count = 0; //Data chunks count.

	in_file = get_file(); //Getting opened file.
	read_file(in_file, &dff, &count, CHUNK_SIZE); //Reading all data chunks.
	fclose(in_file);

	if (dff == NULL) { //If reading returned null pointer, then the file was empty or memory allocation failed.
		return 1;
	}

	//DEBUG_PRINT
	for (unsigned int i = 0; i < count; i++) {
		printf("%d (%d byte):", i + 1, dff[i].size);
		for (int q = 0; q < dff[i].size; q++) {
			
			printf("%x ", dff[i].data[q]);
		}
		printf("\n");
	}
	//DEBUG_PRINT
	
	//Calculating checksums.
	for (unsigned int i = 0; i < count; i++) {
		dff[i].crc = crc16(&dff[i].data, dff[i].size);
	}

	printf("\nChecksum calculation:\n");
	for (unsigned int i = 0; i < count; i++) {
		printf("%d (%d byte): %x\n", i + 1, dff[i].size, dff[i].crc);
	}

	free(dff);

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

//Reads file with data writing it by bytes into the data_array. One chunk of data equals chunk_size bytes. Counts number of chunks with data.
void read_file(FILE *file, CHUNK **data_array, unsigned int *count, const unsigned int chunk_size) {
	*data_array = (CHUNK *)malloc(sizeof(CHUNK)); //Allocating memory for the first chunk.
	unsigned int i = 0; //Used to read file byte by byte.
	*count = 1; //Counts number of chunks.

	if (*data_array == NULL) {
		return;
	}

	data_array[0][0].size = 0;

	//Loop through the whole file slowly reading it byte by byte.
	while (!feof(file) && !ferror(file)) {
		//We are reding data chunk until it haven't reached specified size.
		if (i < chunk_size && fread(&data_array[0][(*count) - 1].data[i], 1, 1, file) == 1) {
			i++; //Increase byte to be read,
			data_array[0][(*count) - 1].size++; //Size is one byte bigger.
		}
		//Reached chunk size. Reallocating space for another chunk.
		else if (i >= chunk_size){
			CHUNK *temp_data_chunk; //Temporary pointer to the array of data chunks.

			(*count)++; //Number of chunks increases.
			i = 0; //Bytes are zeroed.
			temp_data_chunk = (CHUNK *)realloc(*data_array, (*count) * sizeof(CHUNK)); //Reallocating space for additional data chunk
			if (temp_data_chunk == NULL) { //Realloc can fail, it is critical mistake -> return with the ammount of data we already have.
				(*count)--; //Was unable to increase count.
				return;
			}
			//Reallocated space -> get the correct pointer back.
			*data_array = temp_data_chunk;
			data_array[0][(*count) - 1].size = 0; //Size of the new chunk is zero.
		}
		//Since reading returned 0 on the last chunk and there was no more data -> right ammount of data was given in the file or it was empty.
		else if (i == 0) {
			CHUNK *temp_data_chunk; //Temporary pointer to the array of data chunks.

			(*count)--; //It is important to match allocated space in memory with actual number of chunks collected.
			temp_data_chunk = (CHUNK *)realloc(*data_array, (*count) * sizeof(CHUNK)); //Reallocating space for the whole array of data chunks. (Shrinking it)
		
			//Either realloc failed or the file was empty. In any case, it is critical mistake -> exit function.
			if (temp_data_chunk == NULL) {
				//If file was empty, we want to return NULL pointer. Realloc already made sure to free up the space.
				if (*count == 0) {
					*data_array = NULL;
				}
				return;
			}
			//Realloc successful -> get correct pointer back. 
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
		printf("\nNo such file found. Enter the name again\nPlease, enter the name of the file: ");
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