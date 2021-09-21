#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>
/*
#define MAX_STUDENT_COUNT 2048 //2^11

float ave_of_array(int integers[], int size); //counts average of the received array

int main() {

	int course_grades[MAX_STUDENT_COUNT] = { 0 }; //array with course grades
	bool correct_input = false; //boolean variable to check if the user gave correct input

	int student_count = -1; //number of students given by the user

	//User is constantly asked for the correct number of students.
	while (!correct_input) {
		printf("Please, enter the number of students: ");
		scanf("%d", &student_count);
		if (student_count <= 0 || student_count > MAX_STUDENT_COUNT) {
			printf("Incorrect student count. Number must be positive, not greater than %d\n", MAX_STUDENT_COUNT);
		}
		else {
			correct_input = true;
		}
	}

	//User asked to give a grade for each student
	printf("Enter grade for each student:\n");
	for (int i = 0; i < student_count; i++) {
		printf("Student %d : ", i + 45000);
		scanf("%d", &course_grades[i]);
		
		if (course_grades[i] < 0 || course_grades[i] > 5) {
			printf("Only numbers from 0 to 5 are allowed. Please, enter the grade again.\n");
			i--;
		}
	}

	//Counting and printing average grade
	printf("Average grade: %.2f", ave_of_array(course_grades, student_count));

	return 0;
}

float ave_of_array(int integers[], int size) {
	int sum = 0;
	for (int i = 0; i < size; i++) { //Summing every array element
		sum += integers[i];
	}
	return (float)sum / size; //Giving back average
}
*/