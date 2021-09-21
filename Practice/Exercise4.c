#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>

//Checks the time bounds.
bool input_is_correct(int hours, int minutes, int accepted_hours_max, int accepted_minutes_max);
//Uses minute-formatted time to count wakining time.
int count_wakining_time(int current_time_minutes, int to_sleep_minutes);
//Converts all hours to minutes.
int convert_dig_time_to_minutes(int hours, int minutes);
//Returns hours. Leaves minutes as a leftover in a received variable.
int convert_minutes_to_dig_time(int *minutes);

int main() {
	int hours = 0; //current hours, user input
	int minutes = 0; //current minutes, user input
	int hours_to_sleep = 0; //hours to sleep, user input
	int minutes_to_sleep = 0; //minutes to sleep, user input
	int hours_to_wake = 0; //hours when to wake
	int minutes_to_wake = 0; //minutes when to wake

	do { //Reading current time in digital format.
		printf("Enter current time (hh:mm): ");
		while (scanf("%d:%d", &hours, &minutes) != 2) { //Program must get correct input (hh:mm)
			printf("\nError!\nIncorrect time format, should be (hh:mm)\n(0 - 23 : 0 - 59)\n\n");
			while (getchar() != '\n'); //getting rid of trash
			printf("Enter current time (hh:mm): ");
		}
	} while (!input_is_correct(hours, minutes, 23, 59));

	do { //Reading time to sleep. Only 1 digit ammount of hours is acceptable
		printf("How long do you want to sleep (h:mm): ");
		while (scanf("%d:%d", &hours_to_sleep, &minutes_to_sleep) != 2) { //Program must get correct input (h:mm)
			printf("\nError!\nIncorrect time format, should be (h:mm)\n(0 - 9 : 0 - 59)\n\n");
			while (getchar() != '\n'); //getting rid of trash
			printf("How long do you want to sleep(h:mm): ");
		}
	} while (!input_is_correct(hours_to_sleep, minutes_to_sleep, 9, 59));
	
	minutes_to_wake = count_wakining_time(convert_dig_time_to_minutes(hours, minutes), convert_dig_time_to_minutes(hours_to_sleep, minutes_to_sleep));
	hours_to_wake = convert_minutes_to_dig_time(&minutes_to_wake);

	printf("If you go to bed now you must wake up at %02d:%02d.\n", hours_to_wake, minutes_to_wake);

	return 0;
}

//Checks the time bounds.
bool input_is_correct(int hours, int minutes, int accepted_hours_max, int accepted_minutes_max) {
	if (hours < 0 || minutes < 0 || hours > accepted_hours_max || minutes > accepted_minutes_max) {
		printf("\nError! Time limits are: %d hours and %d minutes\n", accepted_hours_max, accepted_minutes_max);
		return false;
	}
	return true;
}

//Uses minute-formatted time to count wakining time.
int count_wakining_time(int current_time_minutes, int to_sleep_minutes) {
	return (current_time_minutes + to_sleep_minutes) % 3600; //Loop is at 3600 minute. Number of loops is obsolete.
}

//Converts all hours to minutes.
int convert_dig_time_to_minutes(int hours, int minutes) {
	return hours * 60 + minutes;
}

//Returns hours. Leaves minutes as a leftover in a received variable.
int convert_minutes_to_dig_time(int *minutes) {
	int hours = *minutes / 60;
	if (hours >= 24) { //keeping time looping
		int loops = hours / 24;
		hours -= 24 * loops;
	}
	*minutes %= 60; // counting leftover minutes
	return hours;
}