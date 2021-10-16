#pragma warning(disable:4996) //disabling warning

/*
Write a function called count_args.
Function takes an integer pointer and variable number of other arguments as parameters and returns an integer.
The variabale arguments are pointers to strings.
The end of arguments is indicated by a NULL pointer. The function returns the number of strings.
Function also counts total number of characters in the strings and stores the total through the first parameter.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

int count_args(int *nchars, ...);

int main(int arcg, char **argv)
{
	char string1[] = "Ahh\n";
	char string2[] = "Why\n";
	char string3[] = "Noooo\n";
	char *string4 = NULL;
	int wha = 0;
	int strings = 0;
	strings = count_args(&wha, string1, string2, string3, string4);
	printf("Strings: %d\nCharacters: %d\n", strings, wha);
	return 0;
}

int count_args(int *nchars, ...) {
	va_list args_ptr;
	int scount = 0;
	char *string = NULL;
	*nchars = 0;

	va_start(args_ptr, nchars);

	do {
		string = (char *) va_arg(args_ptr, char *);
		if (string != NULL) {
			scount++;
			*nchars += strlen(string);
		}
	} while (string != NULL);
	va_end(args_ptr);

	return scount;
}