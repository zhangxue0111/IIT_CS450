
 // This is a program that allocates memory using malloc() but forgets to free it before exiting.
 // Compile with: gcc -o pa31 pa31.c -Wall
 // Run with ./pa31
 // Debug with gdb ./pa31
 // Debug with valgrind valgrind --leak-check=yes ./pa31

#include<stdlib.h>

int main() 
{
	int* p;
	p = malloc(sizeof(int));
	*p = 30;
	return 0;
}
