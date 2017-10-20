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
#include <linux/sysrq.h>

static void my_handler(int val){
	
	pr_info("sysrq my handler called\n");

}

struct sysrq_key_op my_sysrq_op = {
	.handler = my_handler,
	.help_msg = "My Help Msg!\n",
	.action_msg = "My Action Msg print!\n",
	.enable_mask = 0xff
};

static int __init my_init(void)
{
	if(register_sysrq_key('x', &my_sysrq_op)){

		pr_info("Register sysrq key err!\n");
		return -1;
	}
	pr_info("Hello: module loaded at 0x%p\n", my_init);

	return 0;
}

static void __exit my_exit(void)
{
	
	if(unregister_sysrq_key('x', &my_sysrq_op)){
		pr_info("Un Register sysrq key err!\n");
	return;
	}

        pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("A GENIUS");
MODULE_LICENSE("GPL v2");
