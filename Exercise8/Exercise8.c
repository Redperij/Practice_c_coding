#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#define SPACEBAR 32
#define UPPERCASE_A 65 //not used
#define UPPERCASE_Z 90 //not used, left as a tip
#define LOWERCASE_A 97
#define LOWERCASE_Z 122
#define LOWER_UPPER_CASE_DIFF 32 //difference between upercase and corresponding lowercase letter ids
#define STRING_LIMIT 4096 //2^12

//Checks if 'stop' or 'STOP' command is entered
bool stop(const char* string);
//Capitalizes all first word letters in the string
void caps_words_in_string(char* string);
//Checks if the symbol is lowercase letter
bool is_lowercase_letter(char symbol);

int main() {
	char entered_string[STRING_LIMIT]; //Holds string, that user enters every loop iteration

	printf("Please, enter string or stop execution with 'stop' command.\n");
	fgets(entered_string, STRING_LIMIT, stdin);
	while (!stop(entered_string)) {
		caps_words_in_string(entered_string);
		printf("You entered:\n%s\n", entered_string);
		printf("Please, enter next string or stop execution with 'stop' command.\n");
		fgets(entered_string, STRING_LIMIT, stdin);
	}
	printf("Stopped execution\n");
	return 0;
}

//Checks if 'stop' or 'STOP' command is entered
bool stop(const char* string) {
	if ((!strncmp(string, "stop", 4) || !strncmp(string, "STOP", 4)) && strlen(string) <= 5) {
		return true;
	}
	return false;
}

//Capitalizes all first word letters in the string
void caps_words_in_string(char* string) {
	bool new_word = true; //Used to check if the new word was met
	for (int i = 0; i < strlen(string); i++) {
		if (string[i] == SPACEBAR || string[i] == '\t' || string[i] == '\n') {
			new_word = true; //Read whitespace -> word is completed, waiting for the new word
		}
		else if (is_lowercase_letter(string[i]) && new_word) {
			new_word = false; //Capitalizing word's first letter -> no new word comes next
			string[i] -= LOWER_UPPER_CASE_DIFF;
		}
		else {
			new_word = false; //Just a symbol -> don't care about it.
		}
	}
}

//Checks if the symbol is lowercase letter
bool is_lowercase_letter(char symbol) {
	if (symbol >= LOWERCASE_A && symbol <= LOWERCASE_Z) {
		return true;
	}
	return false;
}