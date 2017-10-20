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
#include <linux/timer.h>

struct timer_list my_timer;

static int counter = 1;
static void my_timer_handler(unsigned long val)
{
	
    mod_timer(&my_timer, jiffies + counter * HZ);

    printk("timer expire! %d\n", counter++);
    return;
}

static int __init my_init(void)
{
    init_timer(&my_timer);
    my_timer.expires = jiffies + HZ;
    my_timer.function = my_timer_handler;

    add_timer(&my_timer);
	pr_info("Hello: module loaded at 0x%p\n", my_init);
	return 0;
}

static void __exit my_exit(void)
{
        del_timer_sync(&my_timer);
        pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("A GENIUS");
MODULE_LICENSE("GPL v2");
