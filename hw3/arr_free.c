 // Team members: Xue Zhang (A20494478) and Xiaoxu Li (A20522966)

 // Compile with: gcc -o arr_free arr_free.c -Wall
 // Debug with gdb ./arr
 // Debug with valgrind valgrind --leak-check=yes ./arr_free

#include <stdlib.h>
#include <stdio.h>

void f(void)
{
	int* x = malloc(10*sizeof(int));
	for(int i=0;i<10;i++){
		x[i] = i;
	}
	free(x);
	printf("%d\n", x[0]);
}

int main(void)
{
	f();
	return 0;
}