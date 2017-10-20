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
#include <linux/kref.h>

struct number_struct {
	int val;
    struct kref refcount;
};

struct number_struct *nump;

static void __init number_init(void){
    nump = kmalloc(sizeof(struct number_struct), GFP_KERNEL);	
    nump->val = 100;
    kref_init(&(nump->refcount));
    kref_get(&nump->refcount);
}

void myrelease(struct kref *kref)
{
   struct number_struct *ns = container_of(kref, struct number_struct, refcount);

   pr_info("my release called\n");
   kfree(ns);
}

void destroy_number(void){
    pr_info("destroy number struct\n");
    kref_get(&nump->refcount);
    kref_put(&(nump->refcount), myrelease);
    kref_put(&(nump->refcount), myrelease);
    kref_put(&(nump->refcount), myrelease);
}

static int __init my_init(void)
{
	number_init();
	pr_info("Hello: module loaded at 0x%p\n", my_init);
	return 0;
}

static void __exit my_exit(void)
{
        destroy_number();
        pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("A GENIUS");
MODULE_LICENSE("GPL v2");
