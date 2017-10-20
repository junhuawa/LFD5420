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

extern int my_export_func(int val);

static int __init my_init(void)
{

	struct task_struct *s;
	for_each_process(s){

	pr_info("tgid: %d, pid: %d, name: %s, state: %d\n", (int)s->tgid, (int)s->pid, s->comm, (int)s->state);

	}
	my_export_func(5);
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
