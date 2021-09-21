#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
/*
int main() {
	int value; //holds user input
	int sum = 0; //holds sum of the numbers inputted by the user
	int count = 0; //counts numbers inputted by the user

	printf("Enter positive numbers.\nZero stops the execution and counts average.\nNegative numbers are ignored.\n");
	
	do {
		scanf("%d", &value);

		if (value < 0) { //ignore negative input
			printf("Only positive numbers are accepted. Negative number will be ignored.\n");
		}
		else if (value > 0) { //count positive input in
			sum += value;
			count++;
		}
	} while (value != 0);

	if (count == 0) { //in case user entered 0 right away
		printf("Average: %d\n", sum);
	}
	else {
		printf("Average: %f\n", (float)sum / count);
	}
	return 0;
}
*/