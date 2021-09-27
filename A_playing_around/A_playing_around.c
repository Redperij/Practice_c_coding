#pragma warning(disable:4996) //disabling warning

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LEN 32

typedef struct student_ {
    char name[MAX_LEN];
    int group;
    int id;
} student;

int array_size(const student *array);

int move(student *source, int group, student *target, int size);

int main()
{
    student source[5];
    student target[5];
    int size = 5;
    source[0].name[0] = 'V';
    source[0].name[1] = 'a';
    source[0].name[2] = 's';
    source[0].name[3] = 'i';
    source[0].name[4] = 'a';
    source[0].name[5] = '\0';
    source[0].group = 1;
    source[0].id = 10;
    source[1].name[0] = 'K';
    source[1].name[1] = 'a';
    source[1].name[2] = 't';
    source[1].name[3] = 'y';
    source[1].name[4] = 'a';
    source[1].name[5] = '\0';
    source[1].group = 2;
    source[1].id = 2;
    source[2].name[0] = 'M';
    source[2].name[1] = 'u';
    source[2].name[2] = 'f';
    source[2].name[3] = 'f';
    source[2].name[4] = 'i';
    source[2].name[5] = 'n';
    source[2].name[6] = '\0';
    source[2].group = 3;
    source[2].id = 3;
    source[3].name[0] = 'P';
    source[3].name[1] = 'e';
    source[3].name[2] = 't';
    source[3].name[3] = 'y';
    source[3].name[4] = 'a';
    source[3].name[5] = '\0';
    source[3].group = 4;
    source[3].id = 4;
    source[4].name[0] = '\0';
    source[4].group = 0;
    source[4].id = 0;

    target[0].name[0] = '\0';
    target[0].group = 0;
    target[0].id = 0;

    printf("All students:\n");
    for (int i = 0; i < size; i++) {
        printf("%s\n", source[i].name);
    }

    int count = move(source, 1, target, size);

    printf("All students left:\n");
    for (int i = 0; i < size; i++) {
        printf("%s\n", source[i].name);
    }

    printf("Group 1:\n");
    for (int i = 0; i < count; i++) {
        printf("%s\n", target[i].name);
    }

    count += move(source, 2, target, size);

    printf("All students left:\n");
    for (int i = 0; i < size; i++) {
        printf("%s\n", source[i].name);
    }

    printf("Group 1 & 2:\n");
    for (int i = 0; i < count; i++) {
        printf("%s\n", target[i].name);
    }

	return 0;
}

int array_size(const student *array) {
    int i = 0;
    while (array[i].id != 0) {
        i++;
    }
    return i;
}

int move(student *source, int group, student *target, int size) {
    int count = 0;
    int target_size = array_size(target);
    int actual_size = array_size(source);

    for (int i = 0; i < actual_size; i++) {
        if (source[i].group == group && target_size < size - 1) {
            target_size++;
            count++;
            target[target_size] = target[target_size - 1];
            target[target_size - 1] = source[i];
            for (int q = i + 1; q <= actual_size; q++) {
                source[q - 1] = source[q];
            }
            actual_size--;
            i--;
        }
    }

    return count;
}