/* **************** LFD5420:4.13 s_18/marker_test.c **************** */
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
#include <stdio.h>
#include <stdlib.h>
int main()
{
        FILE *fp_on, *fp_marker;
        fp_on = fopen("/sys/kernel/debug/tracing/tracing_on", "w");
        fp_marker = fopen("/sys/kernel/debug/tracing/trace_marker", "w");
        fprintf(fp_on, "1");
        fprintf(fp_marker, "Here is my trace marker Number: %d\n", 1);
        fprintf(fp_marker, "Here is my trace marker Number: %d\n", 2);
        fprintf(fp_on, "0");
        exit(EXIT_SUCCESS);
}
