#include <conio.h>
#include <iostream>

void swap(char& a, char& b) {
	int temp = a;
	a = b;
	b = temp;
}

char getDuplicate(char* arr) {
	int size = strlen(arr);
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (arr[i] == arr[j]) return arr[i];
		}
	}

	return '\0';
}

void clasterize(char* arr, int size) {
	int left = 0;
	int tempNumOfElem = 1;

	char duplicate;
	char* remainder = arr;
	while ((duplicate = getDuplicate(remainder)) != '\0') {
		int remSize = strlen(remainder);
		int dublicateCount = 0;
		for (int i = 0; i < remSize; i++) {
			if (remainder[i] == duplicate) {
				swap(remainder[dublicateCount], remainder[i]);
				dublicateCount++;
			}
		}

		remainder = remainder + dublicateCount;
	}
}

int main(int argc, char* argv[])
{
	_cputs("I am created.\n");

	int size = strlen(argv[0]);
	char* arr = argv[0];
	_cputs(arr);
	_cputs("\n");
	clasterize(arr, size);
	_cputs(arr);

	_cputs("\nPress any key to finish.\n");
	_getch();
	return 0;
}
