#pragma warning(disable:4996) //disabling warning

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define TEST_SIZE 12

void sort3(int *pa[3]);

int main()
{
	const int v[TEST_SIZE] = { 1, 2, 3, 7, 4, 9, 12, 4, -1, 67, 67, 34 };
	int a[TEST_SIZE];
	int *ta[3];

	for (int i = 0; i < TEST_SIZE; i++) a[i] = v[i];

	for (int k = 0; k < TEST_SIZE; k += 3) {
		for (int i = 0; i < 3; ++i) {
			ta[i] = &a[i + k];
		}
		sort3(ta);

		printf("%d, %d, %d\n", *ta[0], *ta[1], *ta[2]);
	}

	for (int i = 0; i < TEST_SIZE; ++i) {
		if (a[i] != v[i]) printf("Data corrupted\n");
	}

	return 0;

}

void sort3(int *pa[3]) {
	int *temp_p;
	for (int i = 0; i < 3; i++) {
		for (int q = 1; q < 3; q++) {
			if (*pa[q - 1] > *pa[q]) {
				temp_p = pa[q - 1];
				pa[q - 1] = pa[q];
				pa[q] = temp_p;
			}
		}
	}
}