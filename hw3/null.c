 
 // Compile with: gcc -g null null.c -o -Wall
 // Run with ./null
 // Debug with gdb ./null
 // Debug with valgrind valgrind --leak-check=yes ./null

#include <stdlib.h>

int main(void)
{
	int *x;
	x = NULL;
	return 0;
}
