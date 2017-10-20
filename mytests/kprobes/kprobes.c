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
#include <linux/kprobes.h>

struct kprobe my_kp;

static int my_kp_pre_handler(struct kprobe *kp, struct pt_regs *pg)
{
	pr_info("kprobe pre handler current->comm: %s\n", current->comm);
	return 0;
}

static void my_kp_post_handler(struct kprobe *kp, struct pt_regs *pg, unsigned long flags)

{
	pr_info("kprobe post handler!\n");
	return ;
}

static int my_kp_break_handler(struct kprobe *kp, struct pt_regs *pg)
{
	pr_info("kprobe break handler!\n");
	return 0;
}

static int my_kp_fault_handler(struct kprobe *kp, struct pt_regs *pg, int trapnr)
{
	pr_info("kprobe fault handler!\n");
	return 0;
}

static int __init my_init(void)
{
    my_kp.pre_handler = my_kp_pre_handler;
    my_kp.post_handler = my_kp_post_handler;
    my_kp.break_handler = my_kp_break_handler;
    my_kp.fault_handler = my_kp_fault_handler;

    //my_kp.symbol_name = "_do_fork";
    //my_kp.symbol_name = "do_fork";
    my_kp.symbol_name = "_raw_spin_lock";
   
    //my_kp.addr = (kprobe_opcode_t *)mod_timer;

    if(register_kprobe(&my_kp)) {
        pr_info("register kprobe failed\n");
        return -1;
    }
	pr_info("Hello: module loaded at 0x%p\n", my_init);
	return 0;
}

static void __exit my_exit(void)
{
    unregister_kprobe(&my_kp);
    pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("A GENIUS");
MODULE_LICENSE("GPL v2");
