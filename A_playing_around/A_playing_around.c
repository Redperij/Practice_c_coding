#pragma warning(disable:4996) //disabling warning

/*
Assume the following declarations:

#define MAX_LEN 32

typedef struct student_ {
    char name[MAX_LEN];
    int group;
    int id;
} student;

typedef enum { byGroup, byLastName, byFirstName } sort_order;

void sort_students(student *students, int count, sort_order sb);


Implement function sort_students.

First parameter is array of students.
Second parameter is number of studenst to sort from the beginning of the array.
If zero or negative value is given then all student should be sorted.
The end of array marker has id set to zero (id==0).
Third parameter is the sort criteria.
Sort_students must use qsort() from standard library to sort the

The sort criterias are:

byGroup :Compare group numbers
byFirstName : First name comes first in the string and is separated from the last name by space
byLastName : As above but comparison must find last name from the strings and compare them
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LEN 32

typedef struct student_ {
    char name[MAX_LEN];
    int group;
    int id;
} student;

typedef enum { byGroup, byLastName, byFirstName } sort_order;

void sort_students(student *students, int count, sort_order sb);

int main(int arcg, char **argv) {
    student students[6];
    char *name = malloc(33 * sizeof(char));

    if (name == NULL) return 1;

    name = "Vasya Petrov";
    for (int i = 0; i <= strlen(name); i++) {
        students[0].name[i] = name[i];
    }
    students[0].id = 1;
    students[0].group = 1;

    name = "Anatolii Stepanov";
    for (int i = 0; i <= strlen(name); i++) {
        students[1].name[i] = name[i];
    }
    students[1].id = 2;
    students[1].group = 4;

    name = "Daniil Marinec";
    for (int i = 0; i <= strlen(name); i++) {
        students[2].name[i] = name[i];
    }
    students[2].id = 3;
    students[2].group = 2;

    name = "Sergey Konev";
    for (int i = 0; i <= strlen(name); i++) {
        students[3].name[i] = name[i];
    }
    students[3].id = 4;
    students[3].group = 5;

    name = "Andrey Kulikov";
    for (int i = 0; i <= strlen(name); i++) {
        students[4].name[i] = name[i];
    }
    students[4].id = 5;
    students[4].group = 3;

    name = "End";
    for (int i = 0; i <= strlen(name); i++) {
        students[5].name[i] = name[i];
    }
    students[5].id = 0;
    students[5].group = 0;

    printf("Students before sorting:\n");
    for (int i = 0; i < 6; i++) {
        printf("Student %d: Name: '%s' Group: %d\n", students[i].id, students[i].name, students[i].group);
    }

    sort_students(students, 5, byGroup);

    printf("Sorted by group:\n");
    for (int i = 0; i < 6; i++) {
        printf("Student %d: Name: '%s' Group: %d\n", students[i].id, students[i].name, students[i].group);
    }

    sort_students(students, 5, byLastName);

    printf("Sorted by lastname:\n");
    for (int i = 0; i < 6; i++) {
        printf("Student %d: Name: '%s' Group: %d\n", students[i].id, students[i].name, students[i].group);
    }

    sort_students(students, 0, byFirstName);

    printf("Sorted by firstname:\n");
    for (int i = 0; i < 6; i++) {
        printf("Student %d: Name: '%s' Group: %d\n", students[i].id, students[i].name, students[i].group);
    }
	
	return 0;
}

void sort_students(student *students, int count, sort_order sb) {
    if (students == NULL) return;
    int nstudents = 0;
    student temp_student = students[0];
    char *firstname1 = malloc((MAX_LEN + 1) * sizeof(char));
    char *lastname1 = malloc((MAX_LEN + 1) * sizeof(char));
    char *firstname2 = malloc((MAX_LEN + 1) * sizeof(char));
    char *lastname2 = malloc((MAX_LEN + 1) * sizeof(char));

    while (temp_student.id != 0) {
        temp_student = students[nstudents];
        nstudents++;
    }
    nstudents--;

    if (count <= 0) { //Negative or zero sorts whole array.
        count = nstudents;
    }

    switch (sb)
    {
    case byGroup:
        for (int i = 0; i < count; i++) {
            for (int q = 1; q < count - i; q++) {
                if (students[q].id != 0 && students[q - 1].group > students[q].group) {
                    temp_student = students[q - 1];
                    students[q - 1] = students[q];
                    students[q] = temp_student;
                }
            }
        }
        break;
    case byLastName:
        for (int i = 0; i < count; i++) {
            for (int q = 1; q < count - i; q++) {
                if (students[q].id != 0) {
                    sscanf(students[q - 1].name, "%s %s", firstname1, lastname1);
                    sscanf(students[q].name, "%s %s", firstname2, lastname2);
                    if (strcmp(lastname1, lastname2) > 0) {
                        temp_student = students[q - 1];
                        students[q - 1] = students[q];
                        students[q] = temp_student;
                    }
                }
            }
        }
        break;
    case byFirstName:
        for (int i = 0; i < count; i++) {
            for (int q = 1; q < count - i; q++) {
                if (students[q].id != 0) {
                    sscanf(students[q - 1].name, "%s %s", firstname1, lastname1);
                    sscanf(students[q].name, "%s %s", firstname2, lastname2);
                    if (strcmp(firstname1, firstname2) >= 0) {
                        temp_student = students[q - 1];
                        students[q - 1] = students[q];
                        students[q] = temp_student;
                    }
                }
            }
        }
        break;
    default:
        break;
    }
}