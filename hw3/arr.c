
 // Compile with: gcc -o arr arr.c -Wall
 // Debug with gdb ./arr
 // Debug with valgrind valgrind --leak-check=yes ./arr

#include <stdlib.h>

void f(void)
{
	int* x = malloc(10*sizeof(int));
	x[10] = 0;
}

int main(void)
{
	f();
	return 0;
}
