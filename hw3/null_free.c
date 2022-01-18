
 // Compile with: gcc -g null_free null_free.c -o -Wall
 // Run with ./null_free
 // Debug with gdb ./null_free
 // Debug with valgrind valgrind --leak-check=yes ./null_free

#include <stdlib.h>

int main(void)
{
	int *x;
	x = NULL;
	free(x);
	return 0;
}
