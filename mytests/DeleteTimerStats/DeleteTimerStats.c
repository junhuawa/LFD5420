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

struct kprobe my_del_timer_kp;
struct kprobe my_del_timer_sync_kp;
struct kprobe my_add_timer_kp;
struct kprobe my_mod_timer_kp;

static int del_timer_count = 0;
static int del_timer_sync_count = 0;
static int add_timer_count = 0;
static int mod_timer_count = 0;

static int my_add_timer_pre_handler(struct kprobe *kp, struct pt_regs *pg)
{
    add_timer_count ++;
    if(add_timer_count%100 == 0){
        pr_info("kprobe add_timer called %d times!\n", add_timer_count);
    }
    return 0;
}

static int my_mod_timer_pre_handler(struct kprobe *kp, struct pt_regs *pg)
{
    mod_timer_count ++;
    if(mod_timer_count%100 == 0){
        pr_info("kprobe mod_timer called %d times!\n", mod_timer_count);
    }
    return 0;
}

static int my_del_timer_pre_handler(struct kprobe *kp, struct pt_regs *pg)
{
    del_timer_count ++;
    if(del_timer_count%100 == 0){
        pr_info("kprobe delete_timer called %d times!\n", del_timer_count);
    }
    return 0;
}

static int my_del_timer_sync_pre_handler(struct kprobe *kp, struct pt_regs *pg)
{
    del_timer_sync_count ++;
    if(del_timer_sync_count%100 == 0){
        pr_info("kprobe delete_timer_sync called %d times!\n", del_timer_sync_count);
    }
    return 0;
}

//   static void my_del_timer_kp_post_handler(struct kprobe *kp, struct pt_regs *pg, unsigned long flags)
//
//   {
//       pr_info("kprobe post handler!\n");
//       return ;
//   }
//
//   static int my_del_timer_kp_break_handler(struct kprobe *kp, struct pt_regs *pg)
//   {
//       pr_info("kprobe break handler!\n");
//       return 0;
//   }
//
//   static int my_del_timer_kp_fault_handler(struct kprobe *kp, struct pt_regs *pg, int trapnr)
//   {
//       pr_info("kprobe fault handler!\n");
//       return 0;
//   }

static int __init my_init(void)
{
    my_del_timer_kp.pre_handler = my_del_timer_pre_handler;

    my_del_timer_kp.symbol_name = "del_timer";
   
    if(register_kprobe(&my_del_timer_kp)) {
        pr_info("register kprobe failed\n");
        return -1;
    }

    my_del_timer_sync_kp.pre_handler = my_del_timer_sync_pre_handler;

    my_del_timer_sync_kp.symbol_name = "del_timer_sync";
   

    if(register_kprobe(&my_del_timer_sync_kp)) {
        pr_info("register kprobe failed\n");
        return -1;
    }

    my_add_timer_kp.pre_handler = my_add_timer_pre_handler;

    my_add_timer_kp.symbol_name = "add_timer";
   
    if(register_kprobe(&my_add_timer_kp)) {
        pr_info("register kprobe failed\n");
        return -1;
    }

    my_mod_timer_kp.pre_handler = my_mod_timer_pre_handler;

    my_mod_timer_kp.symbol_name = "mod_timer";
   
    if(register_kprobe(&my_mod_timer_kp)) {
        pr_info("register kprobe failed\n");
        return -1;
    }
	pr_info("Hello: module loaded at 0x%p\n", my_init);
	return 0;
}

static void __exit my_exit(void)
{
    unregister_kprobe(&my_del_timer_kp);
    unregister_kprobe(&my_del_timer_sync_kp);
    unregister_kprobe(&my_add_timer_kp);
    unregister_kprobe(&my_mod_timer_kp);
    pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("A GENIUS");
MODULE_LICENSE("GPL v2");
