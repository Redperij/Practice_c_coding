#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#define SPACEBAR 32 //haven't used
#define UPPERCASE_A 65
#define UPPERCASE_Z 90
#define LOWERCASE_A 97
#define LOWERCASE_Z 122
#define LOWER_UPPER_CASE_DIFF 32 //difference between upercase and corresponding lowercase letter ids
#define STRING_LIMIT 4096 //2^12
#define LETTERS 26

//Returns shift value in respect of the alphabet size. 
//If we are shifting 27 letters using 26 letter alphabet -> we will be shifting just 1 letter (A -> B etc...)
int validate_shift(int shift);

//Encrypts text by the rules of Caesar cipher. https://en.wikipedia.org/wiki/Caesar_cipher
void encrypt_text(char *string, int shift);

//Checks if the symbol is uppercase letter
bool is_uppercase_letter(char symbol);

//Checks if the symbol is lowercase letter
bool is_lowercase_letter(char symbol);

//Converts lowercase letters in the string to uppercase
void convert_lowercase_to_uppercase_string (char *string);

int main() {
	char entered_string[STRING_LIMIT]; //Holds string, that user entered. Is changed by encryption functions
	int shift = 0; //Holds shift value, user input.
	//Reading user string
	printf("Enter the string to encrypt:\n");
	fgets(entered_string, STRING_LIMIT, stdin);
	//Getting raw shift value
	printf("Enter the shift value: ");
	while (scanf("%d", &shift) != 1) {
		printf("Incorrect input. Only numbers are acceptable.\n");
		while (getchar() != '\n'); //getting rid of trash
		printf("Enter the shift value: ");
	}
	
	printf("\nYou entered:\n%s\n", entered_string);

	convert_lowercase_to_uppercase_string(entered_string);

	encrypt_text(entered_string, shift); //encrypting text
	printf("\nEncrypted text:\n%s\n", entered_string);
	encrypt_text(entered_string, shift * -1); //decrypting text
	printf("\nDecrypted text:\n%s\n", entered_string);

	return 0;
}

//Returns shift value in respect of the alphabet size. 
//If we are shifting 27 letters using 26 letter alphabet -> we will be shifting just 1 letter (A -> B etc...)
int validate_shift(int shift) {
	if(shift >= 0) { //If it is more than possible -> just treating LETTERS as a loop value
		shift = shift % LETTERS;
	}
	else { //If it is negative -> shifting backwards. LETTERS = 26 -> given -1 it will change it to 25
		shift = 26 + (shift % LETTERS);
	}
	return shift;
}

//Encrypts text by the rules of Caesar cipher. https://en.wikipedia.org/wiki/Caesar_cipher
void encrypt_text(char *string, int shift) {
	shift = validate_shift(shift);
	for(int i = 0; i < strlen(string); i++) {
		if(is_uppercase_letter(string[i])) {
			int temp_var = string[i] + shift; //For the sake of similarity it is handled with temporary integer. Check lowercase letters handling.
			if (temp_var > UPPERCASE_Z) { //Over the limit
				string[i] = temp_var - LETTERS; //Return 26 ids back
			}
			else {
				string[i] = temp_var; //Just use new letter id
			}
		}
		else if(is_lowercase_letter(string[i])) { //Left it here, should not be executed if string is converted to uppercase string.
			int temp_var = string[i] + shift; //It is possible to go over permitted id, in that case temporary integer is needed to perform the loop.
			if(temp_var > LOWERCASE_Z) { //Over the limit
				string[i] = temp_var - LETTERS; //Return 26 ids back
			}
			else {
				string[i] = temp_var; //Just use new letter id
			}
		}
	}
}

//Checks if the symbol is uppercase letter
bool is_uppercase_letter(char symbol) {
	if(symbol >= UPPERCASE_A && symbol <= UPPERCASE_Z) {
		return true; 
	}
	return false;
}

//Checks if the symbol is lowercase letter
bool is_lowercase_letter(char symbol) {
	if(symbol >= LOWERCASE_A && symbol <= LOWERCASE_Z) {
		return true; 
	}
	return false;
}
//Converts lowercase letters in the string to uppercase
void convert_lowercase_to_uppercase_string (char *string) {
	for(int i = 0; i < strlen(string); i++) {
		if(is_lowercase_letter(string[i])) {
			string[i] -= LOWER_UPPER_CASE_DIFF;
		}
	}
}