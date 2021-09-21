#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#define SPACEBAR 32
#define UNDERSCORE 95
#define STRING_LIMIT 4096 //2^12

//Checks if string first 4 letters are 'stop' or 'STOP'
bool stop(const char *string);
//Replaces all whitespaces in the string with underscores
void replace_whitespaces_in_string(char* string);

int main() {
	char entered_string[STRING_LIMIT]; //Holds string, that user enters every loop iteration.

	printf("Please, enter string or stop execution with 'stop' in start of the string.\n");
	fgets(entered_string, STRING_LIMIT, stdin);
	while (!stop(entered_string)) {
		replace_whitespaces_in_string(entered_string);
		printf("You entered:\n%s\n", entered_string);
		printf("Please, enter next string or stop execution with 'stop' in start of the string.\n");
		fgets(entered_string, STRING_LIMIT, stdin);
	}
	printf("Stopped execution\n");
	return 0;
}

//Checks if string first 4 letters are 'stop' or 'STOP'
bool stop(const char *string) {
	if (!strncmp(string, "stop", 4) || !strncmp(string, "STOP", 4)) {
		return true;
	}
	return false;
}

//Replaces all whitespaces in the string with underscores
void replace_whitespaces_in_string(char *string) {
	for (int i = 0; i < strlen(string); i++) {
		if (string[i] == SPACEBAR) {
			string[i] = UNDERSCORE;
		}
	}
}