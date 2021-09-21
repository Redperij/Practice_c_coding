#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#define STRING_LIMIT 40
#define SUPPLIER_SIZE 32 //2^5
#define CATEGORY_SIZE 32 //2^5
#define REGISTRATION_PLATE_LIMIT 16 //2^4
#define NUMBER_OF_CARS_IN_DB 15

typedef struct rentalCar {
	char supplier_name[SUPPLIER_SIZE]; //Maker's name
	char model_name[STRING_LIMIT]; //Car model's name
	char rental_category[CATEGORY_SIZE]; //Car category (see 'Available rental categories')
	char registration_plate[REGISTRATION_PLATE_LIMIT]; //Car number on the registration plate
	int current_mileage; //km
	bool available; //Is car available right now
} car;
typedef struct rentalCar *car_ptr;

/*
*	Available rental categories:
*	Mini
*	Economy/Supermini
*	Compact/Small Family
*	Estate/Small MPV
*	Large Family
*	Large MPV/People Carrier
*	Executive
*	Jeep/SUV
*	Luxury/Prestige
*/

//Prints car for the table
int printCar(const car_ptr car);
//Prints table with given list of cars
//Horrible way to print table. Console should be used in full screen in order to make use of this.
int printTable(const car_ptr cars_list, const int size);
//Prints menu
void printMenu();
//Prints available categories
void printCategories();
//Asks user to choose a category and calls printing function
void handleCategory(const car_ptr cars_list, const int size);
//Prints table with the cars of selected category
int printFreeCategoryCars(const car_ptr cars_list, const int size, const char *category);
//Asks user for car registration plate and changes car state
void handleCarStateChange(const car_ptr cars_list, const int size);
//Changes car state from Available to Currently rented and vise versa.
int changeCarState(const car_ptr cars_list, const int size, const char *reg_plate);
//Erases \n and replaces it with \0 in the received string
int validate_input_string(char* string);

int main() {
	bool quit = false; //Checks whether user decided to quit
	int user_choice; //Holds option number, typed by the user

	car cars_list[NUMBER_OF_CARS_IN_DB] = {
		{"KTM", "450 SX - F FACTORY EDITION", "Estate/Small MPV", "ABC-123", 60, true},
		{"HARLEY DAVIDSON", "FXSTSSE2 SCREAMIN EAGLE SOFTAIL SPR", "Large Family", "HDS-333", 1380, true},
		{"MERCEDES-BENZ", "SL550", "Luxury/Prestige", "ABA-183", 13453, true},
		{"HINO", "268", "Executive", "QTH-166", 1000000, false},
		{"MERCEDES-BENZ", "C180", "Luxury/Prestige", "COK-111", 14352, false},
		{"ARCTIC CAT", "500 4X4 AUTO TBX", "Large Family", "YEP-777", 48547, false},
		{"PONTIAC", "MATIZ", "Large MPV/People Carrier", "HEI-013", 0, true},
		{"ISUZU", "HTR", "Economy/Supermini", "HAY-423", 489741, false},
		{"TRIUMPH", "BONNEVILLE", "Jeep/SUV", "SAY-020", 548974, true},
		{"MERCEDES-BENZ", "S600", "Luxury/Prestige", "SQL-626", 47045, true},
		{"MITSUBISHI FUSO", "FK", "Mini", "FBK-000", 0, true},
		{"HYUNDAI", "AZERA", "Compact/Small Family", "CSF-793", 648, true},
		{"BMW", "750I", "Compact/Small Family", "CSF-187", 1010, true},
		{"CAN-AM", "MAVERICK X RS 1000R", "Jeep/SUV", "SUV-337", 694814, true},
		{"APRILIA", "RS 125", "Large MPV/People Carrier", "LAM-907", 40497, true},
	};

	while(!quit) {
		printMenu();
		printf("Your choice: ");
		while (scanf("%d", &user_choice) != 1) {
			printf("Incorrect input. Write a corresponding number to the option.\n");
			while (getchar() != '\n'); //getting rid of trash
			printMenu();
			printf("Your choice: ");
		}
		switch(user_choice) {
			case 1: //print whole list
				printTable(cars_list, NUMBER_OF_CARS_IN_DB);
				break;
			case 2: //cars by category
				handleCategory(cars_list, NUMBER_OF_CARS_IN_DB);
				break;
			case 3: //change state of a car
				handleCarStateChange(cars_list, NUMBER_OF_CARS_IN_DB);
				break;
			case 4: //quit
				quit = true;
				printf("\nTerminating execution.\n");
				break;
			default:
				printf("Incorrect input. Try again.\n\n");
		}
	}

	return 0;
}

//Prints car for the table
int printCar(const car_ptr car) {
	char *availability; //Chooses correct word to show current car state
	if(car->available) {
		availability = "Available";
	} 
	else {
		availability = "Currently rented";
	}

	printf("%-32s | %-40s | %-32s | %-9s | %10d | %-16s\n", car->supplier_name, car->model_name, car->rental_category, car->registration_plate, car->current_mileage, availability);
	return 0;
}

//Prints table with given list of cars
//Horrible way to print table. Console should be used in full screen in order to make use of this.
int printTable(const car_ptr cars_list, const int size) {
	printf("%-32s | %-40s | %-32s | %-9s | %-10s | %-16s\n", "Supplier Name", "Car Model", "Rental category", "Re Plate", "Mileage km", "Availability");
	for (int i = 0; i < size; i++) {
		printCar(&cars_list[i]);
	}
	return 0;
}

//Prints menu
void printMenu() {
	printf("Choose an option:\n");
	printf("1. Print all cars\n2. Print free cars of given category\n3. Change state of a car\n4. Quit the program\n");
}

//Prints available categories
void printCategories() {
	printf("\nWrite exactly one category. (case sensitive)\n");
	printf("Write '*' to print whole list.\n");
	printf("Available categories:\n");
	printf("Mini\nEconomy/Supermini\nCompact/Small Family\nEstate/Small MPV\nLarge Family\nLarge MPV/People Carrier\nExecutive\nJeep/SUV\nLuxury/Prestige\n");
}

//Asks user to choose a category and calls printing function
void handleCategory(const car_ptr cars_list, const int size) {
	char input_string[CATEGORY_SIZE]; //String with written category by the user

	printCategories();
	printf("\nPlease, write full category name or *:\n");
	while (getchar() != '\n'); //getting rid of trash
	fgets(input_string, CATEGORY_SIZE, stdin);
	validate_input_string(input_string);

	printFreeCategoryCars(cars_list, size, input_string);
}

//Prints table with the cars of selected category
int printFreeCategoryCars(const car_ptr cars_list, const int size, const char *category) {
	int results = 0; //Counts how many cars met the filtering requirements
	car found_cars_list[NUMBER_OF_CARS_IN_DB]; //Array with found cars

	if (!strcmp(category, "*")) { //If user gave star -> print out all available cars
		for (int i = 0; i < size; i++) {
			if (cars_list[i].available) {
				results++;
				found_cars_list[results - 1] = cars_list[i];
			}
		}
		printTable(found_cars_list, results);
	}
	else {
		for(int i = 0; i < size; i++) { //Choosing all cars, that fit chosen category and are available
			if(!strcmp(cars_list[i].rental_category, category) && cars_list[i].available) {
				results++;
				found_cars_list[results - 1] = cars_list[i];
			}
		}

		if(results > 0) { //If something is found -> Print it out as a table
			printTable(found_cars_list, results);
		}
		else { //Nothing is found, user could misstype category or search for non-existing one
			printf("No results found.\n");
		}
	}
	return 0;
}

//Asks user for car registration plate and changes car state
void handleCarStateChange(const car_ptr cars_list, const int size) {
	char input_string[REGISTRATION_PLATE_LIMIT];
	printf("\nPlease, write car registration plate name. Use capital letters:\n");
	while (getchar() != '\n'); //getting rid of trash
	fgets(input_string, REGISTRATION_PLATE_LIMIT, stdin);
	validate_input_string(input_string);

	changeCarState(cars_list, size, input_string);
}

//Changes car state from Available to Currently rented and vise versa.
int changeCarState(const car_ptr cars_list, const int size, const char *reg_plate) {
	car_ptr selected_car = NULL; //Initializing NULL pointer in case user have given wrong registration plate

	for(int i = 0; i < size; i++) { //Searching for car with matching registration plate
		if (!strcmp(cars_list[i].registration_plate, reg_plate)) {
			selected_car = &cars_list[i];
		}
	}
	if (selected_car != NULL) {
		if (selected_car->available) { //From available to rented
			selected_car->available = false;
			printf("\n%s %s car with %s registration plate is selected. Changed state to 'Currently rented'.\n", selected_car->supplier_name, selected_car->model_name, selected_car->registration_plate);
		}
		else { //From rented to available
			selected_car->available = true;
			printf("\n%s %s car with %s registration plate is selected. Changed state to 'Available'.\n", selected_car->supplier_name, selected_car->model_name, selected_car->registration_plate);
		}
	}
	else {
		printf("\nNo car with %s registration plate found.\n", reg_plate);
	}
	return 0;
}

//Erases \n and replaces it with \0 in the received string
int validate_input_string(char *string) {
	for(unsigned int i = 0; i < strlen(string); i++) {
		if (string[i] == '\n') {
			string[i] = '\0';
			break;
		}
	}
	return 0;
}