#include <stdio.h>

#define SIZE 5

#ifdef DEBUG
#define DEBUGMSG(x, y) printf(x, y)
#else
#define DEBUGMSG(x, y)
#endif // DEBUG


int main() {
	int array[SIZE];
	size_t count = 0;

	printf("Please, enter %d integers:\n", SIZE);
	printf("Input: ");
	while (count < 5) {
		DEBUGMSG("count = %d\n", count);
		while (scanf("%d", &array[count]) != 1) {
			while (getchar() != '\n'); //getting rid of trash
			DEBUGMSG("count = %d\n", count);
			printf("Incorrect input.\nEnter a number: ");
		}
		count++;
	}

	return 0;
}