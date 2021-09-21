#pragma warning(disable:4996) //disabling warning

#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#define MAX_FILENAME 255
#define NAME_LENGTH 41

typedef struct product {
	char name[NAME_LENGTH];
	float price;
} PRODUCT;

//Gets name of the file from the user. (Accepts names with whitespaces)
void get_filename(char *filename, bool incorect_input);
//Reads products array from the given file.
PRODUCT *read_products(FILE *input_file, int *count);
//Prints products array to the console in formatted columns.
void print_products(const PRODUCT *products, const int count);
//Checks if the whole string is blank.
bool is_blank(const char *string);

int main() {
	FILE *file; //Input file.
	PRODUCT *products; //Array of products from the file.
	int count; //Count of items in the 'products' array.
	char filename[MAX_FILENAME]; //Name of the file specified by the user.

	//Ask user for a filename. Try to open it.
	get_filename(filename, false);
	file = fopen(filename, "r");

	while (file == NULL) { //If file does not exist -> keep asking user for the correct filename.
		get_filename(filename, true);
		file = fopen(filename, "r");
	}

	products = read_products(file, &count); //Reading products.
	fclose(file); //Closing file after reading.

	if(products == NULL || count == 0) { //Stop the execution if got nothing from the file.
		printf("No products provided correctly in the file.\n");
		return 1;
	}

	print_products(products, count); //Printing out the result.
	return 0;
}

//Gets name of the file from the user. (Accepts names with whitespaces)
void get_filename(char* filename, bool incorect_input) {
	if (!incorect_input) { //User haven't attempted anything yet.
		printf("Please, enter the name of the file: ");
		fgets(filename, MAX_FILENAME, stdin);
	}
	else { //User tries to give correct filename again.
		printf("Specified file is not found. It must be in the same directory with the .c file.\n");
		printf("Please, enter the name of the file: ");
		fgets(filename, MAX_FILENAME, stdin);
	}

	//Getting rid of the newline character.
	for (unsigned int i = 0; i < strlen(filename); i++) {
		if (filename[i] == '\n') {
			filename[i] = '\0';
		}
	}
}

//Reads products array from the given file.
PRODUCT *read_products(FILE *input_file, int *count) {
	PRODUCT *products = (PRODUCT *) malloc(sizeof(PRODUCT)); //Array of products.
	PRODUCT *temp_products; //Used for memory reallocation in order not to lose existing result.
	*count = 0;

	if (products == NULL) { //malloc can fail -> stop executing the function. (Unlikely to happen, but just in case)
		printf("Failed to allocate memory for reading. Restart the program.\n");
		return NULL;
	}

	while (!feof(input_file)) {
		if (fscanf(input_file, "%f", &products[*count].price) && fgetc(input_file) == ';') { //Reading price first and then searching for semicolon.
			fgets(products[*count].name, NAME_LENGTH, input_file); //Format was correct, reading name. If name is longer than allowed -> drop the last part of the name.
			if (products[*count].name != NULL && !is_blank(products[*count].name)) { //Reading can fail, or there can be blank space instead of the name.
				//Getting rid of the newline character.
				for (unsigned int i = 0; i < strlen(products[*count].name); i++) {
					if (products[*count].name[i] == '\n') {
						products[*count].name[i] = '\0';
					}
				}

				*count = *count + 1;
				temp_products = realloc(products, sizeof(PRODUCT) * (*count + 1)); //Reallocating memory for products.

				if (temp_products == NULL) { //realloc can fail -> stop executing the function. (Unlikely to happen, but just in case)
					printf("Failed to reallocate memory for reading. Result is incomplete. Restart the program.\n");
					*count = *count - 1;
					return products;
				}
				
				products = temp_products; //New item added to the array.
			}
		}
		else { //If the formatting was incorrect -> skip to the next line or stop the execution (EOF).
			while (fgetc(input_file) != '\n' && !feof(input_file));
		}
		
	}
	return products;
}

//Prints products array to the console in formatted columns.
void print_products(const PRODUCT *products, const int count) {
	printf("Read %d items.\n", count);
	printf("%-40s %8s\n", "Name", "Price");
	for(int i = 0; i < count; i++) {
		printf("%-40s %8.2f\n", products[i].name, products[i].price);
	}
}

//Checks if the whole string is blank.
bool is_blank(const char *string) {
	for (int i = 0; i < strlen(string); i++) {
		if(!isspace(string[i])) { //non-space character encountered
			return false;
		}
	}
	return true;
}