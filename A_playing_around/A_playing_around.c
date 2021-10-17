#pragma warning(disable:4996) //disabling warning

/*
Implement a function that takes two parameters: a number to print and field width.
The function prints the number in binary with the specified field width filling the field with leading zeros if necessary.
If printing requires more space than the field width specifies the number is still printed in full.
The function must not print any white space before or after the number.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>


void print_binaryw(uint32_t value, uint32_t width);

int main(int arcg, char **argv)
{
	print_binaryw(5, 4);
	printf("\n");
	print_binaryw(0x2A, 6);
	printf("\n");
	print_binaryw(5, 0);
	printf("\n");
	print_binaryw(0xCAFEBEEF, 0);
	return 0;
}

void print_binaryw(uint32_t value, uint32_t width) {
	uint32_t required_field = 0;
	char *string;
	//Getting correct field width.
	for (uint32_t i = value; i > 0; i /= 2) required_field++;
	if (required_field > width) width = required_field;
	//Allocating memory for string of 01's
	string = malloc((width + 1) * sizeof(char));
	if (string == NULL) return;

	string[width] = '\0';
	while (width > 0) {
		if (value % 2 == 1) string[width - 1] = '1';
		else string[width - 1] = '0';
		value >>= 1;
		width--;
	}
	printf("%s", string);
	free(string);
	string = NULL;
}