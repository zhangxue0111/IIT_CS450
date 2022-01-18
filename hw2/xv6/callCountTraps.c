#include "types.h"
#include "stat.h"
#include "user.h"

/*test how many system calls would be executed when the system runs countTraps*/
void test01() {
	countTraps(0);
}

/*test different system calls, taking write for example*/
void test02() {
	printf(3, "Hello!\n");
	countTraps(0);

}

/*test software interrupts, using division by zero as an example*/
void test03() {
	if(fork() == 0) {
		printf(3, "this is a child process.\n");
		int a = 1/0;
		printf(2, "a = %d\n", a);
		exit();
	}
	wait();
	countTraps(0);
}

/*test the parent and the child process, using fork as an example*/
void test04() {
	if(fork() == 0) {
		printf(3, "this is a child process.\n");
		exec("echo", "A");
		countTraps(0);
		exit();
	}
	wait();
	printf(3, "this is a parent process.\n");
	countTraps(0);
}

/*test hardware interrupts, using sleep to mimic*/
void test05() {
	for(int i = 0; i< 10; i++) {
		sleep(1);
		uptime();
	}
	countTraps(0);
}

/*test hardware interrupts, using sleep to mimic*/
void test06() {
	for(int i = 0; i< 10; i++) {
		sleep(1);
		uptime();
	}
	countTraps(0);
}


int main(int argc, char *argv[]) {
	// test01();
	// test02();
	// test03();
	// test04();
	test06();
	exit();
}