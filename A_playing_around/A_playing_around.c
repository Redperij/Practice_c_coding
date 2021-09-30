#pragma warning(disable:4996) //disabling warning


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define FILENAME "in.txt"

char *my_gets(char *line, int size, FILE *stream);
int count_chars(const char *characters, FILE *stream);

int main() {
	FILE *file = fopen(FILENAME, "r");
//	char *a = malloc(2 * sizeof(char));
//	a[0] = 'a';
//	a[1] = '\0';

	printf("%d", count_chars("a", file));

	return 0;
}

char *my_gets(char *line, int size, FILE *stream) {
	return fgets(line, size, stream);
}
int count_chars(const char *characters, FILE *stream) {
	char *return_value = malloc(sizeof(char));
	int count = 0;
	char *c = malloc(2 * sizeof(char));

	if (c == NULL) {
		return 0;
	}

	while (true) {
		return_value = my_gets(c, 2, stream);
		if (return_value == NULL) {
			break;
		}
		for (unsigned int i = 0; i < strlen(characters); i++) {
			if (characters[i] == c[0]) {
				count++;
				break;
			}
		}
	}

	return count;
}