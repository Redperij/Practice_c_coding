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

//Generates random password with one of the words from array_of_words of array_size elements.
//Returns generated password into *password.
void gen_password(char **password, unsigned int array_size, const char **array_of_words);
//Fills given char array of 'size' size with random printable non-whitespace characters and terminates it in the end.
void random_prstring(char *array, size_t size);
//Returns random number. min < x <= max
unsigned int get_rand_num(unsigned int min, unsigned int max);

int main(void)
{
	char *generated_password = NULL;
	char **array_of_words = malloc(WORDS * sizeof(char *));
	if (array_of_words == NULL) return 1; //malloc() failed -> escape.

	srand(time(NULL)); //Current time is seed for rand() function.
	
	//Filling in array of strings.
	array_of_words[0] = "Hello";
	array_of_words[1] = "Again";
	array_of_words[2] = "Cat";
	array_of_words[3] = "Track";
	array_of_words[4] = "Road";
	array_of_words[5] = "Hammer";
	array_of_words[6] = "Tree";
	array_of_words[7] = "CQDX";
	array_of_words[8] = "Apple";
	array_of_words[9] = "Pie";

	//Generating 'PASSWORDS_TO_GENERATE' passwords.
	for (int i = 0; i < PASSWORDS_TO_GENERATE; i++) {
		gen_password(&generated_password, WORDS, array_of_words);

		printf("Password %d: %s\n", i + 1, generated_password);
		//Free memory.
		if (generated_password != NULL) {
			free(generated_password);
			generated_password = NULL;
		}
	}
	
	//Free memory.
	if (array_of_words != NULL) {
		free(array_of_words);
		array_of_words = NULL;
	}
	return 0;
}

//Generates random password with one of the words from array_of_words of array_size elements.
//Returns generated password into *password.
void gen_password(char **password, unsigned int array_size, const char **array_of_words) {
	if (array_of_words == NULL) return; //array is empty -> escape.
	unsigned int rnd_chars = 0; //Random chars to add to the word.
	unsigned int word = 0; //Which word was randomly selected.
	size_t password_size = 0; //Size of the password.

	//Password must be empty.
	if (*password != NULL) {
		free(*password);
		*password = NULL;
	}
	//Getting number of chars to add.
	rnd_chars = get_rand_num(0, MAX_CHARS_TO_APPEND);
	//Choosing word.
	word = get_rand_num(0, WORDS) - 1;
	//Getting password size. word size + chars to add.
	password_size = strlen(array_of_words[word]) + rnd_chars;

	//Allocating memory for the password.
	*password = (char *)malloc((password_size + 1) * sizeof(char));
	
	if (*password != NULL) {
		//Choosing place where to add word.
		int place_to_add_word = get_rand_num(0, password_size - strlen(array_of_words[word]) + 1 ) - 1;
		int q = 0;
		//Filling password with characters and terminating it.
		random_prstring(*password, password_size);
		//Placing word into the string overwriting random characters.
		for (int i = place_to_add_word; i < place_to_add_word + strlen(array_of_words[word]); i++) {
			password[0][i] = array_of_words[word][q];
			q++;
		}
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