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
#include <linux/mm.h>

static char * memptr = NULL;

void dump_mem(char *ptr)
{
    int i;
    for(i=0; i<4096; i++)
    {
        printk("0x%2d", ptr[i]);
        if((i+1)%16 == 0) {
            pr_info("\n");
        }
    }
}

extern struct page *mem_map;

static int __init my_init(void)
{
    memptr = (char *)get_zeroed_page(GFP_KERNEL);
    if(!memptr) {
        pr_info("get mem page failed\n");
        return -1;
    }
    dump_mem(memptr);
	pr_info("Hello: module loaded at 0x%p\n", my_init);
	pr_info("size of (struct page): %d\n", (int)sizeof(struct page));

	return 0;
}

static void __exit my_exit(void)
{
    free_page((unsigned long) memptr);
        pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("A GENIUS");
MODULE_LICENSE("GPL v2");
