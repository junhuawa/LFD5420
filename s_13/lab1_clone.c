/* **************** LFD5420:4.13 s_13/lab1_clone.c **************** */
/*
 * The code herein is: Copyright the Linux Foundation, 2017
 *
 * This Copyright is retained for the purpose of protecting free
 * redistribution of source.
 *
 *     URL:    http://training.linuxfoundation.org
 *     email:  trainingquestions@linuxfoundation.org
 *
 * This code is distributed under Version 2 of the GNU General Public
 * License, which you should have received with the source.
 *
 */
/*
 * Cloning
 *
 * Write a short program that uses the clone() system call to create
 * master and slave threads that share all memory resources.
 *
 * Have a global variable and show that either thread can modify it
 * and the value can be seen by the other.
 *
 * Be careful with creation of the slave thread's stack pointer, to
 * point to the top of the stack since it grows down.
 *
 * What happens if the slave thread dies before the master thread?
 *
 * Note you can use this as a nucleus of your own multi-threaded
 * programming method that does not use the standard threading
 * libraries.  You can also play with sharing file descriptors or not
 * etc.
 @*/

#define _GNU_SOURCE

#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>

int param = 0;

int slav(void *data)
{
	int j;
	printf("starting off slave thread, pid=%d\n", getpid());
	for (j = 0; j < 10; j++) {
		param = j + 1000;
		sleep(1);
		printf("\nslave thread running: j=%d, param=%d secs\n", j,
		       param);
	}
	printf("\nSlave thread saying goodbye\n");
	return 0;
}

int main(int argc, char **argv)
{
	int j, tid, pagesize, stacksize;
	void *stack;

	printf("starting off master thread, pid=%d\n", getpid());

	pagesize = getpagesize();
	stacksize = 4 * pagesize;

	/* could probably just use malloc(), but this is safer */
	/* stack = (char *)memalign (pagesize, stacksize); */
	posix_memalign(&stack, pagesize, stacksize);

	printf("Setting off a clone thread with stacksize = %d....", stacksize);
	tid = clone(slav, (char *)stack + stacksize, CLONE_VM | SIGCHLD, 0);
	printf(" with tid=%d\n", tid);
	if (tid < 0)
		exit(EXIT_FAILURE);

	/* could do a  wait (&status) here if required */

	for (j = 0; j < 6; j++) {
		param = j;
		sleep(1);
		printf("\nmaster thread running: j=%d, param=%d secs\n", j,
		       param);
	}
	printf("master killitself\n");
	/* We shouldn't free(stack) here since the child using it is still running */
	exit(EXIT_SUCCESS);
}
