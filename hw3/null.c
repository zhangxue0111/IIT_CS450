 // Team members: Xue Zhang (A20494478) and Xiaoxu Li (A20522966) 

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