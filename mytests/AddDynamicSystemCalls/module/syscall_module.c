/* **************** LFD5420:4.13 s_05/trivial.c **************** */
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
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/slab.h>

extern struct my_sc_struct my_sc;

static long test_func(unsigned long val)
{
    pr_info("val: %lu!\n", val);

    return val + 1;
}

static int __init my_init(void)
{
    my_sc.fun = test_func;
	pr_info("Hello: module loaded at 0x%p\n", my_init);
	return 0;
}

static void __exit my_exit(void)
{
        pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("A GENIUS");
MODULE_LICENSE("GPL v2");
