#pragma warning(disable:4996) //disabling warning

/*
Assume following declarations and definitions:

#define MAX_LEN 32

typedef struct student_ {
	char name[MAX_LEN];
	int group;
	int id;
	struct student_ *next;
} student;

int move(student **source, int group, student **target);


Implement function move() that moves elements from source to target.
Second parameter, group, determines which elements to move.
Only the elements of matching group are moved to target list.
Both source and target are linked lists that are initialized by the caller.
The end of list is market by setting next pointer of the last element to NULL.

When you remove elements from the source you must preserve the order of remaining elements.
When you add an element to the target it must be added at the beginning of the traget list.

Move returns the number of elements moved to the target.
Function returns zero if no matching elements were found.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define FILENAME "in.txt"

#define MAX_LEN 32

typedef struct student_ {
	char name[MAX_LEN];
	int group;
	int id;
	struct student_ *next;
} student;

int move(student **source, int group, student **target);

int main() {
	student *save_me = malloc(10 * sizeof(student));
	student *target = malloc(10 * sizeof(student));

	//source
	save_me[0].name[0] = 'a';
	save_me[0].name[1] = '\0';
	save_me[0].group = 1;
	save_me[0].id = 1;
	save_me[0].next = &save_me[1];

	save_me[1].name[0] = 'b';
	save_me[1].name[1] = '\0';
	save_me[1].group = 2;
	save_me[1].id = 2;
	save_me[1].next = &save_me[2];

	save_me[2].name[0] = 'c';
	save_me[2].name[1] = '\0';
	save_me[2].group = 1;
	save_me[2].id = 3;
	save_me[2].next = &save_me[3];

	save_me[3].name[0] = 'd';
	save_me[3].name[1] = '\0';
	save_me[3].group = 1;
	save_me[3].id = 4;
	save_me[3].next = &save_me[4];

	save_me[4].name[0] = 'e';
	save_me[4].name[1] = '\0';
	save_me[4].group = 3;
	save_me[4].id = 5;
	save_me[4].next = NULL;

	//target
	target[0].name[0] = 'f';
	target[0].name[1] = '\0';
	target[0].group = 1;
	target[0].id = 6;
	target[0].next = &target[1];

	target[1].name[0] = 'g';
	target[1].name[1] = '\0';
	target[1].group = 3;
	target[1].id = 7;
	target[1].next = NULL;

	int i = 0;
	int finish = 2;
	printf("Source before move():\n");
	while (finish) {
		if (finish == 2) {
			finish = 2;
		}
		printf("%d: (group %d) %s\n", save_me[i].id, save_me[i].group, save_me[i].name);
		i++;
		if ((save_me[i].next) == NULL || finish == 1) {
			finish--;
		}
	}

	i = 0;
	finish = 2;
	printf("Target before move():\n");
	while (finish){
		if (finish == 2) {
			finish = 2;
		}
		printf("%d: (group %d) %s\n", target[i].id, target[i].group, target[i].name);
		i++;
		if ((target[i].next) == NULL || finish == 1) {
			finish--;
		}
	}

	int count = 0;
	count = move(&save_me, 1, &target);

	student **ppts = &save_me;
	printf("Source after move():\n");
	while (*ppts != NULL) {
		printf("%d: (group %d) %s\n", (*ppts)->id, (*ppts)->group, (*ppts)->name);
		ppts = &((*ppts)->next);
	}

	ppts = &target;
	printf("Target after move():\n");
	while (*ppts != NULL) {
		printf("%d: (group %d) %s\n", (*ppts)->id, (*ppts)->group, (*ppts)->name);
		ppts = &((*ppts)->next);
	}

	printf("Elements moved: %d", count);

	return 0;
}



int move(student **source, int group, student **target) {
	int count = 0;

	while (*source != NULL) {
		if ((*source)->group == group) {
			//Erasing pointer from source.
			student *ptm = *source; //Remember current pointer.
			*source = ptm->next; //Move source to next element.
			//Adding pointer to the target.
			ptm->next = *target; //Next pointer for remembered pointer is target list.
			*target = ptm; //Target list starts from remembered pointer.

			count++; //Increase count of moved elements.
		}
		else {
			source = &(*source)->next; //Move source to next element.
		}
	}

	return count;
}