#pragma warning(disable:4996) //disabling warning

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "listofcars.txt"

//Holds data in binary format in file.
typedef struct car_ {
	char *make; //Make of the car
	char *model; //Model of the car
	int price; //Price of the car
	float co2_emission; // CO2 emission in g/km
} car;

/*
* Gets new car from the user input.
* Returns newly built car pointer.
*/
car *get_new_car();
/*
* Reads string until the newline.
* Returns 0 on success. 1 on memory error. 2 on empty string.
*/
int read_string(char **string);
/*
* Gets all cars from the binary file.
* Returns cars count. In case of memory mistake returns -1. No file returns -2
*/
int get_cars_from_file(const char *filename, car **cars);
/*
* Adds new car to the file.
* Returns 0 on success. 1 on failure.
*/
int add_new_car_to_file(const char *filename, const car *car);
/*
* Prints menu on the console screen.
*/
void print_menu();

int main() {
	car *new_car = NULL; //Holds new car to add to the file.
	car *cars = NULL; //Array with all cars from the file.
	int size = 0; //Size of the cars array.
	int command = 0; //Command received from user.

	while (command != 3) {
		print_menu();
		while (scanf("%d", &command) != 1) {
			while (getc(stdin) != '\n'); //Getting rid of trash.
			printf("Incorrect input. Use only numbers. (1, 2, 3)\nInput: ");
		}
		switch (command)
		{
		case 1: //Print all cars from file.
			printf("\n**********************************************************\n");
			size = get_cars_from_file(FILENAME, &cars);

			//Errors
			if (size < 1) {
				if (size == 0) printf("No cars were found.\n");
				if (size == -1) printf("Memory mistake.\n");
				if (size == -2) printf("File is not found.\n");
			}

			//Printing cars
			for (int i = 0; i < size; i++) {
				printf("\nCar No: %d\n", i + 1);
				printf("Make: %s\n", cars[i].make);
				printf("Model: %s\n", cars[i].model);
				printf("Price: %d\n", cars[i].price);
				printf("CO2 emission: %.2f\n", cars[i].co2_emission);
			}
			printf("\n");

			//Emptying memory.
			if (cars != NULL) {
				free(cars);
				cars = NULL;
			}
			size = 0;
			break;
		case 2: //Add new car to file.
			printf("\n**********************************************************\n");
			//Getting new car from user input.
			new_car = get_new_car();

			//Adding new car to the file.
			if (add_new_car_to_file(FILENAME, new_car)) {
				printf("Was unable to add new car to the file.\n");
			}
			else {
				printf("Successfully added new car to the file.\n");
			}

			//Emptying memory.
			if (new_car != NULL) {
				free(new_car);
				new_car = NULL;
			}
			break;
		case 3: //Quit the program.
			printf("\n**********************************************************\n");
			printf("Exiting the program.\n");
			break;
		default:
			printf("\n**********************************************************\n");
			printf("Unrecognised command '%d'\n\n", command);
			break;
		}
	}

	return 0;
}

/*
* Gets new car from the user input.
* Returns newly built car.
*/
car *get_new_car() {
	car *new_car = malloc(sizeof(car)); //New car to create.
	char *make = malloc(sizeof(char)); //String with make of the car.
	char *model = malloc(sizeof(char)); //String with model of the car.
	int price = 1; //Integer with price of the car.
	float co2_emission = -1; //Float with CO2 emission by the car.

	//malloc() failed -> escaping.
	if (new_car == NULL) {
		return new_car;
	}

	//In the following 'price' is used to check errors!!
	//Getting make of the car.
	while (price) {
		printf("Enter the make of the car: ");
		price = read_string(&make);
		switch (price)
		{
		case 1:
			printf("Memory error occured. Try again.\n\n");
			while (getc(stdin) != '\n'); //Getting rid of trash.
			break;
		case 2:
			printf("Given string is empty. Try again.\n\n");
			break;
		default:
			break;
		}
	}
	
	//Getting model of the car.
	price = 1;
	while (price) {
		printf("Enter the model of the car: ");
		price = read_string(&model);
		switch (price)
		{
		case 1:
			while (getc(stdin) != '\n'); //Getting rid of trash.
			printf("Memory error occured. Try again.\n\n");
			break;
		case 2:
			printf("Given string is empty. Try again.\n\n");
			break;
		default:
			break;
		}
	}
	price = -1; //price suffered enough.
	//Getting price.
	while (price < 0) {
		printf("Enter the price of the car: ");
		while (scanf("%d", &price) != 1) {
			while (getc(stdin) != '\n'); //Getting rid of trash.
			printf("Incorrect input. Try again.\n\n");
			printf("Price of the car: ");
		}
		if (price < 0) {
			printf("Price cannot be negative!\n");
		}
	}
	//Getting c02_emission.
	while (co2_emission < 0) {
		printf("Enter the CO2 emission of the car (g/km): ");
		while (scanf("%f", &co2_emission) != 1) {
			while (getc(stdin) != '\n'); //Getting rid of trash.
			printf("Incorrect input. Try again.\n\n");
			printf("CO2 emission of the car (g/km): ");
		}
		if (co2_emission < 0) {
			printf("CO2 emission cannot be negative!\n");
		}
	}

	//Building new car.
	new_car->make = make;
	new_car->model = model;
	new_car->price = price;
	new_car->co2_emission = co2_emission;

	return new_car;
}

/*
* Reads string until the newline.
* Returns 0 on success. 1 on memory error. 2 on empty string.
*/
int read_string(char **string) {
	int i = 0; //Number of iterations.
	char input; //Char from input

	if (*string == NULL) {
		*string = (char *)malloc(sizeof(char));

		if (*string == NULL) { //malloc() failed -> escaping.
			return 1;
		}
	}

	do {
		input = getc(stdin);
		if (input != '\n') {
			i++;
			char *temp_string = realloc(*string, (i + 1) * sizeof(char));
			if (temp_string == NULL) { //realloc() failed -> escaping.
				if (i == 1) { //Realloc failed on the first char -> string must be empty.
					free(*string);
					*string = NULL;
				}
				return 1;
			}
			else {
				*string = temp_string; //Getting correct pointer.
				string[0][i - 1] = input; //Writing input char to the string.
			}
		}
	} while(input != '\n');

	string[0][i] = '\0'; //Terminating string.
	
	if (i == 0) {
		return 2;
	}

	return 0;
}

/*
* Gets all cars from the binary file.
* Returns cars count. In case of memory mistake returns -1. No file returns -2
*/
int get_cars_from_file(const char *filename, car **cars) {
	FILE *file; //File to read.
	uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t)); //Data array to hold a variable in binary format.
	uint8_t *byte = (uint8_t *)malloc(sizeof(uint8_t)); //Read byte.
	uint8_t *prev_byte = (uint8_t *)malloc(sizeof(uint8_t)); //Previous read byte.
	uint8_t loop = 1; //Which part of the car is read. 1 - make, 2 - model, 3 - price, 4 - co2_emission.
	int ncars = 0; //Number of car read.

	//malloc() failed -> escape.
	if (prev_byte == NULL || byte == NULL || data == NULL) {
		return -1;
	}

	//Initialize cars array if it is not initialized.
	if (*cars == NULL) {
		*cars = (car *)malloc(sizeof(car));
		//malloc() failed -> escape.
		if (*cars == NULL) {
			return -1;
		}
	}

	//Open the file.
	file = fopen(filename, "rb");
	//file is nullptr -> escape.
	if (file == NULL) {
		return -2;
	}

	//Loop until the end of the file.
	while (!feof(file) && !ferror(file)) {
		int i = 0; //Iteration count for data array.
		//Reading byte.
		fread(byte, 1, 1, file);
		//If managed to read byte.
		if (!feof(file)) {
			*prev_byte = 0; //Previous byte is reset to 0
			//Loop until the delimiter while collecting data. If the file will end, well, it can end in a very unexpected way.
			while (!(*prev_byte == 92 && *byte == 59) && !feof(file)) { //!(prev_byte == '\\' && byte == ';') (in the file: "\;" or "5c 3b")
				uint8_t *temp_data = realloc(data, (i + 1) * sizeof(uint8_t));

				//realloc() failed -> escape.
				if (temp_data == NULL) {
					return -1;
				}
				data = temp_data;

				data[i] = *byte;
				*prev_byte = *byte;
				i++;
				//Reading byte.
				fread(byte, 1, 1, file);
			}
			//get rid of the '\'
			data[i - 1] = '\0'; //Warning is prevented in the *prev_byte = 0 line. *prev_byte will never be 92 after exiting any of the cases.
			uint8_t *temp_data = realloc(data, i * sizeof(uint8_t));
			//realloc() failed -> escape.
			if (temp_data == NULL) {
				return -1;
			}
			data = temp_data;
			//Decide what to do with the collected data.
			switch (loop)
			{
			case 1: //char *make
				loop = 2;
				cars[0][ncars].make = data;
				data = NULL; //Must be null in order not to free string from the memory later.
				break;
			case 2: //char *model
				loop = 3;
				cars[0][ncars].model = data;
				data = NULL; //Must be null in order not to free string from the memory later.
				break;
			case 3: //int price
				loop = 4;
				int *temp_int = (int *)data; //Getting pointer.
				cars[0][ncars].price = *temp_int;
				break;
			case 4: //float co2_emission
				loop = 1;
				float *temp_float = (float *)data; //Getting pointer.
				cars[0][ncars].co2_emission = *temp_float;

				//Add memory for another car.
				ncars++;
				car *temp_cars = realloc(*cars, (ncars + 1) * sizeof(car));
				//realloc() failed -> escape.
				if (temp_cars == NULL) {
					return -1;
				}
				*cars = temp_cars; //Get the correct pointer.

				//Reset the loop.
				free(data);
				data = NULL;
				break;
			default:
				//What? https://youtu.be/dQw4w9WgXcQ (I regret nothing)
				break;
			}
		}
		else if(ncars > 0){ //Reached end of file. Get rid of the redundant allocated memory.
			car *temp_cars = realloc(*cars, (ncars) * sizeof(car));
			//realloc() failed -> escape.
			if (temp_cars == NULL) {
				return -1;
			}
			*cars = temp_cars; //Get the correct pointer.
		}
		else { //Was unable to read first car. Free the memory.
			free(*cars);
			*cars = NULL;
		}
	}

	fclose(file);
	if (data != NULL) {
		free(data);
	}
	if (byte != NULL) {
		free(byte);
	}
	if (prev_byte != NULL) {
		free(prev_byte);
	}

	return ncars;
}

/*
* Adds new car to the file.
* Returns 0 on success. 1 on failure.
*/
int add_new_car_to_file(const char *filename, const car *car) {
	FILE *file;
	char *separator = "\\;";
	//car or filename is nullptr -> escape.
	if (car == NULL || filename == NULL) {
		return 1;
	}

	file = fopen(filename, "ab");
	//file is nullptr -> escape. Should never happen.
	if (file == NULL) {
		return 1;
	}
	//Writing data to the file. csv with "\;" as a separator.
	fwrite(car->make, sizeof(char), strlen(car->make) + 1, file);
	fwrite(separator, sizeof(char), strlen(separator), file);
	fwrite(car->model, sizeof(char), strlen(car->model), file);
	fwrite(separator, sizeof(char), strlen(separator), file);
	fwrite(&car->price, sizeof(car->price), 1, file);
	fwrite(separator, sizeof(char), strlen(separator), file);
	fwrite(&car->co2_emission, sizeof(car->co2_emission), 1, file);
	fwrite(separator, sizeof(char), strlen(separator), file);

	fclose(file);
	return 0;
}

/*
* Prints menu on the console screen.
*/
void print_menu() {
	printf("--------------------------------------\n|Please, choose one of the following:|\n\
|1 - Print all cars from file.       |\n\
|2 - Add new car to the file.        |\n\
|3 - Quit the program.               |\n\
--------------------------------------\n\
Enter the number of command: ");
}