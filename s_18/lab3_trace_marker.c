/* **************** LFD5420:4.13 s_18/lab3_trace_marker.c **************** */
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
 * Trace Markers
 *  
 * Expand the example of writing a user application that uses trace
 * markers to write at least a few lines.
 *
 * Set the function tracer to filter on sys_write before running your
 * application, and make sure your output appears in the trace.
 *      
 @*/

#include <stdio.h>
#include <stdlib.h>
int main()
{
	FILE *fp_on, *fp_marker;
	int j;
	fp_on = fopen("/sys/kernel/debug/tracing/tracing_on", "w");
	fp_marker = fopen("/sys/kernel/debug/tracing/trace_marker", "w");
	fprintf(fp_on, "1");
	fflush(fp_on);
	for (j = 0; j < 10; j++) {
		fprintf(fp_marker, "Here is my trace marker Number: %d\n", j);
		fflush(fp_marker);
	}
	fprintf(fp_on, "0");
	exit(EXIT_SUCCESS);
}
