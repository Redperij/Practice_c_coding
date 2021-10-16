#pragma warning(disable:4996) //disabling warning

/*
Implement two functions:

int log(loglevel level, const char *format, ...); works like printf but prints only when level lower than or equal to global variable log_level.
When printing precedes printouts with "LOG[%s]: " where %s is the parameter level printed as a string.

const char *log_level_to_str(loglevel level); returns a constant string corresponding to the level parameter.
Note that the string returned may not be a variable that is allocated on the stack.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum { logCritical, logWarning, logInfo, logVerbose } loglevel;
// Log levels as strings:  "Critical", "Warning", "Info", "Verbose" 

const char *log_level_to_str(loglevel level);
// implement this function
int log(loglevel level, const char *format, ...);

loglevel log_level = logCritical;

int main(int arcg, char **argv)
{
	log(logCritical, "OH NO\n");
	log(logInfo, "OH NOx2\n");
    return 0;
}

const char *log_level_to_str(loglevel level) {
	switch (level)
	{
	case logCritical:
		return "Critical";
	case logWarning:
		return "Warning";
	case logInfo:
		return "Info";
	case logVerbose:
		return "Verbose";
	default:
		return NULL;
	}
}
int log(loglevel level, const char *format, ...) {
	va_list argptr; //List with arguments.
	int cnt = 0; //Count printed characters.

	if (level <= log_level) {
		va_start(argptr, format);
		//Printing message.
		cnt += printf("LOG[%s]: ", log_level_to_str(level));
		cnt += vprintf(format, argptr);
		va_end(argptr);
	}

	return cnt;
}