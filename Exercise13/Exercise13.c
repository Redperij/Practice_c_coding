#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#define OUTPUT_FILENAME "out.txt"
#define EST_STR_LEN 16 //Estimated String Length

//Reads whole line from the console and returns size of it. Gets rid of the line feed.
//Returns 0 if it is impossible.
unsigned int read_line(char **string, unsigned int len);
//Merges two strings into one.
//Strings, passed as parameters are lost in case of success.
//Returns merged string in case of success and NULL otherwise.
char *merge_strings(char *string1, char *string2);
//Writes string to the specified file.
void write_string(FILE *output_file, char *str_to_write, int len);

int main() {
	FILE *output_file; //File to write strings into.
	char *input_string = NULL; //String from input.
	bool stop = false; //Flag. Stops the execution.
	unsigned int string_length; //Length of the string, returned by the read_line function.
	int inc_i = 0; //Incorrect iterations


	do {
		string_length = read_line(&input_string, EST_STR_LEN);
		//Was able to get string. Continue.
		if(input_string != NULL) {
			inc_i = 0; //This iteration is correct. Stop counting incorrect iterations.

			if (input_string[0] != '.' || strlen(input_string) > 2) { //If it is not a stopping dot -> continue.
				if (input_string[0] != '\0') { //If it is not blank string -> write it to file.
					output_file = fopen(OUTPUT_FILENAME, "a");
					write_string(output_file, input_string, string_length);

					fclose(output_file);
					free(input_string);
				}
			}
			else { //It is stopping dot. Exit the loop.
				stop = true;
			}
		}
		//This iteration failed due to the memory allocation. Try again until 100th time.
		else {
			inc_i++;
		}
	} while (!stop && inc_i < 100);

	//Notify user, if the programm closed due to the lack of memory.
	if(inc_i >= 100) {
		printf("Not enough memory.\n");
		return 1;
	}
	return 0;
}

//Reads whole line from the console and returns size of it. Gets rid of the line feed.
//Returns 0 if it is impossible.
unsigned int read_line(char **string, unsigned int len) {
	*string = (char *) malloc(sizeof(char) * len);
	char *temp_string;
	int prev_length = 0; //Used to save data in case of failure. Remmembers length before the allocation.

	if (string == NULL) { //Stop the execution if lacking memory.
		return 0;
	}

	//Getting user input.
	printf("\nPlease, write your string.\nOr stop the execution with single '.' on a line.\n> ");
	fgets(*string, len, stdin);

	//No line feed at the end of the string -> Continue reading the line.
	if (string[0][strlen(*string) - 1] != '\n') {
		do {
			temp_string = (char *) malloc(len * sizeof(char)); //Initializing temporary string in order to read additional data and be safe from memory leaks.
			fgets(temp_string, len, stdin); //Reading more data.
			temp_string = merge_strings(*string, temp_string); //Trying to merge new data with the old.

			//Failed to merge strings. (Memory reallocation failed)
			//Attempting to save all the old data inside existing string.
			if (temp_string == NULL) {
				prev_length = strlen(*string);
				temp_string = realloc(*string, strlen(*string) * sizeof(char) + 1);
				if (temp_string == NULL) { //Failed to reallocate memory for additional char. Last sybmbol will be lost.
					string[0][strlen(*string) - 1] = '\n';
				}
				else { //Managed to save last symbol.
					*string = temp_string;
					string[0][prev_length + 1] = '\0';
					string[0][prev_length] = '\n';
				}
			}

			//Merge succeded. Continue reading line.
			else {
				*string = temp_string; //Use new pointer.
			}
		} while (string[0][strlen(*string) - 1] != '\n'); //If the line feed was read -> stop reading additional data.

	}

	if (strlen(*string) < len) { //If the string is lesser than allocated memory for it -> shrink allocated memory.
		int temp_len = strlen(*string);
		char *temp_string = realloc(*string, temp_len * sizeof(char) + 1); //Being safe and using temporary pointer to string in order not to lose data.
		if (temp_string != NULL) { //In case of success get the new pointer.
			*string = temp_string;
		}
	}
	for (int i = 0; i < strlen(*string); i++) { //Getting rid of the line feed. Looping through the whole string just to be sure.
		if (string[0][i] == '\n' || string[0][i] == '\r') {
			string[0][i] = '\0';
		}
	}

	len = strlen(*string);
	return len;
}

//Merges two strings into one.
//Strings, passed as parameters are lost in case of success.
//Returns merged string in case of success and NULL otherwise.
char *merge_strings(char *string1, char *string2) {
	if (string1 == NULL || string2 == NULL) { //No point to continue operation.
		//One of the strings given is null pointer.
		return NULL;
	}

	int new_string_len = strlen(string1) + strlen(string2); //Length of the merged string.
	int str1_len = strlen(string1); //Length of the first string in order not to lose it after memory reallocation.
	int q = 0; //Loops through second string.
	char *temp_string = realloc(string1, sizeof(char) * new_string_len + 1); //String to merge strings into.

	if (temp_string == NULL) { //No point to continue operation. Memory reallocation failed
		return NULL;
	}

	//In simple words this loop moves second string at the end of the first.
	//It loops through spare space of merged string, which already contains first string due to the memory reallocation
	//and through second string chars.
	for (int i = str1_len; i < new_string_len; i++) {
		temp_string[i] = string2[q];
		q++;
	}
	temp_string[new_string_len] = '\0'; //Adding null terminator in the end.

	free(string2); //Free memory for the second string. First got reallocated, so no need to free the memory for it.

	return temp_string;
}

//Writes string to the specified file.
void write_string(FILE *output_file, char *str_to_write, int len) {
	if (output_file == NULL) { //No output file provided.
		return;
	}
	fprintf(output_file, "%d:%s\n", len, str_to_write);
	printf("Successfully written to file.\n");
}