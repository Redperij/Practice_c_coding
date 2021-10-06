#pragma warning(disable:4996) //disabling warning

/*
Write a function called filter_alpha.
The function takes three parameters:

Character pointer (=string)
String maximum size including terminating nul character
Pointer to a function that takes no parameters and returns a character

Filter_alpha calls the function given as a parameter to read input one character at a time until a newline is read,  the maximum size is reached, or the read character is a zero.
Function must ensure that the string is always properly terminated.
Function saves the received character in the string only if the character is a letter.
Filter alpha returns the total number of characters it has read.
The count must include letters and other characters except zero.

My_getchar reads a character and returns it.
When end of input occurs my_getchar returns zero.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MY_STR_SIZE 1024

char my_getchar(void);

int filter_alpha(char *string, size_t size, char (*read_char)(void));

int main(int arcg, char **argv)
{
    char str[MY_STR_SIZE] = { 0 };
    int count = 0;

    do {
        count = filter_alpha(str, MY_STR_SIZE, my_getchar);
        printf("%d:%d:%s\n", count, strlen(str), str);
    } while (count > 0);

    return 0;

}
char my_getchar(void) {
    char character = getc(stdin);
    if (character == '\\') character = 0;
    return character;
}

int filter_alpha(char *string, size_t size, char (*read_char)(void)) {
    int count = 0;
    int i = 0;
    char character;
    bool flag_stop = false;

    do {
        character = (*read_char)();
        if (character == '\0' || character == '\n') {
            if (character == '\n') i++;
            flag_stop = true;
        }
        if (!flag_stop) {
            i++;
            if (isalpha(character)) {
                string[count] = character;
                count++;
            }
        }
    } while (count < (size - 1) && !flag_stop);
    string[count] = '\0';
    return i;
}