#pragma warning(disable:4996) //disabling warning

/*
Write implementation of function printd that takes same parameters as printf and produces same output as printf but precedes all output with "DEBUG: ".
Function returns number of output characters including the debug prefix in the output.

For example, if you call printd("test") then output is "DEBUG: test" and return value is 11.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

int printd(const char *format, ...);

int main(int arcg, char **argv)
{
    printd("Holy why?\n");
    return 0;

}

int printd(const char *format, ...) {
    va_list argptr;
    int cnt = 0; //count

    va_start(argptr, format);
    //Printing message.
    cnt += printf("DEBUG: ");
    cnt += vprintf(format, argptr);
    va_end(argptr);
    
    return cnt;
}