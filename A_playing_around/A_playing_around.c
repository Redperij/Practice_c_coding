#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#define OUTPUT_FILENAME "out.txt"
#define EST_STR_LEN 4 //Estimated String Length = 2^7

int read_string(char **string, int len);

char *merge_strings(char *string1, char *string2);

int main() {
	FILE *output_file = fopen(OUTPUT_FILENAME, "a");
	char *string1 = (char *)malloc(7 * sizeof(char)); //
	char *string2 = (char *)malloc(7 * sizeof(char));
	char *string3 = (char *) malloc(4 * sizeof(char));
	
	string1[0] = 'M';
	string1[1] = 'u';
	string1[2] = 't';
	string1[3] = 'h';
	string1[4] = 'e';
	string1[5] = 'r';
	string1[6] = '\0';

	string2[0] = 'F';
	string2[1] = 'u';
	string2[2] = 'c';
	string2[3] = 'k';
	string2[4] = 'e';
	string2[5] = 'r';
	string2[6] = '\0';
	printf("String1: %s\nString2: %s\n", string1, string2);
	string1 = merge_strings(string1, string2);
	printf("String1: %s\nString2: %s\n", string1, string2);
	return 0;
}

int read_string(char **string, int len) {
	*string = (char *) malloc(sizeof(char) * (len));

	printf("Write string: ");

	printf("Inside LOOP:\n");
	do {
		fgets(*string, len, stdin);
		printf("%d: %s", strlen(*string), *string);
	} while (*string[0] != '.' || strlen(*string) > 2);

	printf("\nInside:\n%s\n", *string);

	return len;
}

char *merge_strings(char *string1, char *string2) {
	if (string1 == NULL || string2 == NULL) { //No point to continue operation.
		//One of the strings given is null pointer.
		return NULL;
	}
	int new_string_len = strlen(string1) + strlen(string2);
	int str1_len = strlen(string1);
	int q = 0;
	char *temp_string = NULL;
	temp_string = realloc(string1, sizeof(char) * new_string_len);

	if (temp_string == NULL) { //No point to continue operation.
		return NULL;
	}
	for (int i = str1_len; i < new_string_len; i++) {
		temp_string[i] = string2[q];
		q++;
	}
	temp_string[new_string_len] = '\0';

	free(string2);

	return temp_string;
}