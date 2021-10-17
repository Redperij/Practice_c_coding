#pragma warning(disable:4996) //disabling warning

/*
Write a function that shifts bits to right and masks out unwanted bits.
The first parameter of the function is the number to process, the second parameter is number shifts to right,
and the last parameter is the number of bits to keep starting from the least significant bit (the rightmost bit).
The function returns the result of shift-and-mask operation.

To keep certain number of bits you need to create a mask where the bits that you want to keep are set to one and the rest of the bits are set to zero.
Then perform bitwise and between the value and mask.
For example to keep four bits from the right the mask is 0xF.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

uint32_t get_bits(uint32_t value, uint32_t shift, uint32_t bits);

int main(int arcg, char **argv)
{
	printf("get_bits(0X7E3, 4, 6) --> %x", get_bits(0X7E3, 4, 6));
	return 0;
}

uint32_t get_bits(uint32_t value, uint32_t shift, uint32_t bits) {
	value >>= shift;
	return value & (0xFFFFFFFF >> (32 - bits)); // 0xFFFFFFFF - 32 bits '1' are shifted right until 'bits' number of '1' bits left -> it is needed mask.
}