#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINESIZE 15
#define DOT '.'
#pragma warning(disable:4996)

bool get_newline(char *string);
void remove_newline(char *string);

int main() {
	char *input = NULL;
	size_t leftover = 0;
	size_t size = 4;
	FILE *out_file = NULL;
	out_file = fopen("output.txt", "w");
	do
	{
		input = (char *)calloc(size, sizeof(char));
		if (input == NULL) {
			printf("Could not allocate memory");
			break;
		}
		fgets(input, size, stdin);
		while (!get_newline(input)) {
			leftover = strlen(input);
			input = (char *)realloc(input, 2 * size);
			if (input == NULL) {
				printf("Could not allocate memory");
				break;
			}
			fgets(input + leftover, 2 * size, stdin);
			size *= 2;
		}
		remove_newline(input);
		if (input[0] != DOT && strlen(input) > 1) {
			fprintf(out_file, "%d:%s\n", strlen(input), input);
		}
		size = 4;

	} while (input[0] != DOT && strlen(input) > 1);
	//printf("%s", input);
	fclose(out_file);
	free(input);
	printf("Existing program...");
	return 0;
}

bool get_newline(char *string) {
	for (int i = 0; i < strlen(string); i++) {
		if (string[i] == '\n') {
			return true;
		}
	}
	return false;
}

void remove_newline(char *string) {
	for (int i = 0; i < strlen(string); i++) {
		if (string[i] == '\n' || string[i] == '\r') {
			string[i] = '\0';
		}
	}
}