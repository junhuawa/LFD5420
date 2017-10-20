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
#include <linux/mutex.h>

DEFINE_MUTEX (my_mutex);
LIST_HEAD (my_list);

struct number_struct {
	struct list_head list;
	int val;
};

#define ENTRY 10
void walk_list(void);

static void __init mylist_init(void){
	int i;
	struct number_struct *nume;

	for(i=0; i<ENTRY; i++){
		nume = kmalloc(sizeof(struct number_struct), GFP_KERNEL);	
		nume->val = i+1;
		list_add(&nume->list, &my_list);
		pr_info("add val: %d\n", nume->val);
	}
	walk_list();
}

void walk_list(void){
	struct number_struct *nume;
	struct list_head *l;

	list_for_each(l, &my_list){
		nume = list_entry(l, struct number_struct, list);
		pr_info("val: %d\n", nume->val);
	}
}

void destroy_list(void){
	struct number_struct *nume;
	struct list_head *l;
	struct list_head *tmp;

	list_for_each_safe(l, tmp, &my_list){
		nume = list_entry(l, struct number_struct, list);
		pr_info("val: %d\n", nume->val);
		//list_del(l);
		list_del(&nume->list);
		//kfree(tmp);
		kfree(nume);
	}
}

static int __init my_init(void)
{
	mylist_init();
	pr_info("Hello: module loaded at 0x%p\n", my_init);
	return 0;
}

static void __exit my_exit(void)
{
	destroy_list();
        pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

EXPORT_SYMBOL(my_mutex);
module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("A GENIUS");
MODULE_LICENSE("GPL v2");
