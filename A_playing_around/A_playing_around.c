#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

void merge_whitespace(char *);

int main() {
    char hello[] = "Hello    my         world.   .   \n";
    printf("%s\n", hello);
    merge_whitespace(hello);
    printf("%s\n", hello);
	return 0;
}

void merge_whitespace(char *string) {
    int previous_whitespace = 0;
    int count = strlen(string);

    for (int i = 0; i < strlen(string); i++) {
        if (isspace(string[i])) {
            string[i] = ' ';
        }
    }

    for (int i = 0; i < count; i++) {
        if (isspace(string[i])) {
            if (previous_whitespace == 1) {
                for (int q = i; q < count; q++) {
                    string[q] = string[q + 1];
                }
                count--;
                i--;
            }
            previous_whitespace = 1;
        }
        else {
            previous_whitespace = 0;
        }
    }
}