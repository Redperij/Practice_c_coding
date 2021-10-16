#pragma warning(disable:4996) //disabling warning

/*
Implement functions text_reader_init and text_reader_read.
Text reader_init sets the position to zero and stores the pointer in the context.
Text_reader_read takes context pointer, buffer and buffer size as parameters.
The function copies characters from the text in context, starting from current position, until a line feed is encountered, end of text is reached or buffer is full.
If reading stops at a line feed the line feed is copied into string.
Text_reader_read returns the number of characters copied to buffer.
When end of text has been reached following reads must return a zero.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
	const char *text;
	int position;
} text_reader_ctx;


// implement the following two functions
void text_reader_init(text_reader_ctx *ctc, const char *str);
int text_reader_read(text_reader_ctx *ctx, char *buffer, int size);

int main(int arcg, char **argv)
{
	text_reader_ctx ctc;
	char *string = malloc(256 * sizeof(char));
	char *string_to_read = malloc(256 * sizeof(char));
	printf("Enter string to read:\n");
	fgets(string_to_read, 255, stdin);

	text_reader_init(&ctc, string_to_read);
	text_reader_read(&ctc, string, 3);
	printf("Read: %s", string);
	return 0;
}

void text_reader_init(text_reader_ctx *ctc, const char *str) {
	ctc->position = 0;
	ctc->text = str;
}
int text_reader_read(text_reader_ctx *ctx, char *buffer, int size) {
	int count = 0; //Counts charactes.
	int text_size = strlen(ctx->text);
	bool exit = false;

	while (count < size - 1 && ctx->position < text_size && !exit) {
		buffer[count] = ctx->text[ctx->position];
		count++;
		ctx->position++;

		if (buffer[count - 1] == '\n') {
			exit = true;
		}
	}
	buffer[count] = '\0';
	return count;
}