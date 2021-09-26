#pragma warning(disable:4996) //disabling warning

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME 255

typedef struct chunk {
	uint8_t *data;
	uint16_t size;
	uint16_t capacity;
	uint16_t crc;
} CHUNK;

//CRC-calculation
uint16_t crc16(const uint8_t *data_p, unsigned int length);
//Reads file with data writing it by bytes into the data_array. One chunk of data equals chunk_size bytes. Counts number of chunks with data.
//Return values:
// -1 - non critical memory mistake
// 0 - no mistakes
// 1 - file is empty
// 2 - crytical memory mistake
int read_file(FILE *file, CHUNK **data_array, unsigned int *count, const unsigned int chunk_size);
//Gets the file from the user. (Accepts names with whitespaces)
FILE *get_file();
//Gets rid of the newline characters in the string.
void no_newline(char **string);
//Reads positive size of chunk from user.
unsigned int read_size();
//Asks user to continue or stop the programm.
bool ask_usr_cont();

int main() {
	FILE *in_file;
	CHUNK *dff = NULL; //Data from file
	unsigned int count = 0; //Data chunks count.
	unsigned int chunk_size = 0;
	bool cont = true; //Continues execution of the programm.
	int count_files = 0; //Counts number of files inside the array.
	int mistake_flag = 0;

	while (cont) {
		in_file = get_file(); //Getting opened file.
		chunk_size = read_size(); //Getting chunk size.

		mistake_flag = read_file(in_file, &dff, &count, chunk_size); //Reading all data chunks.
		fclose(in_file);
		if (dff != NULL && (mistake_flag == 0 || mistake_flag == -1)) {
			//Calculating checksums.
			for (unsigned int i = 0; i < count; i++) {
				dff[i].crc = crc16(dff[i].data, dff[i].size);
			}

			//Printing checksums.
			printf("\nList of checksums:\n");
			for (unsigned int i = 0; i < count; i++) {
				printf("%-3d (%3d byte capacity): %4x (%3d byte size)\n", i + 1, dff[i].capacity, dff[i].crc, dff[i].size);
			}
			count_files++;
			printf("----------------------------------------------------------------\n");
			printf("%d chunks read total.\n%d files are included into the array.\n", count, count_files);
		}
		else if (mistake_flag == 1){
			printf("The file is empty.\n");
		}
		else {
			printf("Memory mistake. All data is lost.\n");
			break;
		}
		
		//Asks user to continue or stop the programm.
		cont = ask_usr_cont();
	}
	
	printf("Stopping execution.\n");
	if (dff != NULL) {
		free(dff);
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

//Reads file with data writing it by bytes into the data_array. One chunk of data equals chunk_size bytes. Counts number of chunks with data.
//Return values:
// -1 - non critical memory mistake
// 0 - no mistakes
// 1 - file is empty
// 2 - crytical memory mistake
int read_file(FILE *file, CHUNK **data_array, unsigned int *count, const unsigned int chunk_size) {
	unsigned int i = 0; //Used to read file byte by byte.
	unsigned int local_count = 0;
	(*count)++; //Counts number of chunks.
	if (*data_array == NULL) {
		*data_array = (CHUNK *)malloc(sizeof(CHUNK)); //Allocating memory for the first chunk.
	}
	else {
		CHUNK *temp_data_chunk; //Temporary pointer to the array of data chunks.
		temp_data_chunk = (CHUNK *)realloc(*data_array, (*count) * sizeof(CHUNK)); //Reallocating space for additional data chunk
		if (temp_data_chunk == NULL) { //Realloc can fail, it is critical mistake -> return with the ammount of data we already have.
			(*count)--; //Was unable to increase count.
			return -1;
		}
		//Reallocated space -> get the correct pointer back.
		*data_array = temp_data_chunk;
	}

	if (*data_array == NULL) { //Allocation failed -> critical mistake.
		return 2;
	}
	if (chunk_size <= 0) { //Size of chunk cannot be 0 or lower.
		free(*data_array);
		return 2;
	}

	data_array[0][(*count) - 1].size = 0; //Actual size is 0
	data_array[0][(*count) - 1].capacity = chunk_size; //Potential size of chunk.
	data_array[0][(*count) - 1].data = (uint8_t *) malloc(sizeof(uint8_t) * data_array[0][(*count) - 1].capacity); //Allocating memory for the data chunk.

	if (data_array[0][(*count) - 1].data == NULL) { //Allocation failed -> critical mistake.
		return 2;
	}

	//Loop through the whole file slowly reading it byte by byte.
	while (!feof(file) && !ferror(file)) {
		//We are reading data chunk until it haven't reached specified size.
		if (i < chunk_size && fread(&data_array[0][(*count) - 1].data[i], 1, 1, file) == 1) {
			i++; //Increase byte to be read,
			data_array[0][(*count) - 1].size++; //Size is one byte bigger.
		}
		//Reached chunk size. Reallocating space for another chunk.
		else if (i >= chunk_size) {
			CHUNK *temp_data_chunk = NULL; //Temporary pointer to the array of data chunks.
			local_count++;
			(*count)++; //Number of chunks increases.
			i = 0; //Bytes are zeroed.
			temp_data_chunk = (CHUNK *)realloc(*data_array, (*count) * sizeof(CHUNK)); //Reallocating space for additional data chunk
			if (temp_data_chunk == NULL) { //Realloc can fail, it is critical mistake -> return with the ammount of data we already have.
				(*count)--; //Was unable to increase count.
				return -1;
			}
			//Reallocated space -> get the correct pointer back.
			*data_array = temp_data_chunk;

			data_array[0][(*count) - 1].size = 0; //Actual size of the new chunk is zero.
			data_array[0][(*count) - 1].capacity = chunk_size; //Potential size of the new chunk.
			data_array[0][(*count) - 1].data = (uint8_t *) malloc(sizeof(uint8_t) * data_array[0][(*count) - 1].capacity); //Allocating memory for the new data chunk.

			if (data_array[0][(*count) - 1].data == NULL) { //Allocation failed -> critical mistake.
				return -1;
			}
		}
		//Since reading returned 0 on the last chunk and there was no more data -> right ammount of data was given in the file or it was empty.
		else if (i == 0) {
			CHUNK *temp_data_chunk = NULL; //Temporary pointer to the array of data chunks.

			if (data_array[0][(*count) - 1].data != NULL) { //Freeing allocated memory for data. (I know about the warning, the trick is that after this else statement loop will never continue)
				free(data_array[0][(*count) - 1].data);
			}

			(*count)--; //It is important to match allocated space in memory with actual number of chunks collected.
			temp_data_chunk = (CHUNK *)realloc(*data_array, (*count) * sizeof(CHUNK)); //Reallocating space for the whole array of data chunks. (Shrinking it)

			//The file was empty.
			if (local_count == 0 && temp_data_chunk != NULL) {
				return 1;
			}
			//Either realloc failed or the file was empty. In any case, it is critical mistake -> exit function.
			if (temp_data_chunk == NULL) {
				if (*count == 0) {
					*data_array = NULL;
				}
				if (local_count == 0) { //File was empty. It is not critical, though is handled the other way around.
					return 1;
				}
				return -1;
			}
			//Realloc successful -> get correct pointer back. 
			*data_array = temp_data_chunk;

			break; //It is here to suspend the warning. Can be commented.
		}
	}
	return 0;
}

//Gets the file from the user. (Accepts names with whitespaces)
FILE *get_file() {
	char *filename = (char *)malloc(MAX_FILENAME * sizeof(char));
	FILE *file = NULL;
	int length = 0;

	printf("Please, enter the name of the file: ");
	fgets(filename, MAX_FILENAME, stdin);
	no_newline(&filename);

	if (strlen(filename) == 0) { //Mistake with newline is possible. Just throwing in another fgets not to be confused by random error. (If everything is in order, size won't be 0)
		fgets(filename, MAX_FILENAME, stdin);
		no_newline(&filename);
	}

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

//Reads positive size of chunk from user.
unsigned int read_size() {
	int read_input_size = 0;

	printf("Chunk size in bytes: ");
	do {
		while (scanf("%d", &read_input_size) != 1) {
			while (getchar() != '\n'); //getting rid of trash
			printf("Incorrect input. Enter a positive number\nEnter chunk size: ");
		}
		if (read_input_size <= 0) {
			printf("Incorrect input. Enter a positive number\nEnter chunk size: ");
		}
	} while (read_input_size <= 0);

	return read_input_size;
}

//Asks user to continue or stop the programm.
bool ask_usr_cont() {
	int read_input = 2;
	printf("\nDo you wish to read another file? (0 - no, 1 - yes)\nInput: ");
	do {
		while (scanf("%d", &read_input) != 1) {
			while (getchar() != '\n'); //getting rid of trash
			printf("Incorrect input. Enter 0 or 1\nInput: ");
		}
		if (read_input != 1 && read_input != 0) {
			printf("Incorrect input. Enter 0 or 1\nInput: ");
		}
	} while (!(read_input == 0 || read_input == 1));

	if (read_input == 1) {
		return true;
	}
	else {
		return false;
	}
}