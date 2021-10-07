#pragma warning(disable:4996) //disabling warning

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define WORDS 10
#define MAX_CHARS_TO_APPEND 12
#define MAX_PRINTABLE_CHAR 127
#define PASSWORDS_TO_GENERATE 3

void gen_password(char *password, unsigned int array_size, const char **array_of_words);
//Fills given char array of 'size' size with random printable non-whitespace characters and terminates it in the end.
void random_prstring(char *array, size_t size);
//Returns random number. min < x <= max
unsigned int get_rand_num(unsigned int min, unsigned int max);

int main(void)
{
	char *generated_password = NULL;
	char **array_of_words = malloc(WORDS * sizeof(char *));
	if (array_of_words == NULL) return 1;
	array_of_words[0] = "Hello";
	array_of_words[1] = "Again";
	array_of_words[2] = "SQDX";
	array_of_words[3] = "Track";
	array_of_words[4] = "Road";
	array_of_words[5] = "Hello";
	array_of_words[6] = "Again";
	array_of_words[7] = "SQDX";
	array_of_words[8] = "Track";
	array_of_words[9] = "Road";

	srand(time(NULL));
	gen_password(generated_password, WORDS, array_of_words);

	//Free memory.
	if (generated_password != NULL) {
		free(generated_password);
		generated_password = NULL;
	}
	if (array_of_words != NULL) {
		free(array_of_words);
		array_of_words = NULL;
	}
	return 0;
}

void gen_password(char *password, unsigned int array_size, const char **array_of_words) {
	if (array_of_words == NULL) return; //array is empty -> escape.
	unsigned int rnd_chars = 0; //Random chars to add to the word.
	unsigned int word = 0; //Which word was randomly selected.
	size_t password_size = 0; //Size of the password.

	//Password must be empty.
	if (password != NULL) {
		free(password);
		password = NULL;
	}
	//Getting number of chars to add.
	rnd_chars = get_rand_num(0, MAX_CHARS_TO_APPEND);
	//Choosing word.
	word = get_rand_num(0, WORDS) - 1;
	//Getting password size. word size + chars to add.
	password_size = strlen(array_of_words[word]) + rnd_chars;

	//Allocating memory for the password.
	password = (char *)malloc((password_size + 1) * sizeof(char));
	
	if (password != NULL) {
		//Choosing place where to add word.
		int place_to_add_word = get_rand_num(0, password_size - strlen(array_of_words[word]) ) - 1;
		int q = 0;
		//Filling password with characters and terminating it.
		random_prstring(password, password_size);
		for (int i = place_to_add_word; i < strlen(array_of_words[word]); i++) {
			password[i] = array_of_words[word][q];
			q++;
		}
		//Loop is incorrect.


		printf("Password: %s\n", password);
		printf("Chosen word: %s\n", array_of_words[word]);
		printf("Number of chars to add: %d\n", rnd_chars);
		printf("Place to add word: %d\n", place_to_add_word);
	}
}

//Fills given char array with random printable non-whitespace characters and terminates it in the end.
void random_prstring (char *array, size_t size) {
	int i = 0;
	char char_to_append;
	while (i < size) {
		char_to_append = get_rand_num(0, MAX_PRINTABLE_CHAR);
		if (isprint(char_to_append) && !isspace(char_to_append)) {
			array[i] = char_to_append;
			i++;
		}
	}
	array[size] = '\0';
}

//Returns random number. min < x <= max
unsigned int get_rand_num(unsigned int min, unsigned int max) {
	return (rand() % (max - min) + min) + 1;
}