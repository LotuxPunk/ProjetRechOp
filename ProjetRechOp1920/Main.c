#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 2

bool isHullDobell(int a, int c, int m, int x0);
void decompose(int n, int* facteursPremier);
int pgcd(int, int);
void push(int* arr, int index, int value, int* size, int* capacity);

void main(void)
{
	/*int a, c, m, x0;
	char* buffer;

	printf("Encodez le a : ");
	fgets(buffer, INT_MAX, stdin);
	a = atoi(buffer);

	printf("Encodez le c : ");
	fgets(buffer, INT_MAX, stdin);
	c = atoi(buffer);

	printf("Encodez le m : ");
	fgets(buffer, INT_MAX, stdin);
	m = atoi(buffer);

	printf("Encodez le x0 : ");
	fgets(buffer, INT_MAX, stdin);
	x0 = atoi(buffer);*/

	int* arr = malloc(INITIAL_CAPACITY * sizeof(int));

	decompose(55, &arr);
	system("pause");
}

bool isHullDobell(int a, int c, int m, int x0)
{
	bool cond1 = pgcd(c, m) == 1;
	bool cond2 = (a - 1) % (c - 1) == 0;
}
