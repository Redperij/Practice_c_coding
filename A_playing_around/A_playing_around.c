#pragma warning(disable:4996) //disabling warning

/*
Write a program that reads input lines using read_line-function. read_line returns true if input was read and false when input ends.
Each line contains two numbers and a command "add", "sub", "mul", or "div".
Program reads lines one by one and checks that input is valid (first 2 integers then a string that is a valid command).
If line is not properly formatted the program prints the line and displays an error message.
For each properly formatted line the program prints out a calculation which is determined by the command.

For example input that contains:
7 2 sub
43 -3 add

Will make the program print:
7 - 2 = 5
43 + -3 = 40

If the input can't be parsed then program must print the invalid line and an error message that describes the location of the error. See the example solution output for details.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define FILENAME "in.txt"

/* call this function to read input */
bool read_line(char *line, size_t size, FILE *file);

#define CHARS_IN_LINE 32
#define ADDITION "add"
#define SUBTRACTION "sub"
#define MULTIPLICATION "mul"
#define DIVISION "div"

//Returns number, indicating place of error or 0.
int validate_input(const char *line, int *first_int, int *second_int, int *command, char **raw_command);
//Creates new line if the input is correct.
void print_new_line(char *line, int first_int, int second_int, int command);
//Gets rid of the newline characters in the string. Terminates string on the first newline character.
void no_newline(char **string);
int main() {
	FILE *file = fopen(FILENAME, "r"); //NOPE. DELETE THIS LINE! (just for checks)
	char *line = malloc(CHARS_IN_LINE * sizeof(char)); //Line to read.
	bool flag = true; //flag to continue.
	int first_int; //First integer from input.
	int second_int; //Second integer from input.
	int command; //1 - add, 2 - sub, 3 - mul, 4 - div
	int error_check = 0; //Flag to chack error.
	char *invalid_command = malloc(CHARS_IN_LINE * sizeof(char)); //In case of failure this command will be displaied.

	while(flag) {
		flag = read_line(line, CHARS_IN_LINE, file); //ATTENTION!!!!!!! DELETE 'file' ARGUMENT. It is not needed in the task. (just for checks)
		if (flag) {
			//printf("%s\n", line);
			error_check = validate_input(line, &first_int, &second_int, &command, &invalid_command);
			switch (error_check)
			{
			case 1: //First argument invalid.
			case 2: //Second argument invalid.
			case 3: //Third argument invalid.
				no_newline(&line);
				printf("Input: \"%s\" Invalid argument in position: %d\n", line, error_check);
				break;
			case 4: //Unknown command.
				no_newline(&line);
				printf("Input: \"%s\" Unknown command: %s\n", line, invalid_command);
				break;
			default: //Everything in order.
				no_newline(&line);
				print_new_line(line, first_int, second_int, command);
				break;
			}
		}
	}

	return 0;
}

//Returns number, indicating place of error or 0.
int validate_input(const char *line, int *first_int, int *second_int, int *command, char **raw_command) {
	int error_flag = 0;

	error_flag = sscanf(line, "%d %d %s", first_int, second_int, *raw_command);
	switch (error_flag)
	{
	case 0:
		return 1;
	case 1:
		return 2;
	case 2:
		return 3;
	default:
		break;
	}

	if (!strcmp(*raw_command, ADDITION)) {
		*command = 1;
	}
	else if (!strcmp(*raw_command, SUBTRACTION)) {
		*command = 2;
	}
	else if (!strcmp(*raw_command, MULTIPLICATION)) {
		*command = 3;
	}
	else if (!strcmp(*raw_command, DIVISION)) {
		*command = 4;
	}
	else {
		return 4;
	}

	return 0;
}

//Creates new line if the input is correct.
void print_new_line(char *line, int first_int, int second_int, int command) {
	int result;
	switch (command)
	{
	case 1: // +
		result = first_int + second_int;
		printf("%d + %d = %d\n", first_int, second_int, result);
		break;
	case 2: // -
		result = first_int - second_int;
		printf("%d - %d = %d\n", first_int, second_int, result);
		break;
	case 3: // *
		result = first_int * second_int;
		printf("%d * %d = %d\n", first_int, second_int, result);
		break;
	case 4: // /
		result = first_int / second_int;
		printf("%d / %d = %d\n", first_int, second_int, result);
		break;
	default:
		//What?
		break;
	}
}

//Gets rid of the newline characters in the string. Terminates string on the first newline character.
void no_newline(char **string) {
	for (unsigned int i = 0; i < strlen(*string); i++) {
		if (string[0][i] == '\n') {
			string[0][i] = '\0';
		}
	}
}

bool read_line (char *line, size_t size, FILE *file){
	char *return_value;
	return_value = fgets(line, size, file);
	if(return_value != NULL) {
		return true;
	}
	return false;
}