 // Team members: Xue Zhang (A20494478) and Xiaoxu Li (A20522966)

 // Compile with: gcc -o p4 p4.c -Wall
 // Debug with gdb ./p4
 // Debug with valgrind valgrind --leak-check=yes ./p4

#include <stdlib.h>
#include <stdio.h>

void f(void)
{
	int* x = malloc(10*sizeof(int));
	for(int i=0;i<10;i++){
		x[i] = i;
	}
	free(&x[4]);
}

int main(void)
{
	f();
	return 0;
}