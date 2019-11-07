#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isHullDobell(int a, int c, int m, int x0);
int pgcd(int, int);

void main(void)
{
	int a, c, m, x0;
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
	x0 = atoi(buffer);
}

bool isHullDobell(int a, int c, int m, int x0)
{
	bool cond1 = pgcd(c, m) == 1;
}

int pgcd(int a, int b)
{
	int r;
	while (b != 0)
	{
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}
