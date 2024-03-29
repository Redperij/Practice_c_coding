#pragma warning(disable:4996) //disabling warning

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "listofcars.txt"
#define MAX_FILENAME 255
#define MAKE_TAG "\"make\""
#define MODEL_TAG "\"model\""
#define PRICE_TAG "\"price\""
#define EMISSIONS_TAG "\"emissions\""

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
/*
* Gets the file from the user. (Accepts names with whitespaces)
*/
void get_filename(char **filename);
/*
* Gets cars from the specified file.
* Returns number of cars read or -1 in case of lack of the file.
*/
int get_cars(const char *filename, car **cars);
/*
* Removes object_string from string. Takes out one scope of {}. Must be used only in 'array of objects' scope.
* In simple words moves one {object} from string_ptr to object_string_ptr.
* IMPORTANT NOTE: Memory must be handled outside of the function! Function returns only two pointers and terminates object_string_ptr in place of '}'.
* Advice: Feed the starting pointer to the function and use it again to get leftover and process it again. Original pointer must be saved in order to free memory.
* Returns false on failure and true on success.
*/
bool get_obj_str(char **object_string_ptr, char **string_ptr);
/*
* Gets car from object_string.
* Fails in case of lack of variables. Must get '{"make":"string","model":"string","price":int,"emission":float}' in any order.
* car_obj is a mess in case of failure, so it must be handled outside.
* Returns false on failure and true on success.
*/
bool get_car_from_obj_str(const char *object_string, car *car_obj);
/*
* Clears out all '\n' from the string.
*/
void clear_newlines(char **string);
/*
* Reads whole file until EOF and puts it into the string. (Dynamic memory)
*/
void read_file_to_string(FILE *file, char **string);
/*
* Clears received json string from whitespaces and newlines. Doesn't affect variable scopes.
* ( '{ "make" : "My Car" }' -> '{"make":"My Car"}' )
*/
void clean_json_string(char **string);
/*
* Terminates json string on specified character outside of the variable scope.
*/
void term_json_on_char(char **jsonstr, const char terminator);

int main() {
	car *new_car = NULL; //Holds new car to add to the file.
	car *cars = NULL; //Array with all cars from the file.
	int size = 0; //Size of the cars array.
	int command = 0; //Command received from user.
	char *filename;
	int cars_to_add = 0; //cars_to_add for adding cars from the file.

	while (command != 4) {
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
				printf("Was unable to add new car to the \'%s\' file.\n", FILENAME);
			}
			else {
				printf("Successfully added new car to the \'%s\' file.\n", FILENAME);
			}

			//Emptying memory.
			if (new_car != NULL) {
				free(new_car);
				new_car = NULL;
			}
			break;
		case 3: //Add cars from the file.
			cars_to_add = 0;
			get_filename(&filename); //Getting filename.
			cars_to_add = get_cars(filename, &cars); //Getting cars from the json formatted file.

			if (cars_to_add == -1) { //File mistake.
				printf("\nWas unable to open the \'%s\' file\n", filename);
			}
			else if (cars_to_add == 0) { //Memory mistake or no cars.
				printf("\nGot no cars from the \'%s\' file. Please, check the formatting. It must follow json standarts.\n", filename);
			}
			else {
				int addititions = 0;
				for(int i = 0; i < cars_to_add; i++) {
					if (add_new_car_to_file(FILENAME, &cars[i]) == 0) {
						addititions++;
					}
				}
				if(addititions == cars_to_add) { //Added to file cars must match the cars to add.
					printf("\nSuccessfully added all cars to the \'%s\' file.\n", FILENAME);
				}
				else {
					printf("\nWas unable to add all cars to the file.\nSomething went wrong. Please, check the \'%s\' file.\n", FILENAME);
				}
			}
			break;
		case 4: //Quit the program.
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
			char *temp_string = realloc(*string, ((size_t)i + 1) * sizeof(char));
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
	} while (input != '\n');

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
				uint8_t *temp_data = realloc(data, ((size_t)i + 1) * sizeof(uint8_t));

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
				car *temp_cars = realloc(*cars, ((size_t)ncars + 1) * sizeof(car));
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
		else if (ncars > 0) { //Reached end of file. Get rid of the redundant allocated memory.
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
	//Free the memory.
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
|3 - Add cars from the file.         |\n\
|4 - Quit the program.               |\n\
--------------------------------------\n\
Enter the number of command: ");
}

//Gets the filename from the user. (Accepts names with whitespaces)
void get_filename(char **filename) {
	*filename = (char *)malloc(MAX_FILENAME * sizeof(char));
	FILE *file = NULL;
	int length = 0;

	printf("Please, enter the name of the file: ");
	fgets(*filename, MAX_FILENAME, stdin);
	clear_newlines(filename);

	if (strlen(*filename) == 0) { //Mistake with newline is possible. Just throwing in another fgets not to be confused by random error. (If everything is in order, size won't be 0)
		fgets(*filename, MAX_FILENAME, stdin);
		clear_newlines(filename);
	}

	file = fopen(*filename, "rb");
	while (file == NULL) { //Requesting user to give correct filename.
		printf("\nNo such file found. Enter the name again\nPlease, enter the name of the file: ");
		fgets(*filename, MAX_FILENAME, stdin);
		clear_newlines(filename);
		file = fopen(*filename, "rb");
	}
	rewind(file); //Returning to the start.
	fclose(file); //Closing the file.
}

/*
* Gets cars from the specified file.
* Returns number of cars read or -1 in case of lack of the file.
*/
int get_cars(const char *filename, car **cars) {
	FILE *file = NULL; //File to read cars from.
	int count = 0; //Car count.
	char *raw_input = NULL; //String with whole file.
	char *object_string = NULL; //object_string received from function.
	char *unparsed_json_ptr = NULL; //Pointer to the raw_input string. Is changed by get_obj_str()
	//flags
	bool got_object = false;
	bool got_car = false;
	bool corrupt_data = false;

	if(*cars != NULL) {
		free(*cars);
		*cars = NULL;
	}

	*cars = malloc(sizeof(car));

	file = fopen(filename, "r");
	//Unable to find file -> escape.
	if (file == NULL || *cars == NULL) {
		if (file == NULL) return -1; //File not found -> escape.
		return count; //Memory allocation failure -> escape.
	}

	read_file_to_string(file, &raw_input); //Getting raw json string.
	fclose(file);

	clean_json_string(&raw_input); //Getting clean json string.

	unparsed_json_ptr = raw_input; //Getting pointer to the raw_input string.

	//Getting cars from the file.
	do {
		got_object = get_obj_str(&object_string, &unparsed_json_ptr); //Getting object string to convert.
		if (got_object) {
			got_car = get_car_from_obj_str(object_string, &cars[0][count]); //Converting object string into the car.
			if (got_car) {
				car *temp_cars = NULL; //Temporary pointer for memory reallocation.
				count++; //Increase number of cars.
				temp_cars = realloc(*cars, (count + 1) * sizeof(car)); //Reallocate memory for anothe car.
				if (temp_cars == NULL) { //Return what already have.
					return count; //realloc() failed -> escape.
				}
				*cars = temp_cars;
			}
			else {
				corrupt_data = true;
			}
		}
	} while (got_object);

	if (corrupt_data) {
		printf("Data in the file \'%s\' is corrupted. Results may be unexpected!\n", filename);
	}

	//Get rid of the redundant memory.
	if (count != 0) {
		car *temp_cars = NULL; //Temporary pointer for memory reallocation.
		temp_cars = realloc(*cars, count * sizeof(car)); //Reallocate memory for an array (shrink it).
		if (temp_cars == NULL) { //Return what already have.
			return count; //realloc() failed -> escape.
		}
		*cars = temp_cars;
	}

	//Free memory. 
	if (count == 0) {
		free(*cars);
		*cars = NULL;
	}
	if (raw_input != NULL) {
		free(raw_input);
		raw_input = NULL;
	}

	return count;
}

/*
* Removes object_string from string. Takes out one scope of {}. Must be used only in 'array of objects' scope.
* In simple words moves one {object} from string_ptr to object_string_ptr.
* IMPORTANT NOTE: Memory must be handled outside of the function! Function returns only two pointers and terminates object_string_ptr in place of '}'.
* Advice: Feed the starting pointer to the function and use it again to get leftover and process it again. Original pointer must be saved in order to free memory.
* Returns false on failure and true on success.
*/
bool get_obj_str(char **object_string_ptr, char **string_ptr) {
	if (*string_ptr == NULL) return false; //String doesn't exist -> escape.
	char *start = NULL;
	char *end = NULL;
	*object_string_ptr = NULL; //Again. These two are just pointers, not arrays with allocated memory!

	//Loops through whole string remembering first '{' and '}' - it is first object.
	for (unsigned int i = 0; i < strlen(*string_ptr); i++) {
		if (start == NULL && string_ptr[0][i] == '{') {
			start = &string_ptr[0][i];
		}
		if (end == NULL && string_ptr[0][i] == '}') {
			end = &string_ptr[0][i];
		}
	}
	if (start == NULL || end == NULL) {
		return false; //No object found -> escape.
	}

	//1. Increasing start pointer by one place.
	if (start + 1 != end) {
		start++;
	}
	//2. Setting object pointer to the start.
	*object_string_ptr = start;
	//3. Moving string pointer to the (end + 1) place if possible.
	if (*(end + 1) != '\0') {
		*string_ptr = end + 1;
	}
	else {
		*string_ptr = NULL;
	}
	//4. Changing end value ('}') to the '\0'
	*end = '\0';

	return true;
}

/*
* Gets car from object_string.
* Fails in case of lack of variables. Must get '"make":"string","model":"string","price":int,"emission":float' in any order.
* car_obj is a mess in case of failure, so it must be handled outside.
* Returns false on failure and true on success.
*/
bool get_car_from_obj_str(const char *object_string,car *car_obj) {
	char *ptr_to_element; //Pointers to the variables inside the object_string.
	char *string_with_variable = malloc(strlen(object_string) * sizeof(char)); //String with one variable.
	char *make = NULL; //Make of the car.
	char *model = NULL; //Model of the car.
	int price = 0; //Price of the car.
	float co2_emissions = 0; //CO2 emissions of the car.

	if (string_with_variable == NULL) {
		return false; //malloc() failure -> escape.
	}
	//Getting make
	ptr_to_element = strstr(object_string, MAKE_TAG);
	if (ptr_to_element != NULL) {
		strcpy(string_with_variable, ptr_to_element);
		term_json_on_char(&string_with_variable, ',');
		make = malloc(strlen(string_with_variable) * sizeof(char));
		if(make == NULL) {
			return false; //malloc() failure -> escape.
		}
		else {
			char *temp_pointer = strstr(string_with_variable, ":");
			if(temp_pointer == NULL) {
				return false; //No ':' found -> escape.
			}
			if(strlen(temp_pointer) <= 2) {
				return false; // if it is "make":"" -> escape.
			}
			temp_pointer += 2; //Move pointer to the second element
			strcpy(make, temp_pointer); //Copy string until the end.
			make[strlen(make) - 1] = '\0'; //Terminate string on the '"'.
		}
	}
	//Getting model
	ptr_to_element = strstr(object_string, MODEL_TAG);
	if (ptr_to_element != NULL) {
		strcpy(string_with_variable, ptr_to_element);
		term_json_on_char(&string_with_variable, ',');
		model = malloc(strlen(string_with_variable) * sizeof(char));
		if (model == NULL) {
			return false; //malloc() failure -> escape.
		}
		else {
			char *temp_pointer = strstr(string_with_variable, ":");
			if (temp_pointer == NULL) {
				return false; //No ':' found -> escape.
			}
			if (strlen(temp_pointer) <= 2) {
				return false; // if it is "make":"" -> escape.
			}
			temp_pointer += 2; //Move pointer to the second element
			strcpy(model, temp_pointer); //Copy string until the end.
			model[strlen(model) - 1] = '\0'; //Terminate string on the '"'.
		}
	}
	//Getting price
	ptr_to_element = strstr(object_string, PRICE_TAG);
	if (ptr_to_element != NULL) {
		strcpy(string_with_variable, ptr_to_element);
		term_json_on_char(&string_with_variable, ',');
		if (sscanf(string_with_variable, "\"price\":%d", &price) != 1) {
			return false; //Must get all variables -> escape.
		}
	}
	//Getting co2_emissions
	ptr_to_element = strstr(object_string, EMISSIONS_TAG);
	if (ptr_to_element != NULL) {
		strcpy(string_with_variable, ptr_to_element);
		term_json_on_char(&string_with_variable, ',');
		if (sscanf(string_with_variable, "\"emissions\":%f", &co2_emissions) != 1) {
			return false; //Must get all variables -> escape.
		}
	}

	//make and model must exist and be at least 1 char long.
	if (make == NULL || model == NULL || strlen(make) == 0 || strlen(model) == 0) {
		return false; // -> escape.
	}

	car_obj->make = malloc((strlen(make) + 1) * sizeof(char));
	car_obj->model = malloc((strlen(model) + 1) * sizeof(char));

	if (car_obj->make == NULL || car_obj->model == NULL) {
		return false; //malloc() failed -> escape.
	}
	if (price < 0 || co2_emissions < 0) {
		return false; //price and co2_emissions cannot be negative -> escape.
	}

	//Building car.
	strcpy(car_obj->make, make);
	strcpy(car_obj->model, model);
	car_obj->price = price;
	car_obj->co2_emission = co2_emissions;

	return true;
}

/*
* Clears out all '\n' from the string.
*/
void clear_newlines(char **string) {
	if (*string == NULL) return; //String doesn't exist -> escape.

	int i = 0;
	int w = strlen(*string);
	for (i = 0; i < w; i++) {
		if (string[0][i] == '\n' || string[0][i] == '\r') {
			for (int q = i + 1; q <= w; q++) {
				string[0][q - 1] = string[0][q];
			}
			w--;
			i--;
		}
	}
}
/*
* Reads whole file until EOF and puts it into the string. (Dynamic memory)
*/
void read_file_to_string(FILE *file, char **string) {
	int i = 0; //Number of iterations.
	char input; //Char from input

	//Initializing string.
	if (*string != NULL) {
		free(*string);
		*string = NULL;
	}
	*string = (char *)malloc(((size_t)i + 1) * sizeof(char));
	//malloc() failed or no file given -> escape.
	if (*string == NULL || file == NULL) {
		return;
	}

	do {
		input = getc(file);
		if (!feof(file) && !ferror(file)) {
			i++;
			char *temp_string = realloc(*string, ((size_t)i + 1) * sizeof(char));
			if (temp_string == NULL) { //realloc() failed -> escaping.
				if (i == 1) { //Realloc failed on the first char -> string must be empty.
					free(*string);
					*string = NULL;
				}
				return;
			}
			else {
				*string = temp_string; //Getting correct pointer.
				string[0][i - 1] = input; //Writing input char to the string.
			}
		}
	} while (!feof(file) && !ferror(file));

	string[0][i] = '\0'; //Terminating string.
}

/*
* Clears received json string from whitespaces and newlines. Doesn't affect variable scopes.
* ( '{ "make" : "My Car" }' -> '{"make":"My Car"}' )
*/
void clean_json_string(char **string) {
	if (*string == NULL) return; //Received null pointer.
	bool var_scope = false; //Flag to check whether whitespace is located inside the variable scope. (We don't want to ruin the names)
	int i = 0;
	int w = strlen(*string);
	char prev_char = ' '; //Not the best way to check '\', but there shouldn't be '\' in the end of the make or model of the car. (it should be enough)

	for (i = 0; i < w; i++) {
		//Clear all whitespaces outside the variable scopes.
		if (isspace(string[0][i]) && !var_scope) {
			for (int q = i + 1; q <= w; q++) {
				string[0][q - 1] = string[0][q];
			}
			w--;
			i--;
		}
		//It is '"' -> flip variable scope.
		else if (prev_char != '\\' && string[0][i] == '\"') {
			if (var_scope) {
				var_scope = false;
			}
			else {
				var_scope = true;
			}
		}
		else {
			prev_char = string[0][i];
		}
	}
}
/*
* Terminates json string on specified character outside of the variable scope.
*/
void term_json_on_char (char **jsonstr, const char terminator) {
	if (*jsonstr == NULL) return; //Received null pointer.
	bool var_scope = false;
	char prev_char = ' '; //Not the best way to check '\', but there shouldn't be '\' in the end of the make or model of the car. (it should be enough)

	for (unsigned int i = 0; i < strlen(*jsonstr); i++) {
		if (!var_scope && jsonstr[0][i] == terminator) {
			jsonstr[0][i] = '\0';
			break;
		}
		//It is '"' -> flip variable scope.
		else if (prev_char != '\\' && jsonstr[0][i] == '\"') {
			if(var_scope) {
				var_scope = false;
			}
			else {
				var_scope = true;
			}
		}
		else {
			prev_char = jsonstr[0][i];
		}
	}
}